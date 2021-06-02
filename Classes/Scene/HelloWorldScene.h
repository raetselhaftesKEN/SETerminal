/**
* @file HelloWorldScene.h
*/

#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

/**
* @brief 标记角色，怪物和飞镖的tag
*/
#define ME 100
#define ENEMY 200
#define ME_BULLET 10
#define ENEMY_BULLET 20

#include "cocos2d.h"
#include "./Character/Player.h"
#include "Component/HealthBar/HealthBar.h"

class HelloWorld : public cocos2d::Scene
{
private:
    Player* player_;
    //地图类,对应相关的tmx文件
<<<<<<< Updated upstream
    cocos2d::TMXTiledMap* _tileMap;
    //地图中的一个层
    cocos2d::TMXLayer* _background;
=======
    HealthBar* healthBar_;
    cocos2d::TMXTiledMap* _tileMap;
    //地图中的一个层
    cocos2d::TMXLayer* _background;

    cocos2d::Vec2 TouchLocation;

    bool TouchHolding = false;
>>>>>>> Stashed changes
public:

    /**
* @brief 创建场景实例
* @return 指向生成的场景的指针
*/
    static cocos2d::Scene* createScene();


    /**
* @brief 初始化场景
* @return 初始化是否成功
*/
    virtual bool init();


    /**
* @brief 生成怪物
*/
    void addMonster(float dt);
    

    /**
* @brief 响应场景内的屏幕触摸（即鼠标单击）
*/
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* unusedEvent);

    /*
    触摸（鼠标单击）结束
    */
    bool onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* unusedEvent);


    void onMouseMove(cocos2d::EventMouse* mouse);


    void onMouseMove(cocos2d::EventMouse* mouse);

    /**
* @brief 响应场景内的物理碰撞，击中敌人后，将敌人删除
*/
    bool onContactBegan(cocos2d::PhysicsContact& physicsContact);


    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);


};

#endif // __HELLOWORLD_SCENE_H__
