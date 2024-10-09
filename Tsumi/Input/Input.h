#pragma once

#define DIRECTINPUT_VERSION 0x0800 // DirectInputnoバージョン指定
#include <dinput.h>
#include <Xinput.h>
#define XINPUT_GAMEPAD_MAX 16

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib,"xinput.lib")
#pragma comment(lib, "dxguid.lib")

#include "WinApp.h"
#include "Struct.h"

#include <cassert>
#include <wrl.h>
#include <array>


/// <summary>
/// ゲームパッドのボタンデータ
/// </summary>
enum class PadData {
	UP = 0x0001,	
	DOWN = 0x0002,	
	LEFT = 0x0004,	
	RIGHT = 0x0008,
	START = 0x0010,			
	BACK = 0x0020,			
	LEFT_THUMB = 0x0040,	
	RIGHT_THUMB = 0x0080,	
	LEFT_SHOULDER = 0x0100,	
	RIGHT_SHOULDER = 0x0200,
	A = 0x1000,
	B = 0x2000,
	X = 0x4000,
	Y = 0x8000,
};



// -------------------------------------------------------------------------
// 入力
// -------------------------------------------------------------------------
// KeyInput : キーボード
// -------------------------------------------------------------------------
class KeysInput {

public: // メンバ関数

	/// <summary>
	/// インスタンスの取得
	/// </summary>
	static KeysInput* GetInstance();

	/// <summary>
	/// 初期化処理
	/// </summary>
	static void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	static void BeginFrame();

	/// <summary>
	/// 押されていない
	/// </summary>
	static bool NoneKey(uint32_t keyNum);

	/// <summary>
	/// 押した瞬間
	/// </summary>
	static bool TriggerKey(uint32_t keyNum);

	/// <summary>
	/// 押しっぱなし
	/// </summary>
	static bool PressKeys(uint32_t keyNum);

	/// <summary>
	/// 離された瞬間
	/// </summary>
	static bool ReleaseKeys(uint32_t keyNum);


private: // メンバ変数

	ComPtr<IDirectInput8> directInput_ = nullptr;
	ComPtr<IDirectInputDevice8> keyboard_ = nullptr;

	BYTE Keys[256] = {};
	BYTE preKeys[256] = {};
};





// -------------------------------------------------------------------------
// 入力
// -------------------------------------------------------------------------
// GamePadInput : ゲームパッド
// -------------------------------------------------------------------------
class GamePadInput {

public: // メンバ関数

	/// <summary>
	/// インスタンスの取得
	/// </summary>
	static GamePadInput* GetInstance();

	/// <summary>
	/// 初期化処理
	/// </summary>
	static void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	static void BeginFrame();

	/// <summary>
	/// パッドの更新
	/// </summary>
	static bool GetJoyState();

	/// <summary>
	/// 各ボタンのトリガー状態の初期化処理
	/// </summary>
	void ResetButtonTriggers();

	/// <summary>
	/// ジョイコンの入力の取得
	/// </summary>
	static bool GetJoyStickState();

	/// <summary>
	/// 押されていない
	/// </summary>
	static bool NoneButton(PadData button);

	/// <summary>
	/// 押した瞬間
	/// </summary>
	static bool TriggerButton(PadData button);

	/// <summary>
	/// 押しっぱなし
	/// </summary>
	static bool PressButton(PadData button);

	/// <summary>
	/// 離された瞬間
	/// </summary>
	static bool ReleaseButton(PadData button);

	/// <summary>
	/// Lスティック
	/// </summary>
	static Vector2 GetLStick(const float& mode = SHRT_MAX);

	/// <summary>
	/// Rスティック
	/// </summary>
	static Vector2 GetRStick(const float& mode = SHRT_MAX);



private: // メンバ変数

	XINPUT_STATE joyState_{};
	XINPUT_STATE preJoyState_{};

	WORD buttonTriggers[XINPUT_GAMEPAD_MAX];
};