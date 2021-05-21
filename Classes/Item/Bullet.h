/**
* @file Bullet.h
*/

#ifndef __BULLET_H__
#define __BULLET_H__

#include "cocos2d.h"

/**
*@brief 子弹类
*/

class Bullet : public cocos2d::Sprite
{
public:

	/**
* @brief 生成一个Bullet实例
* @param Bullet实例对应的素材文件名
* @return 指向该Bullet实例的指针
*/
	static Bullet* create(const std::string& filename);


	/**
* @brief 为Bullet实例绑定发射动画
* @param 指向发射方向的单位向量
* @return 发射是否成功
*/
	bool shoot(const cocos2d::Vec2 shootDirection);

protected:

	cocos2d::Sprite* sprite_;

	float bulletSpeed_;									//子弹飞行速度

	float bulletRange_;									//子弹最大飞行距离

	//待更新子弹攻击力等
};

#endif // !__BULLET_H__
