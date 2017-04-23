#include "ShapeLayer.h"
#include "Global.h"
#include <assert.h>

ShapeLayer::ShapeLayer(){}    

ShapeLayer::~ShapeLayer()
{
	removeAllChildren();
	for (int i = 0; i < 21; ++i)
	{
		if (NULL != m_shapes[i])
		{
			delete m_shapes[i];
			m_shapes[i] = NULL;
		}
	}
}

bool ShapeLayer::init()
{
	if (!Layer::init()) return false;

	auto winSize = Director::getInstance()->getWinSize();
	
	srand(time(NULL));
	//彩虹七色r,g,b
	m_colors[0] = Color3B(255, 0, 0);
	m_colors[1] = Color3B(255, 128, 0);
	m_colors[2] = Color3B(255, 255, 0);
	m_colors[3] = Color3B(0, 255, 0);
	m_colors[4] = Color3B(255, 52, 179);
	m_colors[5] = Color3B(0, 0, 255);
	m_colors[6] = Color3B(128, 0, 255);

	for (int i = 0; i < 21; ++i)
	{
		m_shapes[i] = new BlockShape(i, m_colors[i % 7]);
		//DP1("=================i > %d\n",i);
		assert(m_shapes[i] != NULL);
	}

	for (int i = 0; i < 21; ++i)
	{
		m_refer[i] = i;
	}

	// 生成3个在值在0-21范围内不重复的随机数
	for (int i = 0; i < 3; ++i)
	{
		int rm = 20 - i;
		int ri = rand() % (rm + 1);
		if (rm != ri){
			int temp = m_refer[ri];
			m_refer[ri] = m_refer[rm];
			m_refer[rm] = temp;
		}
		int index = m_refer[rm];
		m_currentShapes[i] = m_shapes[index];
	}

	float difH = gSpriteW * sqrt(3) / 6;
	// 各行纵坐标之间相差的距离
	float disY = gSpriteH - difH + gSpace;
	// 同一行中横坐标相差的距离
	float disOneLineX = gSpriteW + gSpace;
	float ySpace = (winSize.height - 8 * disY) / 2;
	float xSpace = (winSize.width - 8 * disOneLineX) / 2;

	m_currentShapes[1]->setOriPos(winSize.width - xSpace / 2, winSize.height / 2, true);
	m_currentShapes[2]->setOriPos(winSize.width - xSpace / 2, (winSize.height / 2 - gSpriteH * 3), true);
	m_currentShapes[0]->setOriPos(winSize.width - xSpace / 2, (winSize.height / 2 + gSpriteH * 3), true);
	addChild(m_currentShapes[0]);
	addChild(m_currentShapes[1]);
	addChild(m_currentShapes[2]);

	//int posx = 60;
	//int posy = 60;
	//for (int i = 0; i < 21; ++i)
	//{
	//	if (i > 0)
	//		posx = m_shapes[i - 1]->m_oriPosX + 150;
	//	if (posx >= winSize.width - 40)
	//	{
	//		posx = 60;
	//		posy += 150;
	//	}
	//	addChild(m_shapes[i]);
	//	m_shapes[i]->setOriPos(posx, posy);
	//}

	return true;
}

// 生成一个与当前已存在形状不同的另一个随机形状
void ShapeLayer::updateShape(int oriI, float oriX, float oriY)
{
	// 此时的m_refer里0 - 17是未存在的值，当然(20 - oriI)是另外的可选值之一,这里+1是
	// 当生成的ri = 18时，就把它当做取到了(20 - oriI)这个值
	int rm = 20 - 3 + 1;
	int ri = rand() % (rm + 1);
	if (ri != 18){
		int temp = m_refer[ri];
		m_refer[ri] = m_refer[20 - oriI];
		m_refer[20 - oriI] = temp;
	}
	int index = m_refer[20 - oriI];
	m_currentShapes[oriI] = m_shapes[index];
	auto winSize = Director::getInstance()->getWinSize();
	m_currentShapes[oriI]->setPosition(Vec2(winSize.width - 40, 0 - gSpriteH * 2));
	m_currentShapes[oriI]->setVisible(true);
	// 有可能在移除的时候动作还没有有执行完被中断，所以这里在执行一次
	m_currentShapes[oriI]->runScaleAction();
	addChild(m_currentShapes[oriI]);
	if (oriI == 0)
	{
		// 坐标更新
		auto oriX1 = m_currentShapes[1]->m_oriPosX;
		auto oriY1 = m_currentShapes[1]->m_oriPosY;
		auto oriX2 = m_currentShapes[2]->m_oriPosX;
		auto oriY2 = m_currentShapes[2]->m_oriPosY;
		m_currentShapes[1]->runAction(MoveTo::create(0.3f, Vec2(oriX, oriY)));
		m_currentShapes[1]->setOriPos(oriX, oriY);
		m_currentShapes[2]->runAction(MoveTo::create(0.3f, Vec2(oriX1, oriY1)));
		m_currentShapes[2]->setOriPos(oriX1, oriY1);
		m_currentShapes[oriI]->runAction(MoveTo::create(0.2f, Vec2(oriX2, oriY2)));
		m_currentShapes[oriI]->setOriPos(oriX2, oriY2);
		// m_currentShapes 对应更新
		auto tmp = m_currentShapes[0];
		m_currentShapes[0] = m_currentShapes[1];
		m_currentShapes[1] = m_currentShapes[2];
		m_currentShapes[2] = tmp;
		// m_refer 对应更新
		int tmp1 = m_refer[20];
		m_refer[20] = m_refer[19];
		m_refer[19] = m_refer[18];
		m_refer[18] = tmp1;
	}
	else if (oriI == 1)
	{
		auto oriX2 = m_currentShapes[2]->m_oriPosX;
		auto oriY2 = m_currentShapes[2]->m_oriPosY;
		m_currentShapes[2]->runAction(MoveTo::create(0.3f, Vec2(oriX, oriY)));
		m_currentShapes[2]->setOriPos(oriX, oriY);
		m_currentShapes[oriI]->runAction(MoveTo::create(0.2f, Vec2(oriX2, oriY2)));
		m_currentShapes[oriI]->setOriPos(oriX2, oriY2);
		// m_currentShapes 对应更新
		auto tmp = m_currentShapes[1];
		m_currentShapes[1] = m_currentShapes[2];
		m_currentShapes[2] = tmp;
		// m_refer 对应更新
		int tmp1 = m_refer[19];
		m_refer[19] = m_refer[18];
		m_refer[18] = tmp1;
	}
	else{
		m_currentShapes[oriI]->runAction(MoveTo::create(0.2f, Vec2(oriX, oriY)));
		m_currentShapes[oriI]->setOriPos(oriX, oriY);
	}
}

/*
	重新生成一组方块，以重新游戏
*/
void ShapeLayer::updateShapes()
{
	auto delayF = CallFunc::create([&](){
		float oriX = m_currentShapes[0]->m_oriPosX;
		float oriY = m_currentShapes[0]->m_oriPosY;
		m_currentShapes[0]->removeFromParent();
		updateShape(0, oriX, oriY);
	});
	auto delayRun = Sequence::create(delayF, DelayTime::create(0.3), nullptr);
	runAction(Sequence::create(delayRun, delayRun->clone(), delayF->clone(), nullptr));
}