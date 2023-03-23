//========================================================================================
//
// �G�t�F�N�g�̏���[Effect.cpp]
// Author: �O�� ���V��
//
//========================================================================================

#include "Effect.h"


//�v���g�^�C�v�錾
#define MAX_Effect (40480)
#define NUM_EFFECT (2)

//�\���̂�錾
typedef struct
{
	D3DXVECTOR3 pos; //�ʒu
	D3DXCOLOR col; //�F
	D3DXVECTOR3 move;
	D3DXMATRIX  mtx;
	float fRot;
	float fRadius; //���a
	int nLife; //����
	int nType;
	bool bUse; //�g�p���Ă��邩
}Effect;

//�O���[�o���ϐ�
D3DXVECTOR3 g_rotEffect;
float g_fLengthEffect;
float g_fAngleEffect;


LPDIRECT3DTEXTURE9 g_pTextureEffect[NUM_EFFECT] = {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;
Effect g_aEffect[MAX_Effect];

//======================================
//����������
//======================================
void InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntEffect;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Shadow000.jpg",
		&g_pTextureEffect[0]);
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\effect002.tga",
		&g_pTextureEffect[1]);

	
	//�e�̏��̏�����
	for (nCntEffect = 0; nCntEffect < MAX_Effect; nCntEffect++)
	{
		g_aEffect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aEffect[nCntEffect].bUse = false;
	}
	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_Effect,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEffect,
		NULL);

	VERTEX_3D *pVtx;
	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_Effect; nCntEffect++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3((- 10), (- 10), 0.0f);
		pVtx[1].pos = D3DXVECTOR3((+ 10), (- 10), 0.0f);
		pVtx[2].pos = D3DXVECTOR3((- 10), (+ 10), 0.0f);
		pVtx[3].pos = D3DXVECTOR3((+ 10), (+ 10), 0.0f);

		//�@���x�N�g���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}
	g_pVtxBuffEffect->Unlock();
}

//======================================
//�I������
//======================================
void UninitEffect(void)
{
	for (int nCnt = 0; nCnt < NUM_EFFECT; nCnt++)
	{
		if (g_pTextureEffect[nCnt] != NULL)
		{
			g_pTextureEffect[nCnt]->Release();
			g_pTextureEffect[nCnt] = NULL;
		}
	}

	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}
}
//======================================
//�X�V����
//======================================
void UpdateEffect(void)
{
	int nCntEffect;

	VERTEX_3D *pVtx;
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_Effect; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{//�G�t�F�N�g���g�p����Ă���
			
			g_aEffect[nCntEffect].nLife -= 1;
			g_aEffect[nCntEffect].fRadius -= g_aEffect[nCntEffect].fRadius / g_aEffect[nCntEffect].nLife;
			g_aEffect[nCntEffect].col.a *= (g_aEffect[nCntEffect].nLife * 0.015f);
			g_aEffect[nCntEffect].pos += g_aEffect[nCntEffect].move;
			

			pVtx[0].pos = D3DXVECTOR3(( - g_aEffect[nCntEffect].fRadius), ( + g_aEffect[nCntEffect].fRadius), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(( + g_aEffect[nCntEffect].fRadius), ( + g_aEffect[nCntEffect].fRadius), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(( - g_aEffect[nCntEffect].fRadius), ( - g_aEffect[nCntEffect].fRadius), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(( + g_aEffect[nCntEffect].fRadius), ( - g_aEffect[nCntEffect].fRadius), 0.0f);

			pVtx[0].col = g_aEffect[nCntEffect].col;
			pVtx[1].col = g_aEffect[nCntEffect].col;
			pVtx[2].col = g_aEffect[nCntEffect].col;
			pVtx[3].col = g_aEffect[nCntEffect].col;

			if (g_aEffect[nCntEffect].nLife <= 0 || g_aEffect[nCntEffect].fRadius <= 0.0f)
			{
				g_aEffect[nCntEffect].bUse = false;
			}

		}pVtx += 4;
	}

	g_pVtxBuffEffect->Unlock();
}

//======================================
//�`�揈��
//======================================
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice; //�f�o�C�X�̃|�C���^
	pDevice = GetDevice();

	D3DXMATRIX  mtxTrans; //�v�Z�p�}�g���N�X
	D3DXMATRIX mtxView; //�r���[�}�g���N�X�擾�p

	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
						
	//�A���t�@�e�X�g��L����
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 80);

	////Z�e�X�g�𖳌���
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	for (int nCnt = 0; nCnt < MAX_Effect; nCnt++)
	{
		if (g_aEffect[nCnt].bUse == true)
		{
			if (g_aEffect[nCnt].nType == 0)
			{
				//a�u�����f�B���O�����Z�����ɐݒ�
				pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
				pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			}
			
			//���[���h�}�g���N�X�̏�����
			D3DXMatrixIdentity(&g_aEffect[nCnt].mtx);

			//�r���[�}�g���N�X�擾
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//�|���S�����J�����ɑ΂��Đ��ʂɌ�����
			D3DXMatrixInverse(&g_aEffect[nCnt].mtx, NULL, &mtxView);

			g_aEffect[nCnt].mtx._41 = 0.0f;
			g_aEffect[nCnt].mtx._42 = 0.0f;
			g_aEffect[nCnt].mtx._43 = 0.0f;

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aEffect[nCnt].pos.x, g_aEffect[nCnt].pos.y, g_aEffect[nCnt].pos.z);

			D3DXMatrixMultiply(&g_aEffect[nCnt].mtx, &g_aEffect[nCnt].mtx, &mtxTrans);

			//���[���h�}�g���N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aEffect[nCnt].mtx);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureEffect[g_aEffect[nCnt].nType]);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
		}
	}
	////Z�e�X�g��L����
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//�A���t�@�e�X�g�𖳌���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 80);

	//a�u�����f�B���O��ʏ�ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}
//======================================
//�ݒ菈��
//======================================
void SetEffect(D3DXVECTOR3 pos, D3DXCOLOR col, D3DXVECTOR3 move,float fRadius, int nLife,int nType)
{
	int nCntEffect;
	VERTEX_3D *pVtx;
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_Effect; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == false)
		{
			g_aEffect[nCntEffect].pos = pos;
			g_aEffect[nCntEffect].col = col;
			g_aEffect[nCntEffect].fRadius = fRadius;
			g_aEffect[nCntEffect].nLife = nLife;
			g_aEffect[nCntEffect].move = move;
			g_aEffect[nCntEffect].nType = nType;
			g_aEffect[nCntEffect].bUse = true;
			break;
		}

		pVtx += 4;
	}
	g_pVtxBuffEffect->Unlock();
}

