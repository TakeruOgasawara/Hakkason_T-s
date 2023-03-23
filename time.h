//==================================================================================================
//
//Time.h
//Author:髙山桃也
//
//==================================================================================================

#ifndef _TIME_H_
#define _TIME_H_

#include"main.h"

//****************************
//マクロ定義
//****************************
#define INITIAL_MINITE			(3)											//初期の時間（分）
#define INITIAL_SECOND			(0)											//初期の時間（秒）

//プロトタイプ宣言
void InitTime(void);
void UninitTime(void);
void UpdateTime(void);
void DrawTime(void);
void SetTime(int nTime);
void AddTime(int nValue);
int GetTime(void);
bool GetTimeUp(void);
#endif