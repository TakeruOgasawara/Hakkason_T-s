//===========================
//
//ダイレクトX.playerファイル
//Author:��山桃也
//
//===========================
#include "main.h"
#include "player.h"
#include "input.h"
#include "model.h"
#include <stdio.h>

//***************************
// マクロ定義
//***************************
#define MAX_STRING	(256)	//文字数の最大

//***************************
//グローバル宣言
//***************************
Player g_player;

//===========================
//プレイヤーの初期化処理
//===========================
void InitPlayer(void)
{
	//各種変数の初期化
	ZeroMemory(&g_player,sizeof(Player));
}

//===========================
//ファイル読み込み
//===========================
void LoadMotion(FILE *pFile)
{
	//変数宣言
	char cTemp[MAX_STRING];
	int nCntMotion = 0;
	int nCntPart = 0;

	//情報取得
	int nNumPart = GetNumPlayerPart();

	while (1)
	{
		//文字読み込み
		fscanf(pFile, "%s", &cTemp[0]);

		//キャラクター設定===========================================
		if (strcmp(cTemp, "CHARACTERSET") == 0)
		{//キャラクタースタート
			while (strcmp(cTemp, "END_CHARACTERSET") != 0)
			{//終わりまでキャラクター設定
				fscanf(pFile, "%s", &cTemp[0]);

				if (strcmp(cTemp, "PARTSSET") == 0)
				{//パーツスタート
					while (strcmp(cTemp, "END_PARTSSET") != 0)
					{//終わりまでパーツ設定
						fscanf(pFile, "%s", &cTemp[0]);

						if (strcmp(cTemp, "INDEX") == 0)
						{//番号読み込み
							fscanf(pFile, "%s", &cTemp[0]);

							fscanf(pFile, "%d", &g_player.part[nCntPart].nIdx);
						}

						if (strcmp(cTemp, "PARENT") == 0)
						{//親番号読み込み
							fscanf(pFile, "%s", &cTemp[0]);

							fscanf(pFile, "%d", &g_player.part[nCntPart].nIdxParent);
						}

						if (strcmp(cTemp, "POS") == 0)
						{//位置読み込み
							fscanf(pFile, "%s", &cTemp[0]);

							for (int nCntPos = 0; nCntPos < 3; nCntPos++)
							{
								fscanf(pFile, "%f", &g_player.part[nCntPart].pos[nCntPos]);
							}
						}

						if (strcmp(cTemp, "ROT") == 0)
						{//向き読み込み
							fscanf(pFile, "%s", &cTemp[0]);

							for (int nCntRot = 0; nCntRot < 3; nCntRot++)
							{
								fscanf(pFile, "%f", &g_player.part[nCntPart].rot[nCntRot]);
							}
						}

					}//END_PART

					nCntPart++;
				}
			}//END_CHAR
		}
		//===========================================================

		if (strcmp(cTemp, "END_SCRIPT") == 0)
		{
			break;
		}
	}//while閉じ
}

//===========================
//プレイヤーの終了処理
//===========================
void UninitPlayer(void)
{
}

//===========================
//プレイヤーの更新処理
//===========================
void UpdatePlayer(void)
{
}

//===========================
//プレイヤーの描画処理
//===========================
void DrawPlayer(void)
{
	//デバイスポインタ設定
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//変数宣言
	D3DXMATRIX mtxRot, mtxTrans;
	D3DMATERIAL9 matDef;			//現在のマテリアル保存用
	D3DXMATERIAL *pMat;				//マテリアルデータへのポインタ

	//ワールドマトリックス初期化
	D3DXMatrixIdentity(&g_player.mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		g_player.rot.y, g_player.rot.x, g_player.rot.z);
	D3DXMatrixMultiply(&g_player.mtxWorld, &g_player.mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans,
		g_player.pos.x, g_player.pos.y, g_player.pos.z);
	D3DXMatrixMultiply(&g_player.mtxWorld, &g_player.mtxWorld, &mtxTrans);

	//ワールドマトリックス設定
	pDevice->SetTransform(D3DTS_WORLD, &g_player.mtxWorld);

	//情報取得
	int nNumPart = GetNumPlayerPart();
	Model *pModelPlayer = GetplayerModel();

	for (int nCntPlayer = 0; nCntPlayer < nNumPart; nCntPlayer++)
	{
		//変数宣言
		D3DXMATRIX mtxRotModel, mtxTransModel;
		D3DXMATRIX mtxParent;

		//ワールドマトリックス初期化
		D3DXMatrixIdentity(&g_player.part[nCntPlayer].mtxWorld);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRotModel,
			g_player.part[nCntPlayer].rot.y, g_player.part[nCntPlayer].rot.x, g_player.part[nCntPlayer].rot.z);
		D3DXMatrixMultiply(&g_player.part[nCntPlayer].mtxWorld, &g_player.part[nCntPlayer].mtxWorld, &mtxRotModel);

		//位置を反映
		D3DXMatrixTranslation(&mtxTransModel,
			g_player.part[nCntPlayer].pos.x, g_player.part[nCntPlayer].pos.y, g_player.part[nCntPlayer].pos.z);
		D3DXMatrixMultiply(&g_player.part[nCntPlayer].mtxWorld, &g_player.part[nCntPlayer].mtxWorld, &mtxTransModel);

		if (g_player.part[nCntPlayer].nIdxParent != -1)
		{//親パーツがある場合
			mtxParent = g_player.part[g_player.part[nCntPlayer].nIdxParent].mtxWorld;
		}

		else
		{//親パーツがない場合
			mtxParent = g_player.mtxWorld;
		}

		//親パーツとパーツのワールドマトリックスをかけ合わせる。
		D3DXMatrixMultiply(&g_player.part[nCntPlayer].mtxWorld, &g_player.part[nCntPlayer].mtxWorld, &mtxParent);

		//ワールドマトリックス設定
		pDevice->SetTransform(D3DTS_WORLD, &g_player.part[nCntPlayer].mtxWorld);

		//現在のマテリアル取得
		pDevice->GetMaterial(&matDef);

		//マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)pModelPlayer[nCntPlayer].pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)pModelPlayer[nCntPlayer].dwNumMat; nCntMat++)
		{
			//マテリアル設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			//テクスチャ設定
			pDevice->SetTexture(0, pModelPlayer[nCntPlayer].apTexture[nCntMat]);

			//モデル（パーツ）描画
			pModelPlayer[nCntPlayer].pMesh->DrawSubset(nCntMat);
		}

		//マテリアルを戻す
		pDevice->SetMaterial(&matDef);
	}
}

//===========================
//プレイヤーの情報取得処理
//===========================
Player *GetPlayer(void)
{
	return &g_player;
}