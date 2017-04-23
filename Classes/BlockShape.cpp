#include "BlockShape.h"
#include "Global.h"

BlockShape* BlockShape::sTarget = NULL;
bool BlockShape::sTouchEnd = true;
bool BlockShape::sTouchBegan = false;

BlockShape::BlockShape(int shape, Color3B color) : m_shape(shape), m_color(color)
{
	for (int i = 0; i < 4; ++i)
	{
		m_blocks[i] = (Block*)Block::create();
		/*
			������һ������,��ΪBlock��create()��������ģ�create��������������
			autoReleasePool(�Զ����ճ�����)���û��addChild�Ļ��ͻ��Զ������ա���
			shape 0ֻ��һ������(m_blocks[0])addChild()���Բ���shape 0����漰��
			��ֻ��m_blocks[0]�Ĳ����������Ī����������⡣�����ԭ���������
			��new����Ļ���ȽϺá�
		*/
		m_blocks[i]->retain();
	}
	m_space = m_dspace;
	setAnchorPoint(Point(0.5, 0.5));
	setZOrder(0);

	if (shape < 0 || shape > 20)
	{
		shape = 0;
	}

	if (shape == 0){
		addChild(m_blocks[0]);
	}
	else{
		for (int i = 0; i < 4; ++i)
			addChild(m_blocks[i]);
	}
	generateShape();
	runScaleAction();
	addTouchEvent();
}

BlockShape::~BlockShape(){
	for (int i = 0; i < 4; ++i)
	{
		m_blocks[i]->release();
	}
	_eventDispatcher->removeAllEventListeners();
	_actionManager->removeAllActions();
	removeAllChildrenWithCleanup(true);
}

void BlockShape::setBlock(Block* block, int lI, int rI)
{
	block->setLRIndex(lI, rI);
	block->setSpriteColor(m_color);
}

void BlockShape::setColor(Color3B color)
{
	for (int i = 0; i < 4; ++i){
		if (NULL != m_blocks[i])
		{
			m_blocks[i]->setSpriteColor(m_color);
		}
	}
}

/*
 space��ԭ����space = 2�Ļ����Ͻ��мӼ�
*/
void BlockShape::plusSpace(float space)
{
	m_space = m_dspace + (space);
	float exp = 0.1;
	if (m_space < exp)	m_space = m_dspace;
	generateShape(true);
}

void BlockShape::addTouchEvent()
{
	//���������¼�������
	auto listener = EventListenerTouchOneByOne::create();
	//���ɴ���
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [&](Touch* touch, Event* event){
		auto target = event->getCurrentTarget();
		target->setZOrder(1);
		sTarget = (BlockShape*)target;
		auto pos = target->convertToNodeSpace(touch->getLocation());
		auto size = target->getContentSize();
		Rect rect = Rect(0, 0, size.width, size.height);
		if (rect.containsPoint(pos))
		{
			BlockShape::sTouchBegan = true;
			BlockShape::sTouchEnd = false;
			plusSpace(5);
			auto scale = ScaleTo::create(0, 0.9);
			scale->setTag(2);
			target->stopActionByTag(2);
			target->runAction(scale);
			target->setAnchorPoint(Point(0.4, 0.3));
			return true;
		}
		return false;
	};
	listener->onTouchMoved = [](Touch* touch, Event* event){
		auto target = event->getCurrentTarget();
		auto delta = touch->getDelta();
		auto pos = target->getPosition();
		target->setPosition(Point(pos.x + delta.x, pos.y + delta.y));
	};
	listener->onTouchEnded = [&](Touch* touch, Event* event){
		auto target = event->getCurrentTarget();
		BlockShape::sTouchEnd = true;
		BlockShape::sTouchBegan = false;
		target->setAnchorPoint(Point(0.5, 0.5));
		auto moveback = MoveTo::create(0.3, Point(m_oriPosX, m_oriPosY));
		auto scaleback = ScaleTo::create(0.3, 0.7);
		auto actions = Spawn::create(moveback, scaleback, NULL);
		actions->setTag(1);
		// ��һ��������һ��ִ�л�û�������ִ����������ǻ�������ģ�����֮��ụ����š�
		target->stopActionByTag(1);
		target->runAction(actions);
		plusSpace(0);
		target->setZOrder(0);
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void BlockShape::generateShape(bool onlyHandlePos)
{
	// ͨ��m_space�����Ƹ�С����֮��ļ��
	//����������Ƕ��ʱ�ص��ľ���
	float difH = gSpriteW * sqrt(3) / 6;
	// ����������֮�����ľ���
	float disY = gSpriteH - difH + (m_space / sqrt(3)) * 2;
	// ͬһ���к��������ľ���
	float disOneLineX = gSpriteW + m_space;
	// ��ͬ���к��������ľ���
	float disRowLineX = disOneLineX / 2;

	/*
		@param pSize ��ǰ��״(����)�Ĵ�С(��һ�����εĿ�͸ߣ����������Ҫ�պ��ܷ���
		��ɴ���״������С����)
		�ڴ���״������һ��С������Ϊ����״�Ĳο���(ԭ��)����ѡ�ķ��龡��ѡ���Ͻǻ������Ͻǵķ���,
		Ȼ���������Ƿ��ѵ�ͼ���������ԣ����ô���״���������������������
		ÿ����״�и���������������ã����ͼ�з�����������÷�����ͬ��
	*/
	switch (m_shape){
	case 0:
	{
			  auto pSize = Size(gSpriteW, gSpriteH);
			  setContentSize(pSize);
			  m_blocks[0]->setPosition(pSize.width / 2, pSize.height / 2);
			  if (onlyHandlePos) break;
			  setBlock(m_blocks[0], 0, 0);
			  setBlock(m_blocks[1], 0, 0);
			  setBlock(m_blocks[2], 0, 0);
			  setBlock(m_blocks[3], 0, 0);
	}break;
	case 1:
	{
		auto pSize = Size(2 * disOneLineX + gSpriteW, gSpriteH + disY);
		setContentSize(pSize);
		m_blocks[0]->setPosition(gSpriteW / 2, pSize.height - gSpriteH / 2);
		m_blocks[1]->setPosition(gSpriteW / 2 + disOneLineX, pSize.height - gSpriteH / 2);
		m_blocks[2]->setPosition(gSpriteW / 2 + disOneLineX * 2, pSize.height - gSpriteH / 2);
		m_blocks[3]->setPosition(gSpriteW / 2 + disOneLineX * 2 - disRowLineX, gSpriteH / 2);
		if (onlyHandlePos) break;
		setBlock(m_blocks[0], 0, 0);
		setBlock(m_blocks[1], 0, 1);
		setBlock(m_blocks[2], 0, 2);
		setBlock(m_blocks[3], 1, 2);
	}break;
	case 2:
	{
		auto pSize = Size(2 * disOneLineX + gSpriteW, gSpriteH + disY);
		setContentSize(pSize);
		m_blocks[0]->setPosition(gSpriteW / 2 + disOneLineX * 2 - disRowLineX, pSize.height - gSpriteH / 2);
		m_blocks[1]->setPosition(gSpriteW / 2, gSpriteH / 2);
		m_blocks[2]->setPosition(gSpriteW / 2 + disOneLineX, gSpriteH / 2);
		m_blocks[3]->setPosition(pSize.width - gSpriteW / 2, gSpriteH / 2);
		if (onlyHandlePos) break;
		setBlock(m_blocks[0], 0, 0);
		setBlock(m_blocks[1], 1, -1);
		setBlock(m_blocks[2], 1, 0);
		setBlock(m_blocks[3], 1, 1);
	}break;
	case 3:
	{
		auto pSize = Size(3 * disRowLineX + gSpriteW, 3 * disY + gSpriteH);
		setContentSize(pSize);
		m_blocks[0]->setPosition(gSpriteW / 2, pSize.height - gSpriteH / 2);
		m_blocks[1]->setPosition(gSpriteW / 2 + disRowLineX, pSize.height - gSpriteH / 2 - disY);
		m_blocks[2]->setPosition(gSpriteW / 2 + disRowLineX * 2, pSize.height - gSpriteH / 2 - disY * 2);
		m_blocks[3]->setPosition(pSize.width - gSpriteW / 2, gSpriteH / 2);
		if (onlyHandlePos) break;
		setBlock(m_blocks[0], 0, 0);
		setBlock(m_blocks[1], 1, 1); 
		setBlock(m_blocks[2], 2, 2); 
		setBlock(m_blocks[3], 3, 3); 
	}break;
	case 4:
	{
		auto pSize = Size(3 * disRowLineX + gSpriteW, 3 * disY + gSpriteH);
		setContentSize(pSize);
		m_blocks[0]->setPosition(pSize.width - gSpriteW / 2, pSize.height - gSpriteH / 2);
		m_blocks[1]->setPosition(gSpriteW / 2 + disRowLineX * 2, gSpriteH / 2 + disY * 2);
		m_blocks[2]->setPosition(gSpriteW / 2 + disRowLineX, gSpriteH / 2 + disY);
		m_blocks[3]->setPosition(gSpriteW / 2, gSpriteH / 2);
		if (onlyHandlePos) break;
		setBlock(m_blocks[0], 0, 0);
		setBlock(m_blocks[1], 1, 0);
		setBlock(m_blocks[2], 2, 0);
		setBlock(m_blocks[3], 3, 0);
	}break;
	case 5:
	{
		auto pSize = Size(gSpriteW + disOneLineX + disRowLineX, gSpriteH + disY);
		setContentSize(pSize);
		m_blocks[0]->setPosition(gSpriteW / 2, pSize.height - gSpriteH / 2);
		m_blocks[1]->setPosition(gSpriteW / 2 + disOneLineX, pSize.height - gSpriteH / 2);
		m_blocks[2]->setPosition(gSpriteW / 2 + disRowLineX, gSpriteH / 2);
		m_blocks[3]->setPosition(pSize.width - gSpriteW / 2, gSpriteH / 2);
		if (onlyHandlePos) break;
		setBlock(m_blocks[0], 0, 0);
		setBlock(m_blocks[1], 0, 1);
		setBlock(m_blocks[2], 1, 1);
		setBlock(m_blocks[3], 1, 2);
	}break;
	case 6:
	{
		auto pSize = Size(gSpriteW + disOneLineX + disRowLineX, gSpriteH + disY);
		setContentSize(pSize);
		m_blocks[0]->setPosition(gSpriteW / 2 + disRowLineX, pSize.height - gSpriteH / 2);
		m_blocks[1]->setPosition(pSize.width - gSpriteW / 2, pSize.height - gSpriteH / 2);
		m_blocks[2]->setPosition(gSpriteW / 2, gSpriteH / 2);
		m_blocks[3]->setPosition(gSpriteW / 2 + disOneLineX, gSpriteH / 2);
		if (onlyHandlePos) break;
		setBlock(m_blocks[0], 0, 0);
		setBlock(m_blocks[1], 0, 1);
		setBlock(m_blocks[2], 1, 0);
		setBlock(m_blocks[3], 1, 1);
	}break;
	case 7:
	{
		auto pSize = Size(gSpriteW + 2 * disOneLineX, gSpriteH + disY);
		setContentSize(pSize);
		m_blocks[0]->setPosition(gSpriteW / 2, pSize.height - gSpriteH / 2);
		m_blocks[1]->setPosition(pSize.width - gSpriteW / 2, pSize.height - gSpriteH / 2);
		m_blocks[2]->setPosition(gSpriteW / 2 + disRowLineX, gSpriteH / 2);
		m_blocks[3]->setPosition(gSpriteW / 2 + disRowLineX + disOneLineX, gSpriteH / 2);
		if (onlyHandlePos) break;
		setBlock(m_blocks[0], 0, 0);
		setBlock(m_blocks[1], 0, 2);
		setBlock(m_blocks[2], 1, 1);
		setBlock(m_blocks[3], 1, 2);
	}break;
	case 8:
	{
		auto pSize = Size(disOneLineX + disRowLineX + gSpriteW, gSpriteH + 2 * disY);
		setContentSize(pSize);
		m_blocks[0]->setPosition(gSpriteW / 2 + disOneLineX, pSize.height - gSpriteH / 2);
		m_blocks[1]->setPosition(pSize.width - gSpriteW / 2, pSize.height - gSpriteH / 2 - disY);
		m_blocks[2]->setPosition(gSpriteW / 2, gSpriteH / 2);
		m_blocks[3]->setPosition(gSpriteW / 2 + disOneLineX, gSpriteH / 2);
		if (onlyHandlePos) break;
		setBlock(m_blocks[0], 0, 0);
		setBlock(m_blocks[1], 1, 1);
		setBlock(m_blocks[2], 2, 0);
		setBlock(m_blocks[3], 2, 1);
	}break;
	case 9:
	{
		auto pSize = Size(gSpriteW + 2 * disOneLineX, gSpriteH + disY);
		setContentSize(pSize);
		m_blocks[0]->setPosition(gSpriteW / 2 + disRowLineX, pSize.height - gSpriteH / 2);
		m_blocks[1]->setPosition(gSpriteW / 2 + disRowLineX + disOneLineX, pSize.height - gSpriteH / 2);
		m_blocks[2]->setPosition(gSpriteW / 2, gSpriteH / 2);
		m_blocks[3]->setPosition(pSize.width - gSpriteW / 2, gSpriteH / 2);
		setBlock(m_blocks[0], 0, 0);
		setBlock(m_blocks[1], 0, 1);
		setBlock(m_blocks[2], 1, 0);
		setBlock(m_blocks[3], 1, 2);
	}break;
	case 10:
	{
		auto pSize = Size(disOneLineX + disRowLineX + gSpriteW, gSpriteH + 2 * disY);
		setContentSize(pSize);
		m_blocks[0]->setPosition(gSpriteW / 2 + disRowLineX, pSize.height - gSpriteH / 2);
		m_blocks[1]->setPosition(gSpriteW / 2, pSize.height - gSpriteH / 2 - disY);
		m_blocks[2]->setPosition(gSpriteW / 2 + disRowLineX, gSpriteH / 2);
		m_blocks[3]->setPosition(pSize.width - gSpriteW / 2, gSpriteH / 2);
		if (onlyHandlePos) break;
		setBlock(m_blocks[0], 0, 0);
		setBlock(m_blocks[1], 1, 0);
		setBlock(m_blocks[2], 2, 1);
		setBlock(m_blocks[3], 2, 2);
	}break;
	case 11:
	{
		auto pSize = Size(disOneLineX + gSpriteW, gSpriteH + 2 * disY);
		setContentSize(pSize);
		m_blocks[0]->setPosition(gSpriteW / 2 + disRowLineX, pSize.height - gSpriteH / 2);
		m_blocks[1]->setPosition(gSpriteW / 2, pSize.height - gSpriteH / 2 - disY);
		m_blocks[2]->setPosition(pSize.width - gSpriteW / 2, pSize.height - gSpriteH / 2 - disY);
		m_blocks[3]->setPosition(gSpriteW / 2 + disRowLineX, gSpriteH / 2);
		if (onlyHandlePos) break;
		setBlock(m_blocks[0], 0, 0);
		setBlock(m_blocks[1], 1, 0);
		setBlock(m_blocks[2], 1, 1);
		setBlock(m_blocks[3], 2, 1);
	}break;
	case 12:
	{
		auto pSize = Size(disOneLineX + gSpriteW, gSpriteH + 2 * disY);
		setContentSize(pSize);
		m_blocks[0]->setPosition(gSpriteW / 2, pSize.height - gSpriteH / 2);
		m_blocks[1]->setPosition(pSize.width - gSpriteW / 2, pSize.height - gSpriteH / 2);
		m_blocks[2]->setPosition(gSpriteW / 2 + disRowLineX, pSize.height - gSpriteH / 2 - disY);
		m_blocks[3]->setPosition(gSpriteW / 2, gSpriteH / 2);
		if (onlyHandlePos) break;
		setBlock(m_blocks[0], 0, 0);
		setBlock(m_blocks[1], 0, 1);
		setBlock(m_blocks[2], 1, 1);
		setBlock(m_blocks[3], 2, 1);
	}break;
	case 13:
	{
		auto pSize = Size(disOneLineX + gSpriteW, gSpriteH + 2 * disY);
		setContentSize(pSize);
		m_blocks[0]->setPosition(gSpriteW / 2, pSize.height - gSpriteH / 2);
		m_blocks[1]->setPosition(pSize.width - gSpriteW / 2, pSize.height - gSpriteH / 2);
		m_blocks[2]->setPosition(gSpriteW / 2 + disRowLineX, pSize.height - gSpriteH / 2 - disY);
		m_blocks[3]->setPosition(pSize.width - gSpriteW / 2, gSpriteH / 2);
		if (onlyHandlePos) break;
		setBlock(m_blocks[0], 0, 0);
		setBlock(m_blocks[1], 0, 1);
		setBlock(m_blocks[2], 1, 1);
		setBlock(m_blocks[3], 2, 2);
	}break;
	case 14:
	{
		auto pSize = Size(disOneLineX + gSpriteW, gSpriteH + 2 * disY);
		setContentSize(pSize);
		m_blocks[0]->setPosition(gSpriteW / 2, pSize.height - gSpriteH / 2);
		m_blocks[1]->setPosition(gSpriteW / 2 + disRowLineX, pSize.height - gSpriteH / 2 - disY);
		m_blocks[2]->setPosition(gSpriteW / 2, gSpriteH / 2);
		m_blocks[3]->setPosition(pSize.width - gSpriteW / 2, gSpriteH / 2);
		if (onlyHandlePos) break;
		setBlock(m_blocks[0], 0, 0);
		setBlock(m_blocks[1], 1, 1);
		setBlock(m_blocks[2], 2, 1);
		setBlock(m_blocks[3], 2, 2);
	}break;
	case 15:
	{
		auto pSize = Size(disOneLineX + gSpriteW, gSpriteH + 2 * disY);
		setContentSize(pSize);
		m_blocks[0]->setPosition(pSize.width - gSpriteW / 2, pSize.height - gSpriteH / 2);
		m_blocks[1]->setPosition(gSpriteW / 2 + disRowLineX, pSize.height - gSpriteH / 2 - disY);
		m_blocks[2]->setPosition(gSpriteW / 2, gSpriteH / 2);
		m_blocks[3]->setPosition(pSize.width - gSpriteW / 2, gSpriteH / 2);
		if (onlyHandlePos) break;
		setBlock(m_blocks[0], 0, 0);
		setBlock(m_blocks[1], 1, 0);
		setBlock(m_blocks[2], 2, 0);
		setBlock(m_blocks[3], 2, 1);
	}break;
	case 16:
	{
		auto pSize = Size(disOneLineX + disRowLineX + gSpriteW, gSpriteH + 2 * disY);
		setContentSize(pSize);
		m_blocks[0]->setPosition(gSpriteW / 2 + disRowLineX, pSize.height - gSpriteH / 2);
		m_blocks[1]->setPosition(gSpriteW / 2, pSize.height - gSpriteH / 2 - disY);
		m_blocks[2]->setPosition(gSpriteW / 2 + disOneLineX, pSize.height - gSpriteH / 2 - disY);
		m_blocks[3]->setPosition(pSize.width - gSpriteW / 2, gSpriteH / 2);
		if (onlyHandlePos) break;
		setBlock(m_blocks[0], 0, 0);
		setBlock(m_blocks[1], 1, 0);
		setBlock(m_blocks[2], 1, 1);
		setBlock(m_blocks[3], 2, 2);
	}break;
	case 17:
	{
		auto pSize = Size(disOneLineX + disRowLineX + gSpriteW, gSpriteH + 2 * disY);
		setContentSize(pSize);
		m_blocks[0]->setPosition(gSpriteW / 2 + disOneLineX, pSize.height - gSpriteH / 2);
		m_blocks[1]->setPosition(gSpriteW / 2 + disRowLineX, pSize.height - gSpriteH / 2 - disY);
		m_blocks[2]->setPosition(pSize.width - gSpriteW / 2, pSize.height - gSpriteH / 2 - disY);
		m_blocks[3]->setPosition(gSpriteW / 2, gSpriteH / 2);
		if (onlyHandlePos) break;
		setBlock(m_blocks[0], 0, 0);
		setBlock(m_blocks[1], 1, 0);
		setBlock(m_blocks[2], 1, 1);
		setBlock(m_blocks[3], 2, 0);
	}break;
	case 18:
	{
		auto pSize = Size(disOneLineX + disRowLineX + gSpriteW, gSpriteH + 2 * disY);
		setContentSize(pSize);
		m_blocks[0]->setPosition(pSize.width - gSpriteW / 2, pSize.height - gSpriteH / 2);
		m_blocks[1]->setPosition(gSpriteW / 2, pSize.height - gSpriteH / 2 - disY);
		m_blocks[2]->setPosition(pSize.width - gSpriteW / 2 - disRowLineX, pSize.height - gSpriteH / 2 - disY);
		m_blocks[3]->setPosition(gSpriteW / 2 + disRowLineX, gSpriteH / 2);
		if (onlyHandlePos) break;
		setBlock(m_blocks[0], 0, 0);
		setBlock(m_blocks[1], 1, -1);
		setBlock(m_blocks[2], 1, 0);
		setBlock(m_blocks[3], 2, 0);
	}break;
	case 19:
	{
		auto pSize = Size(disOneLineX + disRowLineX + gSpriteW, gSpriteH + 2 * disY);
		setContentSize(pSize);
		m_blocks[0]->setPosition(gSpriteW / 2, pSize.height - gSpriteH / 2);
		m_blocks[1]->setPosition(gSpriteW / 2 + disRowLineX, pSize.height - gSpriteH / 2 - disY);
		m_blocks[2]->setPosition(pSize.width - gSpriteW / 2, pSize.height - gSpriteH / 2 - disY);
		m_blocks[3]->setPosition(pSize.width - gSpriteW / 2 - disRowLineX, gSpriteH / 2);
		if (onlyHandlePos) break;
		setBlock(m_blocks[0], 0, 0);
		setBlock(m_blocks[1], 1, 1);
		setBlock(m_blocks[2], 1, 2);
		setBlock(m_blocks[3], 2, 2);
	}break;
	case 20:
	{
		auto pSize = Size(3 * disOneLineX + gSpriteW, gSpriteH);
		setContentSize(pSize);
		m_blocks[0]->setPosition(gSpriteW / 2, gSpriteH / 2);
		m_blocks[1]->setPosition(gSpriteW / 2 + disOneLineX, gSpriteH / 2);
		m_blocks[2]->setPosition(gSpriteW / 2 + disOneLineX * 2, gSpriteH / 2);
		m_blocks[3]->setPosition(pSize.width - gSpriteW / 2, gSpriteH / 2);
		if (onlyHandlePos) break;
		setBlock(m_blocks[0], 0, 0);
		setBlock(m_blocks[1], 0, 1);
		setBlock(m_blocks[2], 0, 2);
		setBlock(m_blocks[3], 0, 3);
	}break;
	default:break;
	}
}

void BlockShape::setOriPos(float posx, float posy, bool setitpos)
{
	m_oriPosX = posx;
	m_oriPosY = posy;
	if (setitpos){
		setPosition(posx, posy);
	}
}

void BlockShape::runScaleAction()
{
	Action* action = ScaleTo::create(0, 0.7);
	runAction(action);
}