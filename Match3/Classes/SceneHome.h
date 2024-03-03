#ifndef __SCENE_HOME_H__
#define __SCENE_HOME_H__

#include "stdafx.h"

class SceneHome : public Scene {
public:
	static SceneHome* create();
	virtual bool init() override;

	Button* btnStart;
	Button* btnQuit;
};

#endif // !__SCENE_HOME_H__

