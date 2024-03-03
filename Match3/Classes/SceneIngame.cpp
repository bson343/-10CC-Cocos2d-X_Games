#include "stdafx.h"
#include "SceneHome.h"
#include "SceneIngame.h"
#include "Environment.h"


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
    
    //onEnter()처럼 다형성으로 함수가 호출되는 것이 아닌, std::bind()를 통해 콜백함수를 등록하는 방식, 오버라이드 처럼 꼭 함수명 통일 시킬 필요는 없음
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
    addChild(ui = LayerIngameUI::create());
    ui->setLocalZOrder(1);

    ui->btnPause->addClickEventListener([=](Ref* r) {
        if (state == GameState::PLAYING)
        {
            ui->showPausePanel();
            state = GameState::PAUSE;
        }
    });

    ui->btnResume->addClickEventListener([=](Ref* r) {
        if (state == GameState::PAUSE)
        {
            ui->hidePausePanel();
            state = GameState::PLAYING;
        }
    });

    ui->btnRestart->addClickEventListener([=](Ref* r) {
        if (state == GameState::PAUSE)
        {
            ui->hidePausePanel();
            ui->setScore(0);
            this->destroyGame();
            this->initGame();
            this->startGame();
            state = GameState::PLAYING;
        }
    });

    ui->btnHome->addClickEventListener([=](Ref* r) {
        if (state == GameState::PAUSE)
        {
            auto scene = SceneHome::create();
            auto transit = TransitionSlideInL::create(0.125f, scene);
            Director::getInstance()->replaceScene(transit);
            state = GameState::PLAYING;
        }
    });
}

void SceneIngame::initGame()
{
    initGameField();

    Global::getInstance()->setScore(0);
    ui->setScore(Global::getInstance()->getScore());

    for (int i = 0; i < BLOCK_HORIZONTAL; i++)
    {
        for (int k = 0; k < BLOCK_VERTICAL; k++)
        {
            createBlock(i, k, rand() % BLOCK_VAR + 1);
        }
    }
    this->alignBlockSprite();
}

void SceneIngame::destroyUI()
{
}

void SceneIngame::destroyGame()
{
    for (int i = 0; i < BLOCK_HORIZONTAL; i++)
    {
        for (int j = 0; j < BLOCK_VERTICAL; j++)
        {
            setBlockData(i, j, 0);
            getBlockSprite(i, j)->removeFromParent();
            setBlockSprite(i, j, nullptr);
        }
    }
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

    if (!(p.x >= 0 && p.x < BLOCK_HORIZONTAL) || !(p.y >= 0 && p.y < BLOCK_VERTICAL))
    {
        return true;
    }

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
    //CCLOG("%d", callbackCount);
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
            createBlockRand(x, empty_y);
            blockSprite[empty_y][x]->setPosition(convertBlockCoordToGameCoord(Vec2(x, BLOCK_VERTICAL + 1)));
            blockSprite[empty_y][x]->runAction(
                MoveTo::create(0.125f, convertBlockCoordToGameCoord(Vec2(x, empty_y)))
            );
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

    if (!isMatch3())
    {
        clearCheckedList();

        tryEndBlockMoving();

        return;
    }

    CCLOG("test score: %d", matchCount * WEIGHT_SCORE);
    Global::getInstance()->addScore(matchCount * WEIGHT_SCORE);
    ui->setScore(Global::getInstance()->getScore());

    destroyBlocksForCheckedList();
}


int SceneIngame::checkSameBlock(int x, int y, int blockType)
{
    if (blockType == 0) return 0;

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
    spr->setScale(2);
    addChild(spr);
    setBlockData(x, y, type);
    setBlockSprite(x, y, spr);
}

void SceneIngame::createBlockRand(int x, int y)
{
    createBlock(x, y, rand() % BLOCK_VAR + 1);
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

//게임 좌표계: 해상도
//블록 좌표계: 임의로 지정한 2차원 블록배열의 인덱스 순서쌍 모음
//cocos2d-x에서 sprite의 기본 pivot(기준)값은 가운데 값임

//blockSprite의 2차원 배열 인덱스 값을 좌표로 사용하고자 한다. 
//하지만 그 좌표는 임의로 지정한 좌표이지 실제 화면에 그리는데 사용하는 좌표가 아니므로 적절히 변경을 해야한다.

Vec2 SceneIngame::convertGameCoordToBlockCoord(Vec2 gameCoord)
{
    //blockOrigin: 블록 공간의 좌하단 값 + 단일 블록 가운데 값 = 블록 공간에서 좌하단 블록의 pivot 값
    Vec2 blockOrigin = BLOCK_OFFSET 
        - Vec2((BLOCK_HORIZONTAL * BLOCK_WIDTH) / 2, (BLOCK_VERTICAL * BLOCK_HEIGHT) / 2)
        + Vec2(BLOCK_WIDTH, BLOCK_HEIGHT) / 2;

    Vec2 delta = gameCoord - blockOrigin;

    //origin값이 가운데 값인데 그 값의 좌하단 영역의 좌표로 이벤트가 들어온다면?
    Vec2 pos = Vec2((int)(delta.x / BLOCK_WIDTH + 0.5), (int)(delta.y / BLOCK_HEIGHT + 0.5)); // + 0.5는 단순히 int자료형에서 반올림을 구현한 것

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

bool SceneIngame::isMatch3()
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
            return true;
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
            return true;
    }
    return false;
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

void SceneIngame::initGameField()
{
    callbackCount = 0;
    state = GameState::PLAYING;

    clearCheckedList();
    clearBlockSprite();
    clearBlockData();
}

