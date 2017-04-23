#include "StatusLayer.h"
#include "Global.h"
#include "ScoreInfo.h"

StatusLayer::StatusLayer()
{

}

StatusLayer::~StatusLayer(){
	_eventDispatcher->removeAllEventListeners();
}

bool StatusLayer::init()
{
	if (!Layer::init())
		return false;

	m_score = 0;

	auto winSize = Director::getInstance()->getWinSize();
	m_scoreLabel = LabelAtlas::create();
	m_scoreLabel->initWithString("0", "num.png", 28, 39, 48);
	m_scoreLabel->setPosition(winSize.width / 2, winSize.height - 60);
	m_scoreLabel->setAnchorPoint(Vec2(0.5, 0.5));
	addChild(m_scoreLabel);
	char buf[20];
	sprintf(buf, "最高分：%d", 0);
	m_highScoreLabel = LabelTTF::create(FontToUTF8(buf), "SimHei", 30);
	m_highScoreLabel->setAnchorPoint(Vec2(1, 0.5));
	m_highScoreLabel->setPosition(winSize.width - 40, winSize.height - 40);
	addChild(m_highScoreLabel);

	auto listener = EventListenerCustom::create(UPDATESCOR_EVENT, [&](EventCustom* event){
		ScoreInfo* info = static_cast<ScoreInfo*>(event->getUserData());
		updateScore(info);
	});
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	return true;
}

void StatusLayer::updateScore(ScoreInfo* info)
{
	// 更新分数
	unsigned int goal = info->score + m_score;
	int delv = info->score / 20;
	m_scoreLabel->setScale(0.7);
	if (delv > 0 || delv < 0)
	{
		plusplusScore(delv, goal);
	}

	// 显示语句
	auto winSize = Director::getInstance()->getWinSize();
	switch (info->cleanCountOnce)
	{
	case 0:break;
	case 1:break;
	case 2:
	{
		auto sprite = Sprite::create("jixunvli.png");
		auto showA = Sequence::create(Spawn::create(FadeIn::create(0.4), MoveTo::create(0.6, Vec2(winSize.width / 2, winSize.height / 2)), nullptr),
			MoveTo::create(0.5, Vec2(winSize.width / 2, winSize.height / 5 * 4)),
			CallFunc::create(CC_CALLBACK_0(StatusLayer::actionCb, this, sprite)), nullptr);
		sprite->setPosition(Vec2(winSize.width / 2, 0 - 40));
		addChild(sprite);
		sprite->runAction(showA);
	}break;
	case 3:
	{
		auto sprite = Sprite::create("welldone.png");
		sprite->setPosition(Vec2(winSize.width / 2, 0 - 40));
		auto showA = Sequence::create(Spawn::create(FadeIn::create(0.4), MoveTo::create(0.6, Vec2(winSize.width / 2, winSize.height / 2)), nullptr),
			MoveTo::create(0.5, Vec2(winSize.width / 2, winSize.height / 5 * 4)),
			CallFunc::create(CC_CALLBACK_0(StatusLayer::actionCb, this, sprite)), nullptr);
		sprite->setPosition(Vec2(winSize.width / 2, 0 - 40));
		addChild(sprite);
		sprite->runAction(showA);
	}break;
	case 4:
	{
		auto sprite = Sprite::create("ku.png");
		auto showA = Sequence::create(Spawn::create(FadeIn::create(0.4), MoveTo::create(0.6, Vec2(winSize.width / 2, winSize.height / 2)), nullptr),
			MoveTo::create(0.5, Vec2(winSize.width / 2, winSize.height / 5 * 4)),
			CallFunc::create(CC_CALLBACK_0(StatusLayer::actionCb, this, sprite)), nullptr);
		sprite->setPosition(Vec2(winSize.width / 2, 0 - 40));
		sprite->setPosition(Vec2(winSize.width / 2, 0 - 40));
		addChild(sprite);
		sprite->runAction(showA);
	}break;
	default:
	{
		auto sprite = Sprite::create("waixingren.png");
		auto showA = Sequence::create(Spawn::create(FadeIn::create(0.4), MoveTo::create(0.6, Vec2(winSize.width / 2, winSize.height / 2)), nullptr),
			MoveTo::create(0.5, Vec2(winSize.width / 2, winSize.height / 5 * 4)),
			CallFunc::create([&](){
				this->removeChildByName("ispire");
			}), nullptr);
		sprite->setPosition(Vec2(winSize.width / 2, 0 - 40));
		sprite->setPosition(Vec2(winSize.width / 2, 0 - 40));
		sprite->setName("ispire");
		addChild(sprite);
		sprite->runAction(showA);
	}
	}
}

void StatusLayer::actionCb(Sprite* target)
{
	target->removeFromParent();
}

void StatusLayer::plusplusScore(int delv, int now)
{
	auto addNumA = Sequence::create(DelayTime::create(0.01f), CallFunc::create([=](){
		m_score += delv;
		if (delv > 0)
		{
			char buf[20];
			if (m_score >= now)
			{
				m_score = now;
				sprintf(buf, "%d", m_score);
				m_scoreLabel->setString(buf);
				m_scoreLabel->setScale(1);
				return;
			}
			else
			{
				sprintf(buf, "%d", m_score);
				m_scoreLabel->setString(buf);
				plusplusScore(delv, now);
			}
		}
		else
		{
			char buf[20];
			if (m_score <= now)
			{
				m_score = now;
				sprintf(buf, "%d", m_score);
				m_scoreLabel->setString(buf);
				m_scoreLabel->setScale(1);
				return;
			}
			else
			{
				sprintf(buf, "%d", m_score);
				m_scoreLabel->setString(buf);
				plusplusScore(delv, now);
			}
		}
	}), nullptr);
	runAction(addNumA);
}

int StatusLayer::getScore()
{
	return m_score;
}

char *StatusLayer::FontToUTF8(char* font){
	int len = MultiByteToWideChar(CP_ACP, 0, font, -1, NULL, 0);
	wchar_t *wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_ACP, 0, font, -1, wstr, len);
	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	memset(font, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, font, len, NULL, NULL);
	if (wstr)delete[] wstr;
	return font;
}