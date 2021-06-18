/**
* @file SETimer.h
*/

#ifndef __SETIMER_H__
#define __SETIMER_H__

#include "cocos2d.h"

class SETimer : public cocos2d::Node
{
public:

	static SETimer* create();


	void update(float dt);


	static float getCurrentTime();

protected:

	float TimePassed = 0;

	cocos2d::Sprite* timeInfoBG_;
	cocos2d::Label* timeInfo_;
	static SETimer* timerInstance_;

};

#endif // !__SETIMER_H__
