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

#define BLOCK_HORIZONTAL 7 //���η� 7ĭ
#define BLOCK_VERTICAL 9 //���η� 9ĭ
#define BLOCK_OFFSET Vec2(360/2, 480/2) //���� ���߾�
#define BLOCK_WIDTH 40 //��� �ʺ�� 40�ȼ�
#define BLOCK_HEIGHT 40 //��� ���̴� 40�ȼ�
#define BLOCK_VAR 4 // ��� �ּ� ����

#define SCREEN_WIDTH 360
#define SCREEN_HEIGHT 480

#endif // !__ENVIRONMENT_H__
