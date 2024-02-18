#include "stdafx.h"
#include "LayerIngameUI.h"


LayerIngameUI* LayerIngameUI::create()
{
	auto ret = new LayerIngameUI();
	if (ret && ret->init()) ret->autorelease();
	else CC_SAFE_DELETE(ret);
	return ret;
}

bool LayerIngameUI::init()
{
	if (!Node::init()) return false;

	addChild(lbScore = Label::createWithTTF("test", "fonts/SDSamliphopangcheTTFBasic.ttf", 48.0f));
	lbScore->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	lbScore->setPosition(Vec2(30, SCREEN_HEIGHT - 70));

	addChild(btnPause = Button::create("res/btn_pause_normal.png", "res/btn_pause_pressed.png", "res/btn_pause_disabled.png"));
	btnPause->setPosition(Vec2(SCREEN_WIDTH - 70, SCREEN_HEIGHT -70));


	return true;
}

void LayerIngameUI::setScore(long long score)
{
}

long long LayerIngameUI::getScore()
{
	return 0;
}

void LayerIngameUI::showPausePanel()
{
}

void LayerIngameUI::hidePausePanel()
{
}
