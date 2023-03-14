//======================================================================================
//
//���U���g����[result.cpp]
//Author;���}���@�V
//
//======================================================================================
#include "result.h"
#include "main.h"
#include "input.h"
#include "fade.h"
#include "title.h"
#include "sound.h"

//�摜�̃t�@�C����
#define MAX_TEX		(2)

//���U���g�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;
} Result;

//�e�N�X�`���t�@�C����
const char *c_apFilenameResult[MAX_TEX] =
{
	"data\\TEXTURE\\no.png",			//�w�i�̉摜��ǂݍ���
	"data\\TEXTURE\\RESULT\\result.png",			//�w�i�̉摜��ǂݍ���
};

//�v���g�^�C�v�錾
void ResultBg(int nCnt);
void InitYourScore(int nCnt);

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureResult[MAX_TEX] = {};					//�e�N�X�`���[�ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult = NULL;				//���_�o�b�t�@�ւ̃|�C���^
Result g_Result;
int g_Score;

//========================================================================
// ���U���g�̏���������
//========================================================================
void InitResult(void)
{
	//�f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	for (int nCntTexture = 0; nCntTexture < MAX_TEX; nCntTexture++)
	{
		D3DXCreateTextureFromFile(pDevice, c_apFilenameResult[nCntTexture], &g_pTextureResult[nCntTexture]);
	}

	g_Result.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TEX,	//�厖
		D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED,
		&g_pVtxBuffResult,
		NULL);

	for (int nCnt = 0; nCnt < MAX_TEX; nCnt++)
	{
		switch (nCnt)
		{
		case 0:
			ResultBg(nCnt);
			break;
		case 1:
			InitYourScore(nCnt);
			break;
		}
	}

	//�T�E���h�̍Đ�
	//PlaySound(SOUND_LABEL_BGM001);
}

//---------------------------------------------------------
// ���U���g�w�i
//---------------------------------------------------------
void ResultBg(int nCnt)
{
	//���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += 4 * nCnt;

	g_Result.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_Result.pos.x, g_Result.pos.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Result.pos.x + 1280.0f, g_Result.pos.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Result.pos.x, g_Result.pos.y + 720.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Result.pos.x + 1280.0f, g_Result.pos.y + 720.0f, 0.0f);
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
	//�e�N�X�`���̍��W�ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffResult->Unlock();
}

//-------------------------------------------
// �X�R�A���(YourScore)
//-------------------------------------------
void InitYourScore(int nCnt)
{
	//���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += 4 * nCnt;

	g_Result.pos = D3DXVECTOR3(100.0f, 100.0f, 0.0f);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_Result.pos.x, g_Result.pos.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Result.pos.x + 300.0f, g_Result.pos.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Result.pos.x, g_Result.pos.y + 50.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Result.pos.x + 300.0f, g_Result.pos.y + 50.0f, 0.0f);
	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
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

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffResult->Unlock();
}

//========================================================================
// ���U���g�̏I������
//========================================================================
void UninitResult(void)
{
	//�T�E���h�̒�~
	//StopSound();

	//�e�N�X�`���̔j��
	for (int nCnt = 0; nCnt < MAX_TEX; nCnt++)
	{
		if (g_pTextureResult[nCnt] != NULL)
		{
			g_pTextureResult[nCnt]->Release();
			g_pTextureResult[nCnt] = NULL;
		}
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffResult != NULL)
	{
		g_pVtxBuffResult->Release();
		g_pVtxBuffResult = NULL;
	}
}

//========================================================================
// ���U���g�̍X�V����
//========================================================================
void UpdateResult(void)
{
	//�t�F�[�h���̎擾
	Fade pFade = GetFade();

	if (pFade == FADE_NONE)
	{
		if (GetKeyboardTrigger(DIK_RETURN) == true || GetJoyPadTrigger(BUTTON_A, 0) == true)
		{//����L�[(ENTER�L�[)�������ꂽ
		 //���[�h�̐ݒ�(�Q�[����ʂɈڍs)
			SetFade(MODE_TITLE);
		}
	}
}

//========================================================================
// ���U���g�̕`�揈��
//========================================================================
void DrawResult(void)
{
	//�f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();;

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffResult, 0,
		sizeof(VERTEX_2D));			//���_���\���̂̃T�C�Y

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCnt = 0; nCnt < MAX_TEX; nCnt++)
	{
		//�e�N�X�`���̐ݒ�z
		pDevice->SetTexture(0, g_pTextureResult[nCnt]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,				//�v���~�e�B�u�̎��
			nCnt * 4,						//�`�悷��ŏ��̃C���f�b�N�X(�厖)
			2);						//�v���~�e�B�u(�|���S��)��
	}
}