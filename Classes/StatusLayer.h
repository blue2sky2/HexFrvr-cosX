#ifndef STATUS_LAYER_H_
#define STATUS_LAYER_H_

#include "cocos2d.h"
#include "ScoreInfo.h"
using namespace cocos2d;

class StatusLayer : public Layer{
public:
	StatusLayer();
	~StatusLayer();
	virtual bool init();
	CREATE_FUNC(StatusLayer);
	char*  FontToUTF8(char* font);
	int getScore();

private:
	void updateScore(ScoreInfo* info);
	void plusplusScore(int delv, int now);
	void actionCb(Sprite* target);

	LabelAtlas* m_scoreLabel;
	LabelTTF* m_highScoreLabel;
	int m_score;
};

#endif