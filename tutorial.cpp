//======================================================================================
//
//�`���[�g���A������[tutorial.cpp]
//Author;���}���@�V
//
//======================================================================================
#include <stdio.h>
#include "tutorial.h"
#include "input.h"
#include "fade.h"

//*****************************
//�}�N����`
//*****************************
#define NUM_TEX			(1)		//�g�p����e�N�X�`���\�̐�

//*****************************
//�e�N�X�`���t�@�C����
//*****************************
const char *c_apFilenameTutorial[NUM_TEX] =
{
	"data\\TEXTURE\\TUTORIAL\\tutorial.png",					//�w�i�̉摜��ǂݍ���
};

//*****************************
//�O���[�o���ϐ�
//*****************************
LPDIRECT3DTEXTURE9 g_pTextureTutorial[NUM_TEX] = {};					//�e�N�X�`���[�ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTutorial = NULL;				//���_�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3 g_pos;
D3DXCOLOR g_EnterCol;
STARTT g_StartState;
int g_nCntStateTile;
bool g_TutorialState;

//========================================================================
//�`���[�g���A������������
//========================================================================
void InitTutorial(void)
{
	//�f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice;

	//���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	for (int nCntTexture = 0; nCntTexture < NUM_TEX; nCntTexture++)
	{
		D3DXCreateTextureFromFile(pDevice, c_apFilenameTutorial[nCntTexture], &g_pTextureTutorial[nCntTexture]);
	}

	//�O���[�o���錾�̏�����
	g_pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
	g_EnterCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_StartState = STARTT_IN;
	g_nCntStateTile = 0;
	g_TutorialState = false;

	//=============================================================================
	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_TEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTutorial,
		NULL);

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTutorial->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�(�^�C�g���w�i)
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
	pVtx[0].col = D3DXCOLOR(1.0, 1.0, 1.0, 1.0);
	pVtx[1].col = D3DXCOLOR(1.0, 1.0, 1.0, 1.0);
	pVtx[2].col = D3DXCOLOR(1.0, 1.0, 1.0, 1.0);
	pVtx[3].col = D3DXCOLOR(1.0, 1.0, 1.0, 1.0);
	//�e�N�X�`���̍��W�ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTutorial->Unlock();
}

//========================================================================
//�`���[�g���A���̏I������
//========================================================================
void UninitTutorial(void)
{
	//�ϐ��錾
	int nCntTutorial;

	for (nCntTutorial = 0; nCntTutorial < NUM_TEX; nCntTutorial++)
	{
		//�e�N�X�`���̔j��
		if (g_pTextureTutorial[nCntTutorial] != NULL)
		{
			g_pTextureTutorial[nCntTutorial]->Release();
			g_pTextureTutorial[nCntTutorial] = NULL;
		}
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffTutorial != NULL)
	{
		g_pVtxBuffTutorial->Release();
		g_pVtxBuffTutorial = NULL;
	}
}

//========================================================================
//�`���[�g���A���̍X�V����
//========================================================================
void UpdateTutorial(void)
{
	//�t�F�[�h���̎擾
	Fade pFade = GetFade();
	
	if (pFade == FADE_NONE)
	{
		if (GetKeyboardTrigger(DIK_RETURN) == true || GetJoyPadTrigger(BUTTON_A, 0) == true && pFade == FADE_NONE)
		{//����L�[(ENTER�L�[)�������ꂽ
			//���[�h�̐ݒ�(�Q�[����ʂɈڍs)
			SetFade(MODE_GAME);
		}
	}
}

//========================================================================
//�`���[�g���A���̕`�揈��
//========================================================================
void DrawTutorial(void)
{
	//�ϐ��錾
	int nCntTutorial;

	//�f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice;
	
	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//=============================================================================
	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTutorial, 0,
		sizeof(VERTEX_2D));			//���_���\���̂̃T�C�Y
	
	for (nCntTutorial = 0; nCntTutorial < NUM_TEX; nCntTutorial++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureTutorial[nCntTutorial]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,				//�v���~�e�B�u�̎��
			nCntTutorial * 4,						//�`�悷��ŏ��̃C���f�b�N�X(�厖)
			2);						//�v���~�e�B�u(�|���S��)��
	}
	
}
