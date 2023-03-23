#include "Enemy.h"
#include "model.h"
#include "player.h"

//マクロ定義
#define MAX_ENEMY (256)

//グローバル変数
ENEMY g_aEnemy[MAX_ENEMY];
int g_nEnemyCountTime;
void InitEnemy(void)
{
	g_nEnemyCountTime = 0;
	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
	{
		ZeroMemory(&g_aEnemy[nCnt], sizeof(ENEMY));
	}
}
void UninitEnemy(void)
{

}
void UpdateEnemy(void)
{
	Player*pPlayer = GetPlayer();
	g_nEnemyCountTime ++;
	g_nEnemyCountTime += (int)pPlayer->move.z / 50;
	if (g_nEnemyCountTime  >= 60)
	{
		g_nEnemyCountTime = 0;
			int nRand = rand() % 4;
			float fDis = (float)(rand() % 7000) + 5000.0f;
			switch (nRand)
			{
			case 0:
				SetEnemy(D3DXVECTOR3(-100.0f, 0.0f, fDis), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 10.0f));
				break;
			case 1:
				SetEnemy(D3DXVECTOR3(-40.0f, 0.0f, fDis), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 10.0f));
				break;
			case 2:
				SetEnemy(D3DXVECTOR3(40.0f, 0.0f, fDis), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, -10.0f));
				break;
			case 3:
				SetEnemy(D3DXVECTOR3(100.0f, 0.0f, fDis), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, -10.0f));
				break;
			default:
				break;
			}
			
	}
	
	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
	{
		if (g_aEnemy[nCnt].bUse == true)
		{
			g_aEnemy[nCnt].posOld = g_aEnemy[nCnt].pos;
			g_aEnemy[nCnt].pos += g_aEnemy[nCnt].move;

			if ((g_aEnemy[nCnt].pos.z - pPlayer->pos.z) < -1000.0f)
			{
				g_aEnemy[nCnt].bUse = false;
			}
		}
	}
}
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice(); //デバイスのポインタ
	D3DXMATRIX mtxRot, mtxTrans; //計算用マトリクス
	D3DMATERIAL9 matDef; //現在のマテリアル保存用
	D3DXMATERIAL *pMat; //マテリアルデータへのポインタ
	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
	{
		if (g_aEnemy[nCnt].bUse == true)
		{
			Model *pEnemyModel = GetEnemyModel(g_aEnemy[nCnt].nType);

			//ワールドマトリクスの初期化
			D3DXMatrixIdentity(&g_aEnemy[nCnt].mtxWorld);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aEnemy[nCnt].rot.y, g_aEnemy[nCnt].rot.x, g_aEnemy[nCnt].rot.z);
			D3DXMatrixMultiply(&g_aEnemy[nCnt].mtxWorld, &g_aEnemy[nCnt].mtxWorld, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aEnemy[nCnt].pos.x, g_aEnemy[nCnt].pos.y, g_aEnemy[nCnt].pos.z);
			D3DXMatrixMultiply(&g_aEnemy[nCnt].mtxWorld, &g_aEnemy[nCnt].mtxWorld, &mtxTrans);

			//ワールドマトリクスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aEnemy[nCnt].mtxWorld);

			//現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			//マテリアルデータのポインタを取得
			pMat = (D3DXMATERIAL*)pEnemyModel->pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)pEnemyModel->dwNumMat; nCntMat++)
			{
				//マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//テクスチャの設定
				pDevice->SetTexture(0, pEnemyModel->apTexture[nCntMat]);

				//モデル(パーツ)の描写
				pEnemyModel->pMesh->DrawSubset(nCntMat);
			}
			//保存していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}

	}
}
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot,D3DXVECTOR3 move)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	pDevice = GetDevice();

	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
	{
		if (g_aEnemy[nCnt].bUse == false)
		{

			int nNumVtx; //頂点数
			DWORD dwSizeFVF; //頂点フォーマットのサイズ
			BYTE*pVtxBuff; //頂点バッファへのポインタ

			g_aEnemy[nCnt].pos = pos;
			g_aEnemy[nCnt].move = move;
			g_aEnemy[nCnt].rot = rot;
			g_aEnemy[nCnt].nType = rand()%NUM_ENEMY;

			g_aEnemy[nCnt].bUse = true;
			break;
		}
	}
}
bool CollisionOuterProductEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove)
{
	//移動ベクトルを割り出す
	D3DXVECTOR3 vecMove = *pPos - *pPosOld;

	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
	{
		if (g_aEnemy[nCnt].bUse == true)
		{
			Model *EnemyModel = GetEnemyModel(g_aEnemy[nCnt].nType);
			for (int nLine = 0; nLine < 4; nLine++)
			{
				bool bHit = false;
				bool bHit1 = false;
				bool bHit2 = false;

				D3DXVECTOR3 pos0, pos1;
				switch (nLine)
				{//紙片の当たり判定
				case 0:
					pos0 = D3DXVECTOR3(g_aEnemy[nCnt].pos.x + EnemyModel->vtxMin.x, g_aEnemy[nCnt].pos.y, g_aEnemy[nCnt].pos.z + EnemyModel->vtxMax.z);
					pos1 = D3DXVECTOR3(g_aEnemy[nCnt].pos.x + EnemyModel->vtxMax.x, g_aEnemy[nCnt].pos.y, g_aEnemy[nCnt].pos.z + EnemyModel->vtxMax.z);
					break;
				case 1:
					pos0 = D3DXVECTOR3(g_aEnemy[nCnt].pos.x + EnemyModel->vtxMax.x, g_aEnemy[nCnt].pos.y, g_aEnemy[nCnt].pos.z + EnemyModel->vtxMax.z);
					pos1 = D3DXVECTOR3(g_aEnemy[nCnt].pos.x + EnemyModel->vtxMax.x, g_aEnemy[nCnt].pos.y, g_aEnemy[nCnt].pos.z + EnemyModel->vtxMin.z);
					break;
				case 2:
					pos0 = D3DXVECTOR3(g_aEnemy[nCnt].pos.x + EnemyModel->vtxMax.x, g_aEnemy[nCnt].pos.y, g_aEnemy[nCnt].pos.z + EnemyModel->vtxMin.z);
					pos1 = D3DXVECTOR3(g_aEnemy[nCnt].pos.x + EnemyModel->vtxMin.x, g_aEnemy[nCnt].pos.y, g_aEnemy[nCnt].pos.z + EnemyModel->vtxMin.z);
					break;
				case 3:
					pos0 = D3DXVECTOR3(g_aEnemy[nCnt].pos.x + EnemyModel->vtxMin.x, g_aEnemy[nCnt].pos.y, g_aEnemy[nCnt].pos.z + EnemyModel->vtxMin.z);
					pos1 = D3DXVECTOR3(g_aEnemy[nCnt].pos.x + EnemyModel->vtxMin.x, g_aEnemy[nCnt].pos.y, g_aEnemy[nCnt].pos.z + EnemyModel->vtxMax.z);
					break;
				default:
					break;
				}

				//ベクトルの目標地点

				D3DXVECTOR3 vecLine = pos1 - pos0;

				D3DXVECTOR3 vecToPos = *pPos - pos0;

				D3DXVECTOR3 vecToPos2 = *pPosOld - pos0;

				float A, B, fRate;
				A = (vecToPos.z * vecMove.x) - (vecToPos.x * vecMove.z);
				B = (vecLine.z * vecMove.x) - (vecLine.x * vecMove.z);
				if (B != 0)
				{
					fRate = A / B;
				}
				else
				{
					fRate = 10.0f;
				}

				if (fRate >= 0.0f &&fRate <= 1.0f)
				{//vecLineを横切ったとき
					if ((vecLine.z * vecToPos.x) - (vecLine.x * vecToPos.z) < 0)
					{
						bHit1 = true;
					}

					if ((vecLine.z * vecToPos2.x) - (vecLine.x * vecToPos2.z) < 0)
					{
						bHit2 = true;
					}

					if (bHit1 != bHit2)
					{
						bHit = true;
					}

					bHit1 = false;
					bHit2 = false;

					if ((vecLine.z * vecToPos.x) + (vecLine.x * vecToPos.z) > 0)
					{
						bHit1 = true;
					}

					if ((vecLine.z * vecToPos2.x) + (vecLine.x * vecToPos2.z) > 0)
					{
						bHit2 = true;
					}

					if (bHit1 != bHit2)
					{
						bHit = true;
					}
				}
				if (bHit == true)
				{
					return true;
				}
			}
		}

	}
	return false;
}