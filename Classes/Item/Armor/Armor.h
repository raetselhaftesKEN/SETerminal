/**
* @file Armor.h
*/

#ifndef __ARMOR_H__
#define __ARMOR_H__

#include "cocos2d.h"
#include "Item/Item.h"
#include "Const/Const.h"

class Armor : public Item
{
public:

	static Armor* create(int amount = 30);


	virtual void interact();


	virtual bool bindPhysicsBody();

protected:

	int armorAmount = 30;

};

#endif // !__ARMOR_H__

