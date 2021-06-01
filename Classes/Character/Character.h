/**
* @file Character.h
*/

#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#include "cocos2d.h"

/**
*@brief 角色类，预计包含玩家和怪物
*/

class Character : public cocos2d::Sprite
{
public:

    virtual ~Character() = default;


    /**
*@brief 绑定贴图精灵
* @param 指向被绑定给角色对象的精灵
* @author 孟宇
*/
    void bindPictureSprite(cocos2d::Sprite* sprite);


	virtual void receiveDamage(int damage);


    /**
*@brief 死亡
* @author 孟宇
*/
    void die();


	//获取角色是否生存
	bool isAlive();


	void updateMoveAnimate();


	virtual void updateFacingStatus() = 0;


	virtual void updateWalkingStatus() = 0;


    /**
*@brief 绑定物理躯干
* @author 孟宇
*/
    virtual bool bindPhysicsBody() = 0;


    static cocos2d::Animate* createAnimate(const char* animateName, cocos2d::Size size, int frames = 4);


	void bindAnimate(const std::string& characterName);

protected:

	friend class HealthBar;

    cocos2d::Sprite* sprite_;

    int health_;

    float shield_;

    bool isAlive_ = true;
	
    float moveSpeed_;

	//角色朝向的点的坐标
	cocos2d::Vec2 facingPoint_;

	//移动状态和朝向状态
	enum class WalkingStatus { idle, walk };
	enum class FacingStatus { up, down, left, right };

	//移动和静止时的动画
	cocos2d::Animate* walkUp_;
	cocos2d::Animate* walkDown_;
	cocos2d::Animate* walkLeft_;
	cocos2d::Animate* walkRight_;
	cocos2d::Animate* idleUp_;
	cocos2d::Animate* idleDown_;
	cocos2d::Animate* idleLeft_;
	cocos2d::Animate* idleRight_;
	cocos2d::Animate* animateBeingPlayed_;

	//当前帧状态和上一帧状态
	WalkingStatus curWalkingStatus_ = WalkingStatus::idle;
	WalkingStatus preWalkingStatus_ = WalkingStatus::idle;
	FacingStatus curFacingStatus_ = FacingStatus::down;
	FacingStatus preFacingStatus_ = FacingStatus::down;

	//状态是否发生改变
	bool statusChanged_ = false;

};

#endif // !__CHARACTER_H__
