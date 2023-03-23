//====================================================================
//
// カメラの処理[camera.h]
// Author: 小笠原彪
//
//====================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

//***********************************************************
//マクロ定義
//***********************************************************
#define MAX_CAMERA			(1)			//カメラの最大数

//************************************************************
//カメラ構造体の定義
//************************************************************
typedef struct
{
	D3DXVECTOR3 posV;			//視点
	D3DXVECTOR3 posR;			//注視点
	D3DXVECTOR3 posVDest;		//目的の視点
	D3DXVECTOR3 posRDest;		//目的の注視点
	D3DXVECTOR3 vecU;			//上方向ベクトル
	D3DXMATRIX mtxProjection;	//プロジェクションマトリックス
	D3DXMATRIX mtxView;			//ビューマトリックス
	D3DXVECTOR3 rot;			//向き
	D3DVIEWPORT9 viewport;		//ビューポート
	bool bUse;					//使っているかどうか

	float fRoll;				 //カメラのロール角度
	float fLengthCamera;		//長さ
	bool bFollow;				//追従処理のON/OFF
} Camera;

//************************************************************
//プロトタイプ宣言
//************************************************************
void InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);

void SetCamera(int nIdx);

void FollowPlayer(void);

Camera *GetCamera(void);


#endif
