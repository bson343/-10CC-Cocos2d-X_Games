#ifndef __SCENE_INGAME_H__
#define __SCENE_INGAME_H__

#include "stdafx.h"

#include "Environment.h"

class SceneIngame : public Scene 
{
public:
	static SceneIngame* create();

	virtual bool init() override;
	virtual void onEnter() override;

	void initUI();
	void initGame();
	void destroyUI();
	void destroyGame();

	void alignBlockSprite();

	bool onTouchBeganTest(Touch* t, Event* e);
	void onTouchMoved(Touch* t, Event* e);
	void onTouchEnded(Touch* t, Event* e);

	virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

	void startGame();
	void pauseGame();
	void winGame();
	void loseGame();
	void restartGame();
	
	int findEmptyBlockYIndex(int x, int y);
	int findFilledBlockYIndex(int x, int y);
	void dropBlocks(int x);
	void dropBlocks();

	void confirmMatch3Over(int x, int y);

	int checkSameBlock(int x, int y, int blockType);
	int checkSameBlockRecursive(int x, int y, int blockType);

private:
	GameState state;
	int blockData[BLOCK_VERTICAL][BLOCK_HORIZONTAL];
	Sprite* blockSprite[BLOCK_VERTICAL][BLOCK_HORIZONTAL];

	int blockCheckedList[BLOCK_VERTICAL][BLOCK_HORIZONTAL];

	void createBlock(int x, int y, int type);
	void createBlockRand(int x, int y);

	int getBlockData(int x, int y);
	void setBlockData(int x, int y, int type);
	void clearBlockData();
	
	Sprite* getBlockSprite(int x, int y);
	void setBlockSprite(int x, int y, Sprite* s);
	void clearBlockSprite();
	
	void destroyBlock(int x, int y);

	Vec2 convertGameCoordToBlockCoord(Vec2 gameCoord);
	Vec2 convertBlockCoordToGameCoord(Vec2 blockCoord);

	void clearCheckedList();
	void checkCheckedList(int x, int y);
	int getCheckedList(int x, int y);
	int isMatch3();
	void destroyBlocksForCheckedList();

	void fullFillEmptyBlocks();
};
#endif // !__SCENE_INGAME_H__

