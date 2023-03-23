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

//***************************
// �}�N����`
//***************************
#define MAX_STRING	(256)	//�������̍ő�

//***************************
//�O���[�o���錾
//***************************
Player g_player;

//===========================
//�v���C���[�̏���������
//===========================
void InitPlayer(void)
{
	//�e��ϐ��̏�����
	ZeroMemory(&g_player,sizeof(Player));
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
//�v���C���[�̏I������
//===========================
void UninitPlayer(void)
{
}

//===========================
//�v���C���[�̍X�V����
//===========================
void UpdatePlayer(void)
{
}

//===========================
//�v���C���[�̕`�揈��
//===========================
void DrawPlayer(void)
{
	//�f�o�C�X�|�C���^�ݒ�
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�ϐ��錾
	D3DXMATRIX mtxRot, mtxTrans;
	D3DMATERIAL9 matDef;			//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;				//�}�e���A���f�[�^�ւ̃|�C���^

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

//===========================
//�v���C���[�̏��擾����
//===========================
Player *GetPlayer(void)
{
	return &g_player;
}