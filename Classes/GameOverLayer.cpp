#include "GameOverLayer.h"
#include "GameScene.h"

bool GameOverLayer::init()
{
	if (!Layer::init())
		return false;

	//´´½¨´¥ÃşÊÂ¼ş¼àÌıÆ÷
	auto listener = EventListenerTouchOneByOne::create();
	//ÍÌÊÉ´¥Ãş
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [](Touch* touch, Event* event){return true; };
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	setOpacity(70);

	auto winSize = Director::getInstance()->getWinSize();
	
	auto bg = Scale9Sprite::create("dialog.png");
	bg->setCapInsets(Rect(12, 12, 81, 78));
	bg->setContentSize(Size(400, 300));
	bg->setPosition(winSize.width / 2, winSize.height / 2);
	addChild(bg);

	MenuItemFont::setFontName("fonts/Marker Felt.ttf");
	MenuItemFont::setFontSize(30);
	auto item0 = MenuItemFont::create("Continue", CC_CALLBACK_0(GameOverLayer::retryCb, this, 0));
	auto item1 = MenuItemFont::create("Try again", CC_CALLBACK_0(GameOverLayer::retryCb, this, 1));
	auto item2 = MenuItemFont::create("Exit", CC_CALLBACK_0(GameOverLayer::exitGameCb, this));
	item0->setColor(Color3B::RED);
	item1->setColor(Color3B::RED);
	item2->setColor(Color3B::RED);
	auto menu = Menu::create(item0, item1, item2, nullptr);
	menu->alignItemsVerticallyWithPadding(30);
	menu->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
	addChild(menu);

	return true;
}

void GameOverLayer::retryCb(int tag)
{
	GameScene* parent = (GameScene*)(this->getParent());
	if (this->getParent())
	{
		parent->replayGame(tag);
		parent->removeChild(this);
	}
}

void GameOverLayer::exitGameCb()
{
	Director::getInstance()->end();
}