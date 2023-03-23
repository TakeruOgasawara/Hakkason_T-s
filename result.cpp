//======================================================================================
//
//リザルト処理[result.cpp]
//Author;小笠原　彪
//
//======================================================================================
#include "result.h"
#include "main.h"
#include "input.h"
#include "fade.h"
#include "title.h"
#include "sound.h"
#include "point.h"
#include "point_log.h"

//画像のファイル名
#define MAX_TEX		(1)

//リザルト構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;
} Result;

//テクスチャファイル名
const char *c_apFilenameResult[MAX_TEX] =
{
	"data\\TEXTURE\\RESULT\\result.png",			//背景の画像を読み込む
};

//プロトタイプ宣言
void ResultBg(void);

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureResult[MAX_TEX] = {};				//テクスチャーへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult = NULL;				//頂点バッファへのポインタ
Result g_Result;

//========================================================================
// リザルトの初期化処理
//========================================================================
void InitResult(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	for (int nCntTexture = 0; nCntTexture < MAX_TEX; nCntTexture++)
	{
		D3DXCreateTextureFromFile(pDevice, c_apFilenameResult[nCntTexture], &g_pTextureResult[nCntTexture]);
	}

	g_Result.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TEX,	//大事
		D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED,
		&g_pVtxBuffResult,
		NULL);

	//リザルト背景の初期化
	ResultBg();

	//取得スコア初期化
	InitPoint();

	//スコアメッセージ初期化
	InitPointLog();
}

//---------------------------------------------------------
// リザルト背景
//---------------------------------------------------------
void ResultBg(void)
{
	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_Result.pos.x, g_Result.pos.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Result.pos.x + 1280.0f, g_Result.pos.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Result.pos.x, g_Result.pos.y + 720.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Result.pos.x + 1280.0f, g_Result.pos.y + 720.0f, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラー(0.0f～1.0f内で設定)
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャの座標設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffResult->Unlock();
}

//========================================================================
// リザルトの終了処理
//========================================================================
void UninitResult(void)
{
	//サウンドの停止
	//StopSound();

	//テクスチャの破棄
	for (int nCnt = 0; nCnt < MAX_TEX; nCnt++)
	{
		if (g_pTextureResult[nCnt] != NULL)
		{
			g_pTextureResult[nCnt]->Release();
			g_pTextureResult[nCnt] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffResult != NULL)
	{
		g_pVtxBuffResult->Release();
		g_pVtxBuffResult = NULL;
	}

	//取得スコア終了
	UninitPoint();

	//スコアメッセージ終了
	UninitPointLog();
}

//========================================================================
// リザルトの更新処理
//========================================================================
void UpdateResult(void)
{
	//フェード情報の取得
	Fade pFade = GetFade();

	if (pFade == FADE_NONE)
	{//フェード状態じゃない場合

		if (GetKeyboardTrigger(DIK_RETURN) == true || GetJoyPadTrigger(BUTTON_A, 0) == true)
		{//決定キー(ENTERキー)が押された
			//モードの設定(タイトル画面に移行)
			SetFade(MODE_TITLE);
		}
	}

	//取得スコア更新
	UpdatePoint();

	//スコアメッセージ更新
	UpdatePointLog();
}

//========================================================================
// リザルトの描画処理
//========================================================================
void DrawResult(void)
{
	//デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();;

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffResult, 0,
		sizeof(VERTEX_2D));			//頂点情報構造体のサイズ

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCnt = 0; nCnt < MAX_TEX; nCnt++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureResult[nCnt]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,				//プリミティブの種類
			nCnt * 4,						//描画する最初のインデックス(大事)
			2);						//プリミティブ(ポリゴン)数
	}

	//取得スコア描画
	DrawPoint();

	//スコアメッセージ描画
	DrawPointLog();
}
