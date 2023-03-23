//======================================================================================
//
// オブジェクトの処理[object.cpp]
// Author;小笠原　彪
//
//======================================================================================
#include <stdio.h>
#include "object.h"
#include "input.h"
#include "camera.h"
#include "player.h"

//*****************************
//マクロ定義
//*****************************
#define SIZE				(5.0f)
#define NUM_OBJ_TYPE		(2)

//モデルファイル名
const char *c_apModelnameObject[] =
{
	"data\\MODEL\\house.x",
};

//*****************************
//ステージのファイル名
//*****************************
const char *c_apObjTxtName[MAX_WAVE] =
{
	"data\\MODEL\\object\\SetObject00.txt",
};

//*****************************
//プロトタイプ宣言
//*****************************
//void ColliObj(void);
void ObjSize(int nCntObj);

//*****************************
//グローバル宣言
//*****************************
LPD3DXMESH g_pMeshObject[MAX_OBJ] = {};
LPD3DXBUFFER g_pBuffMatObject[MAX_OBJ] = {};
DWORD g_dwNumMatObject[MAX_OBJ] = {};
Object g_object[MAX_OBJ];				//オブジェクト情報
Obj g_obj[MAX_OBJ];						//テクスチャ情報
int g_SetNumObj;
int g_NumObj;
bool bLoadObj;

//====================================================================
// オブジェクトの初期化処理
//====================================================================
void InitObject(void)
{
	//変数宣言
	int nCntObj;			//forカウント用
	//int nNumVtx;			//頂点数
	//DWORD dwSizeFVF;		//頂点フォーマットのサイズ
	//BYTE *pVtxBuff;			//頂点バッファへのポインタ

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (nCntObj = 0; nCntObj < MAX_OBJ; nCntObj++)
	{//変数の初期化
		g_object[nCntObj].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_object[nCntObj].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_object[nCntObj].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_object[nCntObj].vtxMin = D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f);
		g_object[nCntObj].vtxMax = D3DXVECTOR3(-10000.0f, -10000.0f, -10000.0f);
		g_object[nCntObj].nIdxShadow = -1;
		g_object[nCntObj].nType = 0;
		g_object[nCntObj].bUse = false;

		g_obj[nCntObj].pTexture[MAX_OBJTEX] = {};
	}

	bLoadObj = false;
	g_SetNumObj = 0;
	g_NumObj = 0;

	//txtからオブジェクト情報を読み取る
	LoadObj(0);
}

//====================================================================
// オブジェクトの終了処理
//====================================================================
void UninitObject(void)
{
	//変数宣言
	int nCntObj;			//forカウント用

	for (nCntObj = 0; nCntObj < NUM_OBJ_TYPE; nCntObj++)
	{
		for (int nCntMat = 0; nCntMat < MAX_OBJTEX; nCntMat++)
		{
			//テクスチャーの破棄
			if (g_obj[nCntObj].pTexture[nCntMat] != NULL)
			{
				g_obj[nCntObj].pTexture[nCntMat]->Release();
				g_obj[nCntObj].pTexture[nCntMat] = NULL;
			}
		}
	}
	for (nCntObj = 0; nCntObj < NUM_OBJ_TYPE; nCntObj++)
	{
		//メッシュの破棄
		if (g_pMeshObject[nCntObj] != NULL)
		{
			g_pMeshObject[nCntObj]->Release();
			g_pMeshObject[nCntObj] = NULL;
		}
		//マテリアルの破棄
		if (g_pBuffMatObject[nCntObj] != NULL)
		{
			g_pBuffMatObject[nCntObj]->Release();
			g_pBuffMatObject[nCntObj] = NULL;
		}
	}
}

//====================================================================
// オブジェクトの更新処理
//====================================================================
void UpdateObject(void)
{
	if (GetKeyboardTrigger(DIK_F6) == true)
	{
		//txtからオブジェクト情報を読み取る
		LoadObj(0);
	}
	if (GetKeyboardTrigger(DIK_F8) == true)
	{
		//txtからオブジェクト情報を読み取る
		LoadObj(1);
	}

	if (GetKeyboardTrigger(DIK_F5) == true)
	{
		for (int nCnt = 0; nCnt < MAX_OBJ; nCnt++)
		{
			if (g_object[nCnt].bUse == true)
			{
				g_object[nCnt].bUse = false;

				//オブジェクトのカウントを0に
				g_NumObj = 0;
			}
		}
	}

	//影の位置を設定
	//SetPositionShadow(g_object[nCntObj].nIdxShadow, g_object[nCntObj].pos);
}

//====================================================================
// オブジェクトの描画処理
//====================================================================
void DrawObject(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス
	D3DMATERIAL9 matDef;						//現在のマテリアル保存用
	D3DXMATERIAL *pMat;							//マテリアルデータへのポインタ

	for (int nCntObj = 0; nCntObj < MAX_OBJ; nCntObj++)
	{
		if (g_object[nCntObj].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_object[nCntObj].mtxWorld);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_object[nCntObj].rot.y, g_object[nCntObj].rot.x, g_object[nCntObj].rot.z);

			D3DXMatrixMultiply(&g_object[nCntObj].mtxWorld, &g_object[nCntObj].mtxWorld, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_object[nCntObj].pos.x, g_object[nCntObj].pos.y, g_object[nCntObj].pos.z);

			D3DXMatrixMultiply(&g_object[nCntObj].mtxWorld, &g_object[nCntObj].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_object[nCntObj].mtxWorld);

			//現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			//マテリアルへのポインタを取得
			pMat = (D3DXMATERIAL*)g_pBuffMatObject[nCntObj]->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_dwNumMatObject[nCntObj]; nCntMat++)
			{
				//マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//テクスチャの設定
				pDevice->SetTexture(0, g_obj[nCntObj].pTexture[nCntMat]);

				//オブジェクト(パーツ)の描画
				g_pMeshObject[nCntObj]->DrawSubset(nCntMat);
			}

			//保存されていたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

//====================================================================
// オブジェクトのサイズを出す処理
//====================================================================
void ObjSize(int nCntObj)
{
	DWORD dwSizeFVF;		//頂点フォーマットのサイズ
	BYTE *pVtxBuff;			//頂点バッファへのポインタ
	int nNumVtx;			//頂点数

	//頂点数を取得
	nNumVtx = g_pMeshObject[nCntObj]->GetNumVertices();

	//頂点フォーマットのサイズを取得
	dwSizeFVF = D3DXGetFVFVertexSize(g_pMeshObject[nCntObj]->GetFVF());

	//頂点バッファをロック
	g_pMeshObject[nCntObj]->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);
	
	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;			//頂点座標の代入

		if (g_object[nCntObj].vtxMax.x < vtx.x)
		{//xが大きかったら
			g_object[nCntObj].vtxMax.x = vtx.x;
		}
		if (g_object[nCntObj].vtxMax.z < vtx.z)
		{//zの値が大きかったら
			g_object[nCntObj].vtxMax.z = vtx.z;
		}

		if (g_object[nCntObj].vtxMin.x > vtx.x)
		{//xが小さかったら
			g_object[nCntObj].vtxMin.x = vtx.x;
		}
		if (g_object[nCntObj].vtxMin.z > vtx.z)
		{//zの値が大きかったら
			g_object[nCntObj].vtxMin.z = vtx.z;
		}

		if (g_object[nCntObj].vtxMax.y < vtx.y)
		{//xが大きかったら
			g_object[nCntObj].vtxMax.y = vtx.y;
		}
		if (g_object[nCntObj].vtxMin.y > vtx.y)
		{//xが小さかったら
			g_object[nCntObj].vtxMin.y = vtx.y;
		}

		pVtxBuff += dwSizeFVF;			//頂点フォーマットのサイズ分ポインタを進める
	}
	
	//頂点バッファをアンロック
	g_pMeshObject[nCntObj]->UnlockVertexBuffer();

}

//====================================================================
// モデル設置処理
//====================================================================
void SetObject(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得
	D3DXMATERIAL *pMat;			//マテリアルへのポインタ

	for (int nCntObj = 0; nCntObj < MAX_OBJ; nCntObj++)
	{
		if (g_object[nCntObj].bUse == false)
		{
			//引数からの情報を代入
			g_object[nCntObj].pos = pos;
			g_object[nCntObj].rot = rot;
			g_object[nCntObj].nType = nType;
			g_object[nCntObj].bUse = true;

			//Xファイルの読み込み
			D3DXLoadMeshFromX(c_apModelnameObject[nType],
				D3DXMESH_SYSTEMMEM, pDevice,
				NULL, &g_pBuffMatObject[nCntObj],
				NULL, &g_dwNumMatObject[nCntObj],
				&g_pMeshObject[nCntObj]);

			//マテリアルへのポインタを取得
			pMat = (D3DXMATERIAL*)g_pBuffMatObject[nCntObj]->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_dwNumMatObject[nCntObj]; nCntMat++)
			{
				if (pMat[nCntMat].pTextureFilename != NULL)
				{//ファイルからテクスチャを読み込む
					D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_obj[nCntObj].pTexture[nCntMat]);
				}
			}

			//オブジェクトのサイズを計算
			ObjSize(nCntObj);

			//オブジェクト数をカウント(デバッグ用)
			g_NumObj++;

			break;
		}
	}
}

//====================================================================
// オブジェクトの当たり判定
//====================================================================
bool ClsObj(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pVtxMax, D3DXVECTOR3 *pVtxMin, D3DXVECTOR3 *pMove)
{
	bool bUse = false;

	//オブジェクトとの当たり判定
	for (int nCntObj = 0; nCntObj < MAX_OBJ; nCntObj++)
	{
		if (g_object[nCntObj].bUse == true)
		{
			if (pPos->y + pVtxMax->y > g_object[nCntObj].pos.y + g_object[nCntObj].vtxMin.y &&
				pPos->y + pVtxMin->y < g_object[nCntObj].pos.y + g_object[nCntObj].vtxMax.y)
			{//上下の当たり判定
				if (pPos->x + pVtxMax->x > g_object[nCntObj].pos.x + g_object[nCntObj].vtxMin.x &&
					pPos->x + pVtxMin->x < g_object[nCntObj].pos.x + g_object[nCntObj].vtxMax.x)
				{//左右の当たり幅の指定
					if (pPosOld->z + pVtxMax->z <= g_object[nCntObj].pos.z + g_object[nCntObj].vtxMin.z &&
						pPos->z + pVtxMax->z >= g_object[nCntObj].pos.z + g_object[nCntObj].vtxMin.z)
					{//前の当たり判定
						pPos->z = g_object[nCntObj].pos.z + g_object[nCntObj].vtxMin.z - pVtxMax->z;
						pMove->z = 0.0f;			//移動量を0へ
						bUse = true;
					}
					if (pPosOld->z + pVtxMin->z >= g_object[nCntObj].pos.z + g_object[nCntObj].vtxMax.z &&
						pPos->z + pVtxMin->z < g_object[nCntObj].pos.z + g_object[nCntObj].vtxMax.z)
					{//後の当たり判定
						pPos->z = g_object[nCntObj].pos.z + g_object[nCntObj].vtxMax.z - pVtxMin->z /*+ 0.001f*/;
						pMove->z = 0.0f;			//移動量を0へ
						bUse = true;
					}
				}

				if (pPos->z + pVtxMax->z > g_object[nCntObj].pos.z + g_object[nCntObj].vtxMin.z &&
					pPos->z + pVtxMin->z < g_object[nCntObj].pos.z + g_object[nCntObj].vtxMax.z)
				{//上下の当たり幅の指定
					if (pPosOld->x + pVtxMax->x + SIZE <= g_object[nCntObj].pos.x + g_object[nCntObj].vtxMin.x &&
						pPos->x + pVtxMax->x + SIZE >= g_object[nCntObj].pos.x + g_object[nCntObj].vtxMin.x)
					{//左の当たり判定
						pPos->x = g_object[nCntObj].pos.x + g_object[nCntObj].vtxMin.x + pVtxMin->x - SIZE;
						pMove->x = 0.0f;			//移動量を0へ
						bUse = true;
					}
					if (pPosOld->x + pVtxMin->x - SIZE >= g_object[nCntObj].pos.x + g_object[nCntObj].vtxMax.x &&
						pPos->x + pVtxMin->x - SIZE <= g_object[nCntObj].pos.x + g_object[nCntObj].vtxMax.x)
					{//右の当たり判定
						pPos->x = g_object[nCntObj].pos.x + g_object[nCntObj].vtxMax.x + pVtxMax->x + SIZE;
						pMove->x = 0.0f;			//移動量を0へ
						bUse = true;
					}
				}
			}
		}
	}
	return bUse;
}

//=====================================
// オブジェクトの読み込み(txt)
//=====================================
void LoadObj(int nStage)
{
	//変数宣言
	char not[128];
	FILE *pFile;			//ファイルポインタを宣言
	Object *pObject = GetObj();

	//ファイルを開く
	pFile = fopen(c_apObjTxtName[nStage], "r");

	if (pFile != NULL)
	{//ファイルが開けた場合
		while (1)
		{
			fscanf(pFile, "%s", &not[0]);			//文字列を読み込む

			if (strcmp("SCRIPT", &not[0]) == 0)
			{//SCRIPTが読み込めたら
				bLoadObj = true;			//読み込みを開始
			}

			if (bLoadObj == true)
			{
				while (1)
				{
					fscanf(pFile, "%s", &not[0]);			//文字列を読み込む

					if (strcmp("OBJECTSET", &not[0]) == 0)
					{
						fscanf(pFile, "%s", &not[0]);			//文字を読み込む
						fscanf(pFile, "%s", &not[0]);			//=を読み込む
						fscanf(pFile, "%f", &pObject->pos.x);
						fscanf(pFile, "%f", &pObject->pos.y);
						fscanf(pFile, "%f", &pObject->pos.z);

						fscanf(pFile, "%s", &not[0]);			//文字を読み込む
						fscanf(pFile, "%s", &not[0]);			//=を読み込む
						fscanf(pFile, "%f", &pObject->rot.x);
						fscanf(pFile, "%f", &pObject->rot.y);
						fscanf(pFile, "%f", &pObject->rot.z);

						fscanf(pFile, "%s", &not[0]);			//文字を読み込む
						fscanf(pFile, "%s", &not[0]);			//=を読み込む
						fscanf(pFile, "%d", &pObject->nType);
											
						//敵の設置
						SetObject(pObject->pos, pObject->rot, pObject->nType);

						//ポインタを進める
						pObject++;
					}

					if (strcmp("END_SCRIPT", &not[0]) == 0)
					{//SCRIPTが読み込めなかった場合
						bLoadObj = false;			//読み込みを終了
						break;			//処理を抜ける
					}
				}
			}

			if (strcmp("END_SCRIPT", &not[0]) == 0)
			{//SCRIPTが読み込めなかった場合
				bLoadObj = false;			//読み込みを終了
				break;			//処理を抜ける
			}
		}
	}
}

//====================================================================
// オブジェクト数の取得
//====================================================================
Object *GetObj(void)
{
	return &g_object[0];
}

//====================================================================
// オブジェクト数の取得
//====================================================================
int GetNumObj(void)
{
	return g_NumObj;
}