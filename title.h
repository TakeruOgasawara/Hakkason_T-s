//======================================================================================
//
//タイトル画面処理[game.h]
//Author;小笠原　彪
//
//======================================================================================
#ifndef _TITLE_H_				//背景
#define _TITLE_H_				//背景

#include "main.h"

//プロトタイプ宣言
void InitTitle(void);
void UninitTitle(void);
void UpdateTitle(void);
void DrawTitle(void);
//3D
void InitTitleCamera(void);
void UninitTitleCamera(void);
void UpdateTitleCamera(void);
void DrawTitleCamera(void);


#endif