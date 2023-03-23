//======================================================================================
//
//キーボード入力処理[input.cpp]
//Author;小笠原　彪
//
//======================================================================================
#include "input.h"

//*****************************
//マクロ定義
//*****************************
#define NUM_KEY_MAX			(256)			//キーの最大数

#define NUM_PLAYER			(4)				//プレイヤー数

//*****************************
//グローバル定義
//*****************************
LPDIRECTINPUT8 g_pInput = NULL;						//Directinputオブジェクトへのポインタ
LPDIRECTINPUTDEVICE8 g_pDevKeyboard = NULL;			//入力デバイス(キーボード)へのポインタ

BYTE g_aKeyState[NUM_KEY_MAX];						//キーボードプレスの情報
BYTE g_aKeyStateTrigger[NUM_KEY_MAX];				//キーボードトリガーの情報
BYTE g_aKeyStateRipeat[NUM_KEY_MAX];				//キーボードリピートの情報
BYTE g_aKeyStateRelease[NUM_KEY_MAX];				//キーボードリリースの情報

//ゲームパッド
XINPUT_STATE g_aGamePadState[NUM_PLAYER];						//ゲームパッドプレスの情報
XINPUT_STATE g_aGamePadStateTrigger[NUM_PLAYER];				//ゲームパッドトリガーの情報
XINPUT_STATE g_aGamePadStateRelease[NUM_PLAYER];				//ゲームパッドリリースの情報
XINPUT_STATE g_aGamePadStateRepeat[NUM_PLAYER];					//ゲームパッド


//========================================================================
//デバイスの終了処理
//========================================================================
void UninitDevice(void)
{
	//キーボードの初期化処理
	UninitKeyboard();

	//ジョイパッドの終了処理
	UninitJoyPad();
}

//========================================================================
//デバイスの更新処理
//========================================================================
void UpdateDevice(void)
{
	//キーボードの更新処理
	UpdateKeyboard();

	//ジョイパッドの更新処理
	UpdateJoyPad();
}

//========================================================================
//キーボードの初期化
//========================================================================
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	//DirectInputオブジェクトの初期化
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pInput, NULL)))
	{
		return E_FAIL;
	}
	//入力デバイス(キーボード)の生成
	if (FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL)))
	{
		return E_FAIL;
	}
	//データフォーマットを設定
	if (FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}
	//強調モードを設定
	if (FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}
	//キーボードへのアクセス権を獲得
	g_pDevKeyboard->Acquire();

	return S_OK;
}

//========================================================================
//キーボード終了処理
//========================================================================
void UninitKeyboard(void)
{
	//入力デバイス(キーボード)の破棄
	if (g_pDevKeyboard != NULL)
	{
		g_pDevKeyboard->Unacquire();			//キーボードへのアクセス権を放棄
		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}

	//Directinputオブジェクトの破棄
	if (g_pInput != NULL)
	{
		g_pInput->Release();
		g_pInput = NULL;
	}
}

//========================================================================
//キーボード更新処理
//========================================================================
void UpdateKeyboard(void)
{
	BYTE aKeyState[NUM_KEY_MAX];			//キーボードの入力情報
	int nCntKey;

	//入力デバイスからデータを取得
	if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			g_aKeyStateRipeat[nCntKey] = (g_aKeyState[nCntKey] & aKeyState[nCntKey]);		//キーボードのリピート情報を保存

			g_aKeyStateRelease[nCntKey] = (g_aKeyState[nCntKey] | aKeyState[nCntKey]) ^ aKeyState[nCntKey];		//キーボードのリリース情報を保存

			g_aKeyStateTrigger[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];		//キーボードのトリガー情報を保存

			g_aKeyState[nCntKey] = aKeyState[nCntKey];			//キーボードのプレス情報を保存
		}
	}
	else
	{
		g_pDevKeyboard->Acquire();				//キーボードへのアクセス権を獲得
	}
}

//========================================================================
//キーボードのプレス情報を取得
//========================================================================
bool GetKeyboardPress(int nKey)
{
	return (g_aKeyState[nKey] & 0x80) ? true : false;
}

//========================================================================
//キーボードのトリガー情報を取得
//========================================================================
bool GetKeyboardTrigger(int nKey)
{
	return (g_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//========================================================================
//キーボードのリピート情報を取得
//========================================================================
bool GetKeyboardRipeat(int nKey)
{
	return (g_aKeyStateRipeat[nKey] & 0x80) ? true : false;
}

//========================================================================
//キーボードのリリース情報を取得
//========================================================================
bool GetKeyboardRelease(int nKey)
{
	return (g_aKeyStateRelease[nKey] & 0x80) ? true : false;
}

//========================================================================
//ゲームパッド初期化処理
//========================================================================
HRESULT InitGamePad(void)
{
	//変数宣言
	int nCntPad;

	//XInputのステート(使う状態)
	XInputEnable(true);

	//メモリの初期化(プレイヤー分)
	for (nCntPad = 0; nCntPad < NUM_PLAYER; nCntPad++)
	{
		memset(&g_aGamePadState[nCntPad], 0, sizeof(XINPUT_STATE));
		memset(&g_aGamePadStateTrigger[nCntPad], 0, sizeof(XINPUT_STATE));
		memset(&g_aGamePadStateRelease[nCntPad], 0, sizeof(XINPUT_STATE));
	}

	//ないとできない
	return S_OK;
}

//========================================================================
//ゲームパッドの終了処理
//========================================================================
void UninitJoyPad(void)
{
	//XInputのステート(使わない状態)
	XInputEnable(false);
}

//========================================================================
//ゲームパッドの更新処理
//========================================================================
void UpdateJoyPad(void)
{
	//変数宣言
	int nCntPad;

	XINPUT_STATE aGamePadState;			//ゲームパッドの入力情報

	for (nCntPad = 0; nCntPad < NUM_PLAYER; nCntPad++)
	{
		//入力デバイスからデータを取得
		if (XInputGetState(nCntPad, &aGamePadState) == ERROR_SUCCESS)
		{
			//ゲームパッドのトリガー情報を保存
			g_aGamePadStateTrigger[nCntPad].Gamepad.wButtons = 
				(g_aGamePadState[nCntPad].Gamepad.wButtons ^ aGamePadState.Gamepad.wButtons) & aGamePadState.Gamepad.wButtons;

			//リピート情報
			g_aGamePadStateRepeat[nCntPad].Gamepad.wButtons =
				(g_aGamePadState[nCntPad].Gamepad.wButtons & aGamePadState.Gamepad.wButtons);

			//リリース情報
			g_aGamePadStateRelease[nCntPad].Gamepad.wButtons =
				(g_aGamePadState[nCntPad].Gamepad.wButtons | aGamePadState.Gamepad.wButtons) ^ aGamePadState.Gamepad.wButtons;

			//ゲームパッドのプレス情報を保存
			g_aGamePadState[nCntPad] = aGamePadState;
		}
	}
}

//===================================================================
//ジョイスティック情報取得
//===================================================================
float GetJoyStickLX(int nPlayer)
{
	//情報を返す
	return (float)g_aGamePadState[nPlayer].Gamepad.sThumbLX / SHRT_MAX;
}

//===================================================================
//ジョイスティック情報取得
//===================================================================
float GetJoyStickLY(int nPlayer)
{
	//情報を返す
	return (float)g_aGamePadState[nPlayer].Gamepad.sThumbLY / SHRT_MAX;
}

//===================================================================
//ジョイスティック情報取得
//===================================================================
float GetJoyStickRX(int nPlayer)
{
	//情報を返す
	return (float)g_aGamePadState[nPlayer].Gamepad.sThumbRX / SHRT_MAX;
}

//===================================================================
//ジョイスティック情報取得
//===================================================================
float GetJoyStickRY(int nPlayer)
{
	//情報を返す
	return (float)g_aGamePadState[nPlayer].Gamepad.sThumbRY / SHRT_MAX;
}

//========================================================================
//ゲームパッドのプレス情報を取得
//========================================================================
bool GetJoyPadPress(JOYKEY nKey, int nPlayer)
{
	return (g_aGamePadState[nPlayer].Gamepad.wButtons & 0x01 << nKey) ? true : false;
}

//========================================================================
//ゲームパッドのトリガー情報を取得
//========================================================================
bool GetJoyPadTrigger(JOYKEY nKey, int nPlayer)
{
	return (g_aGamePadStateTrigger[nPlayer].Gamepad.wButtons & 0x01 << nKey) ? true : false;
}

//========================================================================
//ゲームパッドのリリース情報を取得
//========================================================================
bool GetGamePadRelease(JOYKEY nKey, int nPlayer)
{
	return (g_aGamePadStateRelease[nPlayer].Gamepad.wButtons & 0x01 << nKey) ? true : false;
}

//===================================================================
//リピート情報取得
//===================================================================
bool GetJoyPadRepeat(JOYKEY nKey, int nPlayer)
{
	//三項演算子
	return(g_aGamePadStateRepeat[nPlayer].Gamepad.wButtons & (0x01 << nKey)) ? true : false;
}
