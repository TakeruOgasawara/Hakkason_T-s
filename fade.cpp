//======================================================================================
//
//フェード処理[Bg.cpp]
//Author;小笠原　彪
//
//======================================================================================
#include "fade.h"

//マクロ定義
#define FADE_TIME		(0.02f)			//フェードの間隔

//グローバル宣言
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFade = NULL;		//頂点バッファへのポインタ
MODE g_modeNext;									//次の画面(モード)
D3DXCOLOR g_colorFade;								//ポリゴン(フェード)の色
Fade g_fade;										//フェードの状態

//========================================================================
//フェードの初期化処理
//========================================================================
void InitFade(MODE modeNext)
{
	//デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//フェードイン状態
	g_fade = FADE_IN;

	//次の画面情報
	g_modeNext = modeNext;

	//黒いポリゴンを不透明に
	g_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffFade,
		NULL);

	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

	//頂点情報の設定
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
	pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffFade->Unlock();

	//モードの設定
	SetMode(g_modeNext);
}

//========================================================================
//フェードの終了処理
//========================================================================
void UninitFade(void)
{
	//頂点バッファの破棄
	if (g_pVtxBuffFade != NULL)
	{
		g_pVtxBuffFade->Release();
		g_pVtxBuffFade = NULL;
	}
}

//========================================================================
//フェードの更新処理
//========================================================================
void UpdateFade(void)
{
	if (g_fade != FADE_NONE)
	{
		if (g_fade == FADE_IN)
		{//フェードイン状態
			g_colorFade.a -= FADE_TIME;			//ポリゴンを透明にしていく

			if (g_colorFade.a <= 0.0f)
			{
				g_colorFade.a = 0.0f;
				g_fade = FADE_NONE;				//何もしていない状態
			}
		}
		else if (g_fade == FADE_OUT)
		{//フェードアウト状態
			g_colorFade.a += FADE_TIME;			//ポリゴンを不透明にしていく

			if (g_colorFade.a >= 1.0f)
			{
				g_colorFade.a = 1.0f;
				g_fade = FADE_IN;				//フェードイン状態

				//モードの設定(次の画面に移動)
				SetMode(g_modeNext);
			}
		}
		//頂点情報へのポインタ
		VERTEX_2D *pVtx;

		//頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

		//頂点カラー(0.0f～1.0f内で設定)
		pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_colorFade.a);
		pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_colorFade.a);
		pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_colorFade.a);
		pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_colorFade.a);

		//頂点バッファをアンロックする
		g_pVtxBuffFade->Unlock();
	}
}

//========================================================================
//フェードの描画処理
//========================================================================
void DrawFade(void)
{
	//デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffFade, 0,
		sizeof(VERTEX_2D));			//頂点情報構造体のサイズ

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, NULL);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,				//プリミティブの種類
		0,						//描画する最初のインデックス(大事)
		2);						//プリミティブ(ポリゴン)数
}

//========================================================================
//フェードの設定処理
//========================================================================
void SetFade(MODE modeNext)
{
	//フェードアウト状態
	g_fade = FADE_OUT;

	//次の画面(モード)を設定
	g_modeNext = modeNext;

	//黒いポリゴンを透明にする
	g_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
}

//========================================================================
//フェードの取得処理
//========================================================================
Fade GetFade(void)
{
	return g_fade;
}
