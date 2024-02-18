#ifndef __LAYER_INGAME_UI_H__
#define __LAYER_INGAME_UI_H__

#include "stdafx.h"
#include "Environment.h"

class LayerIngameUI : public Node {
public:
	Label* lbScore;
	Button* btnPause;
	Scale9Sprite* pausePanel;
	Button* btnResume;
	Button* btnRestart;
	Button* btnHome;

	static LayerIngameUI* create();
	virtual bool init() override;
	 
	void setScore(long long score);
	long long getScore();

	void showPausePanel();
	void hidePausePanel();


private:
	long long score;
	char scoreString[128];

};

#endif // !__LAYER_INGAME_UI_H__
