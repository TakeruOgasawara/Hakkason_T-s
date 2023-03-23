//==========================================================
//
// �t�H�O���� [fog.cpp]
// Author : �O�� ���V��
//
//==========================================================
#include "fog.h"
#include "debugproc.h"

//�O���[�o���ϐ�
FOG g_Fog;

//======================================
//����������
//======================================
void InitFog()
{
	//���l�̃��Z�b�g
	g_Fog.FogMode = D3DFOG_NONE;
	g_Fog.FogStart = 0.0f;
	g_Fog.FogEnd = 0.0f;
	g_Fog.FogDensity = 0.0f;
	g_Fog.FogColor = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
}

//======================================
//�I������
//======================================
void UninitFog()
{
	LPDIRECT3DDEVICE9 pDevice; //�f�o�C�X�̃|�C���^
	pDevice = GetDevice();
	pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE); //�t�H�O�̖�����
}

//======================================
//�X�V����
//======================================
void UpdateFog()
{
	if (g_Fog.FogMode != D3DFOG_NONE)
	{//�t�H�O��NONE�ł͂Ȃ���
		//�f�o�b�O�\��
		switch (g_Fog.FogMode)
		{
		case  D3DFOG_EXP:
			PrintDebugProc("���݂̃t�H�O���[�h�FEXP\n");
			break;
		case  D3DFOG_EXP2:
			PrintDebugProc("���݂̃t�H�O���[�h�FEXP2\n");
			break;
		case  D3DFOG_LINEAR:
			PrintDebugProc("���݂̃t�H�O���[�h�FLINEAR\n");
			break;

		default:
			break;
		}

		PrintDebugProc("�t�H�O�̐F�FR:%f G:%f B:%f A:%f\n", g_Fog.FogColor.r, g_Fog.FogColor.g, g_Fog.FogColor.b, g_Fog.FogColor.a);

		if (g_Fog.FogMode == D3DFOG_LINEAR)
		{
			PrintDebugProc("�t�H�O�̊J�n�n�_�F%f\n",g_Fog.FogStart);
			PrintDebugProc("�t�H�O�̏I���n�_�F%f\n", g_Fog.FogEnd);
		}
		else
		{
			PrintDebugProc("�t�H�O�̖��x�F%f\n", g_Fog.FogDensity);
		}
	}
	else
	{
		PrintDebugProc("���݂̃t�H�O���[�h�FNONE");
	}
}

//======================================
//�`�揈��
//======================================
void DrawFog()
{
	LPDIRECT3DDEVICE9 pDevice; //�f�o�C�X�̃|�C���^
	pDevice = GetDevice();


	//�t�H�O��L��������
		if (g_Fog.FogMode != D3DFOG_NONE)
		{//�t�H�O���[�h���L���Ȏ�
			pDevice->SetRenderState(D3DRS_FOGCOLOR, g_Fog.FogColor);					//�F�̐ݒ�
			pDevice->SetRenderState(D3DRS_FOGTABLEMODE, g_Fog.FogMode);					//���[�h�̐ݒ�
			pDevice->SetRenderState(D3DRS_FOGSTART, *((LPDWORD)(&g_Fog.FogStart)));		//�J�n�n�_�̐ݒ�
			pDevice->SetRenderState(D3DRS_FOGEND, *((LPDWORD)(&g_Fog.FogEnd)));			//�I���n�_�̐ݒ�
			pDevice->SetRenderState(D3DRS_FOGDENSITY, *((LPDWORD)(&g_Fog.FogDensity)));	//�t�H�O���x�̐ݒ�
			pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);								//�t�H�O�̗L����
		}
	else
	{//�t�H�O�𖳌�������
		pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE); //�t�H�O�̖�����
	}
}

//======================================
//�ݒ菈��
//======================================
void SetFog(D3DFOGMODE FogMode, D3DXCOLOR FogColor, float FogStart, float FogEnd, float FogDensity)
{
	g_Fog.FogMode = FogMode;        //���[�h�̐ݒ�
	g_Fog.FogColor = FogColor;		//�F�̐ݒ�
	g_Fog.FogStart = FogStart;		//�J�n�n�_�̐ݒ�		*�t�H�O���[�h��D3DFOG_LINEAR�̎��ɎQ�Ƃ����*
	g_Fog.FogEnd = FogEnd;			//�I���n�_�̐ݒ�		*�t�H�O���[�h��D3DFOG_LINEAR�̎��ɎQ�Ƃ����*
	g_Fog.FogDensity = FogDensity;	//�t�H�O���x�̐ݒ�		0.0f�`1.0f�̊Ԃ̐��l�����	*�t�H�O���[�h��D3DFOG_LINEAR�ł͖������ɎQ�Ƃ����*
}