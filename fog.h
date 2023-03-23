//========================================================================================
//
// フォグヘッダー[fog.h]
// Author: 丹野 竜之介
//
//========================================================================================
#ifndef _FOG_H_
#define _FOG_H_
#include "main.h"

//構造体の定義
typedef struct
{
	D3DFOGMODE FogMode;	//フォグのモード
	D3DXCOLOR FogColor;	//フォグの色
	float FogStart;		//フォグの開始地点
	float FogEnd;		//フォグの終了地点
	float FogDensity;	//フォグの密度
	
}FOG;

//プロトタイプ宣言
void InitFog();
void UninitFog();
void UpdateFog();
void DrawFog();
void SetFog(D3DFOGMODE FogMode, D3DXCOLOR FogColor, float FogStart, float FogEnd, float FogDensity);

#endif // !_FOG_H_
