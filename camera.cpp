//======================================================================================
//
// カメラの処理[camera.cpp]
// Author;小笠原　彪
//
//======================================================================================
#include <stdio.h>
#include "camera.h"
#include "input.h"
#include "debugproc.h"
#include "player.h"

//*****************************
// マクロ定義
//*****************************
#define CAMERA_SPEED		(2.5f)			//カメラの速度
#define CAMERA_DISTANCE		(100.0f)		//視点と注視点の距離
#define CAMELA_AUTO			(0.0001f)		//カメラが後ろに回る時の慣性
#define CAMERA_ROTATE		(0.04f)			//カメラの回転速度

#define TITLE_CAMERA_SISTANCE		(1200.0f)

//*****************************
// プロトタイプ宣言
//*****************************
void CameraOps(void);			//カメラ操作の処理
void CameraMove(int nCnt);			//カメラの移動
void CameraRot(int nCnt);
void CameraV(int nCnt);				//視点操作の処理
void CameraR(int nCnt);				//注視点操作の処理
void CameraRoll(int nCnt);			//カメラのロール処理

//*****************************
// グローバル宣言
//*****************************
Camera g_aCamera[MAX_CAMERA];			//カメラ情報

//====================================================================
// カメラの初期化処理
//====================================================================
void InitCamera(void)
{
	//変数の初期化
	for (int nCntCamera = 0; nCntCamera < MAX_CAMERA; nCntCamera++)
	{
		g_aCamera[nCntCamera].posV	= D3DXVECTOR3(0.0f, 100.0f, 0.0f);
		g_aCamera[nCntCamera].posR	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aCamera[nCntCamera].vecU	= D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		
		g_aCamera[nCntCamera].rot	= D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
		g_aCamera[nCntCamera].fRoll = 0.0f;
		g_aCamera[nCntCamera].fLengthCamera	= CAMERA_DISTANCE;						//注視点から視点間の距離
		g_aCamera[nCntCamera].bUse	= false;
	}

	g_aCamera[0].viewport.X = 0;				// 描画する画面の左上X座標
	g_aCamera[0].viewport.Y = 0;				// 描画する画面の左上Y座標
	g_aCamera[0].viewport.Width = 1280;			// 画面の幅
	g_aCamera[0].viewport.Height = 720;			// 画面の高さ
	g_aCamera[0].viewport.MinZ = 0.0f;			// 
	g_aCamera[0].viewport.MaxZ = 1.0f;			// 
}

//====================================================================
//カメラの終了処理処理
//====================================================================
void UninitCamera(void)
{

}

//====================================================================
//カメラの更新処理
//====================================================================
void UpdateCamera(void)
{
	//カメラの操作が入ってる関数
	CameraOps();

	//プレイヤー追従処理
	//FollowPlayer();
}

//====================================================================
//カメラの操作
//====================================================================
void CameraOps(void)
{
	for (int nCnt = 0; nCnt < MAX_CAMERA; nCnt++)
	{
		//カメラの移動
		CameraMove(nCnt);

		//視点操作
		CameraV(nCnt);

		//注視点移動
		CameraR(nCnt);

		//カメラの向き修正
		CameraRot(nCnt);

		//カメラのロール
		CameraRoll(nCnt);
	}
}

//====================================================================
// プレイヤーの追従
//====================================================================
void FollowPlayer(void)
{
	//情報取得
	Player *pPlayer = GetPlayer();

	for (int nCnt = 0; nCnt < MAX_CAMERA; nCnt++)
	{
		//プレイヤーの位置を注視点に設定
		g_aCamera[nCnt].posR = pPlayer->pos;

		//視点の相対位置設定
		g_aCamera[nCnt].posV.x = g_aCamera[nCnt].posR.x;
		g_aCamera[nCnt].posV.y = g_aCamera[nCnt].posR.y + 50.0f;			//視点Y
		g_aCamera[nCnt].posV.z = g_aCamera[nCnt].posR.z + g_aCamera[nCnt].fLengthCamera;
	}
}

//--------------------------------------------------------------------
// カメラの向き修正
//--------------------------------------------------------------------
void CameraRot(int nCnt)
{
	//向きが3.14以上-3.14以下に行かないようにする
	//y
	if (g_aCamera[nCnt].rot.y > D3DX_PI)
	{
		g_aCamera[nCnt].rot.y = -D3DX_PI;
	}
	if (g_aCamera[nCnt].rot.y < -D3DX_PI)
	{
		g_aCamera[nCnt].rot.y = D3DX_PI;
	}
	//z
	if (g_aCamera[nCnt].rot.z > D3DX_PI)
	{
		g_aCamera[nCnt].rot.z = -D3DX_PI;
	}
	if (g_aCamera[nCnt].rot.z < -D3DX_PI)
	{
		g_aCamera[nCnt].rot.z = D3DX_PI;
	}
	//x
	if (g_aCamera[nCnt].rot.x > D3DX_PI)
	{
		g_aCamera[nCnt].rot.x = -D3DX_PI;
	}
	if (g_aCamera[nCnt].rot.x < -D3DX_PI)
	{
		g_aCamera[nCnt].rot.x = D3DX_PI;
	}
}

//--------------------------------------------------------------------
//カメラの移動
//--------------------------------------------------------------------
void CameraMove(int nCnt)
{
	if (g_aCamera[nCnt].bUse == true)
	{
		//八方向移動
		if (GetKeyboardPress(DIK_A) == true)
		{//Aキーが押された
			if (GetKeyboardPress(DIK_W) == true)
			{//左上移動
				g_aCamera[nCnt].posV.x += sinf(g_aCamera[nCnt].rot.y + -D3DX_PI * 0.25f) * CAMERA_SPEED;
				g_aCamera[nCnt].posR.x += sinf(g_aCamera[nCnt].rot.y + -D3DX_PI * 0.25f) * CAMERA_SPEED;
				g_aCamera[nCnt].posV.z += cosf(g_aCamera[nCnt].rot.y + -D3DX_PI * 0.25f) * CAMERA_SPEED;
				g_aCamera[nCnt].posR.z += cosf(g_aCamera[nCnt].rot.y + -D3DX_PI * 0.25f) * CAMERA_SPEED;
			}
			else if (GetKeyboardPress(DIK_S) == true)
			{//左下移動
				g_aCamera[nCnt].posV.x += sinf(g_aCamera[nCnt].rot.y + -D3DX_PI * 0.75f) * CAMERA_SPEED;
				g_aCamera[nCnt].posR.x += sinf(g_aCamera[nCnt].rot.y + -D3DX_PI * 0.75f) * CAMERA_SPEED;
				g_aCamera[nCnt].posV.z += cosf(g_aCamera[nCnt].rot.y + -D3DX_PI * 0.75f) * CAMERA_SPEED;
				g_aCamera[nCnt].posR.z += cosf(g_aCamera[nCnt].rot.y + -D3DX_PI * 0.75f) * CAMERA_SPEED;
			}
			else
			{//左移動
				//カメラ位置情報の更新
				g_aCamera[nCnt].posV.x += sinf(g_aCamera[nCnt].rot.y + -D3DX_PI * 0.5f) * CAMERA_SPEED;
				g_aCamera[nCnt].posR.x += sinf(g_aCamera[nCnt].rot.y + -D3DX_PI * 0.5f) * CAMERA_SPEED;
				g_aCamera[nCnt].posV.z += cosf(g_aCamera[nCnt].rot.y + -D3DX_PI * 0.5f) * CAMERA_SPEED;
				g_aCamera[nCnt].posR.z += cosf(g_aCamera[nCnt].rot.y + -D3DX_PI * 0.5f) * CAMERA_SPEED;
			}
		}
		else if (GetKeyboardPress(DIK_D) == true)
		{//Dキーが押された
			if (GetKeyboardPress(DIK_W) == true)
			{//右上移動
				g_aCamera[nCnt].posV.x += sinf(g_aCamera[nCnt].rot.y + D3DX_PI * 0.25f) * CAMERA_SPEED;
				g_aCamera[nCnt].posR.x += sinf(g_aCamera[nCnt].rot.y + D3DX_PI * 0.25f) * CAMERA_SPEED;
				g_aCamera[nCnt].posV.z += cosf(g_aCamera[nCnt].rot.y + D3DX_PI * 0.25f) * CAMERA_SPEED;
				g_aCamera[nCnt].posR.z += cosf(g_aCamera[nCnt].rot.y + D3DX_PI * 0.25f) * CAMERA_SPEED;
			}
			else if (GetKeyboardPress(DIK_S) == true)
			{//右下移動
				g_aCamera[nCnt].posV.x += sinf(g_aCamera[nCnt].rot.y + D3DX_PI * 0.75f) * CAMERA_SPEED;
				g_aCamera[nCnt].posR.x += sinf(g_aCamera[nCnt].rot.y + D3DX_PI * 0.75f) * CAMERA_SPEED;
				g_aCamera[nCnt].posV.z += cosf(g_aCamera[nCnt].rot.y + D3DX_PI * 0.75f) * CAMERA_SPEED;
				g_aCamera[nCnt].posR.z += cosf(g_aCamera[nCnt].rot.y + D3DX_PI * 0.75f) * CAMERA_SPEED;
			}
			else
			{//右移動
				//カメラ位置情報の更新
				g_aCamera[nCnt].posV.x += sinf(g_aCamera[nCnt].rot.y + D3DX_PI * 0.5f) * CAMERA_SPEED;
				g_aCamera[nCnt].posR.x += sinf(g_aCamera[nCnt].rot.y + D3DX_PI * 0.5f) * CAMERA_SPEED;
				g_aCamera[nCnt].posV.z += cosf(g_aCamera[nCnt].rot.y + D3DX_PI * 0.5f) * CAMERA_SPEED;
				g_aCamera[nCnt].posR.z += cosf(g_aCamera[nCnt].rot.y + D3DX_PI * 0.5f) * CAMERA_SPEED;
			}
		}
		else if (GetKeyboardPress(DIK_W) == true)
		{//Wキーが押された
			//上移動
			g_aCamera[nCnt].posV.x += sinf(g_aCamera[nCnt].rot.y) * CAMERA_SPEED;
			g_aCamera[nCnt].posR.x += sinf(g_aCamera[nCnt].rot.y) * CAMERA_SPEED;
			g_aCamera[nCnt].posV.z += cosf(g_aCamera[nCnt].rot.y) * CAMERA_SPEED;
			g_aCamera[nCnt].posR.z += cosf(g_aCamera[nCnt].rot.y) * CAMERA_SPEED;
		}
		else if (GetKeyboardPress(DIK_S) == true)
		{//Sキーが押された
			//下移動
			g_aCamera[nCnt].posV.x += sinf(g_aCamera[nCnt].rot.y + D3DX_PI) * CAMERA_SPEED;
			g_aCamera[nCnt].posR.x += sinf(g_aCamera[nCnt].rot.y + D3DX_PI) * CAMERA_SPEED;
			g_aCamera[nCnt].posV.z += cosf(g_aCamera[nCnt].rot.y + D3DX_PI) * CAMERA_SPEED;
			g_aCamera[nCnt].posR.z += cosf(g_aCamera[nCnt].rot.y + D3DX_PI) * CAMERA_SPEED;
		}

		if (GetKeyboardPress(DIK_LSHIFT) == true)
		{
			g_aCamera[nCnt].posR.y += CAMERA_SPEED;
		}
		if (GetKeyboardPress(DIK_LCONTROL) == true)
		{
			g_aCamera[nCnt].posR.y -= CAMERA_SPEED;
		}
	}
	
}

//--------------------------------------------------------------------
// カメラ視点の移動処理
//--------------------------------------------------------------------
void CameraV(int nCnt)
{
	//縦回転
	if (GetKeyboardPress(DIK_Y) == true)
	{//Yキーが押された
		g_aCamera[nCnt].rot.z -= CAMERA_ROTATE;
	}
	else if (GetKeyboardPress(DIK_N) == true)
	{//Nキーが押された
		g_aCamera[nCnt].rot.z += CAMERA_ROTATE;
	}

	//横回転
	if (GetKeyboardPress(DIK_Z) == true)
	{//Zキーが押された
		g_aCamera[nCnt].rot.y += CAMERA_ROTATE;
	}
	else if (GetKeyboardPress(DIK_C) == true)
	{//Cキーが押された
		g_aCamera[nCnt].rot.y -= CAMERA_ROTATE;
	}

	g_aCamera[nCnt].posV.x = g_aCamera[nCnt].posR.x + (cosf(g_aCamera[nCnt].rot.z) * sinf(g_aCamera[nCnt].rot.y)) * -g_aCamera[nCnt].fLengthCamera;
	g_aCamera[nCnt].posV.y = g_aCamera[nCnt].posR.y + sinf(g_aCamera[nCnt].rot.z) * -g_aCamera[nCnt].fLengthCamera;			//視点Y
	g_aCamera[nCnt].posV.z = g_aCamera[nCnt].posR.z + (cosf(g_aCamera[nCnt].rot.z) * cosf(g_aCamera[nCnt].rot.y)) * -g_aCamera[nCnt].fLengthCamera;
}

//--------------------------------------------------------------------
// カメラ注視点の移動処理
//--------------------------------------------------------------------
void CameraR(int nCnt)
{
	//縦回転
	if (GetKeyboardPress(DIK_T) == true)
	{//Tキーが押された
		g_aCamera[nCnt].rot.z += CAMERA_ROTATE;
	}
	else if (GetKeyboardPress(DIK_B) == true)
	{//Bキーが押された
		g_aCamera[nCnt].rot.z -= CAMERA_ROTATE;
	}

	//横回転
	if (GetKeyboardPress(DIK_Q) == true || GetJoyPadPress(BUTTON_LB, 0) == true)
	{//Qキーが押された
		g_aCamera[nCnt].rot.y -= CAMERA_ROTATE;
	}
	else if (GetKeyboardPress(DIK_E) == true || GetJoyPadPress(BUTTON_RB, 0) == true)
	{//Eキーが押された
		g_aCamera[nCnt].rot.y += CAMERA_ROTATE;
	}

	g_aCamera[nCnt].posR.x = g_aCamera[nCnt].posV.x + (cosf(g_aCamera[nCnt].rot.z) * sinf(g_aCamera[nCnt].rot.y)) * g_aCamera[nCnt].fLengthCamera;
	g_aCamera[nCnt].posR.y = g_aCamera[nCnt].posV.y + sinf(g_aCamera[nCnt].rot.z) * g_aCamera[nCnt].fLengthCamera;			//視点Y
	g_aCamera[nCnt].posR.z = g_aCamera[nCnt].posV.z + (cosf(g_aCamera[nCnt].rot.z) * cosf(g_aCamera[nCnt].rot.y)) * g_aCamera[nCnt].fLengthCamera;
}
//====================================================================
//カメラのロール処理
//====================================================================
void CameraRoll(int nCnt)
{
	if (GetKeyboardPress(DIK_RIGHT) == true)
	{
		g_aCamera[nCnt].fRoll += 0.1f;
	}
	else if (GetKeyboardPress(DIK_LEFT) == true)
	{
		g_aCamera[nCnt].fRoll -= 0.1f;
	}

	if (g_aCamera[nCnt].fRoll > D3DX_PI)
	{
		g_aCamera[nCnt].fRoll = -D3DX_PI;
	}
	if (g_aCamera[nCnt].fRoll < -D3DX_PI)
	{
		g_aCamera[nCnt].fRoll = D3DX_PI;
	}
}

//====================================================================
//カメラの設定処理処理
//====================================================================
void SetCamera(int nIdx)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&g_aCamera[nIdx].mtxProjection);

	//プロジェクションマトリックスを作成
	D3DXMatrixPerspectiveFovLH(&g_aCamera[nIdx].mtxProjection,
		D3DXToRadian(45.0f),										//視野角
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,					//画面のアスペクト比
		10.0f,														//z値の最小値
		10000.0f);													//z値の最大値

														//プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &g_aCamera[nIdx].mtxProjection);

	//ビューマトリックスの初期化
	D3DXMatrixIdentity(&g_aCamera[nIdx].mtxView);

	//ビューマトリックスの作成
	D3DXMatrixLookAtLH(&g_aCamera[nIdx].mtxView,
		&g_aCamera[nIdx].posV,						//視点
		&g_aCamera[nIdx].posR,						//注視点
		&g_aCamera[nIdx].vecU);						//情報ベクトル

	//カメラのロール用
	D3DXMATRIX rotationZMatrix;
	D3DXMatrixRotationZ(&rotationZMatrix, g_aCamera[nIdx].fRoll); // Z軸回転行列の計算
	D3DXMATRIX viewMatrixWithRotation = rotationZMatrix * g_aCamera[nIdx].mtxView; // Z軸回転を加えたビュー行列

	//ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &viewMatrixWithRotation);

	//使っている状態へ
	g_aCamera[nIdx].bUse = true;

#ifdef _DEBUG		//デバッグ時のみ
	PrintDebugProc("【カメラ情報】\n");
	PrintDebugProc("視点     [%f  %f  %f]\n", g_aCamera[nIdx].posV.x, g_aCamera[nIdx].posV.y, g_aCamera[nIdx].posV.z);
	PrintDebugProc("注視点   [%f  %f  %f]\n", g_aCamera[nIdx].posR.x, g_aCamera[nIdx].posR.y, g_aCamera[nIdx].posR.z);
	PrintDebugProc("向き     [%f  %f  %f]\n", g_aCamera[nIdx].rot.x, g_aCamera[nIdx].rot.y, g_aCamera[nIdx].rot.z);
	PrintDebugProc("ロール   [%f]\n", g_aCamera[nIdx].fRoll);
#endif
}

//====================================================================
//カメラの情報のポインタ
//====================================================================
Camera *GetCamera(void)
{
	return &g_aCamera[0];
}