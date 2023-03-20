//======================================================================================
//
//チュートリアル処理[tutorial.cpp]
//Author;小笠原　彪
//
//======================================================================================
#include <stdio.h>
#include "tutorial.h"
#include "input.h"
#include "fade.h"

//*****************************
//マクロ定義
//*****************************
#define NUM_TEX			(1)		//使用するテクスチャ―の数

//*****************************
//テクスチャファイル名
//*****************************
const char *c_apFilenameTutorial[NUM_TEX] =
{
	"data\\TEXTURE\\TUTORIAL\\tutorial.png",					//背景の画像を読み込む
};

//*****************************
//グローバル変数
//*****************************
LPDIRECT3DTEXTURE9 g_pTextureTutorial[NUM_TEX] = {};					//テクスチャーへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTutorial = NULL;				//頂点バッファへのポインタ
D3DXVECTOR3 g_pos;
D3DXCOLOR g_EnterCol;
STARTT g_StartState;
int g_nCntStateTile;
bool g_TutorialState;

//========================================================================
//チュートリアル初期化処理
//========================================================================
void InitTutorial(void)
{
	//デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	for (int nCntTexture = 0; nCntTexture < NUM_TEX; nCntTexture++)
	{
		D3DXCreateTextureFromFile(pDevice, c_apFilenameTutorial[nCntTexture], &g_pTextureTutorial[nCntTexture]);
	}

	//グローバル宣言の初期化
	g_pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
	g_EnterCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_StartState = STARTT_IN;
	g_nCntStateTile = 0;
	g_TutorialState = false;

	//=============================================================================
	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_TEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTutorial,
		NULL);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTutorial->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定(タイトル背景)
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(1280.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 720.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(1280.0f, 720.0f, 0.0f);
	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
	//頂点カラー(0.0f～1.0f内で設定)
	pVtx[0].col = D3DXCOLOR(1.0, 1.0, 1.0, 1.0);
	pVtx[1].col = D3DXCOLOR(1.0, 1.0, 1.0, 1.0);
	pVtx[2].col = D3DXCOLOR(1.0, 1.0, 1.0, 1.0);
	pVtx[3].col = D3DXCOLOR(1.0, 1.0, 1.0, 1.0);
	//テクスチャの座標設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffTutorial->Unlock();
}

//========================================================================
//チュートリアルの終了処理
//========================================================================
void UninitTutorial(void)
{
	//変数宣言
	int nCntTutorial;

	for (nCntTutorial = 0; nCntTutorial < NUM_TEX; nCntTutorial++)
	{
		//テクスチャの破棄
		if (g_pTextureTutorial[nCntTutorial] != NULL)
		{
			g_pTextureTutorial[nCntTutorial]->Release();
			g_pTextureTutorial[nCntTutorial] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffTutorial != NULL)
	{
		g_pVtxBuffTutorial->Release();
		g_pVtxBuffTutorial = NULL;
	}
}

//========================================================================
//チュートリアルの更新処理
//========================================================================
void UpdateTutorial(void)
{
	//フェード情報の取得
	Fade pFade = GetFade();
	
	if (pFade == FADE_NONE)
	{
		if (GetKeyboardTrigger(DIK_RETURN) == true || GetJoyPadTrigger(BUTTON_A, 0) == true && pFade == FADE_NONE)
		{//決定キー(ENTERキー)が押された
			//モードの設定(ゲーム画面に移行)
			SetFade(MODE_GAME);
		}
	}
}

//========================================================================
//チュートリアルの描画処理
//========================================================================
void DrawTutorial(void)
{
	//変数宣言
	int nCntTutorial;

	//デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice;
	
	//デバイスの取得
	pDevice = GetDevice();

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//=============================================================================
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTutorial, 0,
		sizeof(VERTEX_2D));			//頂点情報構造体のサイズ
	
	for (nCntTutorial = 0; nCntTutorial < NUM_TEX; nCntTutorial++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureTutorial[nCntTutorial]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,				//プリミティブの種類
			nCntTutorial * 4,						//描画する最初のインデックス(大事)
			2);						//プリミティブ(ポリゴン)数
	}
	
}
