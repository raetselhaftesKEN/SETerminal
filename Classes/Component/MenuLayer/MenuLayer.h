#ifndef  _MENU_LAYER_H_
#define  _MENU_LAYER_H_

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "AudioEngine.h"
#include "Character/Player.h"
#include "Const/Const.h"


class MenuLayer : public cocos2d::Layer 
{
public:
	//static MenuLayer* create();

	virtual bool init() = 0;

	virtual bool open();

	//bool close();

	cocos2d::ui::Button* settingSmallButton(float deviationX, float deviationY, std::string spriteText, std::string texts);

	bool isOpen = false;


protected:
	

	cocos2d::ui::Scale9Sprite* pauseBoardImg_;
	
	//cocos2d::ui::Button* closeButton_;
	
};



#endif