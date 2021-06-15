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

 	static HealthBar* create(Player* character);

	void update(float dt);


protected:

	cocos2d::Sprite* sprite_;

	Player* character_;

	int curHealth_;

	int surShield_;

	int medKitCount_ = 0;

	cocos2d::Sprite* medKit1_;

	cocos2d::Sprite* medKit2_;

	cocos2d::Sprite* medKit3_;

	cocos2d::ProgressTimer* health_;

	cocos2d::ProgressTimer* shield_;

	cocos2d::ProgressTimer* healthR_;

	cocos2d::ProgressTimer* shieldR_;

	float rBarShrinkRate = 50;

	float rBarShrinkBoost1 = 1;
	float rBarShrinkBoost2 = 1;

	//	cocos2d::Label* healthInfo_;


};

#endif // !__HEALTH_BAR_H__
