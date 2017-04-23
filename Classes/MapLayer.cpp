#include "MapLayer.h"
#include "Global.h"

MapLayer::MapLayer(){}
MapLayer::~MapLayer()
{
	removeAllChildren();
	for (int i = 0; i < 4; ++i)
	{
		if (m_shadowS[i])
		{
			m_shadowS[i]->release();
		}
	}
}

bool MapLayer::init()
{
	if (!LayerColor::initWithColor(Color4B(28, 28, 28, 255)))
	{
		return false;
	}
	auto winSize = Director::getInstance()->getWinSize();

	m_color = Color3B(79, 79, 79);

	for (int i = 0; i < 4; ++i)
	{
		m_shadowS[i] = Sprite::create("shadow.png");
		m_shadowS[i]->retain();
		m_shadowV[i] = 0;
	}

	// ��ʼ����ͼ
	for (int i = 0; i < gMapLineM; ++i)
	{
		// ����ÿһ�е�Ԫ�صĸ���(�����η��ѵ�ͼ)
		int lineLength = gMapLineM - abs(4 - i);
		// ÿ��Ԫ�ص�һ��Ԫ�ص�
		int j = 0;
		if (i > 4){
			j = i - 4;
		}
		int rowMaxI = j + lineLength;
		for (int k = 0; k < j; ++k)
			m_Map[i][k] = MAP_NOUSE;
		for (j; j < rowMaxI; ++j)
			m_Map[i][j] = MAP_EMPTY;
		for (j; j < gMapLineM; ++j)
			m_Map[i][j] = MAP_NOUSE;
	}

	/*
		��ͼ��ʼ�����ֵ�ڶ�ά����ռ��ʾΪ��
		{0, 0, 0, 0, 0, 2, 2, 2, 2}
		{0, 0, 0, 0, 0, 0, 2, 2, 2}
		{0, 0, 0, 0, 0, 0, 0, 2, 2}
		{0, 0, 0, 0, 0, 0, 0, 0, 2}
		{0, 0, 0, 0, 0, 0, 0, 0, 0}
		{2, 0, 0, 0, 0, 0, 0, 0, 0}
		{2, 2, 0, 0, 0, 0, 0, 0, 0}
		{2, 2, 2, 0, 0, 0, 0, 0, 0}
		{2, 2, 2, 2, 0, 0, 0, 0, 0}
	*/
	// ����������֮�����ľ���
	float disY = gSpriteH  -(gSpriteW * sqrt(3) / 6)
		+ gSpace / sqrt(3) * 2;
	// ͬһ���к��������ľ���,
	float disOneLineX = gSpriteW + gSpace;
	// ��ͬ���е�һ��Ԫ�غ��������ľ��룬����spaceӦ��Ϊm_space / 2,����ΪdisOneLineX / 2
	float disRowLineX = disOneLineX / 2;
	// �Ե�һ�е�һ��Ԫ�ص�������Ϊ�ο�������,�Խ���ͼ������Ļ����������
	float flagPosY = winSize.height - (winSize.height - 8 * disY) / 2;
	// �����Ԫ������(�м���)�ĵ�һ��Ԫ�صĺ�����Ϊ�ο�������,�ڱ�����Ҳ���ǵ�����...
	float flagPosX = (winSize.width - 8 * disOneLineX) / 2;

	for (int i = 0; i < gMapLineM; ++i){
		// ���㲻ͬ��֮���һ��Ԫ�صĺ�����
		float firstBlockX;
		if (i < 4)
			firstBlockX = flagPosX + (4 - i) * disRowLineX;
		else{
			firstBlockX = flagPosX + (i - 4) * (disRowLineX - disOneLineX);
		}
		// ��Ϊ�Ե�(4,0)Ԫ�صĺ�����Ϊ�ο����꣬���Ե�j <= 4
		// ʱ��ͼ�ϲ�ΪMAP_NOUSE����ʼjֵ����Ϊ0.��ʱ���ǵĵ�
		// һ��Ԫ����ʼ������Ӧ��Ϊ[(i - 4) * disRowLineX - (i - 4) * disOneLineX]
		for (int j = 0; j < gMapRowM; ++j){
			if (MAP_NOUSE != m_Map[i][j])
			{
				auto block = Block::create();
				block->setLRIndex(i, j);
				block->setSpriteColor(m_color);
				block->setPosition(Point(firstBlockX + j * disOneLineX, flagPosY - i * disY));
				addChild(block);
				m_blocks[i][j] = block;
			}
			else
			{
				m_blocks[i][j] = NULL;
			}
		}
	}
	return true;
}

/*
	Ϊ�������һ����Ӱ���顣��ֱ�ӵ���Block��Ľӿ�����Ϊ��ÿ��
	���ʱ����Ҫ�������Ƴ��˱����ա�����Ƶ����
	���ʹ��MapLayer��addShadow**����ôҲҪʹ��MapLayer��removeShadow**
*/
void MapLayer::addShadowForBlock(Block* target)
{
	assert(NULL != target);
	int index = -1;
	for (int i = 0; i < 4; ++i)
	{
		if (0 == m_shadowV[i])
		{
			index = i;
		}
	}
	if (index < 0)
		return;
	if (target->addShadowSprite(m_shadowS[index], 70))
	{
		m_shadowV[index] = 1;
	}
}

void MapLayer::removeShadowForBlock(Block* target)
{
	if (!target) return;
	for (int i = 0; i < 4; ++i)
	{
		if (target->getShadowSprite() == m_shadowS[i])
		{
			m_shadowV[i] = 0;
			target->removeShadowSprite();
		}
	}
}

void MapLayer::mapClear()
{
	// ���������ͼ
	for (int i = 0; i < gMapLineM; ++i)
	{
		for (int j = 0; j < gMapRowM; ++j)
		{
			if (MAP_FILL == m_Map[i][j])
				m_Map[i][j] = MAP_EMPTY;
			if (MAP_EMPTY == m_Map[i][j])
				m_blocks[i][j]->setSpriteColor(m_color);
		}
	}
}

