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

//#define CREATE_FUNC(__TYPE__) \
//static __TYPE__* create() \
//{ \
//    __TYPE__ *pRet = new(std::nothrow) __TYPE__(); \
//    if (pRet && pRet->init()) \
//    { \
//        pRet->autorelease(); \
//        return pRet; \
//    } \
//    else \
//    { \
//        delete pRet; \
//        pRet = nullptr; \
//        return nullptr; \
//    } \
//}

	static FightScene* create(Player* player, cocos2d::TMXTiledMap tiledMap)




protected:

	Player* player_;

	cocos2d::TMXTiledMap* tileMap_;

	

	static cocos2d::Node* generatedNode_;

};

#endif // !__FIGHT_SCENE_H__
