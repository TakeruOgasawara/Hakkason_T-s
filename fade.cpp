//======================================================================================
//
//�t�F�[�h����[Bg.cpp]
//Author;���}���@�V
//
//======================================================================================
#include "fade.h"

//�}�N����`
#define FADE_TIME		(0.02f)			//�t�F�[�h�̊Ԋu

//�O���[�o���錾
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFade = NULL;		//���_�o�b�t�@�ւ̃|�C���^
MODE g_modeNext;									//���̉��(���[�h)
D3DXCOLOR g_colorFade;								//�|���S��(�t�F�[�h)�̐F
Fade g_fade;										//�t�F�[�h�̏��

//========================================================================
//�t�F�[�h�̏���������
//========================================================================
void InitFade(MODE modeNext)
{
	//�f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�t�F�[�h�C�����
	g_fade = FADE_IN;

	//���̉�ʏ��
	g_modeNext = modeNext;

	//�����|���S����s������
	g_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffFade,
		NULL);

	//���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

	//���_���̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(1280.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 720.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(1280.0f, 720.0f, 0.0f);

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[(0.0f�`1.0f���Őݒ�)
	pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffFade->Unlock();

	//���[�h�̐ݒ�
	SetMode(g_modeNext);
}

//========================================================================
//�t�F�[�h�̏I������
//========================================================================
void UninitFade(void)
{
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffFade != NULL)
	{
		g_pVtxBuffFade->Release();
		g_pVtxBuffFade = NULL;
	}
}

//========================================================================
//�t�F�[�h�̍X�V����
//========================================================================
void UpdateFade(void)
{
	if (g_fade != FADE_NONE)
	{
		if (g_fade == FADE_IN)
		{//�t�F�[�h�C�����
			g_colorFade.a -= FADE_TIME;			//�|���S���𓧖��ɂ��Ă���

			if (g_colorFade.a <= 0.0f)
			{
				g_colorFade.a = 0.0f;
				g_fade = FADE_NONE;				//�������Ă��Ȃ����
			}
		}
		else if (g_fade == FADE_OUT)
		{//�t�F�[�h�A�E�g���
			g_colorFade.a += FADE_TIME;			//�|���S����s�����ɂ��Ă���

			if (g_colorFade.a >= 1.0f)
			{
				g_colorFade.a = 1.0f;
				g_fade = FADE_IN;				//�t�F�[�h�C�����

				//���[�h�̐ݒ�(���̉�ʂɈړ�)
				SetMode(g_modeNext);
			}
		}
		//���_���ւ̃|�C���^
		VERTEX_2D *pVtx;

		//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

		//���_�J���[(0.0f�`1.0f���Őݒ�)
		pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_colorFade.a);
		pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_colorFade.a);
		pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_colorFade.a);
		pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_colorFade.a);

		//���_�o�b�t�@���A�����b�N����
		g_pVtxBuffFade->Unlock();
	}
}

//========================================================================
//�t�F�[�h�̕`�揈��
//========================================================================
void DrawFade(void)
{
	//�f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffFade, 0,
		sizeof(VERTEX_2D));			//���_���\���̂̃T�C�Y

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, NULL);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,				//�v���~�e�B�u�̎��
		0,						//�`�悷��ŏ��̃C���f�b�N�X(�厖)
		2);						//�v���~�e�B�u(�|���S��)��
}

//========================================================================
//�t�F�[�h�̐ݒ菈��
//========================================================================
void SetFade(MODE modeNext)
{
	//�t�F�[�h�A�E�g���
	g_fade = FADE_OUT;

	//���̉��(���[�h)��ݒ�
	g_modeNext = modeNext;

	//�����|���S���𓧖��ɂ���
	g_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
}

//========================================================================
//�t�F�[�h�̎擾����
//========================================================================
Fade GetFade(void)
{
	return g_fade;
}
