#pragma once
/**
* @file SurvivorCounter.h
*/

#ifndef __SURVIVOR_COUNTER_H__
#define __SURVIVOR_COUNTER_H__

#include "cocos2d.h"

class SurvivorCounter : public cocos2d::Node
{
public:


	static SurvivorCounter* create();


	void update(float dt);


protected:

	int RemainingSurvivorCount = 0;

	cocos2d::Sprite* surviverInfoBG_;
	cocos2d::Label* surviverInfo_;

	float rBarShrinkRate = 50;

	//	cocos2d::Label* healthInfo_;


};

#endif // !__SURVIVOR_COUNTER_H__
