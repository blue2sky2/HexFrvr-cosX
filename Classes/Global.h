#ifndef GLOBAL_H_
#define GLOBAL_H_

#include <tchar.h>
//��ӡ�����Ϣ
#define DP(fmt) {TCHAR sOut[256];_stprintf_s(sOut,_T(fmt));OutputDebugString(sOut);}
#define DP1(fmt,var) {TCHAR sOut[256];_stprintf_s(sOut,_T(fmt),var);OutputDebugString(sOut);}
#define DP2(fmt,var1,var2) {TCHAR sOut[256];_stprintf_s(sOut,_T(fmt),var1,var2);OutputDebugString(sOut);}    
#define DP3(fmt,var1,var2,var3) {TCHAR sOut[256];_stprintf_s(sOut,_T(fmt),var1,var2,var3);OutputDebugString(sOut);}

#define UPDATESCOR_EVENT "update_score_event"

#define gMapLineM 9
#define gMapRowM 9
//ͼƬ��С
#define gSpriteW 41
#define gSpriteH 45
//��ͼ��϶��С
#define gSpace 3.5
//����һ������ķ�ֵ
#define gScoreBase 40
#define gScoreLabelSpace 20
#define gScoreLabelFontSize 40
#define gHighestLSpaceY 30
#define gHighestLSpaceX 200
#define gHighestLFontSize 30
#define gDoWellLFontSize 40
//��ͼ���
#define MAP_EMPTY 0
#define MAP_FILL 1
#define MAP_NOUSE 2
#define gHighScoreKey  "HighScore"
#endif GLOBAL_H_