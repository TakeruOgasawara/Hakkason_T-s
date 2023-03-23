//===========================
//
//ダイレクトX.playerファイル
//Author:髙山桃也
//
//===========================
#include "main.h"
#include "player.h"
#include "input.h"
#include "model.h"
#include <stdio.h>
#include "debugproc.h"
#include "camera.h"
#include "Enemy.h"
#include "particle.h"
#include "collision.h"
#include "game.h"

//***************************
// マクロ定義
//***************************
#define MAX_STRING	(256)	//文字数の最大
#define MOVE_SPEED	(0.2f)	//移動速度
#define MOVE_FACT	(0.935f)	//移動量の減衰係数
#define FORWARD_FACT	(0.999f)	//移動量の減衰係数
#define ROTATE_FACT	(0.04f)	//向きの補正係数
#define ROT_CURV_Z	(D3DX_PI * 0.9f)	//傾けた時の曲がり方Z
#define ROT_CURV_Y	(D3DX_PI * 0.96f)	//傾けた時の曲がり方Y
#define MAX_MOVE	(1.75f)	//移動量の最大
#define ROLL_FACT	(0.8f)	//カメラが回転する倍率
#define SPEED_FORWARD	(0.055f)	//前進する加速量
#define BREAKE_FACT	(0.98f)	//ブレーキの減衰係数

#define CURV_SPEED	(15.0f)	//曲がれるまでのスピード

//***************************
//グローバル宣言
//***************************
Player g_player;

//===========================
// 初期化処理
//===========================
void InitPlayer(void)
{
	//各種変数の初期化
	ZeroMemory(&g_player,sizeof(Player));

	g_player.bUse = true;

	//ファイルからモデルを読み込む
	FILE *pFile = fopen("data\\MOTION\\baiku.txt", "r");

	if (pFile != NULL)
	{//ファイルが開けた場合
		LoadMotion(pFile);
	}
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
// 終了処理
//===========================
void UninitPlayer(void)
{
}

//===========================
// 更新処理
//===========================
void UpdatePlayer(void)
{
	if (g_player.bUse == true)
	{
		//キーボード操作
		ControlPlayerKeyboard();

		//パッド操作
		ControlPlayerPad();


		//位置に移動量を加算
		g_player.pos += g_player.move;
		g_player.move.x = g_player.move.x * MOVE_FACT;
	if (CollisionEnemy(&g_player.pos, &g_player.posOld) == true)
	{
		SetParticle(g_player.pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.5f, 0.2f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 100.0f, 5.0f, 15, 1, 20, 80, 629, 100);
		SetParticle(g_player.pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.5f, 0.2f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 300.0f, 5.0f, 30, 0, 1, 120, 314, 30);
	
	}
	
	//向き補正処理
	FactingRot(&g_player.rot.z, g_player.rotDest.z);
	FactingRot(&g_player.rot.y,g_player.rotDest.y);

		/*if (CollisionOuterProductEnemy(&g_player.pos, &g_player.posOld, &g_player.move) == true)
		{
			SetParticle(g_player.pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.5f, 0.2f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 100.0f, 5.0f, 15, 1, 20, 80, 629, 100);
			SetParticle(g_player.pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.5f, 0.2f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 300.0f, 5.0f, 30, 0, 1, 120, 314, 30);
		}*/

		//大人の壁
		if (ClsAdultWall(&g_player.pos, 25.0f) == true)
		{
			g_player.bUse = false;
			SetGameState(GAMESTATE_END, 60);
		}

		//向き補正処理
		FactingRot(&g_player.rot.z, g_player.rotDest.z);
		FactingRot(&g_player.rot.y, g_player.rotDest.y);

		//プレイヤーの傾き設定====================================
		if (g_player.move.x < 0.0f)
		{//マイナス方向に進んでいる場合
			g_player.rotDest.z = -D3DX_PI + -(g_player.move.x / MAX_MOVE) * (-ROT_CURV_Z - -D3DX_PI);
		}
		else
		{//プラス方向に進んでいる場合
			g_player.rotDest.z = D3DX_PI + (g_player.move.x / MAX_MOVE) * (ROT_CURV_Z - D3DX_PI);
		}

		if (g_player.move.x < 0.0f)
		{//マイナス方向に進んでいる場合
			g_player.rotDest.y = -D3DX_PI - -(g_player.move.x / MAX_MOVE) * (-ROT_CURV_Y - -D3DX_PI);
		}
		else
		{//プラス方向に進んでいる場合
			g_player.rotDest.y = D3DX_PI - (g_player.move.x / MAX_MOVE) * (ROT_CURV_Y - D3DX_PI);
		}
		//プレイヤーの傾き設定====================================
	}

	else
	{//プラス方向に進んでいる場合
		g_player.rotDest.y = D3DX_PI - (g_player.move.x / MAX_MOVE) * (ROT_CURV_Y - D3DX_PI);
	}
	//プレイヤーの傾き設定====================================

	//情報取得
	Camera *pCamera = GetCamera();

	FactingRot(&pCamera->fRoll, -g_player.rotDest.z);

	pCamera->fRoll *= ROLL_FACT;
}

//===========================
// 向き補正処理
//===========================
void FactingRot(float *pfRot, float fRotDest)
{
	//差分角度を取得
	float fRotDiff = fRotDest - *pfRot;

	//角度の修正
	if (fRotDiff < 0)
	{
		fRotDiff += 6.28f;
	}
	else if (fRotDiff > 0)
	{
		fRotDiff -= 6.28f;
	}

	//角度補正
	*pfRot += fRotDiff * ROTATE_FACT;

	//角度の修正
	if (fRotDiff < 0)
	{
		fRotDiff += 6.28f;
	}
	else if (fRotDiff > 0)
	{
		fRotDiff -= 6.28f;
	}

	//角度補正
	*pfRot += fRotDiff * ROTATE_FACT;
}

//===========================
// キーボード操作処理
//===========================
void ControlPlayerKeyboard(void)
{
	//変数宣言
	int nLeft = DIK_J;
	int nRight = DIK_L;
	int nForward = DIK_I;
	int nBreake = DIK_K;

	

	//移動==================================
	if (GetKeyboardPress(nLeft))
	{//左移動

		if (g_player.move.z < CURV_SPEED)
		{
			//移動量加算
			g_player.move.x -= g_player.move.z / CURV_SPEED * MOVE_SPEED;
		}
		else
		{
			//移動量加算
			g_player.move.x -= MOVE_SPEED;
		}
	}
	else if (GetKeyboardPress(nRight))
	{//右移動
		if (g_player.move.z < CURV_SPEED)
		{
			//移動量加算
			g_player.move.x += g_player.move.z / CURV_SPEED * MOVE_SPEED;
		}
		else
		{
			//移動量加算
			g_player.move.x += MOVE_SPEED;
		}

	}
	else
	{
	}

	if (GetKeyboardPress(nForward))
	{//前進
		g_player.move.z += SPEED_FORWARD;
	}
	else
	{
		if (GetKeyboardPress(nBreake))
		{//ブレーキ
			g_player.move.z = g_player.move.z * BREAKE_FACT;
		}
		else
		{//通常減衰
			g_player.move.z = g_player.move.z * FORWARD_FACT;
		}
	}
	//移動==================================

	//向きを戻す============================
	if (GetKeyboardRelease(nLeft) && GetKeyboardRelease(nRight) == false)
	{//左離した瞬間に右を押してない場合
		//目標の向き設定
		g_player.rotDest.z = D3DX_PI;
		g_player.rotDest.y = D3DX_PI;
	}

	if (GetKeyboardRelease(nRight) && GetKeyboardRelease(nLeft) == false)
	{//左離した瞬間に右を押してない場合
		//目標の向き設定
		g_player.rotDest.z = D3DX_PI;
		g_player.rotDest.y = D3DX_PI;
	}
	//向きを戻す============================
}

//===========================
// パッド操作処理
//===========================
void ControlPlayerPad(void)
{
	//横移動========================
	if (g_player.move.z < CURV_SPEED)
	{
		//移動量加算
		g_player.move.x += g_player.move.z / CURV_SPEED * MOVE_SPEED * GetJoyStickLX(0);
	}
	else
	{
		//移動量加算
		g_player.move.x += GetJoyStickLX(0) * MOVE_SPEED;
	}
	//横移動========================

	if (GetJoyPadPress(BUTTON_RB,0))
	{
		g_player.move.z += SPEED_FORWARD;
	}
	else
	{
		if (GetJoyPadPress(BUTTON_LB, 0))
		{//ブレーキ
			g_player.move.z = g_player.move.z * BREAKE_FACT;
		}
		else
		{//通常減衰
			g_player.move.z = g_player.move.z * FORWARD_FACT;
		}
	}
}

//===========================
// 描画処理
//===========================
void DrawPlayer(void)
{
	//デバイスポインタ設定
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//変数宣言
	D3DXMATRIX mtxRot, mtxTrans;
	D3DMATERIAL9 matDef;			//現在のマテリアル保存用
	D3DXMATERIAL *pMat;				//マテリアルデータへのポインタ

	if (g_player.bUse == true)
	{
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

#ifdef _DEBUG		//デバッグ時のみ
	PrintDebugProc("【プレイヤーの位置：%f,%f,%f】\n", g_player.pos.x, g_player.pos.y, g_player.pos.z);
	PrintDebugProc("【プレイヤーの移動量：%f,%f,%f】\n", g_player.move.x, g_player.move.y, g_player.move.z);
#endif
}

//===========================
// 情報取得処理
//===========================
Player *GetPlayer(void)
{
	return &g_player;
}