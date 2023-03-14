//======================================================================================
//
//�t�F�[�h����[Bg.cpp]
//Author;���}���@�V
//
//======================================================================================
#include"game.h"
#include"pause.h"
#include"input.h"
#include"fade.h"

#define NUM_PAUSE_TEX		(4)			//�e�N�X�`���[�̐�
#define PAUSE_MENU			(3)			//�|�[�Y���j���[�̐�
#define MAX_BUFF			(2)			//�o�b�t�@�̐�

//�e�N�X�`���t�@�C����
const char *c_apFilenamePause[NUM_PAUSE_TEX] =
{
	"data\\TEXTURE\\no.png",
	"data\\TEXTURE\\Pause_000.png",
	"data\\TEXTURE\\Pause_100.png",
	"data\\TEXTURE\\Pause_200.png"
};

//�v���g�^�C�v�錾
void InitFrame(void);
void InitMenu(void);

//�O���[�o���錾
LPDIRECT3DTEXTURE9 g_pTexturePause[NUM_PAUSE_TEX] = {};					//�e�N�X�`���[�ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause[MAX_BUFF] = {};					//���_�o�b�t�@�ւ̃|�C���^
Pause g_Pause;
D3DXCOLOR g_NormalPauseColor;
D3DXCOLOR g_PalePauseColor;
int g_CurrentNumber;

//========================================================================
//�|�[�Y�̏���������
//========================================================================
void InitPause()
{
	//�f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�|�[�Y���̎擾
	Pause *pPause = GetPause();

	//�e�N�X�`���̓ǂݍ���
	for (int nCntTexture = 0; nCntTexture < NUM_PAUSE_TEX; nCntTexture++)
	{
		D3DXCreateTextureFromFile(pDevice, c_apFilenamePause[nCntTexture], &g_pTexturePause[nCntTexture]);
	}

	//�\���̂̏�����
	g_Pause.bPause = false;

	//�O���[�o���錾�̏�����
	g_NormalPauseColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);				//�I������Ă���Ƃ�
	g_PalePauseColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);			//�I������Ă��Ȃ��Ƃ�
	g_CurrentNumber = 0;

	//===============================���j���[�g=========================================
	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY, pDevice->SetFVF(FVF_VERTEX_2D), D3DPOOL_MANAGED,
		&g_pVtxBuffPause[0], NULL);

	InitFrame();

	//===============================���j���[��=========================================
	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * PAUSE_MENU,
		D3DUSAGE_WRITEONLY, pDevice->SetFVF(FVF_VERTEX_2D), D3DPOOL_MANAGED,
		&g_pVtxBuffPause[1], NULL);

	InitMenu();
}

//========================================================================
//�|�[�Y�g�̏���������
//========================================================================
void InitFrame(void)
{
	//���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	g_Pause.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//���_�o�b�t�@�����b�N
	g_pVtxBuffPause[0]->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_Pause.pos.x, g_Pause.pos.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Pause.pos.x + 1280.0f, g_Pause.pos.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Pause.pos.x, g_Pause.pos.y + 720, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Pause.pos.x + 1280.0f, g_Pause.pos.y + 720.0f, 0.0f);
	//rhw�̐ݒ�
	for (int nCntrhw = 0; nCntrhw < 4; nCntrhw++)
	{
		pVtx[nCntrhw].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[nCntrhw].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);
	}
	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPause[0]->Unlock();
}

//========================================================================
//�|�[�Y���j���[���̏���������
//========================================================================
void InitMenu(void)
{
	//���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffPause[1]->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPause = 0; nCntPause < PAUSE_MENU; nCntPause++)
	{
		g_Pause.pos = D3DXVECTOR3(SCREEN_WIDTH / 2, 300.0f, 0.0f);

		if (nCntPause == 0)
		{
			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_Pause.pos.x - 100.0f, g_Pause.pos.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_Pause.pos.x + 100.0f, g_Pause.pos.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_Pause.pos.x - 100.0f, g_Pause.pos.y + 50.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_Pause.pos.x + 100.0f, g_Pause.pos.y + 50.0f, 0.0f);
			//rhw�̐ݒ�
			for (int nCntrhw = 0; nCntrhw < 4; nCntrhw++)
			{
				pVtx[nCntrhw].rhw = 1.0f;

				//���_�J���[�̐ݒ�
				pVtx[nCntrhw].col = g_NormalPauseColor;
			}
			//�e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			pVtx += 4;					//���_�f�[�^��4�����炷
		}
		else
		{
			g_Pause.pos = D3DXVECTOR3(SCREEN_WIDTH / 2, 300.0f + 75.0f * nCntPause, 0.0f);

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_Pause.pos.x - 100.0f, g_Pause.pos.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_Pause.pos.x + 100.0f, g_Pause.pos.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_Pause.pos.x - 100.0f, g_Pause.pos.y + 50.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_Pause.pos.x + 100.0f, g_Pause.pos.y + 50.0f, 0.0f);
			//rhw�̐ݒ�
			for (int nCntrhw = 0; nCntrhw < 4; nCntrhw++)
			{
				pVtx[nCntrhw].rhw = 1.0f;

				//���_�J���[�̐ݒ�
				pVtx[nCntrhw].col = g_PalePauseColor;
			}
			//�e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			pVtx += 4;					//���_�f�[�^��4�����炷
		}
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPause[1]->Unlock();
}


//========================================================================
//�|�[�Y�̏I������
//========================================================================
void UninitPause()
{
	int nCntPause;						//for�J�E���g�p

	for (nCntPause = 0; nCntPause < NUM_PAUSE_TEX; nCntPause++)
	{
		//�e�N�X�`���̔j��
		if (g_pTexturePause[nCntPause] != NULL)
		{
			g_pTexturePause[nCntPause]->Release();
			g_pTexturePause[nCntPause] = NULL;
		}
	}
	for (int nCntBuff = 0; nCntBuff < MAX_BUFF; nCntBuff++)
	{
		//���_�o�b�t�@�̔j��
		if (g_pVtxBuffPause[nCntBuff] != NULL)
		{
			g_pVtxBuffPause[nCntBuff]->Release();
			g_pVtxBuffPause[nCntBuff] = NULL;
		}
	}
}

//========================================================================
//�|�[�Y�̍X�V����
//========================================================================
void UpdatePause()
{
	//�|�[�Y���̎擾
	Pause *pPause = GetPause();

	//�t�F�[�h���̎擾
	Fade pfade = GetFade();

	//���_���̃|�C���^
	VERTEX_2D *pVtx;

	if (/*GetKeyboardTrigger(DIK_S) == true || */GetJoyPadTrigger(BUTTON_1, 0) == true && pfade == FADE_NONE)
	{
		//���_�o�b�t�@�����b�N
		g_pVtxBuffPause[1]->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += g_CurrentNumber * 4;

		//���_�J���[�̐ݒ�
		pVtx[0].col = g_PalePauseColor;
		pVtx[1].col = g_PalePauseColor;
		pVtx[2].col = g_PalePauseColor;
		pVtx[3].col = g_PalePauseColor;
		
		//���_�o�b�t�@���A�����b�N����
		g_pVtxBuffPause[1]->Unlock();

		//���ݒn����
		g_CurrentNumber = (g_CurrentNumber + 1) % PAUSE_MENU;

		//���_�o�b�t�@�����b�N
		g_pVtxBuffPause[1]->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += g_CurrentNumber * 4;

		//���_�J���[�̐ݒ�
		pVtx[0].col = g_NormalPauseColor;
		pVtx[1].col = g_NormalPauseColor;
		pVtx[2].col = g_NormalPauseColor;
		pVtx[3].col = g_NormalPauseColor;

		//���_�o�b�t�@���A�����b�N����
		g_pVtxBuffPause[1]->Unlock();
	}
	else if (/*GetKeyboardTrigger(DIK_W) == true || */GetJoyPadTrigger(BUTTON_0, 0) == true && pfade == FADE_NONE)
	{
		//���_�o�b�t�@�����b�N
		g_pVtxBuffPause[1]->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += g_CurrentNumber * 4;

		//���_�J���[�̐ݒ�
		pVtx[0].col = g_PalePauseColor;
		pVtx[1].col = g_PalePauseColor;
		pVtx[2].col = g_PalePauseColor;
		pVtx[3].col = g_PalePauseColor;

		//���_�o�b�t�@���A�����b�N����
		g_pVtxBuffPause[1]->Unlock();

		//���ݒn��O��
		g_CurrentNumber = (g_CurrentNumber - 1 + PAUSE_MENU) % PAUSE_MENU;
		
		//���_�o�b�t�@�����b�N
		g_pVtxBuffPause[1]->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += 4 * g_CurrentNumber;

		//���_�J���[�̐ݒ�
		pVtx[0].col = g_NormalPauseColor;
		pVtx[1].col = g_NormalPauseColor;
		pVtx[2].col = g_NormalPauseColor;
		pVtx[3].col = g_NormalPauseColor;

		//���_�o�b�t�@���A�����b�N����
		g_pVtxBuffPause[1]->Unlock();
	}

	if (pfade == FADE_NONE)
	{
		if (GetKeyboardTrigger(DIK_RETURN) == true || GetJoyPadTrigger(BUTTON_A, 0) == true && g_CurrentNumber == 0 && pfade == FADE_NONE)
		{//����L�[(ENTER�L�[)�������ꂽ
		 //�|�[�Y��Ԃ���������
			pPause->bPause = false;
		}
		else if (GetKeyboardTrigger(DIK_RETURN) == true || GetJoyPadTrigger(BUTTON_A, 0) == true && g_CurrentNumber == 1 && pfade == FADE_NONE)
		{//����L�[(ENTER�L�[)�������ꂽ
		 //���[�h�̐ݒ�(�Q�[����ʂɈڍs)
			SetFade(MODE_GAME);
		}
		else if (GetKeyboardTrigger(DIK_RETURN) == true || GetJoyPadTrigger(BUTTON_A, 0) == true && g_CurrentNumber == 2 && pfade == FADE_NONE)
		{//����L�[(ENTER�L�[)�������ꂽ
		 //���[�h�̐ݒ�(�^�C�g����ʂɈڍs)
			SetFade(MODE_TITLE);
		}
	}
}

//========================================================================
//�|�[�Y�̕`�揈��
//========================================================================
void DrawPause()
{
	//�f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//=============================================================================
	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffPause[0], 0,
		sizeof(VERTEX_2D));			//���_���\���̂̃T�C�Y
		
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTexturePause[0]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,				//�v���~�e�B�u�̎��
			0,						//�`�悷��ŏ��̃C���f�b�N�X
			2);						//�v���~�e�B�u(�|���S��)��

	//=============================================================================
	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffPause[1], 0,
		sizeof(VERTEX_2D));			//���_���\���̂̃T�C�Y

	for (int nCntPause = 1; nCntPause < PAUSE_MENU + 1; nCntPause++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTexturePause[nCntPause]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,				//�v���~�e�B�u�̎��
			nCntPause * 4 - 4,						//�`�悷��ŏ��̃C���f�b�N�X
			2);						//�v���~�e�B�u(�|���S��)��
	}
}

//========================================================================
//�|�[�Y���̎擾
//========================================================================
Pause *GetPause(void)
{
	return &g_Pause;
}