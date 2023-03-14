//======================================================================================
//
//フェード処理[Bg.cpp]
//Author;小笠原　彪
//
//======================================================================================
#include"game.h"
#include"pause.h"
#include"input.h"
#include"fade.h"

#define NUM_PAUSE_TEX		(4)			//テクスチャーの数
#define PAUSE_MENU			(3)			//ポーズメニューの数
#define MAX_BUFF			(2)			//バッファの数

//テクスチャファイル名
const char *c_apFilenamePause[NUM_PAUSE_TEX] =
{
	"data\\TEXTURE\\no.png",
	"data\\TEXTURE\\Pause_000.png",
	"data\\TEXTURE\\Pause_100.png",
	"data\\TEXTURE\\Pause_200.png"
};

//プロトタイプ宣言
void InitFrame(void);
void InitMenu(void);

//グローバル宣言
LPDIRECT3DTEXTURE9 g_pTexturePause[NUM_PAUSE_TEX] = {};					//テクスチャーへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause[MAX_BUFF] = {};					//頂点バッファへのポインタ
Pause g_Pause;
D3DXCOLOR g_NormalPauseColor;
D3DXCOLOR g_PalePauseColor;
int g_CurrentNumber;

//========================================================================
//ポーズの初期化処理
//========================================================================
void InitPause()
{
	//デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//ポーズ情報の取得
	Pause *pPause = GetPause();

	//テクスチャの読み込み
	for (int nCntTexture = 0; nCntTexture < NUM_PAUSE_TEX; nCntTexture++)
	{
		D3DXCreateTextureFromFile(pDevice, c_apFilenamePause[nCntTexture], &g_pTexturePause[nCntTexture]);
	}

	//構造体の初期化
	g_Pause.bPause = false;

	//グローバル宣言の初期化
	g_NormalPauseColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);				//選択されているとき
	g_PalePauseColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);			//選択されていないとき
	g_CurrentNumber = 0;

	//===============================メニュー枠=========================================
	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY, pDevice->SetFVF(FVF_VERTEX_2D), D3DPOOL_MANAGED,
		&g_pVtxBuffPause[0], NULL);

	InitFrame();

	//===============================メニュー欄=========================================
	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * PAUSE_MENU,
		D3DUSAGE_WRITEONLY, pDevice->SetFVF(FVF_VERTEX_2D), D3DPOOL_MANAGED,
		&g_pVtxBuffPause[1], NULL);

	InitMenu();
}

//========================================================================
//ポーズ枠の初期化処理
//========================================================================
void InitFrame(void)
{
	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

	g_Pause.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//頂点バッファをロック
	g_pVtxBuffPause[0]->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_Pause.pos.x, g_Pause.pos.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Pause.pos.x + 1280.0f, g_Pause.pos.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Pause.pos.x, g_Pause.pos.y + 720, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Pause.pos.x + 1280.0f, g_Pause.pos.y + 720.0f, 0.0f);
	//rhwの設定
	for (int nCntrhw = 0; nCntrhw < 4; nCntrhw++)
	{
		pVtx[nCntrhw].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[nCntrhw].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);
	}
	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffPause[0]->Unlock();
}

//========================================================================
//ポーズメニュー欄の初期化処理
//========================================================================
void InitMenu(void)
{
	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffPause[1]->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPause = 0; nCntPause < PAUSE_MENU; nCntPause++)
	{
		g_Pause.pos = D3DXVECTOR3(SCREEN_WIDTH / 2, 300.0f, 0.0f);

		if (nCntPause == 0)
		{
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_Pause.pos.x - 100.0f, g_Pause.pos.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_Pause.pos.x + 100.0f, g_Pause.pos.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_Pause.pos.x - 100.0f, g_Pause.pos.y + 50.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_Pause.pos.x + 100.0f, g_Pause.pos.y + 50.0f, 0.0f);
			//rhwの設定
			for (int nCntrhw = 0; nCntrhw < 4; nCntrhw++)
			{
				pVtx[nCntrhw].rhw = 1.0f;

				//頂点カラーの設定
				pVtx[nCntrhw].col = g_NormalPauseColor;
			}
			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			pVtx += 4;					//頂点データを4つ分ずらす
		}
		else
		{
			g_Pause.pos = D3DXVECTOR3(SCREEN_WIDTH / 2, 300.0f + 75.0f * nCntPause, 0.0f);

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_Pause.pos.x - 100.0f, g_Pause.pos.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_Pause.pos.x + 100.0f, g_Pause.pos.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_Pause.pos.x - 100.0f, g_Pause.pos.y + 50.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_Pause.pos.x + 100.0f, g_Pause.pos.y + 50.0f, 0.0f);
			//rhwの設定
			for (int nCntrhw = 0; nCntrhw < 4; nCntrhw++)
			{
				pVtx[nCntrhw].rhw = 1.0f;

				//頂点カラーの設定
				pVtx[nCntrhw].col = g_PalePauseColor;
			}
			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			pVtx += 4;					//頂点データを4つ分ずらす
		}
	}

	//頂点バッファをアンロックする
	g_pVtxBuffPause[1]->Unlock();
}


//========================================================================
//ポーズの終了処理
//========================================================================
void UninitPause()
{
	int nCntPause;						//forカウント用

	for (nCntPause = 0; nCntPause < NUM_PAUSE_TEX; nCntPause++)
	{
		//テクスチャの破棄
		if (g_pTexturePause[nCntPause] != NULL)
		{
			g_pTexturePause[nCntPause]->Release();
			g_pTexturePause[nCntPause] = NULL;
		}
	}
	for (int nCntBuff = 0; nCntBuff < MAX_BUFF; nCntBuff++)
	{
		//頂点バッファの破棄
		if (g_pVtxBuffPause[nCntBuff] != NULL)
		{
			g_pVtxBuffPause[nCntBuff]->Release();
			g_pVtxBuffPause[nCntBuff] = NULL;
		}
	}
}

//========================================================================
//ポーズの更新処理
//========================================================================
void UpdatePause()
{
	//ポーズ情報の取得
	Pause *pPause = GetPause();

	//フェード情報の取得
	Fade pfade = GetFade();

	//頂点情報のポインタ
	VERTEX_2D *pVtx;

	if (/*GetKeyboardTrigger(DIK_S) == true || */GetJoyPadTrigger(BUTTON_1, 0) == true && pfade == FADE_NONE)
	{
		//頂点バッファをロック
		g_pVtxBuffPause[1]->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += g_CurrentNumber * 4;

		//頂点カラーの設定
		pVtx[0].col = g_PalePauseColor;
		pVtx[1].col = g_PalePauseColor;
		pVtx[2].col = g_PalePauseColor;
		pVtx[3].col = g_PalePauseColor;
		
		//頂点バッファをアンロックする
		g_pVtxBuffPause[1]->Unlock();

		//現在地を先に
		g_CurrentNumber = (g_CurrentNumber + 1) % PAUSE_MENU;

		//頂点バッファをロック
		g_pVtxBuffPause[1]->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += g_CurrentNumber * 4;

		//頂点カラーの設定
		pVtx[0].col = g_NormalPauseColor;
		pVtx[1].col = g_NormalPauseColor;
		pVtx[2].col = g_NormalPauseColor;
		pVtx[3].col = g_NormalPauseColor;

		//頂点バッファをアンロックする
		g_pVtxBuffPause[1]->Unlock();
	}
	else if (/*GetKeyboardTrigger(DIK_W) == true || */GetJoyPadTrigger(BUTTON_0, 0) == true && pfade == FADE_NONE)
	{
		//頂点バッファをロック
		g_pVtxBuffPause[1]->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += g_CurrentNumber * 4;

		//頂点カラーの設定
		pVtx[0].col = g_PalePauseColor;
		pVtx[1].col = g_PalePauseColor;
		pVtx[2].col = g_PalePauseColor;
		pVtx[3].col = g_PalePauseColor;

		//頂点バッファをアンロックする
		g_pVtxBuffPause[1]->Unlock();

		//現在地を前へ
		g_CurrentNumber = (g_CurrentNumber - 1 + PAUSE_MENU) % PAUSE_MENU;
		
		//頂点バッファをロック
		g_pVtxBuffPause[1]->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += 4 * g_CurrentNumber;

		//頂点カラーの設定
		pVtx[0].col = g_NormalPauseColor;
		pVtx[1].col = g_NormalPauseColor;
		pVtx[2].col = g_NormalPauseColor;
		pVtx[3].col = g_NormalPauseColor;

		//頂点バッファをアンロックする
		g_pVtxBuffPause[1]->Unlock();
	}

	if (pfade == FADE_NONE)
	{
		if (GetKeyboardTrigger(DIK_RETURN) == true || GetJoyPadTrigger(BUTTON_A, 0) == true && g_CurrentNumber == 0 && pfade == FADE_NONE)
		{//決定キー(ENTERキー)が押された
		 //ポーズ状態を解除する
			pPause->bPause = false;
		}
		else if (GetKeyboardTrigger(DIK_RETURN) == true || GetJoyPadTrigger(BUTTON_A, 0) == true && g_CurrentNumber == 1 && pfade == FADE_NONE)
		{//決定キー(ENTERキー)が押された
		 //モードの設定(ゲーム画面に移行)
			SetFade(MODE_GAME);
		}
		else if (GetKeyboardTrigger(DIK_RETURN) == true || GetJoyPadTrigger(BUTTON_A, 0) == true && g_CurrentNumber == 2 && pfade == FADE_NONE)
		{//決定キー(ENTERキー)が押された
		 //モードの設定(タイトル画面に移行)
			SetFade(MODE_TITLE);
		}
	}
}

//========================================================================
//ポーズの描画処理
//========================================================================
void DrawPause()
{
	//デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//=============================================================================
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPause[0], 0,
		sizeof(VERTEX_2D));			//頂点情報構造体のサイズ
		
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTexturePause[0]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,				//プリミティブの種類
			0,						//描画する最初のインデックス
			2);						//プリミティブ(ポリゴン)数

	//=============================================================================
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPause[1], 0,
		sizeof(VERTEX_2D));			//頂点情報構造体のサイズ

	for (int nCntPause = 1; nCntPause < PAUSE_MENU + 1; nCntPause++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTexturePause[nCntPause]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,				//プリミティブの種類
			nCntPause * 4 - 4,						//描画する最初のインデックス
			2);						//プリミティブ(ポリゴン)数
	}
}

//========================================================================
//ポーズ情報の取得
//========================================================================
Pause *GetPause(void)
{
	return &g_Pause;
}