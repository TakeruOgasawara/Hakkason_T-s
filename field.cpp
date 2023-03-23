//======================================================================================
//
// ポリゴンの処理[polygon.cpp]
// Author;小笠原　彪
//
//======================================================================================
#include "field.h"

//*****************************
//マクロ定義
//*****************************
#define BG_TEX_FULE		"data\\TEXTURE\\GAME\\do-ro.png"			//奥背景の画像を読み込む
#define FILED_WIDTH			(150.0f)
#define FILED_HEIGHT		(600000.0f)		//500
#define MAX_FIELD			(128)			//フィールドの最大数

//*****************************
//グローバル定義
//*****************************
LPDIRECT3DTEXTURE9 g_pTextureField = NULL;				//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffField = NULL;			//頂点バッファへのポインタ
Field g_field[MAX_FIELD];			//フィールド情報

//====================================================================
//ポリゴンの初期化処理
//====================================================================
void InitField(void)
{
	//変数宣言
	int nCntField;			//forカウント用

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, BG_TEX_FULE, &g_pTextureField);

	//変数の初期化
	for (nCntField = 0; nCntField < MAX_FIELD; nCntField++)
	{
		g_field[nCntField].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_field[nCntField].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_field[nCntField].bUse = false;
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_FIELD,
		D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED,
		&g_pVtxBuffField, 
		NULL);

	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffField->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntField = 0; nCntField < MAX_FIELD; nCntField++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_field[nCntField].pos .x - FILED_WIDTH, 0.0f, g_field[nCntField].pos.x + FILED_HEIGHT);
		pVtx[1].pos = D3DXVECTOR3(g_field[nCntField].pos.x + FILED_WIDTH, 0.0f, g_field[nCntField].pos.x + FILED_HEIGHT);
		pVtx[2].pos = D3DXVECTOR3(g_field[nCntField].pos.x - FILED_WIDTH, 0.0f, g_field[nCntField].pos.x - FILED_HEIGHT);
		pVtx[3].pos = D3DXVECTOR3(g_field[nCntField].pos.x + FILED_WIDTH, 0.0f, g_field[nCntField].pos.x - FILED_HEIGHT);
		//法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		//頂点カラー(0.0f～1.0f内で設定)
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		//テクスチャの座標設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1200.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1200.0f);

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffField->Unlock();

	SetField(D3DXVECTOR3(0.0f, 0.0f, 599900.0f));
}

//====================================================================
//ポリゴンの終了処理
//====================================================================
void UninitField(void)
{
	//テクスチャの破棄
	if (g_pTextureField != NULL)
	{
		g_pTextureField->Release();
		g_pTextureField = NULL;
	}
	//頂点バッファの破棄
	if (g_pVtxBuffField != NULL)
	{
		g_pVtxBuffField->Release();
		g_pVtxBuffField = NULL;
	}
}

//====================================================================
//ポリゴンの更新処理
//====================================================================
void UpdateField(void)
{

}

//====================================================================
//ポリゴンの描画処理
//====================================================================
void DrawField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス
	
	for (int nCntField = 0; nCntField < MAX_FIELD; nCntField++)
	{
		if (g_field[nCntField].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_field[nCntField].mtxWorldField);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_field[nCntField].rot.y, g_field[nCntField].rot.x, g_field[nCntField].rot.z);
			D3DXMatrixMultiply(&g_field[nCntField].mtxWorldField, &g_field[nCntField].mtxWorldField, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_field[nCntField].pos.x, g_field[nCntField].pos.y, g_field[nCntField].pos.z);
			D3DXMatrixMultiply(&g_field[nCntField].mtxWorldField, &g_field[nCntField].mtxWorldField, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_field[nCntField].mtxWorldField);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffField, 0,
				sizeof(VERTEX_3D));			//頂点情報構造体のサイズ

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureField);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,				//プリミティブの種類
				nCntField * 4,						//描画する最初のインデックス
				2);
		}
	}
}

//====================================================================
// フィールドの設定処理
//====================================================================
void SetField(D3DXVECTOR3 pos)
{
	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffField->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntField = 0; nCntField < MAX_FIELD; nCntField++)
	{
		if (g_field[nCntField].bUse == false)
		{
			g_field[nCntField].pos = pos;

			g_field[nCntField].bUse = true;

			break;
		}
		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffField->Unlock();
}