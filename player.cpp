//===========================
//
//�_�C���N�gX.player�t�@�C��
//Author:���R����
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
// �}�N����`
//***************************
#define MAX_STRING	(256)	//�������̍ő�
#define MOVE_SPEED	(0.2f)	//�ړ����x
#define MOVE_FACT	(0.935f)	//�ړ��ʂ̌����W��
#define FORWARD_FACT	(0.999f)	//�ړ��ʂ̌����W��
#define ROTATE_FACT	(0.04f)	//�����̕␳�W��
#define ROT_CURV_Z	(D3DX_PI * 0.9f)	//�X�������̋Ȃ����Z
#define ROT_CURV_Y	(D3DX_PI * 0.96f)	//�X�������̋Ȃ����Y
#define MAX_MOVE	(1.75f)	//�ړ��ʂ̍ő�
#define ROLL_FACT	(0.8f)	//�J��������]����{��
#define SPEED_FORWARD	(0.055f)	//�O�i���������
#define BREAKE_FACT	(0.98f)	//�u���[�L�̌����W��

#define CURV_SPEED	(15.0f)	//�Ȃ����܂ł̃X�s�[�h

//***************************
//�O���[�o���錾
//***************************
Player g_player;

//===========================
// ����������
//===========================
void InitPlayer(void)
{
	//�e��ϐ��̏�����
	ZeroMemory(&g_player,sizeof(Player));

	g_player.bUse = true;

	//�t�@�C�����烂�f����ǂݍ���
	FILE *pFile = fopen("data\\MOTION\\baiku.txt", "r");

	if (pFile != NULL)
	{//�t�@�C�����J�����ꍇ
		LoadMotion(pFile);
	}
}

//===========================
//�t�@�C���ǂݍ���
//===========================
void LoadMotion(FILE *pFile)
{
	//�ϐ��錾
	char cTemp[MAX_STRING];
	int nCntMotion = 0;
	int nCntPart = 0;

	//���擾
	int nNumPart = GetNumPlayerPart();

	while (1)
	{
		//�����ǂݍ���
		fscanf(pFile, "%s", &cTemp[0]);

		//�L�����N�^�[�ݒ�===========================================
		if (strcmp(cTemp, "CHARACTERSET") == 0)
		{//�L�����N�^�[�X�^�[�g
			while (strcmp(cTemp, "END_CHARACTERSET") != 0)
			{//�I���܂ŃL�����N�^�[�ݒ�
				fscanf(pFile, "%s", &cTemp[0]);

				if (strcmp(cTemp, "PARTSSET") == 0)
				{//�p�[�c�X�^�[�g
					while (strcmp(cTemp, "END_PARTSSET") != 0)
					{//�I���܂Ńp�[�c�ݒ�
						fscanf(pFile, "%s", &cTemp[0]);
						if (strcmp(cTemp, "INDEX") == 0)
						{//�ԍ��ǂݍ���
							fscanf(pFile, "%s", &cTemp[0]);

							fscanf(pFile, "%d", &g_player.part[nCntPart].nIdx);
						}

						if (strcmp(cTemp, "PARENT") == 0)
						{//�e�ԍ��ǂݍ���
							fscanf(pFile, "%s", &cTemp[0]);

							fscanf(pFile, "%d", &g_player.part[nCntPart].nIdxParent);
						}

						if (strcmp(cTemp, "POS") == 0)
						{//�ʒu�ǂݍ���
							fscanf(pFile, "%s", &cTemp[0]);

							for (int nCntPos = 0; nCntPos < 3; nCntPos++)
							{
								fscanf(pFile, "%f", &g_player.part[nCntPart].pos[nCntPos]);
							}
						}

						if (strcmp(cTemp, "ROT") == 0)
						{//�����ǂݍ���
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
	}//while��
}

//===========================
// �I������
//===========================
void UninitPlayer(void)
{
}

//===========================
// �X�V����
//===========================
void UpdatePlayer(void)
{
	if (g_player.bUse == true)
	{
		//�L�[�{�[�h����
		ControlPlayerKeyboard();

		//�p�b�h����
		ControlPlayerPad();


		//�ʒu�Ɉړ��ʂ����Z
		g_player.pos += g_player.move;
		g_player.move.x = g_player.move.x * MOVE_FACT;
	if (CollisionEnemy(&g_player.pos, &g_player.posOld) == true)
	{
		SetParticle(g_player.pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.5f, 0.2f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 100.0f, 5.0f, 15, 1, 20, 80, 629, 100);
		SetParticle(g_player.pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.5f, 0.2f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 300.0f, 5.0f, 30, 0, 1, 120, 314, 30);
	
	}
	
	//�����␳����
	FactingRot(&g_player.rot.z, g_player.rotDest.z);
	FactingRot(&g_player.rot.y,g_player.rotDest.y);

		/*if (CollisionOuterProductEnemy(&g_player.pos, &g_player.posOld, &g_player.move) == true)
		{
			SetParticle(g_player.pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.5f, 0.2f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 100.0f, 5.0f, 15, 1, 20, 80, 629, 100);
			SetParticle(g_player.pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.5f, 0.2f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 300.0f, 5.0f, 30, 0, 1, 120, 314, 30);
		}*/

		//��l�̕�
		if (ClsAdultWall(&g_player.pos, 25.0f) == true)
		{
			g_player.bUse = false;
			SetGameState(GAMESTATE_END, 60);
		}

		//�����␳����
		FactingRot(&g_player.rot.z, g_player.rotDest.z);
		FactingRot(&g_player.rot.y, g_player.rotDest.y);

		//�v���C���[�̌X���ݒ�====================================
		if (g_player.move.x < 0.0f)
		{//�}�C�i�X�����ɐi��ł���ꍇ
			g_player.rotDest.z = -D3DX_PI + -(g_player.move.x / MAX_MOVE) * (-ROT_CURV_Z - -D3DX_PI);
		}
		else
		{//�v���X�����ɐi��ł���ꍇ
			g_player.rotDest.z = D3DX_PI + (g_player.move.x / MAX_MOVE) * (ROT_CURV_Z - D3DX_PI);
		}

		if (g_player.move.x < 0.0f)
		{//�}�C�i�X�����ɐi��ł���ꍇ
			g_player.rotDest.y = -D3DX_PI - -(g_player.move.x / MAX_MOVE) * (-ROT_CURV_Y - -D3DX_PI);
		}
		else
		{//�v���X�����ɐi��ł���ꍇ
			g_player.rotDest.y = D3DX_PI - (g_player.move.x / MAX_MOVE) * (ROT_CURV_Y - D3DX_PI);
		}
		//�v���C���[�̌X���ݒ�====================================
	}

	else
	{//�v���X�����ɐi��ł���ꍇ
		g_player.rotDest.y = D3DX_PI - (g_player.move.x / MAX_MOVE) * (ROT_CURV_Y - D3DX_PI);
	}
	//�v���C���[�̌X���ݒ�====================================

	//���擾
	Camera *pCamera = GetCamera();

	FactingRot(&pCamera->fRoll, -g_player.rotDest.z);

	pCamera->fRoll *= ROLL_FACT;
}

//===========================
// �����␳����
//===========================
void FactingRot(float *pfRot, float fRotDest)
{
	//�����p�x���擾
	float fRotDiff = fRotDest - *pfRot;

	//�p�x�̏C��
	if (fRotDiff < 0)
	{
		fRotDiff += 6.28f;
	}
	else if (fRotDiff > 0)
	{
		fRotDiff -= 6.28f;
	}

	//�p�x�␳
	*pfRot += fRotDiff * ROTATE_FACT;

	//�p�x�̏C��
	if (fRotDiff < 0)
	{
		fRotDiff += 6.28f;
	}
	else if (fRotDiff > 0)
	{
		fRotDiff -= 6.28f;
	}

	//�p�x�␳
	*pfRot += fRotDiff * ROTATE_FACT;
}

//===========================
// �L�[�{�[�h���쏈��
//===========================
void ControlPlayerKeyboard(void)
{
	//�ϐ��錾
	int nLeft = DIK_J;
	int nRight = DIK_L;
	int nForward = DIK_I;
	int nBreake = DIK_K;

	

	//�ړ�==================================
	if (GetKeyboardPress(nLeft))
	{//���ړ�

		if (g_player.move.z < CURV_SPEED)
		{
			//�ړ��ʉ��Z
			g_player.move.x -= g_player.move.z / CURV_SPEED * MOVE_SPEED;
		}
		else
		{
			//�ړ��ʉ��Z
			g_player.move.x -= MOVE_SPEED;
		}
	}
	else if (GetKeyboardPress(nRight))
	{//�E�ړ�
		if (g_player.move.z < CURV_SPEED)
		{
			//�ړ��ʉ��Z
			g_player.move.x += g_player.move.z / CURV_SPEED * MOVE_SPEED;
		}
		else
		{
			//�ړ��ʉ��Z
			g_player.move.x += MOVE_SPEED;
		}

	}
	else
	{
	}

	if (GetKeyboardPress(nForward))
	{//�O�i
		g_player.move.z += SPEED_FORWARD;
	}
	else
	{
		if (GetKeyboardPress(nBreake))
		{//�u���[�L
			g_player.move.z = g_player.move.z * BREAKE_FACT;
		}
		else
		{//�ʏ팸��
			g_player.move.z = g_player.move.z * FORWARD_FACT;
		}
	}
	//�ړ�==================================

	//������߂�============================
	if (GetKeyboardRelease(nLeft) && GetKeyboardRelease(nRight) == false)
	{//���������u�ԂɉE�������ĂȂ��ꍇ
		//�ڕW�̌����ݒ�
		g_player.rotDest.z = D3DX_PI;
		g_player.rotDest.y = D3DX_PI;
	}

	if (GetKeyboardRelease(nRight) && GetKeyboardRelease(nLeft) == false)
	{//���������u�ԂɉE�������ĂȂ��ꍇ
		//�ڕW�̌����ݒ�
		g_player.rotDest.z = D3DX_PI;
		g_player.rotDest.y = D3DX_PI;
	}
	//������߂�============================
}

//===========================
// �p�b�h���쏈��
//===========================
void ControlPlayerPad(void)
{
	//���ړ�========================
	if (g_player.move.z < CURV_SPEED)
	{
		//�ړ��ʉ��Z
		g_player.move.x += g_player.move.z / CURV_SPEED * MOVE_SPEED * GetJoyStickLX(0);
	}
	else
	{
		//�ړ��ʉ��Z
		g_player.move.x += GetJoyStickLX(0) * MOVE_SPEED;
	}
	//���ړ�========================

	if (GetJoyPadPress(BUTTON_RB,0))
	{
		g_player.move.z += SPEED_FORWARD;
	}
	else
	{
		if (GetJoyPadPress(BUTTON_LB, 0))
		{//�u���[�L
			g_player.move.z = g_player.move.z * BREAKE_FACT;
		}
		else
		{//�ʏ팸��
			g_player.move.z = g_player.move.z * FORWARD_FACT;
		}
	}
}

//===========================
// �`�揈��
//===========================
void DrawPlayer(void)
{
	//�f�o�C�X�|�C���^�ݒ�
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�ϐ��錾
	D3DXMATRIX mtxRot, mtxTrans;
	D3DMATERIAL9 matDef;			//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;				//�}�e���A���f�[�^�ւ̃|�C���^

	if (g_player.bUse == true)
	{
		//���[���h�}�g���b�N�X������
		D3DXMatrixIdentity(&g_player.mtxWorld);

		//�����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot,
			g_player.rot.y, g_player.rot.x, g_player.rot.z);
		D3DXMatrixMultiply(&g_player.mtxWorld, &g_player.mtxWorld, &mtxRot);

		//�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans,
			g_player.pos.x, g_player.pos.y, g_player.pos.z);
		D3DXMatrixMultiply(&g_player.mtxWorld, &g_player.mtxWorld, &mtxTrans);

		//���[���h�}�g���b�N�X�ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_player.mtxWorld);

		//���擾
		int nNumPart = GetNumPlayerPart();
		Model *pModelPlayer = GetplayerModel();

		for (int nCntPlayer = 0; nCntPlayer < nNumPart; nCntPlayer++)
		{
			//�ϐ��錾
			D3DXMATRIX mtxRotModel, mtxTransModel;
			D3DXMATRIX mtxParent;

			//���[���h�}�g���b�N�X������
			D3DXMatrixIdentity(&g_player.part[nCntPlayer].mtxWorld);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRotModel,
				g_player.part[nCntPlayer].rot.y, g_player.part[nCntPlayer].rot.x, g_player.part[nCntPlayer].rot.z);
			D3DXMatrixMultiply(&g_player.part[nCntPlayer].mtxWorld, &g_player.part[nCntPlayer].mtxWorld, &mtxRotModel);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTransModel,
				g_player.part[nCntPlayer].pos.x, g_player.part[nCntPlayer].pos.y, g_player.part[nCntPlayer].pos.z);
			D3DXMatrixMultiply(&g_player.part[nCntPlayer].mtxWorld, &g_player.part[nCntPlayer].mtxWorld, &mtxTransModel);

			if (g_player.part[nCntPlayer].nIdxParent != -1)
			{//�e�p�[�c������ꍇ
				mtxParent = g_player.part[g_player.part[nCntPlayer].nIdxParent].mtxWorld;
			}

			else
			{//�e�p�[�c���Ȃ��ꍇ
				mtxParent = g_player.mtxWorld;
			}

			//�e�p�[�c�ƃp�[�c�̃��[���h�}�g���b�N�X���������킹��B
			D3DXMatrixMultiply(&g_player.part[nCntPlayer].mtxWorld, &g_player.part[nCntPlayer].mtxWorld, &mtxParent);

			//���[���h�}�g���b�N�X�ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_player.part[nCntPlayer].mtxWorld);

			//���݂̃}�e���A���擾
			pDevice->GetMaterial(&matDef);

			//�}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)pModelPlayer[nCntPlayer].pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)pModelPlayer[nCntPlayer].dwNumMat; nCntMat++)
			{
				//�}�e���A���ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//�e�N�X�`���ݒ�
				pDevice->SetTexture(0, pModelPlayer[nCntPlayer].apTexture[nCntMat]);

				//���f���i�p�[�c�j�`��
				pModelPlayer[nCntPlayer].pMesh->DrawSubset(nCntMat);
			}

			//�}�e���A����߂�
			pDevice->SetMaterial(&matDef);
		}
	}

#ifdef _DEBUG		//�f�o�b�O���̂�
	PrintDebugProc("�y�v���C���[�̈ʒu�F%f,%f,%f�z\n", g_player.pos.x, g_player.pos.y, g_player.pos.z);
	PrintDebugProc("�y�v���C���[�̈ړ��ʁF%f,%f,%f�z\n", g_player.move.x, g_player.move.y, g_player.move.z);
#endif
}

//===========================
// ���擾����
//===========================
Player *GetPlayer(void)
{
	return &g_player;
}