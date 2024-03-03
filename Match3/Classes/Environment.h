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

#define BLOCK_HORIZONTAL 7 //���η� 7ĭ
#define BLOCK_VERTICAL 9 //���η� 9ĭ
#define BLOCK_OFFSET Vec2(SCREEN_WIDTH/2, SCREEN_HEIGHT/2) //���� ���߾�
#define BLOCK_WIDTH 80 //��� �ʺ�� �ȼ�
#define BLOCK_HEIGHT 80 //��� ���̴� �ȼ�
#define BLOCK_VAR 4 // ��� �ּ� ����

#define FONT_NAME "fonts/SDSamliphopangcheTTFBasic.ttf"

#define WEIGHT_SCORE 10 // ���ھ� ����ġ

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
