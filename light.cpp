//======================================================================================
//
// ���C�g�̏���[light.cpp]
// Author;���}���@�V
//
//======================================================================================
#include "light.h"

//*****************************
//�}�N����`
//*****************************
#define MAX_LIGHT			(3)

//*****************************
//�O���[�o���錾
//*****************************
D3DLIGHT9 g_light[MAX_LIGHT];			//���C�g�̏��

//====================================================================
// ���C�g�̏���������
//====================================================================
void InitLight(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾

	D3DXVECTOR3 vecDir;			//�ݒ�p�����x�N�g��

	for (int nCntLight = 0; nCntLight < MAX_LIGHT; nCntLight++)
	{
		//���C�g�̏����N���A����
		ZeroMemory(&g_light[nCntLight], sizeof(D3DLIGHT9));

		switch (nCntLight)
		{
		case 0:
			//���C�g1�̎�ނ�ݒ�
			g_light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

			//���C�g1�̊g�U����ݒ�
			g_light[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//���C�g1�̕�����ݒ�
			vecDir = D3DXVECTOR3(0.22f, -0.87f, 0.44f);
			D3DXVec3Normalize(&vecDir, &vecDir);			//�x�N�g���𐳋K������
			g_light[nCntLight].Direction = vecDir;
			break;
		case 1:
			//���C�g2�̎�ނ�ݒ�
			g_light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

			//���C�g2�̊g�U����ݒ�
			g_light[nCntLight].Diffuse = D3DXCOLOR(0.65f, 0.65f, 0.65f, 1.0f);

			//���C�g2�̕�����ݒ�
			vecDir = D3DXVECTOR3(-0.18f, 0.88f, -0.44f);
			D3DXVec3Normalize(&vecDir, &vecDir);			//�x�N�g���𐳋K������
			g_light[nCntLight].Direction = vecDir;
			break;
		case 2:
			//���C�g3�̎�ނ�ݒ�
			g_light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

			//���C�g3�̊g�U����ݒ�
			g_light[nCntLight].Diffuse = D3DXCOLOR(0.15f, 0.15f, 0.15f, 1.0f);

			//���C�g3�̕�����ݒ�
			vecDir = D3DXVECTOR3(0.89f, -0.11f, 0.44f);
			D3DXVec3Normalize(&vecDir, &vecDir);			//�x�N�g���𐳋K������
			g_light[nCntLight].Direction = vecDir;
			break;
		}

		//���C�g��ݒ�
		pDevice->SetLight(nCntLight, &g_light[nCntLight]);

		//���C�g��L��������
		pDevice->LightEnable(nCntLight, TRUE);
	}
}

//====================================================================
// ���C�g�̏I������
//====================================================================
void UninitLight(void)
{

}

//====================================================================
// ���C�g�̍X�V����
//====================================================================
void UpdateLight(void)
{

}

