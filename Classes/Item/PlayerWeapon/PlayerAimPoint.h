/**
* @file PlayerAimPoint.h
*/

#ifndef __PLAYERAIMPOINT_H__
#define __PLAYERAIMPOINT_H__

#include "cocos2d.h"
#include "../Item.h"

/*
准星类
	@杨孟臻
	*/

class PlayerAimPoint : public Item
{
public:

	virtual ~PlayerAimPoint() = default;

	/*
	创建一个准星
	@杨孟臻
	*/
	static PlayerAimPoint* create(const std::string& filename);

	virtual bool bindPhysicsBody() { return true; }

	void SetTarget(cocos2d::Vec2 Target);

	void RecoverRecoil(float RecoverSpeed);

	float RecoilStatus = 0;

protected:

	cocos2d::Vec2 TargetPos = cocos2d::Vec2::ZERO;
};

#endif // !__PLAYERAIMPOINT_H__
