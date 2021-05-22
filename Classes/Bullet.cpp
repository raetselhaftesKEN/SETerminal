/**
* @file Bullet.cpp
*/

#include "cocos2d.h"
#include "Bullet.h"
#include "HelloWorldScene.h"

Bullet* Bullet::create(const std::string& filename)
{
    auto bullet = new(std::nothrow) Bullet();
    if (bullet == nullptr)
    {
        return nullptr;
    }
    auto picture = cocos2d::Sprite::create(filename);

    if (bullet && picture)
    {
        //设置子弹属性
        bullet->bulletSpeed_ = 2.0f;
        bullet->bulletRange_ = 500.0f;
        //设置子弹贴图
        bullet->addChild(picture);
        //设置子弹物理躯干
        auto physicsBody = cocos2d::PhysicsBody::createBox(bullet->getContentSize(), cocos2d::PhysicsMaterial(0.0f, 0.0f, 0.0f));
        physicsBody->setDynamic(false);
        physicsBody->setContactTestBitmask(2);
        physicsBody->setCategoryBitmask(5);
        bullet->setPhysicsBody(physicsBody);
        //使用tag标记我方子弹
        bullet->setTag(ME_BULLET);

        return bullet;
    }

    return nullptr;
}

bool Bullet::shoot(const cocos2d::Vec2 shootDirection)
{
    //生成子弹飞行运动动画
    auto actionMove = cocos2d::MoveTo::create(bulletSpeed_, getPosition() + shootDirection * bulletRange_);
    //释放子弹
    auto actionRemove = cocos2d::RemoveSelf::create();
    //为子弹实例绑定飞行-释放的动画
    runAction(cocos2d::Sequence::create(actionMove, actionRemove, nullptr));

    return true;
}