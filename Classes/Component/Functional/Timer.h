/**
* @file Timer.h
*/

#ifndef __TIMER_H__
#define __TIMER_H__

#include "cocos2d.h"

class Timer : public cocos2d::Node
{
public:


	static Timer* create();


	void update(float dt);


protected:

	float TimePassed = 0;

	cocos2d::Sprite* timeInfoBG_;
	cocos2d::Label* timeInfo_;

};

#endif // !__TIMER_H__
