/**
* @file Weapon.h
*/

#ifndef __WEAPON_H__
#define __WEAPON_H__

#include "cocos2d.h"

/**
*@brief 武器类
*/

class Weapon : public cocos2d::Sprite
{
public:

	/**
* @brief 生成一个Weapon实例
* @param Weapon实例对应的素材文件名
* @return 指向该Weapon实例的指针
*/
	static Weapon* create(const std::string& filename);

	
protected:

	//待更新射速，攻击方式等

};

#endif // !__WEAPON_H__
