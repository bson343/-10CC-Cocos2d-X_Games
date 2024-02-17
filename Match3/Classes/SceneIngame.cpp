
#include "stdafx.h"

#include "SceneIngame.h"


SceneIngame* SceneIngame::create()
{
    auto ret = new SceneIngame();
    if (ret && ret->init()) ret->autorelease();
    else CC_SAFE_DELETE(ret);
    return ret;
}

bool SceneIngame::init()
{
    if (!Scene::init()) return false;

    callbackCount = 0;

    srand(time(0));

    Director::getInstance()->getTextureCache()->addImage("res/match3_tiles_px.png");
    
    //onEnter()ó�� ���������� �Լ��� ȣ��Ǵ� ���� �ƴ�, std::bind()�� ���� �ݹ��Լ��� ����ϴ� ���, �������̵� ó�� �� �Լ��� ���� ��ų �ʿ�� ����
    auto touch = EventListenerTouchOneByOne::create();
    touch->onTouchBegan = std::bind(&SceneIngame::onTouchBeganTest, this, std::placeholders::_1, std::placeholders::_2);
    touch->onTouchMoved = std::bind(&SceneIngame::onTouchMoved, this, std::placeholders::_1, std::placeholders::_2);
    touch->onTouchEnded = std::bind(&SceneIngame::onTouchEnded, this, std::placeholders::_1, std::placeholders::_2);
    touch->onTouchCancelled = touch->onTouchEnded;
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(touch, this);

    auto keyInput = EventListenerKeyboard::create();
    keyInput->onKeyPressed = CC_CALLBACK_2(SceneIngame::onKeyPressed, this);
    keyInput->onKeyReleased = CC_CALLBACK_2(SceneIngame::onKeyReleased, this);
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyInput, this);

    return true;
}

void SceneIngame::onEnter()
{
    Scene::onEnter();

    this->initUI();
    this->initGame();
    this->startGame();
}

void SceneIngame::initUI()
{
}

void SceneIngame::initGame()
{
    for (int i = 0; i < BLOCK_HORIZONTAL; i++)
    {
        for (int k = 0; k < BLOCK_VERTICAL; k++)
        {
            createBlock(i, k, rand() % 4 + 1);
        }
    }
    this->alignBlockSprite();
}

void SceneIngame::destroyUI()
{
}

void SceneIngame::destroyGame()
{
}

void SceneIngame::alignBlockSprite()
{
    for (int i = 0; i < BLOCK_HORIZONTAL; i++)
    {
        for (int k = 0; k < BLOCK_VERTICAL; k++)
        {
            auto s = getBlockSprite(i, k);

            if (s != nullptr)
            {
                s->setPosition(convertBlockCoordToGameCoord(Vec2(i, k)));
            }
        }
    }
}

bool SceneIngame::onTouchBeganTest(Touch* t, Event* e)
{
    if (state != GameState::PLAYING)
    {
        return true;
    }

    Vec2 p = convertGameCoordToBlockCoord(t->getLocation());

    CCLOG("%f, %f", p.x, p.y);

    destroyBlock(p.x, p.y);

    //dropBlocks();
    //fullFillEmptyBlocks();

    return true;
}

void SceneIngame::onTouchMoved(Touch* t, Event* e)
{
}

void SceneIngame::onTouchEnded(Touch* t, Event* e)
{
}

void SceneIngame::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
    switch (keyCode)
    {
    case EventKeyboard::KeyCode::KEY_R:
        restartGame();
        break;
    }
}

void SceneIngame::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
}

void SceneIngame::startGame()
{
}

void SceneIngame::pauseGame()
{
}

void SceneIngame::winGame()
{
}

void SceneIngame::loseGame()
{
}

void SceneIngame::restartGame()
{
    clearBlockData();
    clearBlockSprite();
    initGame();
}

void SceneIngame::startBlockMoving()
{
    state = GameState::BLOCK_MOVING;
    increaseCallbackCount();
}

void SceneIngame::tryEndBlockMoving()
{
    CCLOG("%d", callbackCount);
    if (isLastCallback())
    {
        state = GameState::PLAYING;
    }
}

int SceneIngame::findEmptyBlockYIndex(int x, int y)
{
    for (int i = y; i < BLOCK_VERTICAL; i++)
    {
        auto block = getBlockData(x, i);
        
        if (block == 0)
        {
            return i;
        }
    }
    return -1;
}

int SceneIngame::findFilledBlockYIndex(int x, int y)
{
    for (int i = y; i < BLOCK_VERTICAL; i++)
    {
        auto block = getBlockData(x, i);

        if (block != 0)
        {
            return i;
        }
    }
    return -1;
}

void SceneIngame::dropBlocks(int x)
{
    decreaseCallbackCount();

    for (int i = 0; i < BLOCK_VERTICAL; i++)
    {
        auto empty_y = findEmptyBlockYIndex(x, i);

        if (empty_y == -1)
        {
            continue;
        }

        auto filled_y = findFilledBlockYIndex(x, empty_y + 1);

        if (filled_y == -1)
        {
            continue;
        }

        {
            auto emptyBlockData = getBlockData(x, empty_y);
            auto filledBlockData = getBlockData(x, filled_y);

            setBlockData(x, empty_y, filledBlockData);
            setBlockData(x, filled_y, emptyBlockData);
        }

        {
            auto emptyBlockSprite = getBlockSprite(x, empty_y);
            auto filledBlockSprite = getBlockSprite(x, filled_y);

            setBlockSprite(x, empty_y, filledBlockSprite);
            setBlockSprite(x, filled_y, emptyBlockSprite);

            filledBlockSprite->runAction(
                Sequence::create(
                    MoveTo::create(0.125f, convertBlockCoordToGameCoord(Vec2(x, empty_y))),
                    CallFunc::create([=]() { evalMatch3Over(x, empty_y); }),
                    nullptr
                )
            );
            increaseCallbackCount();
        }
    }
    tryEndBlockMoving();
}

void SceneIngame::dropBlocks()
{
    for (int x = 0; x < BLOCK_HORIZONTAL; x++)
    {
        dropBlocks(x);
    }
}

void SceneIngame::evalMatch3Over(int x, int y)
{
    int comfirmBlockType = getBlockData(x, y);
    int matchCount = 0;

    decreaseCallbackCount();

    matchCount = checkSameBlock(x, y, comfirmBlockType);

    if (matchCount < 3)
    {
        clearCheckedList();

        tryEndBlockMoving();

        return;
    }

    destroyBlocksForCheckedList();
}


int SceneIngame::checkSameBlock(int x, int y, int blockType)
{
    clearCheckedList();
    return checkSameBlockRecursive(x, y, blockType);
}

int SceneIngame::checkSameBlockRecursive(int x, int y, int blockType)
{
    if (!(x >= 0 && x < BLOCK_HORIZONTAL) || !(y >= 0 && y < BLOCK_VERTICAL))
    {
        return 0;
    }

    if (getBlockData(x, y) != blockType || getCheckedList(x, y) != 0)
        return 0;

    checkCheckedList(x, y);

    return 1 +
    checkSameBlockRecursive(x - 1, y, blockType) +
    checkSameBlockRecursive(x + 1, y, blockType) +
    checkSameBlockRecursive(x, y - 1, blockType) +
    checkSameBlockRecursive(x, y + 1, blockType);
}

void SceneIngame::createBlock(int x, int y, int type)
{
    auto cache = Director::getInstance()->getTextureCache();
    auto spr = Sprite::createWithTexture(cache->getTextureForKey("res/match3_tiles_px.png"), Rect(0 + (type * 40), 0, 40, 40));
    //spr->setScale(2);
    addChild(spr);
    setBlockData(x, y, type);
    setBlockSprite(x, y, spr);
}

void SceneIngame::createBlockRand(int x, int y)
{
    int type = rand() % 4 + 1;
    auto cache = Director::getInstance()->getTextureCache();
    auto spr = Sprite::createWithTexture(cache->getTextureForKey("res/match3_tiles_px.png"), Rect(0 + (type * 40), 0, 40, 40));
    //spr->setScale(2);
    addChild(spr);
    setBlockData(x, y, type);
    setBlockSprite(x, y, spr);
}

int SceneIngame::getBlockData(int x, int y)
{
    return blockData[y][x];
}

void SceneIngame::setBlockData(int x, int y, int type)
{
    blockData[y][x] = type;
}

void SceneIngame::clearBlockData()
{
    for (int i = 0; i < BLOCK_HORIZONTAL; i++)
    {
        for (int k = 0; k < BLOCK_VERTICAL; k++)
        {
            blockData[k][i] = 0;
        }
    }
}

Sprite* SceneIngame::getBlockSprite(int x, int y)
{
    return blockSprite[y][x];
}

void SceneIngame::setBlockSprite(int x, int y, Sprite* s)
{
    blockSprite[y][x] = s;
}

void SceneIngame::clearBlockSprite()
{

    for (int i = 0; i < BLOCK_HORIZONTAL; i++)
    {
        for (int k = 0; k < BLOCK_VERTICAL; k++)
        {
            auto s = getBlockSprite(i, k);
            if (s == nullptr)
                continue;

            s->removeFromParent();
            setBlockSprite(i, k, nullptr);
        }
    }
    
}

void SceneIngame::destroyBlock(int x, int y)
{
    if (blockData[y][x] == 0)
        return;

    startBlockMoving();

    blockSprite[y][x]->runAction(
        Sequence::create(
            FadeOut::create(0.125f),
            FadeIn::create(0.125f),
            FadeOut::create(0.125f),
            FadeIn::create(0.125f),
            Spawn::create(ScaleTo::create(0.125f, 0.0), FadeOut::create(0.125f), nullptr),
            RemoveSelf::create(),
            nullptr
        )
    );

    blockSprite[y][x] = nullptr;
    blockData[y][x] = 0;

    this->runAction(
        Sequence::create(
            DelayTime::create(0.625f),
            CallFunc::create([=]() {dropBlocks(x); }),
            nullptr
        )
    );
}

//���� ��ǥ��: �ػ�
//��� ��ǥ��: ���Ƿ� ������ 2���� ��Ϲ迭�� �ε��� ������ ����
//cocos2d-x���� sprite�� �⺻ pivot(����)���� ��� ����

//blockSprite�� 2���� �迭 �ε��� ���� ��ǥ�� ����ϰ��� �Ѵ�. 
//������ �� ��ǥ�� ���Ƿ� ������ ��ǥ���� ���� ȭ�鿡 �׸��µ� ����ϴ� ��ǥ�� �ƴϹǷ� ������ ������ �ؾ��Ѵ�.

Vec2 SceneIngame::convertGameCoordToBlockCoord(Vec2 gameCoord)
{
    //blockOrigin: ��� ������ ���ϴ� �� + ���� ��� ��� �� = ��� �������� ���ϴ� ����� pivot ��
    Vec2 blockOrigin = BLOCK_OFFSET 
        - Vec2((BLOCK_HORIZONTAL * BLOCK_WIDTH) / 2, (BLOCK_VERTICAL * BLOCK_HEIGHT) / 2)
        + Vec2(BLOCK_WIDTH, BLOCK_HEIGHT) / 2;

    Vec2 delta = gameCoord - blockOrigin;

    //origin���� ��� ���ε� �� ���� ���ϴ� ������ ��ǥ�� �̺�Ʈ�� ���´ٸ�?
    Vec2 pos = Vec2((int)(delta.x / BLOCK_WIDTH + 0.5), (int)(delta.y / BLOCK_HEIGHT + 0.5)); // + 0.5�� �ܼ��� int�ڷ������� �ݿø��� ������ ��

    return pos;
}

Vec2 SceneIngame::convertBlockCoordToGameCoord(Vec2 blockCoord)
{
    Vec2 blockOrigin = BLOCK_OFFSET
        - Vec2((BLOCK_HORIZONTAL * BLOCK_WIDTH) / 2, (BLOCK_VERTICAL * BLOCK_HEIGHT) / 2)
        + Vec2(BLOCK_WIDTH, BLOCK_HEIGHT) / 2;
    return blockOrigin + Vec2(BLOCK_WIDTH * blockCoord.x, BLOCK_HEIGHT * blockCoord.y);
}

void SceneIngame::clearCheckedList()
{
    for (int i = 0; i < BLOCK_HORIZONTAL; i++)
    {
        for (int j = 0; j < BLOCK_VERTICAL; j++)
        {
            blockCheckedList[j][i] = 0;
        }
    }
}

void SceneIngame::checkCheckedList(int x, int y)
{
    blockCheckedList[y][x] = 1;
}

int SceneIngame::getCheckedList(int x, int y)
{
    return blockCheckedList[y][x];
}

int SceneIngame::isMatch3()
{
    int count = 0;
    
    for (int x = 0; x < BLOCK_HORIZONTAL; x++)
    {
        count = 0;

        for (int y = 0; y < BLOCK_VERTICAL; y++)
        {
            if (getCheckedList(x, y) != 0)
                count++;
        }

        if (count >= 3)
            return 1;
    }

    for (int y = 0; y < BLOCK_VERTICAL; y++)
    {
        count = 0;

        for (int x = 0; x < BLOCK_HORIZONTAL; x++)
        {
            if (getCheckedList(x, y) != 0)
                count++;
        }

        if (count >= 3)
            return 1;
    }
    return 0;
}

void SceneIngame::destroyBlocksForCheckedList()
{
    for (int x = 0; x < BLOCK_HORIZONTAL; x++)
    {
        for (int y = 0; y < BLOCK_VERTICAL; y++)
        {
            if (getCheckedList(x, y) != 0)
                destroyBlock(x, y);
        }
    }
}

void SceneIngame::fullFillEmptyBlocks()
{
    for (int x = 0; x < BLOCK_HORIZONTAL; x++)
    {
        for (int y = 0; y < BLOCK_VERTICAL; y++)
        {
            if (getBlockData(x, y) == 0)
                createBlockRand(x, y);
        }
    }
}

bool SceneIngame::isLastCallback()
{
    if (callbackCount == 0) return true;
    return false;
}

void SceneIngame::increaseCallbackCount()
{
    callbackCount++;
}

void SceneIngame::decreaseCallbackCount()
{
    callbackCount--;
}
