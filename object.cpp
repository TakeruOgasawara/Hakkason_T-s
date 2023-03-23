//======================================================================================
//
// �I�u�W�F�N�g�̏���[object.cpp]
// Author;���}���@�V
//
//======================================================================================
#include <stdio.h>
#include "object.h"
#include "input.h"
#include "camera.h"
#include "player.h"

//*****************************
//�}�N����`
//*****************************
#define SIZE				(5.0f)
#define NUM_OBJ_TYPE		(2)

//���f���t�@�C����
const char *c_apModelnameObject[] =
{
	"data\\MODEL\\house.x",
};

//*****************************
//�X�e�[�W�̃t�@�C����
//*****************************
const char *c_apObjTxtName[MAX_WAVE] =
{
	"data\\MODEL\\object\\SetObject00.txt",
};

//*****************************
//�v���g�^�C�v�錾
//*****************************
//void ColliObj(void);
void ObjSize(int nCntObj);

//*****************************
//�O���[�o���錾
//*****************************
LPD3DXMESH g_pMeshObject[MAX_OBJ] = {};
LPD3DXBUFFER g_pBuffMatObject[MAX_OBJ] = {};
DWORD g_dwNumMatObject[MAX_OBJ] = {};
Object g_object[MAX_OBJ];				//�I�u�W�F�N�g���
Obj g_obj[MAX_OBJ];						//�e�N�X�`�����
int g_SetNumObj;
int g_NumObj;
bool bLoadObj;

//====================================================================
// �I�u�W�F�N�g�̏���������
//====================================================================
void InitObject(void)
{
	//�ϐ��錾
	int nCntObj;			//for�J�E���g�p
	//int nNumVtx;			//���_��
	//DWORD dwSizeFVF;		//���_�t�H�[�}�b�g�̃T�C�Y
	//BYTE *pVtxBuff;			//���_�o�b�t�@�ւ̃|�C���^

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (nCntObj = 0; nCntObj < MAX_OBJ; nCntObj++)
	{//�ϐ��̏�����
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

	//txt����I�u�W�F�N�g����ǂݎ��
	LoadObj(0);
}

//====================================================================
// �I�u�W�F�N�g�̏I������
//====================================================================
void UninitObject(void)
{
	//�ϐ��錾
	int nCntObj;			//for�J�E���g�p

	for (nCntObj = 0; nCntObj < NUM_OBJ_TYPE; nCntObj++)
	{
		for (int nCntMat = 0; nCntMat < MAX_OBJTEX; nCntMat++)
		{
			//�e�N�X�`���[�̔j��
			if (g_obj[nCntObj].pTexture[nCntMat] != NULL)
			{
				g_obj[nCntObj].pTexture[nCntMat]->Release();
				g_obj[nCntObj].pTexture[nCntMat] = NULL;
			}
		}
	}
	for (nCntObj = 0; nCntObj < NUM_OBJ_TYPE; nCntObj++)
	{
		//���b�V���̔j��
		if (g_pMeshObject[nCntObj] != NULL)
		{
			g_pMeshObject[nCntObj]->Release();
			g_pMeshObject[nCntObj] = NULL;
		}
		//�}�e���A���̔j��
		if (g_pBuffMatObject[nCntObj] != NULL)
		{
			g_pBuffMatObject[nCntObj]->Release();
			g_pBuffMatObject[nCntObj] = NULL;
		}
	}
}

//====================================================================
// �I�u�W�F�N�g�̍X�V����
//====================================================================
void UpdateObject(void)
{
	if (GetKeyboardTrigger(DIK_F6) == true)
	{
		//txt����I�u�W�F�N�g����ǂݎ��
		LoadObj(0);
	}
	if (GetKeyboardTrigger(DIK_F8) == true)
	{
		//txt����I�u�W�F�N�g����ǂݎ��
		LoadObj(1);
	}

	if (GetKeyboardTrigger(DIK_F5) == true)
	{
		for (int nCnt = 0; nCnt < MAX_OBJ; nCnt++)
		{
			if (g_object[nCnt].bUse == true)
			{
				g_object[nCnt].bUse = false;

				//�I�u�W�F�N�g�̃J�E���g��0��
				g_NumObj = 0;
			}
		}
	}

	//�e�̈ʒu��ݒ�
	//SetPositionShadow(g_object[nCntObj].nIdxShadow, g_object[nCntObj].pos);
}

//====================================================================
// �I�u�W�F�N�g�̕`�揈��
//====================================================================
void DrawObject(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;				//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;						//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;							//�}�e���A���f�[�^�ւ̃|�C���^

	for (int nCntObj = 0; nCntObj < MAX_OBJ; nCntObj++)
	{
		if (g_object[nCntObj].bUse == true)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_object[nCntObj].mtxWorld);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_object[nCntObj].rot.y, g_object[nCntObj].rot.x, g_object[nCntObj].rot.z);

			D3DXMatrixMultiply(&g_object[nCntObj].mtxWorld, &g_object[nCntObj].mtxWorld, &mtxRot);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_object[nCntObj].pos.x, g_object[nCntObj].pos.y, g_object[nCntObj].pos.z);

			D3DXMatrixMultiply(&g_object[nCntObj].mtxWorld, &g_object[nCntObj].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_object[nCntObj].mtxWorld);

			//���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			//�}�e���A���ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_pBuffMatObject[nCntObj]->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_dwNumMatObject[nCntObj]; nCntMat++)
			{
				//�}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_obj[nCntObj].pTexture[nCntMat]);

				//�I�u�W�F�N�g(�p�[�c)�̕`��
				g_pMeshObject[nCntObj]->DrawSubset(nCntMat);
			}

			//�ۑ�����Ă����}�e���A����߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}

//====================================================================
// �I�u�W�F�N�g�̃T�C�Y���o������
//====================================================================
void ObjSize(int nCntObj)
{
	DWORD dwSizeFVF;		//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;			//���_�o�b�t�@�ւ̃|�C���^
	int nNumVtx;			//���_��

	//���_�����擾
	nNumVtx = g_pMeshObject[nCntObj]->GetNumVertices();

	//���_�t�H�[�}�b�g�̃T�C�Y���擾
	dwSizeFVF = D3DXGetFVFVertexSize(g_pMeshObject[nCntObj]->GetFVF());

	//���_�o�b�t�@�����b�N
	g_pMeshObject[nCntObj]->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);
	
	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;			//���_���W�̑��

		if (g_object[nCntObj].vtxMax.x < vtx.x)
		{//x���傫��������
			g_object[nCntObj].vtxMax.x = vtx.x;
		}
		if (g_object[nCntObj].vtxMax.z < vtx.z)
		{//z�̒l���傫��������
			g_object[nCntObj].vtxMax.z = vtx.z;
		}

		if (g_object[nCntObj].vtxMin.x > vtx.x)
		{//x��������������
			g_object[nCntObj].vtxMin.x = vtx.x;
		}
		if (g_object[nCntObj].vtxMin.z > vtx.z)
		{//z�̒l���傫��������
			g_object[nCntObj].vtxMin.z = vtx.z;
		}

		if (g_object[nCntObj].vtxMax.y < vtx.y)
		{//x���傫��������
			g_object[nCntObj].vtxMax.y = vtx.y;
		}
		if (g_object[nCntObj].vtxMin.y > vtx.y)
		{//x��������������
			g_object[nCntObj].vtxMin.y = vtx.y;
		}

		pVtxBuff += dwSizeFVF;			//���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
	}
	
	//���_�o�b�t�@���A�����b�N
	g_pMeshObject[nCntObj]->UnlockVertexBuffer();

}

//====================================================================
// ���f���ݒu����
//====================================================================
void SetObject(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾
	D3DXMATERIAL *pMat;			//�}�e���A���ւ̃|�C���^

	for (int nCntObj = 0; nCntObj < MAX_OBJ; nCntObj++)
	{
		if (g_object[nCntObj].bUse == false)
		{
			//��������̏�����
			g_object[nCntObj].pos = pos;
			g_object[nCntObj].rot = rot;
			g_object[nCntObj].nType = nType;
			g_object[nCntObj].bUse = true;

			//X�t�@�C���̓ǂݍ���
			D3DXLoadMeshFromX(c_apModelnameObject[nType],
				D3DXMESH_SYSTEMMEM, pDevice,
				NULL, &g_pBuffMatObject[nCntObj],
				NULL, &g_dwNumMatObject[nCntObj],
				&g_pMeshObject[nCntObj]);

			//�}�e���A���ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_pBuffMatObject[nCntObj]->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_dwNumMatObject[nCntObj]; nCntMat++)
			{
				if (pMat[nCntMat].pTextureFilename != NULL)
				{//�t�@�C������e�N�X�`����ǂݍ���
					D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_obj[nCntObj].pTexture[nCntMat]);
				}
			}

			//�I�u�W�F�N�g�̃T�C�Y���v�Z
			ObjSize(nCntObj);

			//�I�u�W�F�N�g�����J�E���g(�f�o�b�O�p)
			g_NumObj++;

			break;
		}
	}
}

//====================================================================
// �I�u�W�F�N�g�̓����蔻��
//====================================================================
bool ClsObj(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pVtxMax, D3DXVECTOR3 *pVtxMin, D3DXVECTOR3 *pMove)
{
	bool bUse = false;

	//�I�u�W�F�N�g�Ƃ̓����蔻��
	for (int nCntObj = 0; nCntObj < MAX_OBJ; nCntObj++)
	{
		if (g_object[nCntObj].bUse == true)
		{
			if (pPos->y + pVtxMax->y > g_object[nCntObj].pos.y + g_object[nCntObj].vtxMin.y &&
				pPos->y + pVtxMin->y < g_object[nCntObj].pos.y + g_object[nCntObj].vtxMax.y)
			{//�㉺�̓����蔻��
				if (pPos->x + pVtxMax->x > g_object[nCntObj].pos.x + g_object[nCntObj].vtxMin.x &&
					pPos->x + pVtxMin->x < g_object[nCntObj].pos.x + g_object[nCntObj].vtxMax.x)
				{//���E�̓����蕝�̎w��
					if (pPosOld->z + pVtxMax->z <= g_object[nCntObj].pos.z + g_object[nCntObj].vtxMin.z &&
						pPos->z + pVtxMax->z >= g_object[nCntObj].pos.z + g_object[nCntObj].vtxMin.z)
					{//�O�̓����蔻��
						pPos->z = g_object[nCntObj].pos.z + g_object[nCntObj].vtxMin.z - pVtxMax->z;
						pMove->z = 0.0f;			//�ړ��ʂ�0��
						bUse = true;
					}
					if (pPosOld->z + pVtxMin->z >= g_object[nCntObj].pos.z + g_object[nCntObj].vtxMax.z &&
						pPos->z + pVtxMin->z < g_object[nCntObj].pos.z + g_object[nCntObj].vtxMax.z)
					{//��̓����蔻��
						pPos->z = g_object[nCntObj].pos.z + g_object[nCntObj].vtxMax.z - pVtxMin->z /*+ 0.001f*/;
						pMove->z = 0.0f;			//�ړ��ʂ�0��
						bUse = true;
					}
				}

				if (pPos->z + pVtxMax->z > g_object[nCntObj].pos.z + g_object[nCntObj].vtxMin.z &&
					pPos->z + pVtxMin->z < g_object[nCntObj].pos.z + g_object[nCntObj].vtxMax.z)
				{//�㉺�̓����蕝�̎w��
					if (pPosOld->x + pVtxMax->x + SIZE <= g_object[nCntObj].pos.x + g_object[nCntObj].vtxMin.x &&
						pPos->x + pVtxMax->x + SIZE >= g_object[nCntObj].pos.x + g_object[nCntObj].vtxMin.x)
					{//���̓����蔻��
						pPos->x = g_object[nCntObj].pos.x + g_object[nCntObj].vtxMin.x + pVtxMin->x - SIZE;
						pMove->x = 0.0f;			//�ړ��ʂ�0��
						bUse = true;
					}
					if (pPosOld->x + pVtxMin->x - SIZE >= g_object[nCntObj].pos.x + g_object[nCntObj].vtxMax.x &&
						pPos->x + pVtxMin->x - SIZE <= g_object[nCntObj].pos.x + g_object[nCntObj].vtxMax.x)
					{//�E�̓����蔻��
						pPos->x = g_object[nCntObj].pos.x + g_object[nCntObj].vtxMax.x + pVtxMax->x + SIZE;
						pMove->x = 0.0f;			//�ړ��ʂ�0��
						bUse = true;
					}
				}
			}
		}
	}
	return bUse;
}

//=====================================
// �I�u�W�F�N�g�̓ǂݍ���(txt)
//=====================================
void LoadObj(int nStage)
{
	//�ϐ��錾
	char not[128];
	FILE *pFile;			//�t�@�C���|�C���^��錾
	Object *pObject = GetObj();

	//�t�@�C�����J��
	pFile = fopen(c_apObjTxtName[nStage], "r");

	if (pFile != NULL)
	{//�t�@�C�����J�����ꍇ
		while (1)
		{
			fscanf(pFile, "%s", &not[0]);			//�������ǂݍ���

			if (strcmp("SCRIPT", &not[0]) == 0)
			{//SCRIPT���ǂݍ��߂���
				bLoadObj = true;			//�ǂݍ��݂��J�n
			}

			if (bLoadObj == true)
			{
				while (1)
				{
					fscanf(pFile, "%s", &not[0]);			//�������ǂݍ���

					if (strcmp("OBJECTSET", &not[0]) == 0)
					{
						fscanf(pFile, "%s", &not[0]);			//������ǂݍ���
						fscanf(pFile, "%s", &not[0]);			//=��ǂݍ���
						fscanf(pFile, "%f", &pObject->pos.x);
						fscanf(pFile, "%f", &pObject->pos.y);
						fscanf(pFile, "%f", &pObject->pos.z);

						fscanf(pFile, "%s", &not[0]);			//������ǂݍ���
						fscanf(pFile, "%s", &not[0]);			//=��ǂݍ���
						fscanf(pFile, "%f", &pObject->rot.x);
						fscanf(pFile, "%f", &pObject->rot.y);
						fscanf(pFile, "%f", &pObject->rot.z);

						fscanf(pFile, "%s", &not[0]);			//������ǂݍ���
						fscanf(pFile, "%s", &not[0]);			//=��ǂݍ���
						fscanf(pFile, "%d", &pObject->nType);
											
						//�G�̐ݒu
						SetObject(pObject->pos, pObject->rot, pObject->nType);

						//�|�C���^��i�߂�
						pObject++;
					}

					if (strcmp("END_SCRIPT", &not[0]) == 0)
					{//SCRIPT���ǂݍ��߂Ȃ������ꍇ
						bLoadObj = false;			//�ǂݍ��݂��I��
						break;			//�����𔲂���
					}
				}
			}

			if (strcmp("END_SCRIPT", &not[0]) == 0)
			{//SCRIPT���ǂݍ��߂Ȃ������ꍇ
				bLoadObj = false;			//�ǂݍ��݂��I��
				break;			//�����𔲂���
			}
		}
	}
}

//====================================================================
// �I�u�W�F�N�g���̎擾
//====================================================================
Object *GetObj(void)
{
	return &g_object[0];
}

//====================================================================
// �I�u�W�F�N�g���̎擾
//====================================================================
int GetNumObj(void)
{
	return g_NumObj;
}