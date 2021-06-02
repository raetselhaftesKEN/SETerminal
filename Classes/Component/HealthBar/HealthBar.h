/**
* @file HealthBar.h
*/

#ifndef __HEALTH_BAR_H__
#define __HEALTH_BAR_H__

#include "cocos2d.h"
#include "Character/Player.h"

class HealthBar : public cocos2d::Node
{
public:


	static HealthBar* create(Player* player);


	void update(float dt);


protected:

	cocos2d::Sprite* sprite_;
	
	Player* player_;

	int curHealth_;

	cocos2d::ProgressTimer* health_;
	cocos2d::Label* healthInfo_;


};

#endif // !__HEALTH_BAR_H__
