//======================================================================================
//
// メッシュフィールドの処理[polygon.cpp]
// Author;小笠原　彪
//
//======================================================================================
#include "meshfield.h"

//**********************************
//マクロ定義
//**********************************
#define MAX_TEX			(1)
#define FILED_WIDTH		(200.0f)			//横幅
#define FIELD_HEIGHT	(500.0f)			//縦幅

#define SPLIT_X			(2)					//分割数(x)
#define SPLIT_Y			(2)					//分割数(z)

//***********************************
//テクスチャの読み込み
//***********************************
const char *c_apTexMeshFeald[] =
{
	"data\\TEXTURE\\GAME\\do-ro.png"
};

//***********************************
//グローバル定義
//***********************************
LPDIRECT3DTEXTURE9 g_pTextureMeshField[MAX_TEX] = {};				//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshField = NULL;			//頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshfield = NULL;			//インデックスバッファへのポインタ
MeshField g_aMeshfield[MAX_MESHFIELD];			//フィールド情報

//====================================================================
// メッシュフィールドの初期化処理
//====================================================================
void InitMeshField(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntTex = 0; nCntTex < MAX_TEX; nCntTex++)
	{//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, c_apTexMeshFeald[nCntTex], &g_pTextureMeshField[nCntTex]);
	}

	//変数の初期化
	for (int nCnt = 0; nCnt < MAX_MESHFIELD; nCnt++)
	{
		g_aMeshfield[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aMeshfield[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 14,
		D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED,
		&g_pVtxBuffMeshField, 
		NULL);

	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffMeshField->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	/*pVtx[0].pos = D3DXVECTOR3(-FILED_WIDTH, 0.0f, FIELD_HEIGHT);
	pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, FIELD_HEIGHT);
	pVtx[2].pos = D3DXVECTOR3(FILED_WIDTH, 0.0f, FIELD_HEIGHT);
	pVtx[3].pos = D3DXVECTOR3(-FILED_WIDTH, 0.0f, 0.0f);
	pVtx[4].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(FILED_WIDTH, 0.0f, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(-FILED_WIDTH, 0.0f, -FIELD_HEIGHT);
	pVtx[7].pos = D3DXVECTOR3(0.0f, 0.0f, -FIELD_HEIGHT);
	pVtx[8].pos = D3DXVECTOR3(FILED_WIDTH, 0.0f, -FIELD_HEIGHT);*/

	//変数の初期化
	int nCnt = 0;

	for (int nCnt0 = 0; nCnt0 < SPLIT_Y + 1; nCnt0++)
	{
		for (int nCnt1 = 0; nCnt1 < SPLIT_X + 1; nCnt1++)
		{
			//頂点座標の計算(x)
			pVtx[nCnt].pos.x = nCnt1 * (FILED_WIDTH * 0.5f) - (FILED_WIDTH * 0.5f);

			//頂点座標の計算(z)
			pVtx[nCnt].pos.z = (FIELD_HEIGHT * 0.5f) - nCnt0 * (FIELD_HEIGHT * 0.5f);

			if (nCnt < ((SPLIT_X + 1) * (SPLIT_Y + 1)))
			{
				//カウント
				nCnt++;
			}
		}
	}

	for (int nCntColor = 0; nCntColor < ((SPLIT_X + 1) * (SPLIT_Y + 1)); nCntColor++)
	{
		//法線ベクトルの設定
		pVtx[nCntColor].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		pVtx[nCntColor].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	//テクスチャの座標設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[3].tex = D3DXVECTOR2(0.0f, 0.5f);
	pVtx[4].tex = D3DXVECTOR2(0.5f, 0.5f);
	pVtx[5].tex = D3DXVECTOR2(1.0f, 0.5f);
	pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(0.5f, 1.0f);
	pVtx[8].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffMeshField->Unlock();

	//インデックスバッファの生成(番号を覚えるためのメモリ)
	pDevice->CreateIndexBuffer(sizeof(WORD) * 14,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshfield, NULL);

	WORD *pIdx;			//インデックス情報へのポインタ

	//インデックスバッファをロックし、頂点番号データへのポインタを取得
	g_pIdxBuffMeshfield->Lock(0, 0, (void**)&pIdx, 0);

	//頂点番号データの設定
	pIdx[0] = 3;
	pIdx[1] = 0;
	pIdx[2] = 4;
	pIdx[3] = 1;
	pIdx[4] = 5;
	pIdx[5] = 2;
	pIdx[6] = 2;
	pIdx[7] = 6;
	pIdx[8] = 6;
	pIdx[9] = 3;
	pIdx[10] = 7;
	pIdx[11] = 4;
	pIdx[12] = 8;
	pIdx[13] = 5;

	//インデックスバッファをアンロックする
	g_pIdxBuffMeshfield->Unlock();

	SetMeshField(D3DXVECTOR3(0.0f, 0.0f, 100.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	SetMeshField(D3DXVECTOR3(100.0f, 0.0f, 600.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
}

//====================================================================
// メッシュフィールドの終了処理
//====================================================================
void UninitMeshField(void)
{
	for (int nCntTex = 0; nCntTex < MAX_TEX; nCntTex++)
	{
		//テクスチャの破棄
		if (g_pTextureMeshField[nCntTex] != NULL)
		{
			g_pTextureMeshField[nCntTex]->Release();
			g_pTextureMeshField[nCntTex] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffMeshField != NULL)
	{
		g_pVtxBuffMeshField->Release();
		g_pVtxBuffMeshField = NULL;
	}
	//インデックスバッファの破棄
	if (g_pIdxBuffMeshfield != NULL)
	{
		g_pIdxBuffMeshfield->Release();
		g_pIdxBuffMeshfield = NULL;
	}
}

//====================================================================
// メッシュフィールドの更新処理
//====================================================================
void UpdateMeshField(void)
{
	
}

//====================================================================
// メッシュフィールドの描画処理
//====================================================================
void DrawMeshField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス
	
	for (int nCnt = 0; nCnt < MAX_MESHFIELD; nCnt++)
	{
		if (g_aMeshfield[nCnt].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aMeshfield[nCnt].mtxWorldField);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aMeshfield[nCnt].rot.y, g_aMeshfield[nCnt].rot.x, g_aMeshfield[nCnt].rot.z);
			D3DXMatrixMultiply(&g_aMeshfield[nCnt].mtxWorldField, &g_aMeshfield[nCnt].mtxWorldField, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aMeshfield[nCnt].pos.x, g_aMeshfield[nCnt].pos.y, g_aMeshfield[nCnt].pos.z);
			D3DXMatrixMultiply(&g_aMeshfield[nCnt].mtxWorldField, &g_aMeshfield[nCnt].mtxWorldField, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aMeshfield[nCnt].mtxWorldField);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffMeshField, 0,
				sizeof(VERTEX_3D));			//頂点情報構造体のサイズ

			//インデックスバッファをデータストリームに設定
			pDevice->SetIndices(g_pIdxBuffMeshfield);

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);
		
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureMeshField[0]);

			//ポリゴンの描画
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0,
				9,			//用意した頂点の数
				4 * nCnt,
				12);		//描画するプリミティブの数
		}
	}
}

//====================================================================
// メッシュフィールドの設定処理
//====================================================================
void SetMeshField(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nTexType)
{
	for (int nCnt = 0; nCnt < MAX_MESHFIELD; nCnt++)
	{
		if (g_aMeshfield[nCnt].bUse == false)
		{
			g_aMeshfield[nCnt].pos = pos;
			g_aMeshfield[nCnt].rot = rot;
			g_aMeshfield[nCnt].nTexType = nTexType;
			g_aMeshfield[nCnt].bUse = true;
		}
	}
}