//*****************************************
//
//���f���Ǘ�(model.cpp)
//Author:���R����
//
//*****************************************

//*****************************************
// �C���N���[�h
//*****************************************
#include "main.h"
#include "model.h"

//*****************************************
// �}�N����`
//*****************************************
#define MAX_STRING	(256)	//�������̍ő�

//*****************************************
// �O���[�o���ϐ��錾
//*****************************************

//========================================
//����������
//========================================
void InitModel(void)
{
}

//========================================
// �I������
//========================================
void UninitModel(void)
{
}

//========================================
// �ǂݍ��ݏ����܂Ƃ�
//========================================
void LoadAllModel(void)
{
}

//========================================
// ���f���̓ǂݍ���(�ėp)
//========================================
void LoadModel(Model *pModel,const char *pFilePass)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	//�v�Z�p���[�J���ϐ��錾
	int nNumVtx;	//���_��
	DWORD dwSizeFVF;	//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;	//���_�o�b�t�@�ւ̃|�C���^
	
	//�}�e���A���f�[�^�ւ̃|�C���^
	D3DXMATERIAL *pMat;

	//X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(pFilePass,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&pModel->pBuffMat,
		NULL,
		&pModel->dwNumMat,
		&pModel->pMesh);

	//�}�e���A�����ɑ΂���|�C���^���擾
	pMat = (D3DXMATERIAL*)pModel->pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)pModel->dwNumMat; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{
			//�e�N�X�`���̓ǂݍ���
			D3DXCreateTextureFromFile(pDevice,
				pMat[nCntMat].pTextureFilename,
				&pModel->apTexture[nCntMat]);
		}
	}

	//�ő�E�ŏ����_���擾================================================
	//���_�����擾
	nNumVtx = pModel->pMesh->GetNumVertices();

	//���_�t�H�[�}�b�g�̃T�C�Y���擾
	dwSizeFVF = D3DXGetFVFVertexSize(pModel->pMesh->GetFVF());

	//���_�o�b�t�@�̃��b�N
	pModel->pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntObject = 0; nCntObject < nNumVtx; nCntObject++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;			//���_���W�̑��

		if (vtx.x < pModel->vtxMin.x)
		{//X���̍ŏ�
			pModel->vtxMin.x = vtx.x;
		}
		if (vtx.x > pModel->vtxMax.x)
		{//X���̍ő�
			pModel->vtxMax.x = vtx.x;
		}

		if (vtx.y < pModel->vtxMin.y)
		{//Y���̍ŏ�
			pModel->vtxMin.y = vtx.y;
		}
		if (vtx.y > pModel->vtxMax.y)
		{//Y���̍ő�
			pModel->vtxMax.y = vtx.y;
		}

		if (vtx.z < pModel->vtxMin.z)
		{//Z���̍ŏ�
			pModel->vtxMin.z = vtx.z;
		}
		if (vtx.z > pModel->vtxMax.z)
		{//Z���̍ő�
			pModel->vtxMax.z = vtx.z;
		}

		pVtxBuff += dwSizeFVF;			//���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
	}

	//���_�o�b�t�@���A�����b�N
	pModel->pMesh->UnlockVertexBuffer();
	//�ő�E�ŏ����_���擾================================================
}

//========================================
// �ėp�I������
//========================================
void UninitXFile(Model *pModel)
{
	if (pModel->pBuffMat != NULL)
	{//�}�e���A���̔j��
		pModel->pBuffMat->Release();
		pModel->pBuffMat = NULL;
	}

	if (pModel->pMesh != NULL)
	{//���b�V���̔j��
		pModel->pMesh->Release();
		pModel->pMesh = NULL;
	}

	for (int nCntTex = 0; nCntTex < NUM_TEX; nCntTex++)
	{//�e�N�X�`���̔j��
		if (pModel->apTexture[nCntTex] != NULL)
		{
			pModel->apTexture[nCntTex]->Release();
			pModel->apTexture[nCntTex] = NULL;
		}
	}
}