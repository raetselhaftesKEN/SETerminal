#ifndef  _SETTING_LAYER_H_
#define  _SETTING_LAYER_H_

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "AudioEngine.h"
#include "Character/Player.h"
#include "Const/Const.h"

class SettingLayer : public cocos2d::Layer 
{
public:
	static SettingLayer* create();

	void initMusic();

	bool init();

	bool open();



protected:
	int backgroundMusicID_;
	bool isBackgroundMusicPlaying_ = true;
	cocos2d::ui::Scale9Sprite* pauseBoardImg_;
	cocos2d::ui::Button* superBodyButton_;

};



#endif