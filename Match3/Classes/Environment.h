#ifndef __ENVIRONMENT_H__
#define __ENVIRONMENT_H__

#include "stdafx.h"

enum class GameState
{
	PLAYING,
	PAUSE,
	WIN,
	LOSE,
	BLOCK_MOVING
};

#define SCREEN_WIDTH 720
#define SCREEN_HEIGHT 1280

#define BLOCK_HORIZONTAL 7 //가로로 7칸
#define BLOCK_VERTICAL 9 //세로로 9칸
#define BLOCK_OFFSET Vec2(SCREEN_WIDTH/2, SCREEN_HEIGHT/2) //기준 정중앙
#define BLOCK_WIDTH 80 //블록 너비는 픽셀
#define BLOCK_HEIGHT 80 //블록 높이는 픽셀
#define BLOCK_VAR 4 // 블록 애셋 갯수

#define FONT_NAME "fonts/SDSamliphopangcheTTFBasic.ttf"

#define WEIGHT_SCORE 10 // 스코어 가중치

class Global {
public:
	static Global* getInstance();

	void setScore(long long value);
	long long getScore();
	void addScore(long long value);

private:
	Global() {};
	long long score = 0;
};

#endif // !__ENVIRONMENT_H__
