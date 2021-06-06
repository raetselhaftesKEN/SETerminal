#include "cocos2d.h"
#include "CameraEffect.h"

CameraEffect* CameraEffect::create()
{
	auto camera = new(std::nothrow) CameraEffect();
	if (camera == nullptr)
	{
		return nullptr;
	}

	auto sizeOfWin = cocos2d::Director::getInstance()->getWinSize();
	auto newCamera = Camera::createOrthographic(sizeOfWin.width, sizeOfWin.height, 1, 1000);
	camera->addChild(newCamera);
	camera->CameraInstance = newCamera;
	newCamera->setPosition(cocos2d::Vec2(sizeOfWin.width / 2, sizeOfWin.height / 2));

	camera->retain();
	camera->autorelease();
	camera->schedule(CC_SCHEDULE_SELECTOR(CameraEffect::update), 0.1f);

	return camera;

}

void CameraEffect::LockPlayer(Player* player)
{
	TargetPlayer = player;
}

void CameraEffect::update(float dt)
{
	if (TargetPlayer != nullptr)
	{
		Target = TargetPlayer->getPosition();
		setPosition(Target);
	}
	
}