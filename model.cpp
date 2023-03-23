//*****************************************
//
//モデル管理(model.cpp)
//Author:髙山桃也
//
//*****************************************

//*****************************************
// インクルード
//*****************************************
#include "main.h"
#include "model.h"
#include <stdio.h>

//*****************************************
// マクロ定義
//*****************************************
#define MAX_STRING	(256)	//文字数の最大

//*****************************************
// グローバル変数宣言
//*****************************************
Model g_aModelPlayer[MAX_PART];	//プレイヤーモデルの情報

int g_nNumPlayerPart;	//プレイヤーパーツの数

//========================================
//初期化処理
//========================================
void InitModel(void)
{
	//プレイヤーモデル情報初期化
	ZeroMemory(&g_aModelPlayer[0], sizeof(Model) * MAX_PART);
}

//========================================
// 終了処理
//========================================
void UninitModel(void)
{
	for (int nCntPart = 0; nCntPart < MAX_PART; nCntPart++)
	{//プレイヤーモデル終了処理
		UninitXFile(&g_aModelPlayer[nCntPart]);
	}
}

//========================================
// 読み込み処理まとめ
//========================================
void LoadAllModel(void)
{
	LoadPlayermodel();
}

//========================================
// プレイヤーモデル読み込み
//========================================
void LoadPlayermodel(void)
{
	//変数宣言
	char cTemp[MAX_STRING];
	int nCntMotion = 0;
	char acFilenamePlayer[MAX_PART][MAX_STRING];

	//ファイルからモデルを読み込む
	FILE *pFile = fopen("data\\MOTION\\player.txt", "r");

	while (1)
	{
		//文字読み込み
		fscanf(pFile, "%s", &cTemp[0]);

		//ファイル名読み込み=========================================
		if (strcmp(cTemp, "NUM_MODEL") == 0)
		{//モデル数読み込み
			//"="読み込み
			fscanf(pFile, "%s", &cTemp[0]);

			//モデル数読み込み
			fscanf(pFile, "%d", &g_nNumPlayerPart);

			for (int nCntFile = 0; nCntFile < g_nNumPlayerPart;)
			{//ファイル名読み込み
			 //文字読み込み
				fscanf(pFile, "%s", &cTemp[0]);

				if (strcmp(cTemp, "MODEL_FILENAME") == 0)
				{//ファイル名読み込み
				 //"="読み込み
					fscanf(pFile, "%s", &cTemp[0]);

					//ファイル名取得
					fscanf(pFile, "%s", &acFilenamePlayer[nCntFile][0]);

					//Xファイルの読み込み
					LoadModel(&g_aModelPlayer[nCntFile], acFilenamePlayer[nCntFile]);

					//読み込んだモデル数カウンタ加算
					nCntFile++;
				}
			}
		}
		//ファイル名読み込み=========================================

		if (strcmp(cTemp, "END_SCRIPT") == 0)
		{
			break;
		}
	}
}

//========================================
// モデルの読み込み(汎用)
//========================================
void LoadModel(Model *pModel,const char *pFilePass)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	//計算用ローカル変数宣言
	int nNumVtx;	//頂点数
	DWORD dwSizeFVF;	//頂点フォーマットのサイズ
	BYTE *pVtxBuff;	//頂点バッファへのポインタ
	
	//マテリアルデータへのポインタ
	D3DXMATERIAL *pMat;

	//Xファイルの読み込み
	D3DXLoadMeshFromX(pFilePass,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&pModel->pBuffMat,
		NULL,
		&pModel->dwNumMat,
		&pModel->pMesh);

	//マテリアル情報に対するポインタを取得
	pMat = (D3DXMATERIAL*)pModel->pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)pModel->dwNumMat; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{
			//テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice,
				pMat[nCntMat].pTextureFilename,
				&pModel->apTexture[nCntMat]);
		}
	}

	//最大・最小頂点数取得================================================
	//頂点数を取得
	nNumVtx = pModel->pMesh->GetNumVertices();

	//頂点フォーマットのサイズを取得
	dwSizeFVF = D3DXGetFVFVertexSize(pModel->pMesh->GetFVF());

	//頂点バッファのロック
	pModel->pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntObject = 0; nCntObject < nNumVtx; nCntObject++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;			//頂点座標の代入

		if (vtx.x < pModel->vtxMin.x)
		{//X軸の最小
			pModel->vtxMin.x = vtx.x;
		}
		if (vtx.x > pModel->vtxMax.x)
		{//X軸の最大
			pModel->vtxMax.x = vtx.x;
		}

		if (vtx.y < pModel->vtxMin.y)
		{//Y軸の最小
			pModel->vtxMin.y = vtx.y;
		}
		if (vtx.y > pModel->vtxMax.y)
		{//Y軸の最大
			pModel->vtxMax.y = vtx.y;
		}

		if (vtx.z < pModel->vtxMin.z)
		{//Z軸の最小
			pModel->vtxMin.z = vtx.z;
		}
		if (vtx.z > pModel->vtxMax.z)
		{//Z軸の最大
			pModel->vtxMax.z = vtx.z;
		}

		pVtxBuff += dwSizeFVF;			//頂点フォーマットのサイズ分ポインタを進める
	}

	//頂点バッファをアンロック
	pModel->pMesh->UnlockVertexBuffer();
	//最大・最小頂点数取得================================================
}

//========================================
// プレイヤーモデル情報取得
//========================================
Model *GetplayerModel(void)
{
	return &g_aModelPlayer[0];
}

//========================================
// プレイヤーモデル数取得
//========================================
int GetNumPlayerPart(void)
{
	return g_nNumPlayerPart;
}

//========================================
// 汎用終了処理
//========================================
void UninitXFile(Model *pModel)
{
	if (pModel->pBuffMat != NULL)
	{//マテリアルの破棄
		pModel->pBuffMat->Release();
		pModel->pBuffMat = NULL;
	}

	if (pModel->pMesh != NULL)
	{//メッシュの破棄
		pModel->pMesh->Release();
		pModel->pMesh = NULL;
	}

	for (int nCntTex = 0; nCntTex < NUM_TEX; nCntTex++)
	{//テクスチャの破棄
		if (pModel->apTexture[nCntTex] != NULL)
		{
			pModel->apTexture[nCntTex]->Release();
			pModel->apTexture[nCntTex] = NULL;
		}
	}
}