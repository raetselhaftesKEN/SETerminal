/**
* @file FightScene.h
*/

#ifndef __FIGHT_SCENE_H__
#define __FIGHT_SCENE_H__

#include "cocos2d.h"
#include "Character/Player.h"

class FightScene : public cocos2d::Scene
{
public:

	




protected:

	Player* player_;

	cocos2d::TMXTiledMap* tileMap_;

	static cocos2d::Node* dropNode_;



};

#endif // !__FIGHT_SCENE_H__
