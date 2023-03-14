//======================================================================================
//
//�G�t�F�N�g�̏���[Player.cpp]
//Author;���}���@�V
//
//======================================================================================
#include <stdio.h>
#include "main.h"
#include "effect.h"

//*****************************
//�}�N����`
//*****************************
#define EFFECT_TEX_FILE		"data\\TEXTURE\\effect000.jpg"			//�e�N�X�`����

//*****************************
//�O���[�o���錾
//*****************************
LPDIRECT3DTEXTURE9 g_pTextureEffect = NULL;					//�e�N�X�`���\�ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;			//���_�o�b�t�@�ւ̃|�C���^
Effect g_aEffect[MAX_EFFECT];								//�G�t�F�N�g�̏��
int g_EffectCnt;

//========================================================================
//�e�̏���������
//========================================================================
void InitEffect(void)
{
	//�ϐ��錾
	int nCntEffect;

	//�f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, EFFECT_TEX_FILE, &g_pTextureEffect);

	//�ϐ��̏�����
	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		//�\���̂̏�����
		g_aEffect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].fRadius = 0.0f;
		g_aEffect[nCntEffect].nLife = 0;
		g_aEffect[nCntEffect].nType = 0;
		g_aEffect[nCntEffect].bUse = false;				//�g�p���Ă��Ȃ���Ԃɂ���
	}

	//�O���[�o���錾�̏�����
	g_EffectCnt = 0;

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_EFFECT,
		D3DUSAGE_WRITEONLY,
		pDevice->SetFVF(FVF_VERTEX_2D),
		D3DPOOL_MANAGED,
		&g_pVtxBuffEffect,
		NULL);

	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);
		//�@���x�N�g���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		//���_�J���[(0.0f�`1.0f���Őݒ�)
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		//�e�N�X�`���̍��W�ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;			//���_�f�[�^�̃|�C���^��4���i�߂�
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEffect->Unlock();
}

//========================================================================
//�G�t�F�N�g�̏I������
//========================================================================
void UninitEffect(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureEffect != NULL)
	{
		g_pTextureEffect->Release();
		g_pTextureEffect = NULL;
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}
}

//========================================================================
//�G�t�F�N�g�̍X�V����
//========================================================================
void UpdateEffect(void)
{
	//�ϐ��錾
	int nCntEffect;

	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{//�G�t�F�N�g���g�p����Ă���ꍇ
			//�d�͏���
			g_aEffect[nCntEffect].move.y -= 0.3f;

			if (g_aEffect[nCntEffect].pos.y <= 0.0f)
			{
				g_aEffect[nCntEffect].move.y += 0.5f;
			}

			//�ʒu�����X�V
			g_aEffect[nCntEffect].pos.y += g_aEffect[nCntEffect].move.y;
			g_aEffect[nCntEffect].pos.x += g_aEffect[nCntEffect].move.x;
			g_aEffect[nCntEffect].pos.z += g_aEffect[nCntEffect].move.z;

			//�����̌��Z
			g_aEffect[nCntEffect].nLife--;

			//�ړ��ʂ̍X�V
			g_aEffect[nCntEffect].pos += g_aEffect[nCntEffect].move;

			//�J���[�̌��Z//���̎������}�b�N�X�̎���
			g_aEffect[nCntEffect].col.a = ((float)g_aEffect[nCntEffect].nLife) / ((float)g_aEffect[nCntEffect].nMaxLife);

			//���a�̌��Z//���̎������}�b�N�X�̎���
			g_aEffect[nCntEffect].fRadius -= ((float)g_aEffect[nCntEffect].nLife) / ((float)g_aEffect[nCntEffect].nMaxLife);

			if (g_aEffect[nCntEffect].nLife <= 0 || g_aEffect[nCntEffect].col.a <= 0 || g_aEffect[nCntEffect].fRadius <= 0)
			{//������0�̏ꍇ
				g_aEffect[nCntEffect].bUse = false;
			}

			//���_�J���[(0.0f�`1.0f���Őݒ�)
			pVtx[0].col = D3DXCOLOR(g_aEffect[nCntEffect].col);
			pVtx[1].col = D3DXCOLOR(g_aEffect[nCntEffect].col);
			pVtx[2].col = D3DXCOLOR(g_aEffect[nCntEffect].col);
			pVtx[3].col = D3DXCOLOR(g_aEffect[nCntEffect].col);

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);
		}
		pVtx += 4;
	}
		//���_�o�b�t�@���A�����b�N����
		g_pVtxBuffEffect->Unlock();
}

//========================================================================
//�G�t�F�N�g�̕`�揈��
//========================================================================
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾
	D3DXMATRIX mtxTrans;						//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxView;							//�r���[�}�g���b�N�X�̎擾�p

	//z�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);		//��r���@
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);			//z�o�b�t�@�X�V�̗L�������ݒ�

	//�A���t�@�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);			//�A���t�@�e�X�g�̗L��/�����̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);		//��r���@
	pDevice->SetRenderState(D3DRS_ALPHAREF, 10);					//��l

	//a�u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);			//���C�e�B���O�̗L��/�����̐ݒ�

	g_EffectCnt = 0;

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aEffect[nCntEffect].mtxWorldBullet);

			//�r���[�}�g���b�N�X���擾
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//�|���S�����J�����ɑ΂��Đ��ʂɌ�����
			D3DXMatrixInverse(&g_aEffect[nCntEffect].mtxWorldBullet, NULL, &mtxView);

			g_aEffect[nCntEffect].mtxWorldBullet._41 = 0.0f;
			g_aEffect[nCntEffect].mtxWorldBullet._42 = 0.0f;
			g_aEffect[nCntEffect].mtxWorldBullet._43 = 0.0f;

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aEffect[nCntEffect].pos.x, g_aEffect[nCntEffect].pos.y, g_aEffect[nCntEffect].pos.z);
			D3DXMatrixMultiply(&g_aEffect[nCntEffect].mtxWorldBullet, &g_aEffect[nCntEffect].mtxWorldBullet, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aEffect[nCntEffect].mtxWorldBullet);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffEffect,
				0, sizeof(VERTEX_3D));			//���_���\���̂̃T�C�Y

												//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureEffect);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,				//�v���~�e�B�u�̎��
				nCntEffect * 4,						//�`�悷��ŏ��̃C���f�b�N�X
				2);						//�v���~�e�B�u(�|���S��)��

			g_EffectCnt++;
		}
	}

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);			//���C�e�B���O�̗L��/�����̐ݒ�

	//a�u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//�A���t�@�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);			//�A���t�@�e�X�g�̗L��/�����̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);		//��r���@
	pDevice->SetRenderState(D3DRS_ALPHAREF, 10);					//��l

	//z�o�b�t�@��L���ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);			//z�o�b�t�@�X�V�̗L�������ݒ�
}

//========================================================================
//�G�t�F�N�g�̐ݒ菈��
//========================================================================
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, int nLife)
{
	//�ϐ��錾
	int nCntEffect;

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == false)
		{//�g���Ă��Ȃ��ꍇ
			g_aEffect[nCntEffect].pos = pos;
			g_aEffect[nCntEffect].col = col;
			g_aEffect[nCntEffect].move = move;
			g_aEffect[nCntEffect].fRadius = fRadius;
			g_aEffect[nCntEffect].nLife = nLife;
			g_aEffect[nCntEffect].nMaxLife = nLife;
			g_aEffect[nCntEffect].bUse = true;				//�g�p���Ă����Ԃɂ���
			break;
		}
	}
}

void SetEffectLIfe(int nLife)
{

}


//========================================================================
//�G�t�F�N�g�̎擾
//========================================================================
Effect *GetEffect(void)
{
	return &g_aEffect[0];
}

//========================================================================
//�G�t�F�N�g�̎擾
//========================================================================
int GetEffectNum(void)
{
	return g_EffectCnt;
}