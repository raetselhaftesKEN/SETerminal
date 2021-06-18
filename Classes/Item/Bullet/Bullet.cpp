/**
* @file Bullet.cpp
*/

#include "cocos2d.h"
#include "Bullet.h"
#include "Const/Const.h"

Bullet* Bullet::create(const std::string& filename)
{
    auto bullet = new(std::nothrow) Bullet();
    if (bullet == nullptr)
    {
        return nullptr;
    }
    bullet->bindPictureSprite(cocos2d::Sprite::create(filename));

    if (bullet && bullet->sprite_)
    {
        //设置子弹属性
        bullet->bulletSpeed_ = 600.0f;
        bullet->bulletRange_ = 1500.0f;
        //设置子弹物理躯干
        bullet->bindPhysicsBody();
        //使用tag标记我方子弹
        bullet->setTag(PLAYER_BULLET_TAG);
        bullet->setScale(1.5f, 1.5f);

        return bullet;
    }

    return nullptr;
}

bool Bullet::bindPhysicsBody()
{
    auto physicsBody = cocos2d::PhysicsBody::createBox(getContentSize(), cocos2d::PhysicsMaterial(0.0f, 0.0f, 0.0f));
    physicsBody->setDynamic(false);
    physicsBody->setContactTestBitmask(PLAYER_BULLET_CONTACT_MASK);
    physicsBody->setCategoryBitmask(PLAYER_BULLET_CATEGORY_MASK);
    setPhysicsBody(physicsBody);

    return true;
}

int Bullet::getBulletAtk()
{
    return bulletAtk_;
}

void Bullet::setBulletAtk(int atk)
{
    if (atk >= 0)
        bulletAtk_ = atk;
}

void Bullet::dieEffect()
{
}

bool Bullet::shoot(const cocos2d::Vec2 shootDirection, float Speed)
{
    //生成子弹飞行运动动画
    auto actionMove = cocos2d::MoveTo::create(bulletRange_ / Speed, getPosition() + shootDirection * bulletRange_);
    //释放子弹
    auto actionRemove = cocos2d::RemoveSelf::create();
    //为子弹实例绑定飞行-释放的动画
    auto end = cocos2d::CallFunc::create([=]() {dieEffect(); });
    runAction(cocos2d::Sequence::create(actionMove, actionRemove, nullptr));

    return true;
}
