/**
* @file Clip.h
*/

#ifndef __CLIP_H__
#define __CLIP_H__

#include "cocos2d.h"
#include "Item/Item.h"
#include "Const/Const.h"

class Clip : public Item
{
public:

	static Clip* create(bulletType_ type, int num = 30);


	virtual void interact();


	virtual bool bindPhysicsBody();


	bulletType_ getType();


protected:

	bulletType_ type_ = bulletType_::type556;
	int bulletNum_ = 90;

};

#endif // !__CLIP_H__

