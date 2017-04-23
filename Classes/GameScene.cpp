#include "GameScene.h"
#include "BlockShape.h"
#include "GameOverLayer.h"
#include "ScoreInfo.h"


USING_NS_CC;

GameScene::GameScene()
{

}

GameScene::~GameScene()
{
	unscheduleUpdate();
}

Scene* GameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
	auto layer = GameScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	m_isGameOver = false;
	m_tmpBSL1 = -1;
	m_tmpBSR1 = -1;

	m_mapLayer = MapLayer::create();
	addChild(m_mapLayer);
	m_shapeLayer = ShapeLayer::create();
	addChild(m_shapeLayer);
	m_statusLayer = StatusLayer::create();
	addChild(m_statusLayer);

	// Ĭ�ϵ��������÷�����ÿ֡����֮ǰ���ᱻ����һ��
	scheduleUpdate();
	// �Զ����������ÿ��һ��ִ��һ��
	//schedule(schedule_selector(GameScene::customUpdate), 1.0f, kRepeatForever, 0);
    
    return true;
}

/*
* �ж��ڵ�ǰ����ͶӰ�µ�ͼ�Ƿ��ܹ����·���
* */
void GameScene::projection()
{
	auto mapbs = m_mapLayer->m_blocks;
	auto target = BlockShape::sTarget;
	Block** targetbs = target->m_blocks;
	assert(targetbs != NULL);
	auto mapv = m_mapLayer->m_Map;
	auto targetposw = mconvertToWorldPos(targetbs[0]->getPosition(), target);
	for (int i = 0; i < gMapLineM; ++i)
	{
		for (int j = 0; j < gMapRowM; ++j)
		{
			if (MAP_EMPTY != mapv[i][j])
			{
				if (MAP_FILL == mapv[i][j])	continue;
				if (i <= 4) break;
				else
				{
					j = i - 4;
					// ����һλ��continue������ִ�������ж�
					--j;continue;
				}
			}	
			Rect rect = Rect(0, 0, mapbs[i][j]->getContentSize().width, mapbs[i][j]->getContentSize().height);
			auto targetposl = mapbs[i][j]->convertToNodeSpace(targetposw);
			if (rect.containsPoint(targetposl))
			{
				if (m_tmpBSL1 >= 0 && m_tmpBSR1 >= 0)
					return;
				int bsl1 = i;
				int bsr1 = j;
				int bsl2 = bsl1 + targetbs[1]->m_lineI;
				int bsr2 = bsr1 + targetbs[1]->m_rowI;
				int bsl3 = bsl1 + targetbs[2]->m_lineI;
				int bsr3 = bsr1 + targetbs[2]->m_rowI;
				int bsl4 = bsl1 + targetbs[3]->m_lineI;
				int bsr4 = bsr1 + targetbs[3]->m_rowI;
				if (bsl2 >= gMapLineM || bsr2 >= gMapRowM || bsl3 >= gMapLineM ||
					bsr3 >= gMapRowM || bsl4 >= gMapLineM || bsr4 >= gMapRowM || 
					MAP_EMPTY != mapv[bsl2][bsr2] || MAP_EMPTY != mapv[bsl3][bsr3] ||
					MAP_EMPTY != mapv[bsl4][bsr4]) break;
				if (0 == BlockShape::sTarget->m_shape)
				{
					mapbs[bsl1][bsr1]->setSpriteColor(target->m_color);
					m_mapLayer->addShadowForBlock(mapbs[bsl1][bsr1]);
				}
				else
				{
					mapbs[bsl1][bsr1]->setSpriteColor(target->m_color);
					mapbs[bsl2][bsr2]->setSpriteColor(target->m_color);
					mapbs[bsl3][bsr3]->setSpriteColor(target->m_color);
					mapbs[bsl4][bsr4]->setSpriteColor(target->m_color);
					m_mapLayer->addShadowForBlock(mapbs[bsl1][bsr1]);
					m_mapLayer->addShadowForBlock(mapbs[bsl2][bsr2]);
					m_mapLayer->addShadowForBlock(mapbs[bsl3][bsr3]);
					m_mapLayer->addShadowForBlock(mapbs[bsl4][bsr4]);
				}
				m_tmpBSL1 = i;
				m_tmpBSR1 = j;
				// �Ѿ��ҵ�Ŀ�꣬����Ҫ����������
				return;
			}
		}
	}
}

/*
	���ͶӰ
*/
void GameScene::projectionC()
{
	auto mapbs = m_mapLayer->m_blocks;
	auto target = BlockShape::sTarget;
	Block** targetbs = target->m_blocks;
	assert(targetbs != NULL);
	if(m_tmpBSL1 >= 0 && m_tmpBSR1 >= 0)
	{
		auto targetposw = mconvertToWorldPos(targetbs[0]->getPosition(), target);
		Rect rect = Rect(0, 0, mapbs[m_tmpBSL1][m_tmpBSR1]->getContentSize().width, mapbs[m_tmpBSL1][m_tmpBSR1]->getContentSize().height);
		auto targetposl = mapbs[m_tmpBSL1][m_tmpBSR1]->convertToNodeSpace(targetposw);
		// ���shapeû�з����ƶ������������
		if (rect.containsPoint(targetposl))
			return;
		int bsl1 = m_tmpBSL1;
		int bsr1 = m_tmpBSR1;
		int bsl2 = bsl1 + targetbs[1]->m_lineI;
		int bsr2 = bsr1 + targetbs[1]->m_rowI;
		int bsl3 = bsl1 + targetbs[2]->m_lineI;
		int bsr3 = bsr1 + targetbs[2]->m_rowI;
		int bsl4 = bsl1 + targetbs[3]->m_lineI;
		int bsr4 = bsr1 + targetbs[3]->m_rowI;
		if (0 == BlockShape::sTarget->m_shape)
		{
			mapbs[bsl1][bsr1]->setSpriteColor(m_mapLayer->m_color);
			m_mapLayer->removeShadowForBlock(mapbs[bsl1][bsr1]);
		}
		else
		{
			mapbs[bsl1][bsr1]->setSpriteColor(m_mapLayer->m_color);
			mapbs[bsl2][bsr2]->setSpriteColor(m_mapLayer->m_color);
			mapbs[bsl3][bsr3]->setSpriteColor(m_mapLayer->m_color);
			mapbs[bsl4][bsr4]->setSpriteColor(m_mapLayer->m_color);
			m_mapLayer->removeShadowForBlock(mapbs[bsl1][bsr1]);
			m_mapLayer->removeShadowForBlock(mapbs[bsl2][bsr2]);
			m_mapLayer->removeShadowForBlock(mapbs[bsl3][bsr3]);
			m_mapLayer->removeShadowForBlock(mapbs[bsl4][bsr4]);
		}
		m_tmpBSL1 = -1;
		m_tmpBSR1 = -1;
		//�Ѿ����Ŀ�겻��Ҫ��������
		return;
	}
}

/*
	����ͼ������ӦͶӰ����TouchEndʱ��ȷ��ͶӰ��䡣
*/
void GameScene::projectioned()
{
	if (m_tmpBSL1 < 0 || m_tmpBSR1 < 0)
	{
		return;
	}
	auto target = BlockShape::sTarget;
	if (NULL == target) 
		return;
	auto mapbs = m_mapLayer->m_blocks;
	auto targetbs = target->m_blocks;
	assert(targetbs != NULL);
	auto mapv = m_mapLayer->m_Map;

	int bsl1 = m_tmpBSL1;
	int bsr1 = m_tmpBSR1;
	int bsl2 = bsl1 + targetbs[1]->m_lineI;
	int bsr2 = bsr1 + targetbs[1]->m_rowI;
	int bsl3 = bsl1 + targetbs[2]->m_lineI;
	int bsr3 = bsr1 + targetbs[2]->m_rowI;
	int bsl4 = bsl1 + targetbs[3]->m_lineI;
	int bsr4 = bsr1 + targetbs[3]->m_rowI;
	/*
		����ͼ���
	*/
	if (0 == BlockShape::sTarget->m_shape)
	{
		mapv[bsl1][bsr1] = MAP_FILL;
		m_mapLayer->removeShadowForBlock(mapbs[bsl1][bsr1]);
	}
	else
	{
		mapv[bsl1][bsr1] = MAP_FILL;
		mapv[bsl2][bsr2] = MAP_FILL;
		mapv[bsl3][bsr3] = MAP_FILL;
		mapv[bsl4][bsr4] = MAP_FILL;
		m_mapLayer->removeShadowForBlock(mapbs[bsl1][bsr1]);
		m_mapLayer->removeShadowForBlock(mapbs[bsl2][bsr2]);
		m_mapLayer->removeShadowForBlock(mapbs[bsl3][bsr3]);
		m_mapLayer->removeShadowForBlock(mapbs[bsl4][bsr4]);
	}
	m_tmpBSL1 = -1;
	m_tmpBSR1 = -1;

	auto curss = m_shapeLayer->m_currentShapes;
	// �ҳ�shapeλ����洢��������λ��
	int ti;
	for (int i = 0; i < 3; ++i)
	{
		if (target == curss[i])
		{
			ti = i; break;
		}
	}
	auto oriposX = target->m_oriPosX;
	auto oriposY = target->m_oriPosY;
	target->stopAllActions();
	target->setVisible(false);
	target->removeFromParent();
	BlockShape::sTarget = NULL;
	// ��������״
	m_shapeLayer->updateShape(ti, oriposX, oriposY);
	// ���·������������ж�
	fullLineClean();
	// �����ͼ����У���Ϸ�����ж�
	projectionCheck();
}

/*
	����ͼ�Ƿ���λ�ý���ͶӰ
*/
void GameScene::projectionCheck()
{
	auto mapbs = m_mapLayer->m_blocks;
	auto mapv = m_mapLayer->m_Map;

	for (int k = 0; k < 3; ++k){
		auto targetbs = m_shapeLayer->m_currentShapes[k]->m_blocks;
		for (int i = 0; i < gMapLineM; ++i){
			for (int j = 0; j < gMapRowM; ++j){
				if (MAP_EMPTY == mapv[i][j]) {
					int bsl1 = i;
					int bsr1 = j;
					int bsl2 = bsl1 + targetbs[1]->m_lineI;
					int bsr2 = bsr1 + targetbs[1]->m_rowI;
					int bsl3 = bsl1 + targetbs[2]->m_lineI;
					int bsr3 = bsr1 + targetbs[2]->m_rowI;
					int bsl4 = bsl1 + targetbs[3]->m_lineI;
					int bsr4 = bsr1 + targetbs[3]->m_rowI;

					bool bConditoion =
						bsl2 >= 0 && bsl2 < gMapLineM &&
						bsr2 >= 0 && bsr2 < gMapRowM &&
						bsl3 >= 0 && bsl3 < gMapLineM &&
						bsr3 >= 0 && bsr3 < gMapRowM &&
						bsl4 >= 0 && bsl4 < gMapRowM &&
						bsr4 >= 0 && bsr4 < gMapRowM &&
						mapv[bsl2][bsr2] == MAP_EMPTY &&
						mapv[bsl3][bsr3] == MAP_EMPTY &&
						mapv[bsl4][bsr4] == MAP_EMPTY;
					if (bConditoion){
						return;
					}
				}
			}
		}
	}
	//m_isGameOver = true;
	// ��Ϸ����
	auto gameOver = GameOverLayer::create();
	addChild(gameOver);
}

/*
	���д����Ƽ���js�汾�Ĵ���ʽ�����´���ʧȥ
	��ͳһ����ı����ԡ�(���綯����ʱ���������ٶȲ��ÿ���)
*/
void GameScene::fullLineClean()
{
	// ��¼������
	int fullLine = 0;
	//��¼�У��У����У��Ƿ�������
	int linef[9] = { 0 };
	int rowf[9] = { 0 };
	int rowff[9] = { 0 };
	auto mapv = m_mapLayer->m_Map;
	// ɨ����
	for (int i = 0; i < gMapLineM; ++i)
	{
		for (int j = 0; j < gMapRowM; ++j)
		{
			if (MAP_EMPTY == mapv[i][j]) break;
			if (gMapRowM - 1 == j)
			{
				linef[i] = 1;
				++fullLine;
			}
		}
	}
	// ɨ���� ������
	for (int i = 4; i >= 0; --i)
	{
		for (int j = i, k = 0; j <= 8, k <= 8 - i; ++j, ++k)
		{
			if (MAP_EMPTY == mapv[j][k]) break;
			if (8 == j)
			{
				rowf[i] = 1;
				++fullLine;
			}
		}
	}
	// ɨ���� ������
	for (int i = 1; i <= 4; ++i)
	{
		for (int j = i, k = 0; j <= 8, k <= 8 - i; ++j, ++k)
		{
			if (MAP_EMPTY == mapv[k][j]) break;
			if (8 == j)
			{
				rowf[4 + i] = 1;
				++fullLine;
			}
		}
	}
	// ɨ�跴��
	for (int i = 0; i <= 8; ++i)
	{
		int j = 0;
		int condition = 4 + i;
		if (i > 4)
		{
			j = i - 4;
			condition = 8;
		}
		for (j; j <= condition; ++j)
		{
			if (MAP_EMPTY == mapv[j][i]) break;
			if (j == condition)
			{
				rowff[i] = 1;
				++fullLine;
			}
		}
	}

	// ������
	auto mapbs = m_mapLayer->m_blocks;
	for(int i = 0; i <= 8; ++i)
	{
		if (0 == linef[i]) continue;
		for (int j = 0; j <= 8; ++j)
		{
			if (MAP_NOUSE == mapv[i][j]) continue;
			// ͳһ��������ɫ���ӳ�ִ����������
			mapbs[i][j]->setSpriteColor(Color3B(255, 255, 255));
			//��������
			auto delayTF = 0.001;
			auto actionTime = 0.1;
			auto fadeOutA = FadeOut::create(0.5);
			// �������ӳ�ʱ��
			auto delayT = DelayTime::create(actionTime * j + delayTF * (8 - j));
			auto mapColor = m_mapLayer->m_color;
			// ����lambda����հ�
			auto cleanAction = Sequence::create(delayT, fadeOutA, CallFunc::create([=](){
				mapbs[i][j]->setSpriteColor(mapColor);
			}), NULL);
			cleanAction->setTag(1);
			//��ִ�ж���֮ǰ��ֹͣ�䶯�����Ա���һ������ִ��ͬ���������
			mapbs[i][j]->stopActionByTag(1);
			mapbs[i][j]->runAction(cleanAction);
			mapv[i][j] = MAP_EMPTY;
		}
	}
	// ������ ������
	for (int i = 4; i >= 0; --i)
	{
		if (0 == rowf[i]) continue;
		for (int j = i, k = 0; j <= 8, k <= 8 - i; ++j, ++k)
		{
			// ͳһ��������ɫ���ӳ�ִ����������
			mapbs[j][k]->setSpriteColor(Color3B(255, 255, 255));
			//��������
			auto delayTF = 0.001;
			auto actionTime = 0.1;
			auto fadeOutA = FadeOut::create(0.5);
			//�������ӳ�ʱ��
			auto delayT = DelayTime::create(actionTime * (8 - i) + delayTF * k);
			auto mapColor = m_mapLayer->m_color;
			auto cleanAction = Sequence::create(delayT, fadeOutA, CallFunc::create([=](){
				mapbs[j][k]->setSpriteColor(mapColor);
			}), NULL);
			cleanAction->setTag(1);
			//��ִ�ж���֮ǰ��ֹͣ�䶯�����Ա���һ������ִ��ͬ���������
			mapbs[j][k]->stopActionByTag(1);
			mapbs[j][k]->runAction(cleanAction);
			mapv[j][k] = MAP_EMPTY;
		}
	}
	// ������ ������
	for (int i = 1; i <= 4; ++i)
	{
		if (0 == rowf[4 + i]) continue;
		for (int j = i, k = 0; j <= 8, k <= 8 - i; ++j, ++k)
		{
			// ͳһ��������ɫ���ӳ�ִ����������
			mapbs[k][j]->setSpriteColor(Color3B(255, 255, 255));
			//��������
			auto delayTF = 0.001;
			auto actionTime = 0.1;
			auto fadeOutA = FadeOut::create(0.5);
			//�������ӳ�ʱ��
			auto delayT = DelayTime::create(actionTime * (8 - i) + delayTF * k);
			auto mapColor = m_mapLayer->m_color;
			auto cleanAction = Sequence::create(delayT, fadeOutA, CallFunc::create([=](){
				mapbs[k][j]->setSpriteColor(mapColor);
			}), NULL);
			cleanAction->setTag(1);
			//��ִ�ж���֮ǰ��ֹͣ�䶯�����Ա���һ������ִ��ͬ���������
			mapbs[k][j]->stopActionByTag(1);
			mapbs[k][j]->runAction(cleanAction);
			mapv[k][j] = MAP_EMPTY;
		}
	}
	// ��������
	for (int i = 0; i <= 8; ++i)
	{
		if (0 == rowff[i]) continue;
		int j = 0;
		int condition = 4 + i;
		if (i > 4)
		{
			j = i - 4;
			condition = 8;
		}
		for (j; j <= condition; ++j)
		{
			// ͳһ��������ɫ���ӳ�ִ����������
			mapbs[j][i]->setSpriteColor(Color3B(255, 255, 255));
			//��������
			auto delayTF = 0.001;
			auto actionTime = 0.1;
			auto fadeOutA = FadeOut::create(0.5);
			//�������ӳ�ʱ��
			auto delayT = DelayTime::create(actionTime * 8 + delayTF * j);
			auto mapColor = m_mapLayer->m_color;
			auto cleanAction = Sequence::create(delayT, fadeOutA, CallFunc::create([=](){
				mapbs[j][i]->setSpriteColor(mapColor);
			}), NULL);
			cleanAction->setTag(1);
			//��ִ�ж���֮ǰ��ֹͣ�䶯�����Ա���һ������ִ��ͬ���������
			mapbs[j][i]->stopActionByTag(1);
			mapbs[j][i]->runAction(cleanAction);
			mapv[j][i] = MAP_EMPTY;
		}
	}
	// ���·���
	EventCustom event(UPDATESCOR_EVENT);
	ScoreInfo info;
	info.cleanCountOnce = fullLine;
	info.score = fullLine * 40 + 40;
	event.setUserData(&info);
	_eventDispatcher->dispatchEvent(&event);
}

/*
	����ĳ��node���child��������������λ��
	pos��Ҫת���ӽڵ�����꣬parrent���׽ڵ�
*/
Vec2 GameScene::mconvertToWorldPos(Vec2 pos, Node* parent)
{
	//���㸸�ڵ����½������������е�λ��
	Size pps = parent->getContentSize();
	auto pp = parent->getPosition();
	auto anchp = parent->getAnchorPoint();
	auto originPP = Vec2(pp.x - pps.width * anchp.x, pp.y - pps.height * anchp.y);
	Vec2 result = Vec2(originPP.x + pos.x, originPP.y + pos.y);
	return result;
}

void GameScene::update(float dt)
{
	if (BlockShape::sTouchBegan && !BlockShape::sTouchEnd)
	{
		projection();
		projectionC();
	}
	if (!BlockShape::sTouchBegan && BlockShape::sTouchEnd)
	{
		projectioned();
	}
}

void GameScene::customUpdate(float dt)
{
	if (BlockShape::sTouchBegan && !BlockShape::sTouchEnd)
	{
		projection();
	}
}

void GameScene::replayGame(int tag)
{
	m_mapLayer->mapClear();
	m_shapeLayer->updateShapes();
	//m_isGameOver = false;
	// ������Ϸ
	if (0 == tag)
	{
		
	}// ������Ϸ
	else
	{
		ScoreInfo info;
		EventCustom event(UPDATESCOR_EVENT);
		info.score = -m_statusLayer->getScore();
		info.cleanCountOnce = 0;
		event.setUserData(&info);
		_eventDispatcher->dispatchEvent(&event);
		return;
	}
}
