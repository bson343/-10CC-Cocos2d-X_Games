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

#define BLOCK_HORIZONTAL 7 //가로로 7칸
#define BLOCK_VERTICAL 9 //세로로 9칸
#define BLOCK_OFFSET Vec2(360/2, 480/2) //기준 정중앙
#define BLOCK_WIDTH 40 //블록 너비는 40픽셀
#define BLOCK_HEIGHT 40 //블록 높이는 40픽셀
#define BLOCK_VAR 4 // 블록 애셋 갯수

#define SCREEN_WIDTH 360
#define SCREEN_HEIGHT 480

#endif // !__ENVIRONMENT_H__
