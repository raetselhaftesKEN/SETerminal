#ifndef  _END_LAYER_H_
#define  _END_LAYER_H_

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "AudioEngine.h"
#include "Character/Player.h"
#include "Const/Const.h"
#include "../MenuLayer.h"



class EndLayer : public MenuLayer 
{
public:
	static EndLayer* create();

	//void initMusic();

	virtual bool init();

	virtual bool open();

	//bool close();

	//cocos2d::ui::Button* settingSmallButton(float deviationX, float deviationY, std::string spriteText, std::string texts);

	/*bool isOpen = false;

	int backgroundMusicID_;
	bool isBackgroundMusicPlaying_ = true;*/


protected:
	

	/*cocos2d::ui::Scale9Sprite* pauseBoardImg_;
	
	cocos2d::ui::Button* closeButton_;
	cocos2d::ui::Button* musicButton_;
	cocos2d::Label* musicLabel_;
	cocos2d::ui::Button* shortMusicButton_;
	cocos2d::ui::Button* superBodyButton_;
	cocos2d::Label* superBodyLabel_;
	cocos2d::ui::Button* superAccuracyButton_;
	cocos2d::Label* superAccuracyLabel_;
	cocos2d::ui::Button* superBulletButton_;
	cocos2d::Label* superBulletLabel_;
	cocos2d::ui::Button* superDamageButton_;
	cocos2d::Label* superDamageLabel_;*/
};



#endif