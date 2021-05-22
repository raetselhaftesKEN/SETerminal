/**
* @file HelloWorldScene.cpp
*/

#include "HelloWorldScene.h"
#include "GameOverScene.h"
#include "./Character/Player.h"
#include "./Item/Bullet.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool HelloWorld::init()
{
    //���ø����ʼ��
    if (!Scene::init())
    {
        return false;
    }
    if (!Scene::initWithPhysics())
    {
        return false;
    }

    //��ȡ���ڴ�С��ԭ������
    auto winSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    //���ƻ�ɫ����
    auto background = DrawNode::create();
    background->drawSolidRect(origin, winSize, cocos2d::Color4F(0.6, 0.6, 0.6, 1));
    this->addChild(background);

    //������ҽ�ɫʵ��
    player_ = Player::create("player.png");
    this->addChild(player_);

    //����addMonster���������λ�����ɹ���
    srand((unsigned int)time(nullptr));
    this->schedule(CC_SCHEDULE_SELECTOR(HelloWorld::addMonster), 1.5);

    //������Ļ����������굥�����¼��ļ�����
    auto touchListener = cocos2d::EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, player_);

    //���ɳ�����������ײ�¼��ļ�����
    auto contactListener = cocos2d::EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(HelloWorld::onContactBegan, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);

    //���ɼ��̲����¼��ļ�����
    auto keyboardListener = cocos2d::EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = CC_CALLBACK_2(Player::listenToKeyPress, player_);
    keyboardListener->onKeyReleased = CC_CALLBACK_2(Player::listenToKeyRelease, player_);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardListener, this);

    return true;
}

void HelloWorld::addMonster(float dt)
{
    //���ɹ���ʵ��
    auto monster = Sprite::create("monster.png");
    if (monster == nullptr)
    {
        problemLoading("monster.png");
    }
    else {
        //�������Ҳ����λ�ó��֣�����������ɺͷ����ӵ��ĺϷ����귶Χ
        auto minY = monster->getContentSize().height / 2;
        auto maxY = this->getContentSize().height - minY;
        auto rangeY = maxY - minY;
        int randomY = (rand() % static_cast<int>(rangeY)) + minY;
        int maxX = this->getContentSize().width;
        int randomX = maxX - (rand() % (int)(maxX / 2));
        float monsterSpeed = 160;
        float randomDuration2 = (float)randomX / monsterSpeed;
        float randomDuration1 = (float)(maxX - randomX) / monsterSpeed;
        //���ù�����������
        monster->setPosition(this->getContentSize().width + monster->getContentSize().width / 2, randomY);
        //move1��������Ҳ��ƶ����м����λ�ã�����ʱ��ΪrandomDuration1
        auto move1 = cocos2d::MoveTo::create(randomDuration1, cocos2d::Vec2(randomX, randomY));
        //move1��������м����λ���ƶ�����࣬����ʱ��ΪrandomDuration2
        auto move2 = cocos2d::MoveTo::create(randomDuration2, cocos2d::Vec2(-monster->getContentSize().width / 2, randomY));
        //actionRemove���ͷŹ������
        auto actionRemove = RemoveSelf::create();
        //���ù�����������
        auto physicsBody = PhysicsBody::createBox(monster->getContentSize(), cocos2d::PhysicsMaterial(0.0f, 0.0f, 0.0f));
        physicsBody->setDynamic(false);
        physicsBody->setContactTestBitmask(1);
        physicsBody->setCategoryBitmask(3);
        monster->setPhysicsBody(physicsBody);
        monster->setTag(ENEMY);
        this->addChild(monster);

        //������move1��move2�м�����λ�÷����ӵ��Ķ�����ʹ��lambda����ʽʵ��
        auto shootStar = CallFunc::create([=]() {
            //���ɵ����ӵ�
            Sprite* enemyBullet = Sprite::create("dart_enemy.png");
            if (enemyBullet == nullptr)
            {
                problemLoading("dart_enemy.png");
            }
            else
            {
                enemyBullet->setPosition(monster->getPosition());
                //���õз��ӵ�����������
                auto physicsBody = PhysicsBody::createBox(enemyBullet->getContentSize(), PhysicsMaterial(0.0f, 0.0f, 0.0f));
                physicsBody->setDynamic(false);
                physicsBody->setCategoryBitmask(3);
                physicsBody->setContactTestBitmask(4);
                enemyBullet->setPhysicsBody(physicsBody);
                enemyBullet->setTag(ENEMY_BULLET);
                this->addChild(enemyBullet);
                //Ϊ�з��ӵ��󶨷��䶯��
                float starSpeed = 120;
                float maxX = this->getContentSize().width;
                float starDuration = (float)randomX / starSpeed;
                auto eDartMove = cocos2d::MoveTo::create(starDuration, player_->getPosition());
                auto eDartRemove = cocos2d::RemoveSelf::create();
                enemyBullet->runAction(cocos2d::Sequence::create(eDartMove, eDartRemove, nullptr));
            }
            });
        //���﷢���ӵ�ʱ��΢ͣ��
        auto delay = cocos2d::DelayTime::create(0.05);
        monster->runAction(Sequence::create(move1, delay, shootStar, move2, actionRemove, nullptr));
    }
}

bool HelloWorld::onTouchBegan(Touch* touch, Event* unusedEvent)
{
    //ȡ�õ����Ļλ�õ�����
    auto touchLocation = touch->getLocation();
    auto offset = touchLocation - player_->getPosition();
    //ȡ���ӵ����䷽��ĵ�λ����
    offset.normalize();
    //�ڳ����ڴ����ӵ�ʵ��
    auto bullet = Bullet::create(player_->getBulletName());
    bullet->setPosition(player_->getPosition());
    this->addChild(bullet, 1);
    //Ϊ�ӵ�ʵ���󶨲��ŷ���ķ��ж���
    bullet->shoot(offset);

    return true;
}

bool HelloWorld::onContactBegan(cocos2d::PhysicsContact& physicsContact)
{
    auto nodeA = physicsContact.getShapeA()->getBody()->getNode();
    auto nodeB = physicsContact.getShapeB()->getBody()->getNode();

    if (nodeA && nodeB)
    {
        auto tagA = nodeA->getTag();
        auto tagB = nodeB->getTag();

        //��һ�ɱ����������ӵ�
        if (tagA == ME_BULLET)
        {
            nodeB->removeFromParentAndCleanup(true);
        }
        else if (tagB == ME_BULLET)
        {
            nodeA->removeFromParentAndCleanup(true);
        }

        //��ұ���ɱ
        if (tagA == ME || tagB == ME)
        {
            //�滻��Gameover����
            Director::getInstance()->replaceScene(TransitionSlideInT::create(0.2f, GameOver::create()));
        }
    }

    return true;
}