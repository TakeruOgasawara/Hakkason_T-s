//==========================================================
//
// フォグ処理 [fog.cpp]
// Author : 丹野 竜之介
//
//==========================================================
#include "fog.h"
#include "debugproc.h"

//グローバル変数
FOG g_Fog;

//======================================
//初期化処理
//======================================
void InitFog()
{
	//数値のリセット
	g_Fog.FogMode = D3DFOG_NONE;
	g_Fog.FogStart = 0.0f;
	g_Fog.FogEnd = 0.0f;
	g_Fog.FogDensity = 0.0f;
	g_Fog.FogColor = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
}

//======================================
//終了処理
//======================================
void UninitFog()
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	pDevice = GetDevice();
	pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE); //フォグの無効化
}

//======================================
//更新処理
//======================================
void UpdateFog()
{
	if (g_Fog.FogMode != D3DFOG_NONE)
	{//フォグがNONEではない時
		//デバッグ表示
		switch (g_Fog.FogMode)
		{
		case  D3DFOG_EXP:
			PrintDebugProc("現在のフォグモード：EXP\n");
			break;
		case  D3DFOG_EXP2:
			PrintDebugProc("現在のフォグモード：EXP2\n");
			break;
		case  D3DFOG_LINEAR:
			PrintDebugProc("現在のフォグモード：LINEAR\n");
			break;

		default:
			break;
		}

		PrintDebugProc("フォグの色：R:%f G:%f B:%f A:%f\n", g_Fog.FogColor.r, g_Fog.FogColor.g, g_Fog.FogColor.b, g_Fog.FogColor.a);

		if (g_Fog.FogMode == D3DFOG_LINEAR)
		{
			PrintDebugProc("フォグの開始地点：%f\n",g_Fog.FogStart);
			PrintDebugProc("フォグの終了地点：%f\n", g_Fog.FogEnd);
		}
		else
		{
			PrintDebugProc("フォグの密度：%f\n", g_Fog.FogDensity);
		}
	}
	else
	{
		PrintDebugProc("現在のフォグモード：NONE");
	}
}

//======================================
//描画処理
//======================================
void DrawFog()
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	pDevice = GetDevice();


	//フォグを有効化する
		if (g_Fog.FogMode != D3DFOG_NONE)
		{//フォグモードが有効な時
			pDevice->SetRenderState(D3DRS_FOGCOLOR, g_Fog.FogColor);					//色の設定
			pDevice->SetRenderState(D3DRS_FOGTABLEMODE, g_Fog.FogMode);					//モードの設定
			pDevice->SetRenderState(D3DRS_FOGSTART, *((LPDWORD)(&g_Fog.FogStart)));		//開始地点の設定
			pDevice->SetRenderState(D3DRS_FOGEND, *((LPDWORD)(&g_Fog.FogEnd)));			//終了地点の設定
			pDevice->SetRenderState(D3DRS_FOGDENSITY, *((LPDWORD)(&g_Fog.FogDensity)));	//フォグ密度の設定
			pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);								//フォグの有効化
		}
	else
	{//フォグを無効化する
		pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE); //フォグの無効化
	}
}

//======================================
//設定処理
//======================================
void SetFog(D3DFOGMODE FogMode, D3DXCOLOR FogColor, float FogStart, float FogEnd, float FogDensity)
{
	g_Fog.FogMode = FogMode;        //モードの設定
	g_Fog.FogColor = FogColor;		//色の設定
	g_Fog.FogStart = FogStart;		//開始地点の設定		*フォグモードがD3DFOG_LINEARの時に参照される*
	g_Fog.FogEnd = FogEnd;			//終了地点の設定		*フォグモードがD3DFOG_LINEARの時に参照される*
	g_Fog.FogDensity = FogDensity;	//フォグ密度の設定		0.0f～1.0fの間の数値を入力	*フォグモードがD3DFOG_LINEARでは無い時に参照される*
}