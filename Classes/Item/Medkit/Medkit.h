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


	virtual bool bindPhysicsBody();


	int getRecovery();


protected:

	int recovery_ = MEDKIT_DEFAULT_RECOVERABILITY;
	

};

#endif // !__MEDKIT_H__
