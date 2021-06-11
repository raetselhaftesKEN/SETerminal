/**
* @file Bullet.h
*/

#ifndef __BULLET_H__
#define __BULLET_H__

#include "cocos2d.h"
#include "../Item.h"

/**
*@brief 子弹类
*/

class Bullet : public Item
{
public:

	virtual ~Bullet() = default;

	int getBulletAtk();

	//生成一个Bullet实例
	static Bullet* create(const std::string& filename);


	//为Bullet对象绑定物理躯干
	virtual bool bindPhysicsBody();


	//子弹死亡特效，比如粒子效果，爆炸
	void dieEffect();


	//为Bullet实例绑定发射动画
	bool shoot(const cocos2d::Vec2 shootDirection, float Speed);

	virtual void interact(){}

	


protected:

	float bulletSpeed_;									//子弹飞行速度

	float bulletRange_;									//子弹最大飞行距离

	float bulletAtk_ = 10.f;

	//待更新子弹攻击力等
};

#endif // !__BULLET_H__
