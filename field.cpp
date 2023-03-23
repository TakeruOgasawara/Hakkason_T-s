//======================================================================================
//
// �|���S���̏���[polygon.cpp]
// Author;���}���@�V
//
//======================================================================================
#include "field.h"

//*****************************
//�}�N����`
//*****************************
#define BG_TEX_FULE		"data\\TEXTURE\\GAME\\do-ro.png"			//���w�i�̉摜��ǂݍ���
#define FILED_WIDTH			(150.0f)
#define FILED_HEIGHT		(600000.0f)		//500
#define MAX_FIELD			(128)			//�t�B�[���h�̍ő吔

//*****************************
//�O���[�o����`
//*****************************
LPDIRECT3DTEXTURE9 g_pTextureField = NULL;				//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffField = NULL;			//���_�o�b�t�@�ւ̃|�C���^
Field g_field[MAX_FIELD];			//�t�B�[���h���

//====================================================================
//�|���S���̏���������
//====================================================================
void InitField(void)
{
	//�ϐ��錾
	int nCntField;			//for�J�E���g�p

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, BG_TEX_FULE, &g_pTextureField);

	//�ϐ��̏�����
	for (nCntField = 0; nCntField < MAX_FIELD; nCntField++)
	{
		g_field[nCntField].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_field[nCntField].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_field[nCntField].bUse = false;
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_FIELD,
		D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED,
		&g_pVtxBuffField, 
		NULL);

	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffField->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntField = 0; nCntField < MAX_FIELD; nCntField++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_field[nCntField].pos .x - FILED_WIDTH, 0.0f, g_field[nCntField].pos.x + FILED_HEIGHT);
		pVtx[1].pos = D3DXVECTOR3(g_field[nCntField].pos.x + FILED_WIDTH, 0.0f, g_field[nCntField].pos.x + FILED_HEIGHT);
		pVtx[2].pos = D3DXVECTOR3(g_field[nCntField].pos.x - FILED_WIDTH, 0.0f, g_field[nCntField].pos.x - FILED_HEIGHT);
		pVtx[3].pos = D3DXVECTOR3(g_field[nCntField].pos.x + FILED_WIDTH, 0.0f, g_field[nCntField].pos.x - FILED_HEIGHT);
		//�@���x�N�g���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		//���_�J���[(0.0f�`1.0f���Őݒ�)
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		//�e�N�X�`���̍��W�ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1200.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1200.0f);

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffField->Unlock();

	SetField(D3DXVECTOR3(0.0f, 0.0f, 599900.0f));
}

//====================================================================
//�|���S���̏I������
//====================================================================
void UninitField(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureField != NULL)
	{
		g_pTextureField->Release();
		g_pTextureField = NULL;
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffField != NULL)
	{
		g_pVtxBuffField->Release();
		g_pVtxBuffField = NULL;
	}
}

//====================================================================
//�|���S���̍X�V����
//====================================================================
void UpdateField(void)
{

}

//====================================================================
//�|���S���̕`�揈��
//====================================================================
void DrawField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;				//�v�Z�p�}�g���b�N�X
	
	for (int nCntField = 0; nCntField < MAX_FIELD; nCntField++)
	{
		if (g_field[nCntField].bUse == true)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_field[nCntField].mtxWorldField);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_field[nCntField].rot.y, g_field[nCntField].rot.x, g_field[nCntField].rot.z);
			D3DXMatrixMultiply(&g_field[nCntField].mtxWorldField, &g_field[nCntField].mtxWorldField, &mtxRot);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_field[nCntField].pos.x, g_field[nCntField].pos.y, g_field[nCntField].pos.z);
			D3DXMatrixMultiply(&g_field[nCntField].mtxWorldField, &g_field[nCntField].mtxWorldField, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_field[nCntField].mtxWorldField);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffField, 0,
				sizeof(VERTEX_3D));			//���_���\���̂̃T�C�Y

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureField);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,				//�v���~�e�B�u�̎��
				nCntField * 4,						//�`�悷��ŏ��̃C���f�b�N�X
				2);
		}
	}
}

//====================================================================
// �t�B�[���h�̐ݒ菈��
//====================================================================
void SetField(D3DXVECTOR3 pos)
{
	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffField->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntField = 0; nCntField < MAX_FIELD; nCntField++)
	{
		if (g_field[nCntField].bUse == false)
		{
			g_field[nCntField].pos = pos;

			g_field[nCntField].bUse = true;

			break;
		}
		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffField->Unlock();
}