/**
* @file WeaponUI.h
*/

#ifndef __WEAPONUI_H__
#define __WEAPONUI_H__

#include "cocos2d.h"
#include "Character/Character.h"
#include "Character/Player.h"


/*
Œ‰∆˜–≈œ¢UI
	@—Ó√œ’È
	*/


class WeaponUI : public cocos2d::Node
{
public:

	static WeaponUI* create(Player* player);


	void update(float dt);

protected:

	Player* player;

	int bulletInMagazine_ = 0;

	cocos2d::Label* bulletInfo_;
};


#endif 
#pragma once
