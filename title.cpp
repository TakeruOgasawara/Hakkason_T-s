//======================================================================================
//
//�^�C�g������[title.cpp]
//Author;���}���@�V
//
//======================================================================================
#include "title.h"
#include "main.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "debugproc.h"
#include "field.h"
#include "camera.h"
#include "light.h"

//*****************************
//�}�N����`
//*****************************
#define NUM_TEX_BUFF0			(1)		//�g�p����e�N�X�`���\�̐�
#define NUM_TEX_BUFF1			(2)		//�g�p����e�N�X�`���\�̐�
#define NUM_BUFF		(2)

//*****************************
// �񋓌^�̒�`
//*****************************
typedef enum
{
	TITLESTATE_NONE,
	TITLESTATE_IN,
	TITLESTATE_OUT,
	TITLESTATE_MAX
} TITLESTATE;

//�^�C�g���̑I�����[�h
typedef enum
{
	TITLEMODE_START,
	TITLEMODE_TUTORIAL,
	TITLEMODE_MAX
} TITLEMODE;

//*****************************
// �t�F�[�h�p�̗񋓌^
//*****************************
typedef struct
{
	D3DXVECTOR3 pos;
} Title;

//*****************************
//�e�N�X�`���t�@�C����
//*****************************
const char *c_apTitleTexFilename[NUM_TEX_BUFF0 + NUM_TEX_BUFF1] =
{
	"data\\TEXTURE\\TITLE\\title.png",			//�^�C�g�����S
	"data\\TEXTURE\\TITLE\\start.png",			//�X�^�[�g���S
	"data\\TEXTURE\\TILE\\tutorial.png",		//�`���[�g���A�����S
};	

//*****************************
//�v���g�^�C�v�錾
//*****************************
//Buff[0]
void InitTitleFont(void);			//�^�C�g�����S
void DrawTitleFont(void);
//Buff[1]
void InitTitleStart(int nCntTitle);			//�Z���N�g���S
void DrawTitleStart(void);
void InitTitleTutorial(int nCntTitle);
//���̑�
void TileFade(void);			//�����̓_��

//�^�C�g��
void InitTitleCamera(void);
void UninitTitleCamera(void);
void UpdateTitleCamera(void);
void DrawTitleCamera(void);


//*****************************
//�O���[�o���ϐ�
//*****************************
LPDIRECT3DTEXTURE9 g_pTextureTitle[NUM_TEX_BUFF0 + NUM_TEX_BUFF1] = {};					//�e�N�X�`���[�ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle[NUM_BUFF] = {};				//���_�o�b�t�@�ւ̃|�C���^

Title g_Title;				//�^�C�g�����(�\����)
TITLESTATE g_Start;			//�^�C�g�����(�񋓌^)

D3DXVECTOR3 g_Centerpos;
D3DXCOLOR g_StartColor;
D3DXCOLOR g_StartColor1;

bool g_TitleState;
int g_nCntTitle;
int g_CurrentNumberTitle;
float g_TexU;

//========================================================================
//�^�C�g���̏���������
//========================================================================
void InitTitle(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();;

	for (int nCntTex = 0; nCntTex < NUM_TEX_BUFF0 + NUM_TEX_BUFF1; nCntTex++)
	{//�e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice, c_apTitleTexFilename[nCntTex], &g_pTextureTitle[nCntTex]);
	}

	//�ϐ��̏�����
	g_Title.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Start = TITLESTATE_IN;

	g_Centerpos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
	g_StartColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_StartColor1 = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	g_nCntTitle = 0;
	g_TitleState = false;
	g_TexU = 0.0f;

	//------------------------------------------------------------------------
	//���_�o�b�t�@�̐���[0]
	//------------------------------------------------------------------------
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_TEX_BUFF0,
		D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED,
		&g_pVtxBuffTitle[0],
		NULL);

	//�^�C�g�����S�̏�����
	InitTitleFont();

	//------------------------------------------------------------------------
	//���_�o�b�t�@�̐���[1]
	//------------------------------------------------------------------------
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_TEX_BUFF1,
		D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED,
		&g_pVtxBuffTitle[1],
		NULL);

	
	//�X�^�[�g���S�̏�����
	InitTitleStart(0);
		
	InitTitleCamera();

	//�T�E���h�̍Đ�
	PlaySound(SOUND_LABEL_BGM000);
}

//----------------------------------------
// �^�C�g�������̏�����
//----------------------------------------
void InitTitleFont(void)
{
	//���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTitle[0]->Lock(0, 0, (void**)&pVtx, 0);

	g_Title.pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);

	//���_���W�̐ݒ�(�^�C�g������)
	pVtx[0].pos = D3DXVECTOR3(g_Title.pos.x - 300, g_Title.pos.y - 300, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Title.pos.x + 300, g_Title.pos.y - 300, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Title.pos.x - 300, g_Title.pos.y - 70, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Title.pos.x + 300, g_Title.pos.y - 70, 0.0f);
	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
	//���_�J���[(0.0f�`1.0f���Őݒ�)
	pVtx[0].col = g_StartColor;
	pVtx[1].col = g_StartColor;
	pVtx[2].col = g_StartColor;
	pVtx[3].col = g_StartColor;
	//�e�N�X�`���̍��W�ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTitle[0]->Unlock();
}

//----------------------------------------
// �X�^�[�g�����̏�����
//----------------------------------------
void InitTitleStart(int nCntTitle)
{
	//���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTitle[1]->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += 4 * nCntTitle;

	g_Title.pos = D3DXVECTOR3(SCREEN_WIDTH / 2, 500.0f, 0.0f);

	//���_���W�̐ݒ�(START����)
	pVtx[0].pos = D3DXVECTOR3(g_Title.pos.x - 130, g_Title.pos.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Title.pos.x + 130, g_Title.pos.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Title.pos.x - 130, g_Title.pos.y + 70, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Title.pos.x + 130, g_Title.pos.y + 70, 0.0f);
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
	g_pVtxBuffTitle[1]->Unlock();
}

//----------------------------------------
// �`���[�g���A�������̏�����
//----------------------------------------
void InitTitleTutorial(int nCntTitle)
{
	//���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTitle[1]->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += 4 * nCntTitle;

	g_Title.pos = D3DXVECTOR3(1100.0f, 600.0f, 0.0f);

	//���_���W�̐ݒ�(START����)
	pVtx[0].pos = D3DXVECTOR3(g_Title.pos.x - 130, g_Title.pos.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Title.pos.x + 130, g_Title.pos.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Title.pos.x - 130, g_Title.pos.y + 70, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Title.pos.x + 130, g_Title.pos.y + 70, 0.0f);
	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
	//���_�J���[(0.0f�`1.0f���Őݒ�)
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
	//�e�N�X�`���̍��W�ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTitle[1]->Unlock();
}

//========================================================================
//�^�C�g���̏I������
//========================================================================
void UninitTitle(void)
{
	//�T�E���h�̒�~
	StopSound();

	for (int nCntTex = 0; nCntTex < NUM_TEX_BUFF0 + NUM_TEX_BUFF1; nCntTex++)
	{
		//�e�N�X�`���̔j��
		if (g_pTextureTitle[nCntTex] != NULL)
		{
			g_pTextureTitle[nCntTex]->Release();
			g_pTextureTitle[nCntTex] = NULL;
		}
	}
	for (int nCntBuff = 0; nCntBuff < NUM_BUFF; nCntBuff++)
	{//���_�o�b�t�@�̔j��
		if (g_pVtxBuffTitle[nCntBuff] != NULL)
		{
			g_pVtxBuffTitle[nCntBuff]->Release();
			g_pVtxBuffTitle[nCntBuff] = NULL;
		}
	}

	UninitTitleCamera();
}

//========================================================================
//�^�C�g���̍X�V����
//========================================================================
void UpdateTitle(void)
{
	//�^�C�g�������̓_��(���̃��[�h��)
	TileFade();

	UpdateTitleCamera();
}

//------------------------------
// �t�F�[�h����(�_��)
//------------------------------
void TileFade(void)
{
	//�t�F�[�h���̎擾
	Fade pFade = GetFade();

	//���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	//�X�^�[�g�̓_�ŏ���
	if (pFade == FADE_NONE)
	{
		if (GetKeyboardTrigger(DIK_RETURN) == true || GetJoyPadTrigger(BUTTON_A, 0) == true)
		{//����L�[(ENTER�L�[)�������ꂽ
			if (g_TitleState == false)
			{
				//�T�E���h�̍Đ�
				PlaySound(SOUND_LABEL_SE_ENTER);

				//�X�^�[�g������_�ŏ�Ԃ�
				g_TitleState = true;
			}
		}
	}

	if (g_TitleState == true)
	{
		if (g_Start == TITLESTATE_IN)
		{//�t�F�[�h�C�����
			g_StartColor.a -= 0.2f;			//�|���S���𓧖��ɂ��Ă���

			if (g_StartColor.a <= 0.4f)
			{
				g_StartColor.a = 0.4f;

				g_Start = TITLESTATE_OUT;
			}
		}

		else if (g_Start == TITLESTATE_OUT)
		{//�t�F�[�h�A�E�g���
			g_StartColor.a += 0.2f;			//�|���S����s�����ɂ��Ă���

			if (g_StartColor.a >= 1.0f)
			{
				g_StartColor.a = 1.0f;

				g_Start = TITLESTATE_IN;
				g_nCntTitle++;
			}
		}
	}
	if (g_nCntTitle == 5)
	{
		//���[�h�̐ݒ�(�Q�[����ʂɈڍs)
		SetFade(MODE_TUTORIAL);
	}

	if (g_Start == TITLESTATE_IN)
	{//�t�F�[�h�C�����
		g_StartColor.a -= 0.02f;			//�|���S���𓧖��ɂ��Ă���

		if (g_StartColor.a <= 0.0f)
		{
			g_StartColor.a = 0.0f;

			g_Start = TITLESTATE_OUT;
		}
	}
	else if (g_Start == TITLESTATE_OUT)
	{//�t�F�[�h�A�E�g���
		g_StartColor.a += 0.02f;			//�|���S����s�����ɂ��Ă���

		if (g_StartColor.a >= 1.0f)
		{
			g_StartColor.a = 1.0f;

			g_Start = TITLESTATE_IN;
		}
	}

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTitle[1]->Lock(0, 0, (void**)&pVtx, 0);

	//���_�J���[(0.0f�`1.0f���Őݒ�)
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_StartColor.a);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_StartColor.a);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_StartColor.a);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_StartColor.a);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTitle[1]->Unlock();
}

//========================================================================
//�^�C�g���̕`�揈��
//========================================================================
void DrawTitle(void)
{
	//�^�C�g���J�����̕`��
	DrawTitleCamera();

	//�^�C�g�����S�̕`�揈��
	DrawTitleFont();

	//�Z���N�g�����̕`�揈��
	DrawTitleStart();
}

//-----------------------------------------------------------
//�^�C�g�����S�̕`�揈��
//-----------------------------------------------------------
void DrawTitleFont(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//=============================================================================
	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTitle[0], 0,
		sizeof(VERTEX_2D));			//���_���\���̂̃T�C�Y

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureTitle[0]);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,				//�v���~�e�B�u�̎��
		0,						//�`�悷��ŏ��̃C���f�b�N�X(�厖)
		2);						//�v���~�e�B�u(�|���S��)��
	
}

//-----------------------------------------------------------
//�Z���N�g�����̕`�揈��
//-----------------------------------------------------------
void DrawTitleStart(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//=============================================================================
	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTitle[1], 0,
		sizeof(VERTEX_2D));			//���_���\���̂̃T�C�Y

	for (int nCnt = 0; nCnt < NUM_TEX_BUFF1; nCnt++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureTitle[nCnt + 1]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,				//�v���~�e�B�u�̎��
			nCnt * 4,						//�`�悷��ŏ��̃C���f�b�N�X(�厖)
			2);						//�v���~�e�B�u(�|���S��)��
	}
}



//============================================================
//
//============================================================
void InitTitleCamera(void)
{
	InitLight();

	InitCamera();
	
	InitField();
}

//============================================================
//
//============================================================
void UninitTitleCamera(void)
{
	UninitField();
}

//============================================================
//
//============================================================
void UpdateTitleCamera(void)
{
	//Camera *pCamera = GetCamera();

	//pCamera->rot.y += 0.004f;

	////y
	//if (pCamera->rot.y > D3DX_PI)
	//{
	//	pCamera->rot.y = -D3DX_PI;
	//}
	//if (pCamera->rot.y < -D3DX_PI)
	//{
	//	pCamera->rot.y = D3DX_PI;
	//}

	////�J�������X�V����
	//pCamera->posV.x = pCamera->posR.x + (cosf(pCamera->rot.z) * sinf(pCamera->rot.y)) * -1200.0f;
	//pCamera->posV.y = pCamera->posR.y + 500.0f + sinf(pCamera->rot.z) * -1200.0f;			//���_Y
	//pCamera->posV.z = pCamera->posR.z + (cosf(pCamera->rot.z) * cosf(pCamera->rot.y)) * -1200.0f;

	UpdateCamera();
}

//============================================================
//
//============================================================
void DrawTitleCamera(void)
{
	Camera *pCamera = GetCamera();

	SetCamera(0);

	DrawField();

#ifdef _DEBUG		//�f�o�b�O���̂�
	PrintDebugProc("�y�J�������z\n");
	PrintDebugProc("���_     [%f  %f  %f]\n", pCamera->posV.x, pCamera->posV.y, pCamera->posV.z);
	PrintDebugProc("�����_   [%f  %f  %f]\n", pCamera->posR.x, pCamera->posR.y, pCamera->posR.z);
	PrintDebugProc("����     [%f  %f  %f]\n", pCamera->rot.x, pCamera->rot.y, pCamera->rot.z);
	PrintDebugProc("�Ǐ]�؂�ւ�[F9])\n");
#endif
}