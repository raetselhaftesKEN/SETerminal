/**
* @file Weapon.h
*/

#ifndef __WEAPON_H__
#define __WEAPON_H__

#include "cocos2d.h"
#include "Item.h"

/**
*@brief 武器类
*/

class Weapon : public Item
{
public:

	virtual ~Weapon() = default;


	/**
* @brief 生成一个Weapon实例
* @param Weapon实例对应的素材文件名
* @return 指向该Weapon实例的指针
* @孟宇
*/
	static Weapon* create(const std::string& filename);


	/**
*@brief 绑定物理躯干
* @author 孟宇
*/
	virtual bool bindPhysicsBody() { return true; }

	
protected:


	//待更新射速，攻击方式等

};

#endif // !__WEAPON_H__
