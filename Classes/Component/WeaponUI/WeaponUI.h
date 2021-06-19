/**
* @file WeaponUI.h
*/

#ifndef __WEAPONUI_H__
#define __WEAPONUI_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Character/Character.h"
#include "Character/Player.h"



class WeaponUI : public cocos2d::Node
{
public:

	static WeaponUI* create(Player* player);

	void addButtonFunc();

	void damageReactAnime();

	void update(float dt);

protected:

	Player* player;

	int bulletInMagazine_ = 0;
	int bulletInStock_ = 0;

	cocos2d::Sprite* weaponIcon_;

	cocos2d::Sprite* reloadWarning1_;
	cocos2d::Sprite* reloadWarning2_;

	cocos2d::Sprite* damageReact_;

	cocos2d::Label* bulletInfo_;
	cocos2d::Label* bulletStockInfo;

	////////
	cocos2d::ui::Button* ReloadBut;
	cocos2d::ui::Button* SwitchBut;
	cocos2d::ui::Button* HealBut;
	cocos2d::ui::Button* InteractBut;
};


#endif 