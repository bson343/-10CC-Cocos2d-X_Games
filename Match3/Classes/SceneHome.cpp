#include "stdafx.h"
#include "SceneHome.h"
#include "Environment.h"
#include "SceneIngame.h"

SceneHome* SceneHome::create()
{
	auto ret = new SceneHome();
	if (ret && ret->init()) ret->autorelease();
	else CC_SAFE_DELETE(ret);
	return ret;
}

bool SceneHome::init()
{
	if (!Scene::init()) return false;

	Sprite* background = Sprite::create("res/title_back.jpg");
	addChild(background);
	background->setOpacity(0);
	background->runAction(FadeIn::create(0.25));
	background->setPosition(Vec2(720 / 2, 700));
	background->setScale(2.0);

	Sprite* title = Sprite::create("res/title.png");
	addChild(title);
	title->setPosition(Vec2(720 / 2, -100));
	title->runAction(EaseSineOut::create(MoveTo::create(0.25f, Vec2(720 / 2, 600))));

	addChild(btnStart = Button::create("res/btn_normal.png", "res/btn_pressed.png", "res/btn_disabled.png"));
	addChild(btnQuit = Button::create("res/btn_normal.png", "res/btn_pressed.png", "res/btn_disabled.png"));

	btnStart->setPosition(Vec2(720 / 2, 400));
	btnQuit->setPosition(Vec2(720 / 2, 250));

	btnStart->setTitleFontName(FONT_NAME);
	btnQuit->setTitleFontName(FONT_NAME);

	btnStart->setTitleFontSize(40.0f);
	btnQuit->setTitleFontSize(40.0f);

	btnStart->setTitleColor(Color3B(0, 50, 50));
	btnQuit->setTitleColor(Color3B(0, 50, 50));

	btnStart->setTitleText("Start Game");
	btnQuit->setTitleText("Quit Game");

	btnStart->addClickEventListener([=](Ref* r) {
		auto scene = SceneIngame::create();
		auto transit = TransitionSlideInR::create(0.25, scene);
		Director::getInstance()->replaceScene(transit);
		});

	btnQuit->addClickEventListener([=](Ref*) {
		Director::getInstance()->end();
		});

	return true;
}
