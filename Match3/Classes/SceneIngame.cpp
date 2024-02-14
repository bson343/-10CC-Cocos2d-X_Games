
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

    srand(time(0));

    Director::getInstance()->getTextureCache()->addImage("res/match3_tiles_px.png");
    
    //onEnter()ó�� ���������� �Լ��� ȣ��Ǵ� ���� �ƴ�, std::bind()�� ���� �ݹ��Լ��� ����ϴ� ���, �������̵� ó�� �� �Լ��� ���� ��ų �ʿ�� ����
    auto touch = EventListenerTouchOneByOne::create();
    touch->onTouchBegan = std::bind(&SceneIngame::onTouchBeganTest, this, std::placeholders::_1, std::placeholders::_2);
    touch->onTouchMoved = std::bind(&SceneIngame::onTouchMoved, this, std::placeholders::_1, std::placeholders::_2);
    touch->onTouchEnded = std::bind(&SceneIngame::onTouchEnded, this, std::placeholders::_1, std::placeholders::_2);
    touch->onTouchCancelled = touch->onTouchEnded;

    getEventDispatcher()->addEventListenerWithSceneGraphPriority(touch, this);

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
    Vec2 p = convertGameCoordToBlockCoord(t->getLocation());

    CCLOG("%f, %f", p.x, p.y);
    destroyBlock(p.x, p.y);
    dropBlocks(p.x);
    return true;
}

void SceneIngame::onTouchMoved(Touch* t, Event* e)
{
}

void SceneIngame::onTouchEnded(Touch* t, Event* e)
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
        }
    }
    alignBlockSprite();
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

Sprite* SceneIngame::getBlockSprite(int x, int y)
{
    return blockSprite[y][x];
}

void SceneIngame::setBlockSprite(int x, int y, Sprite* s)
{
    blockSprite[y][x] = s;
}

void SceneIngame::destroyBlock(int x, int y)
{
    if (blockData[y][x] == 0)
        return;

    blockSprite[y][x]->removeFromParent();
    blockSprite[y][x] = nullptr;
    blockData[y][x] = 0;
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
