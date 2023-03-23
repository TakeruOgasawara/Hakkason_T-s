#include "Enemy.h"
#include "model.h"
#include "player.h"

//�}�N����`
#define MAX_ENEMY (256)

//�O���[�o���ϐ�
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
	LPDIRECT3DDEVICE9 pDevice = GetDevice(); //�f�o�C�X�̃|�C���^
	D3DXMATRIX mtxRot, mtxTrans; //�v�Z�p�}�g���N�X
	D3DMATERIAL9 matDef; //���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat; //�}�e���A���f�[�^�ւ̃|�C���^
	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
	{
		if (g_aEnemy[nCnt].bUse == true)
		{
			Model *pEnemyModel = GetEnemyModel(g_aEnemy[nCnt].nType);

			//���[���h�}�g���N�X�̏�����
			D3DXMatrixIdentity(&g_aEnemy[nCnt].mtxWorld);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aEnemy[nCnt].rot.y, g_aEnemy[nCnt].rot.x, g_aEnemy[nCnt].rot.z);
			D3DXMatrixMultiply(&g_aEnemy[nCnt].mtxWorld, &g_aEnemy[nCnt].mtxWorld, &mtxRot);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aEnemy[nCnt].pos.x, g_aEnemy[nCnt].pos.y, g_aEnemy[nCnt].pos.z);
			D3DXMatrixMultiply(&g_aEnemy[nCnt].mtxWorld, &g_aEnemy[nCnt].mtxWorld, &mtxTrans);

			//���[���h�}�g���N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aEnemy[nCnt].mtxWorld);

			//���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			//�}�e���A���f�[�^�̃|�C���^���擾
			pMat = (D3DXMATERIAL*)pEnemyModel->pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)pEnemyModel->dwNumMat; nCntMat++)
			{
				//�}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, pEnemyModel->apTexture[nCntMat]);

				//���f��(�p�[�c)�̕`��
				pEnemyModel->pMesh->DrawSubset(nCntMat);
			}
			//�ۑ����Ă����}�e���A����߂�
			pDevice->SetMaterial(&matDef);
		}

	}
}
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot,D3DXVECTOR3 move)
{
	LPDIRECT3DDEVICE9 pDevice; //�f�o�C�X�̃|�C���^
	pDevice = GetDevice();

	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
	{
		if (g_aEnemy[nCnt].bUse == false)
		{

			int nNumVtx; //���_��
			DWORD dwSizeFVF; //���_�t�H�[�}�b�g�̃T�C�Y
			BYTE*pVtxBuff; //���_�o�b�t�@�ւ̃|�C���^

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
	//�ړ��x�N�g��������o��
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
				{//���Ђ̓����蔻��
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

				//�x�N�g���̖ڕW�n�_

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
				{//vecLine�����؂����Ƃ�
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