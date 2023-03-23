//======================================================================================
//
// ���b�V���t�B�[���h�̏���[polygon.cpp]
// Author;���}���@�V
//
//======================================================================================
#include "meshfield.h"

//**********************************
//�}�N����`
//**********************************
#define MAX_TEX			(1)
#define FILED_WIDTH		(200.0f)			//����
#define FIELD_HEIGHT	(500.0f)			//�c��

#define SPLIT_X			(2)					//������(x)
#define SPLIT_Y			(2)					//������(z)

//***********************************
//�e�N�X�`���̓ǂݍ���
//***********************************
const char *c_apTexMeshFeald[] =
{
	"data\\TEXTURE\\GAME\\do-ro.png"
};

//***********************************
//�O���[�o����`
//***********************************
LPDIRECT3DTEXTURE9 g_pTextureMeshField[MAX_TEX] = {};				//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshField = NULL;			//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshfield = NULL;			//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^
MeshField g_aMeshfield[MAX_MESHFIELD];			//�t�B�[���h���

//====================================================================
// ���b�V���t�B�[���h�̏���������
//====================================================================
void InitMeshField(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntTex = 0; nCntTex < MAX_TEX; nCntTex++)
	{//�e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice, c_apTexMeshFeald[nCntTex], &g_pTextureMeshField[nCntTex]);
	}

	//�ϐ��̏�����
	for (int nCnt = 0; nCnt < MAX_MESHFIELD; nCnt++)
	{
		g_aMeshfield[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aMeshfield[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 14,
		D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED,
		&g_pVtxBuffMeshField, 
		NULL);

	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffMeshField->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	/*pVtx[0].pos = D3DXVECTOR3(-FILED_WIDTH, 0.0f, FIELD_HEIGHT);
	pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, FIELD_HEIGHT);
	pVtx[2].pos = D3DXVECTOR3(FILED_WIDTH, 0.0f, FIELD_HEIGHT);
	pVtx[3].pos = D3DXVECTOR3(-FILED_WIDTH, 0.0f, 0.0f);
	pVtx[4].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(FILED_WIDTH, 0.0f, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(-FILED_WIDTH, 0.0f, -FIELD_HEIGHT);
	pVtx[7].pos = D3DXVECTOR3(0.0f, 0.0f, -FIELD_HEIGHT);
	pVtx[8].pos = D3DXVECTOR3(FILED_WIDTH, 0.0f, -FIELD_HEIGHT);*/

	//�ϐ��̏�����
	int nCnt = 0;

	for (int nCnt0 = 0; nCnt0 < SPLIT_Y + 1; nCnt0++)
	{
		for (int nCnt1 = 0; nCnt1 < SPLIT_X + 1; nCnt1++)
		{
			//���_���W�̌v�Z(x)
			pVtx[nCnt].pos.x = nCnt1 * (FILED_WIDTH * 0.5f) - (FILED_WIDTH * 0.5f);

			//���_���W�̌v�Z(z)
			pVtx[nCnt].pos.z = (FIELD_HEIGHT * 0.5f) - nCnt0 * (FIELD_HEIGHT * 0.5f);

			if (nCnt < ((SPLIT_X + 1) * (SPLIT_Y + 1)))
			{
				//�J�E���g
				nCnt++;
			}
		}
	}

	for (int nCntColor = 0; nCntColor < ((SPLIT_X + 1) * (SPLIT_Y + 1)); nCntColor++)
	{
		//�@���x�N�g���̐ݒ�
		pVtx[nCntColor].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		pVtx[nCntColor].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	//�e�N�X�`���̍��W�ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[3].tex = D3DXVECTOR2(0.0f, 0.5f);
	pVtx[4].tex = D3DXVECTOR2(0.5f, 0.5f);
	pVtx[5].tex = D3DXVECTOR2(1.0f, 0.5f);
	pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(0.5f, 1.0f);
	pVtx[8].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffMeshField->Unlock();

	//�C���f�b�N�X�o�b�t�@�̐���(�ԍ����o���邽�߂̃�����)
	pDevice->CreateIndexBuffer(sizeof(WORD) * 14,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshfield, NULL);

	WORD *pIdx;			//�C���f�b�N�X���ւ̃|�C���^

	//�C���f�b�N�X�o�b�t�@�����b�N���A���_�ԍ��f�[�^�ւ̃|�C���^���擾
	g_pIdxBuffMeshfield->Lock(0, 0, (void**)&pIdx, 0);

	//���_�ԍ��f�[�^�̐ݒ�
	pIdx[0] = 3;
	pIdx[1] = 0;
	pIdx[2] = 4;
	pIdx[3] = 1;
	pIdx[4] = 5;
	pIdx[5] = 2;
	pIdx[6] = 2;
	pIdx[7] = 6;
	pIdx[8] = 6;
	pIdx[9] = 3;
	pIdx[10] = 7;
	pIdx[11] = 4;
	pIdx[12] = 8;
	pIdx[13] = 5;

	//�C���f�b�N�X�o�b�t�@���A�����b�N����
	g_pIdxBuffMeshfield->Unlock();

	SetMeshField(D3DXVECTOR3(0.0f, 0.0f, 100.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	SetMeshField(D3DXVECTOR3(100.0f, 0.0f, 600.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
}

//====================================================================
// ���b�V���t�B�[���h�̏I������
//====================================================================
void UninitMeshField(void)
{
	for (int nCntTex = 0; nCntTex < MAX_TEX; nCntTex++)
	{
		//�e�N�X�`���̔j��
		if (g_pTextureMeshField[nCntTex] != NULL)
		{
			g_pTextureMeshField[nCntTex]->Release();
			g_pTextureMeshField[nCntTex] = NULL;
		}
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffMeshField != NULL)
	{
		g_pVtxBuffMeshField->Release();
		g_pVtxBuffMeshField = NULL;
	}
	//�C���f�b�N�X�o�b�t�@�̔j��
	if (g_pIdxBuffMeshfield != NULL)
	{
		g_pIdxBuffMeshfield->Release();
		g_pIdxBuffMeshfield = NULL;
	}
}

//====================================================================
// ���b�V���t�B�[���h�̍X�V����
//====================================================================
void UpdateMeshField(void)
{
	
}

//====================================================================
// ���b�V���t�B�[���h�̕`�揈��
//====================================================================
void DrawMeshField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;				//�v�Z�p�}�g���b�N�X
	
	for (int nCnt = 0; nCnt < MAX_MESHFIELD; nCnt++)
	{
		if (g_aMeshfield[nCnt].bUse == true)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aMeshfield[nCnt].mtxWorldField);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aMeshfield[nCnt].rot.y, g_aMeshfield[nCnt].rot.x, g_aMeshfield[nCnt].rot.z);
			D3DXMatrixMultiply(&g_aMeshfield[nCnt].mtxWorldField, &g_aMeshfield[nCnt].mtxWorldField, &mtxRot);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aMeshfield[nCnt].pos.x, g_aMeshfield[nCnt].pos.y, g_aMeshfield[nCnt].pos.z);
			D3DXMatrixMultiply(&g_aMeshfield[nCnt].mtxWorldField, &g_aMeshfield[nCnt].mtxWorldField, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aMeshfield[nCnt].mtxWorldField);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffMeshField, 0,
				sizeof(VERTEX_3D));			//���_���\���̂̃T�C�Y

			//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetIndices(g_pIdxBuffMeshfield);

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);
		
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureMeshField[0]);

			//�|���S���̕`��
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0,
				9,			//�p�ӂ������_�̐�
				4 * nCnt,
				12);		//�`�悷��v���~�e�B�u�̐�
		}
	}
}

//====================================================================
// ���b�V���t�B�[���h�̐ݒ菈��
//====================================================================
void SetMeshField(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nTexType)
{
	for (int nCnt = 0; nCnt < MAX_MESHFIELD; nCnt++)
	{
		if (g_aMeshfield[nCnt].bUse == false)
		{
			g_aMeshfield[nCnt].pos = pos;
			g_aMeshfield[nCnt].rot = rot;
			g_aMeshfield[nCnt].nTexType = nTexType;
			g_aMeshfield[nCnt].bUse = true;
		}
	}
}