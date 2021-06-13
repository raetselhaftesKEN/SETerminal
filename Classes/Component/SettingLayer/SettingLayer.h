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
	
	cocos2d::ui::Button* closeButton_;
	cocos2d::ui::Button* musicButton_;
	cocos2d::Label* musicLabel_;
	cocos2d::ui::Button* superBodyButton_;
	cocos2d::Label* superBodyLabel_;
	cocos2d::ui::Button* superAccuracyButton_;
	cocos2d::Label* superAccuracyLabel_;

};



#endif