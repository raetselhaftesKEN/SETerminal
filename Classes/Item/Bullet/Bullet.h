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


	/**
* @brief 生成一个Bullet实例
* @param Bullet实例对应的素材文件名
* @return 指向该Bullet实例的指针
* @author 孟宇
*/
	static Bullet* create(const std::string& filename);


	/**
* @brief 为Bullet对象绑定物理躯干
* @return 绑定是否成功
* @author 孟宇
*/
	virtual bool bindPhysicsBody();

	/**
* @brief 为Bullet实例绑定发射动画
* @param 指向发射方向的单位向量
* @return 发射是否成功
* @author 孟宇
*/
	bool shoot(const cocos2d::Vec2 shootDirection);

protected:

	float bulletSpeed_;									//子弹飞行速度

	float bulletRange_;									//子弹最大飞行距离

	//待更新子弹攻击力等
};

#endif // !__BULLET_H__
