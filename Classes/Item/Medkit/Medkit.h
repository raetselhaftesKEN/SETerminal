/**
* @file Medkit.h
*/

#ifndef __MEDKIT_H__
#define __MEDKIT_H__

#include "cocos2d.h"
#include "Item/Item.h"
#include "Const/Const.h"

class Medkit : public Item
{
public:


	static Medkit* create();

	static Medkit* create(cocos2d::Vec2 pos);

	virtual bool bindPhysicsBody();


	int getRecovery();


	virtual void interact();


protected:

	int recovery_ = MEDKIT_MAX_NUM;
	

};

#endif // !__MEDKIT_H__
