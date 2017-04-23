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
	//�ʺ���ɫr,g,b
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

	// ����3����ֵ��0-21��Χ�ڲ��ظ��������
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
	// ����������֮�����ľ���
	float disY = gSpriteH - difH + gSpace;
	// ͬһ���к��������ľ���
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

// ����һ���뵱ǰ�Ѵ�����״��ͬ����һ�������״
void ShapeLayer::updateShape(int oriI, float oriX, float oriY)
{
	// ��ʱ��m_refer��0 - 17��δ���ڵ�ֵ����Ȼ(20 - oriI)������Ŀ�ѡֵ֮һ,����+1��
	// �����ɵ�ri = 18ʱ���Ͱ�������ȡ����(20 - oriI)���ֵ
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
	// �п������Ƴ���ʱ������û����ִ���걻�жϣ�����������ִ��һ��
	m_currentShapes[oriI]->runScaleAction();
	addChild(m_currentShapes[oriI]);
	if (oriI == 0)
	{
		// �������
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
		// m_currentShapes ��Ӧ����
		auto tmp = m_currentShapes[0];
		m_currentShapes[0] = m_currentShapes[1];
		m_currentShapes[1] = m_currentShapes[2];
		m_currentShapes[2] = tmp;
		// m_refer ��Ӧ����
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
		// m_currentShapes ��Ӧ����
		auto tmp = m_currentShapes[1];
		m_currentShapes[1] = m_currentShapes[2];
		m_currentShapes[2] = tmp;
		// m_refer ��Ӧ����
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
	��������һ�鷽�飬��������Ϸ
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