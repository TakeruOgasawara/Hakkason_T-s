//======================================================================================
//
//タイトル処理[title.cpp]
//Author;小笠原　彪
//
//======================================================================================
#include "title.h"
#include "main.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "debugproc.h"
#include "field.h"
#include "camera.h"
#include "light.h"

//*****************************
//マクロ定義
//*****************************
#define NUM_TEX_BUFF0			(1)		//使用するテクスチャ―の数
#define NUM_TEX_BUFF1			(2)		//使用するテクスチャ―の数
#define NUM_BUFF		(2)

//*****************************
// 列挙型の定義
//*****************************
typedef enum
{
	TITLESTATE_NONE,
	TITLESTATE_IN,
	TITLESTATE_OUT,
	TITLESTATE_MAX
} TITLESTATE;

//タイトルの選択モード
typedef enum
{
	TITLEMODE_START,
	TITLEMODE_TUTORIAL,
	TITLEMODE_MAX
} TITLEMODE;

//*****************************
// フェード用の列挙型
//*****************************
typedef struct
{
	D3DXVECTOR3 pos;
} Title;

//*****************************
//テクスチャファイル名
//*****************************
const char *c_apTitleTexFilename[NUM_TEX_BUFF0 + NUM_TEX_BUFF1] =
{
	"data\\TEXTURE\\TITLE\\title.png",			//タイトルロゴ
	"data\\TEXTURE\\TITLE\\start.png",			//スタートロゴ
	"data\\TEXTURE\\TILE\\tutorial.png",		//チュートリアルロゴ
};	

//*****************************
//プロトタイプ宣言
//*****************************
//Buff[0]
void InitTitleFont(void);			//タイトルロゴ
void DrawTitleFont(void);
//Buff[1]
void InitTitleStart(int nCntTitle);			//セレクトロゴ
void DrawTitleStart(void);
void InitTitleTutorial(int nCntTitle);
//その他
void TileFade(void);			//文字の点滅

//タイトル
void InitTitleCamera(void);
void UninitTitleCamera(void);
void UpdateTitleCamera(void);
void DrawTitleCamera(void);


//*****************************
//グローバル変数
//*****************************
LPDIRECT3DTEXTURE9 g_pTextureTitle[NUM_TEX_BUFF0 + NUM_TEX_BUFF1] = {};					//テクスチャーへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle[NUM_BUFF] = {};				//頂点バッファへのポインタ

Title g_Title;				//タイトル情報(構造体)
TITLESTATE g_Start;			//タイトル状態(列挙型)

D3DXVECTOR3 g_Centerpos;
D3DXCOLOR g_StartColor;
D3DXCOLOR g_StartColor1;

bool g_TitleState;
int g_nCntTitle;
int g_CurrentNumberTitle;
float g_TexU;

//========================================================================
//タイトルの初期化処理
//========================================================================
void InitTitle(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();;

	for (int nCntTex = 0; nCntTex < NUM_TEX_BUFF0 + NUM_TEX_BUFF1; nCntTex++)
	{//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, c_apTitleTexFilename[nCntTex], &g_pTextureTitle[nCntTex]);
	}

	//変数の初期化
	g_Title.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Start = TITLESTATE_IN;

	g_Centerpos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
	g_StartColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_StartColor1 = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	g_nCntTitle = 0;
	g_TitleState = false;
	g_TexU = 0.0f;

	//------------------------------------------------------------------------
	//頂点バッファの生成[0]
	//------------------------------------------------------------------------
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_TEX_BUFF0,
		D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED,
		&g_pVtxBuffTitle[0],
		NULL);

	//タイトルロゴの初期化
	InitTitleFont();

	//------------------------------------------------------------------------
	//頂点バッファの生成[1]
	//------------------------------------------------------------------------
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_TEX_BUFF1,
		D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED,
		&g_pVtxBuffTitle[1],
		NULL);

	
	//スタートロゴの初期化
	InitTitleStart(0);
		
	InitTitleCamera();

	//サウンドの再生
	PlaySound(SOUND_LABEL_BGM000);
}

//----------------------------------------
// タイトル文字の初期化
//----------------------------------------
void InitTitleFont(void)
{
	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitle[0]->Lock(0, 0, (void**)&pVtx, 0);

	g_Title.pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);

	//頂点座標の設定(タイトル文字)
	pVtx[0].pos = D3DXVECTOR3(g_Title.pos.x - 300, g_Title.pos.y - 300, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Title.pos.x + 300, g_Title.pos.y - 300, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Title.pos.x - 300, g_Title.pos.y - 70, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Title.pos.x + 300, g_Title.pos.y - 70, 0.0f);
	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
	//頂点カラー(0.0f～1.0f内で設定)
	pVtx[0].col = g_StartColor;
	pVtx[1].col = g_StartColor;
	pVtx[2].col = g_StartColor;
	pVtx[3].col = g_StartColor;
	//テクスチャの座標設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffTitle[0]->Unlock();
}

//----------------------------------------
// スタート文字の初期化
//----------------------------------------
void InitTitleStart(int nCntTitle)
{
	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitle[1]->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += 4 * nCntTitle;

	g_Title.pos = D3DXVECTOR3(SCREEN_WIDTH / 2, 500.0f, 0.0f);

	//頂点座標の設定(START文字)
	pVtx[0].pos = D3DXVECTOR3(g_Title.pos.x - 130, g_Title.pos.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Title.pos.x + 130, g_Title.pos.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Title.pos.x - 130, g_Title.pos.y + 70, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Title.pos.x + 130, g_Title.pos.y + 70, 0.0f);
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
	g_pVtxBuffTitle[1]->Unlock();
}

//----------------------------------------
// チュートリアル文字の初期化
//----------------------------------------
void InitTitleTutorial(int nCntTitle)
{
	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitle[1]->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += 4 * nCntTitle;

	g_Title.pos = D3DXVECTOR3(1100.0f, 600.0f, 0.0f);

	//頂点座標の設定(START文字)
	pVtx[0].pos = D3DXVECTOR3(g_Title.pos.x - 130, g_Title.pos.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Title.pos.x + 130, g_Title.pos.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Title.pos.x - 130, g_Title.pos.y + 70, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Title.pos.x + 130, g_Title.pos.y + 70, 0.0f);
	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
	//頂点カラー(0.0f～1.0f内で設定)
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
	//テクスチャの座標設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffTitle[1]->Unlock();
}

//========================================================================
//タイトルの終了処理
//========================================================================
void UninitTitle(void)
{
	//サウンドの停止
	StopSound();

	for (int nCntTex = 0; nCntTex < NUM_TEX_BUFF0 + NUM_TEX_BUFF1; nCntTex++)
	{
		//テクスチャの破棄
		if (g_pTextureTitle[nCntTex] != NULL)
		{
			g_pTextureTitle[nCntTex]->Release();
			g_pTextureTitle[nCntTex] = NULL;
		}
	}
	for (int nCntBuff = 0; nCntBuff < NUM_BUFF; nCntBuff++)
	{//頂点バッファの破棄
		if (g_pVtxBuffTitle[nCntBuff] != NULL)
		{
			g_pVtxBuffTitle[nCntBuff]->Release();
			g_pVtxBuffTitle[nCntBuff] = NULL;
		}
	}

	UninitTitleCamera();
}

//========================================================================
//タイトルの更新処理
//========================================================================
void UpdateTitle(void)
{
	//タイトル文字の点滅(次のモードへ)
	TileFade();

	UpdateTitleCamera();
}

//------------------------------
// フェード処理(点滅)
//------------------------------
void TileFade(void)
{
	//フェード情報の取得
	Fade pFade = GetFade();

	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

	//スタートの点滅処理
	if (pFade == FADE_NONE)
	{
		if (GetKeyboardTrigger(DIK_RETURN) == true || GetJoyPadTrigger(BUTTON_A, 0) == true)
		{//決定キー(ENTERキー)が押された
			if (g_TitleState == false)
			{
				//サウンドの再生
				PlaySound(SOUND_LABEL_SE_ENTER);

				//スタート文字を点滅状態へ
				g_TitleState = true;
			}
		}
	}

	if (g_TitleState == true)
	{
		if (g_Start == TITLESTATE_IN)
		{//フェードイン状態
			g_StartColor.a -= 0.2f;			//ポリゴンを透明にしていく

			if (g_StartColor.a <= 0.4f)
			{
				g_StartColor.a = 0.4f;

				g_Start = TITLESTATE_OUT;
			}
		}

		else if (g_Start == TITLESTATE_OUT)
		{//フェードアウト状態
			g_StartColor.a += 0.2f;			//ポリゴンを不透明にしていく

			if (g_StartColor.a >= 1.0f)
			{
				g_StartColor.a = 1.0f;

				g_Start = TITLESTATE_IN;
				g_nCntTitle++;
			}
		}
	}
	if (g_nCntTitle == 5)
	{
		//モードの設定(ゲーム画面に移行)
		SetFade(MODE_TUTORIAL);
	}

	if (g_Start == TITLESTATE_IN)
	{//フェードイン状態
		g_StartColor.a -= 0.02f;			//ポリゴンを透明にしていく

		if (g_StartColor.a <= 0.0f)
		{
			g_StartColor.a = 0.0f;

			g_Start = TITLESTATE_OUT;
		}
	}
	else if (g_Start == TITLESTATE_OUT)
	{//フェードアウト状態
		g_StartColor.a += 0.02f;			//ポリゴンを不透明にしていく

		if (g_StartColor.a >= 1.0f)
		{
			g_StartColor.a = 1.0f;

			g_Start = TITLESTATE_IN;
		}
	}

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitle[1]->Lock(0, 0, (void**)&pVtx, 0);

	//頂点カラー(0.0f～1.0f内で設定)
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_StartColor.a);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_StartColor.a);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_StartColor.a);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_StartColor.a);

	//頂点バッファをアンロックする
	g_pVtxBuffTitle[1]->Unlock();
}

//========================================================================
//タイトルの描画処理
//========================================================================
void DrawTitle(void)
{
	//タイトルカメラの描画
	DrawTitleCamera();

	//タイトルロゴの描画処理
	DrawTitleFont();

	//セレクト文字の描画処理
	DrawTitleStart();
}

//-----------------------------------------------------------
//タイトルロゴの描画処理
//-----------------------------------------------------------
void DrawTitleFont(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//=============================================================================
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTitle[0], 0,
		sizeof(VERTEX_2D));			//頂点情報構造体のサイズ

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureTitle[0]);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,				//プリミティブの種類
		0,						//描画する最初のインデックス(大事)
		2);						//プリミティブ(ポリゴン)数
	
}

//-----------------------------------------------------------
//セレクト文字の描画処理
//-----------------------------------------------------------
void DrawTitleStart(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//=============================================================================
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTitle[1], 0,
		sizeof(VERTEX_2D));			//頂点情報構造体のサイズ

	for (int nCnt = 0; nCnt < NUM_TEX_BUFF1; nCnt++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureTitle[nCnt + 1]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,				//プリミティブの種類
			nCnt * 4,						//描画する最初のインデックス(大事)
			2);						//プリミティブ(ポリゴン)数
	}
}



//============================================================
//
//============================================================
void InitTitleCamera(void)
{
	InitLight();

	InitCamera();
	
	InitField();
}

//============================================================
//
//============================================================
void UninitTitleCamera(void)
{
	UninitField();
}

//============================================================
//
//============================================================
void UpdateTitleCamera(void)
{
	//Camera *pCamera = GetCamera();

	//pCamera->rot.y += 0.004f;

	////y
	//if (pCamera->rot.y > D3DX_PI)
	//{
	//	pCamera->rot.y = -D3DX_PI;
	//}
	//if (pCamera->rot.y < -D3DX_PI)
	//{
	//	pCamera->rot.y = D3DX_PI;
	//}

	////カメラを更新する
	//pCamera->posV.x = pCamera->posR.x + (cosf(pCamera->rot.z) * sinf(pCamera->rot.y)) * -1200.0f;
	//pCamera->posV.y = pCamera->posR.y + 500.0f + sinf(pCamera->rot.z) * -1200.0f;			//視点Y
	//pCamera->posV.z = pCamera->posR.z + (cosf(pCamera->rot.z) * cosf(pCamera->rot.y)) * -1200.0f;

	UpdateCamera();
}

//============================================================
//
//============================================================
void DrawTitleCamera(void)
{
	Camera *pCamera = GetCamera();

	SetCamera(0);

	DrawField();

#ifdef _DEBUG		//デバッグ時のみ
	PrintDebugProc("【カメラ情報】\n");
	PrintDebugProc("視点     [%f  %f  %f]\n", pCamera->posV.x, pCamera->posV.y, pCamera->posV.z);
	PrintDebugProc("注視点   [%f  %f  %f]\n", pCamera->posR.x, pCamera->posR.y, pCamera->posR.z);
	PrintDebugProc("向き     [%f  %f  %f]\n", pCamera->rot.x, pCamera->rot.y, pCamera->rot.z);
	PrintDebugProc("追従切り替え[F9])\n");
#endif
}