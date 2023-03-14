//======================================================================================
//
// ライトの処理[light.cpp]
// Author;小笠原　彪
//
//======================================================================================
#include "light.h"

//*****************************
//マクロ定義
//*****************************
#define MAX_LIGHT			(3)

//*****************************
//グローバル宣言
//*****************************
D3DLIGHT9 g_light[MAX_LIGHT];			//ライトの情報

//====================================================================
// ライトの初期化処理
//====================================================================
void InitLight(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得

	D3DXVECTOR3 vecDir;			//設定用方向ベクトル

	for (int nCntLight = 0; nCntLight < MAX_LIGHT; nCntLight++)
	{
		//ライトの情報をクリアする
		ZeroMemory(&g_light[nCntLight], sizeof(D3DLIGHT9));

		switch (nCntLight)
		{
		case 0:
			//ライト1の種類を設定
			g_light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

			//ライト1の拡散光を設定
			g_light[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//ライト1の方向を設定
			vecDir = D3DXVECTOR3(0.22f, -0.87f, 0.44f);
			D3DXVec3Normalize(&vecDir, &vecDir);			//ベクトルを正規化する
			g_light[nCntLight].Direction = vecDir;
			break;
		case 1:
			//ライト2の種類を設定
			g_light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

			//ライト2の拡散光を設定
			g_light[nCntLight].Diffuse = D3DXCOLOR(0.65f, 0.65f, 0.65f, 1.0f);

			//ライト2の方向を設定
			vecDir = D3DXVECTOR3(-0.18f, 0.88f, -0.44f);
			D3DXVec3Normalize(&vecDir, &vecDir);			//ベクトルを正規化する
			g_light[nCntLight].Direction = vecDir;
			break;
		case 2:
			//ライト3の種類を設定
			g_light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

			//ライト3の拡散光を設定
			g_light[nCntLight].Diffuse = D3DXCOLOR(0.15f, 0.15f, 0.15f, 1.0f);

			//ライト3の方向を設定
			vecDir = D3DXVECTOR3(0.89f, -0.11f, 0.44f);
			D3DXVec3Normalize(&vecDir, &vecDir);			//ベクトルを正規化する
			g_light[nCntLight].Direction = vecDir;
			break;
		}

		//ライトを設定
		pDevice->SetLight(nCntLight, &g_light[nCntLight]);

		//ライトを有効化する
		pDevice->LightEnable(nCntLight, TRUE);
	}
}

//====================================================================
// ライトの終了処理
//====================================================================
void UninitLight(void)
{

}

//====================================================================
// ライトの更新処理
//====================================================================
void UpdateLight(void)
{

}

