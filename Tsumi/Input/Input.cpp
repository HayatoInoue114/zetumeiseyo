#include "Input.h"



// -------------------------------------------------------------------------
// KeyInput : キーボード
// -------------------------------------------------------------------------

// インスタンスの取得
KeysInput* KeysInput::GetInstance() {
	static KeysInput instance;
	return &instance;
}

// 初期化処理
void KeysInput::Initialize() {

	// DirectInputのインスタンス生成
	HRESULT result = DirectInput8Create(
		WinApp::GetWc().hInstance, DIRECTINPUT_VERSION, 
		IID_IDirectInput8, (void**)&KeysInput::GetInstance()->directInput_, nullptr);
	assert(SUCCEEDED(result));

	// キーボードデバイス生成
	result = KeysInput::GetInstance()->directInput_->CreateDevice(GUID_SysKeyboard, &KeysInput::GetInstance()->keyboard_, NULL);
	assert(SUCCEEDED(result));

	// 入力データ形式のセット
	result = KeysInput::GetInstance()->keyboard_->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(result));

	// 排他制御レベルのセット
	result = KeysInput::GetInstance()->keyboard_->SetCooperativeLevel(
		WinApp::GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
}

// 更新処理
void KeysInput::BeginFrame() {

	// メモリコピー
	memcpy(KeysInput::GetInstance()->preKeys, KeysInput::GetInstance()->Keys, 256);

	// キーボード情報の取得開始
	KeysInput::GetInstance()->keyboard_->Acquire();

	// 全キーの入力状態を取得する
	KeysInput::GetInstance()->keyboard_->GetDeviceState(sizeof(KeysInput::GetInstance()->Keys), KeysInput::GetInstance()->Keys);
}

// 押されていない
bool KeysInput::NoneKey(uint32_t keyNum) {

	if (KeysInput::GetInstance()->preKeys[keyNum] == 0x00 && KeysInput::GetInstance()->Keys[keyNum] == 0x00) {
		return true;
	}
	return false;
}

// 押した瞬間
bool KeysInput::TriggerKey(uint32_t keyNum) {

	if (KeysInput::GetInstance()->preKeys[keyNum] == 0x00 && KeysInput::GetInstance()->Keys[keyNum] == 0x80) {
		return true;
	}
	return false;
}

// 押しっぱなし
bool KeysInput::PressKeys(uint32_t keyNum) {

	if (KeysInput::GetInstance()->preKeys[keyNum] == 0x80 && KeysInput::GetInstance()->Keys[keyNum] == 0x80) {
		return true;
	}
	return false;
}

// 離された瞬間
bool KeysInput::ReleaseKeys(uint32_t keyNum) {

	if (KeysInput::GetInstance()->preKeys[keyNum] == 0x80 && KeysInput::GetInstance()->Keys[keyNum] == 0x00) {
		return true;
	}
	return false;
}

// -------------------------------------------------------------------------
// KeyInput : キーボード
// -------------------------------------------------------------------------





// -------------------------------------------------------------------------
// GamePadInput : ゲームパッド
// -------------------------------------------------------------------------

// インスタンスの取得
GamePadInput* GamePadInput::GetInstance() {
	static GamePadInput instance;
	return &instance;
}

// 初期化処理
void GamePadInput::Initialize() {

	// 各ボタンのトリガー状態の初期化処理
	GamePadInput::GetInstance()->ResetButtonTriggers();

}

// 更新処理
void GamePadInput::BeginFrame() {

	// メモリコピー
	GamePadInput::GetInstance()->preJoyState_ = GamePadInput::GetInstance()->joyState_;

	GamePadInput::GetInstance()->GetJoyState();

	// ジョイスティックの状態をポーリング
	for (DWORD i = 0; i < XUSER_MAX_COUNT; ++i) {
		XINPUT_STATE state;
		ZeroMemory(&state, sizeof(XINPUT_STATE));
	}
}

// パッドの状態更新
bool GamePadInput::GetJoyState()
{
	DWORD dwResult = XInputGetState(0, &GamePadInput::GetInstance()->joyState_);
	if (dwResult == ERROR_SUCCESS) {
		return true;
	}
	return false;
}

// 各ボタンのトリガー状態の初期化処理
void GamePadInput::ResetButtonTriggers()
{
	ZeroMemory(buttonTriggers, sizeof(buttonTriggers));
}

// ジョイコンの入力の取得
bool GamePadInput::GetJoyStickState()
{
	DWORD dwresult = XInputGetState(0, &GamePadInput::GetInstance()->joyState_);

	if (dwresult == ERROR_SUCCESS) {
		return true;
	}
	return false;
}

// 押されていない
bool GamePadInput::NoneButton(PadData button)
{
	button;
	return false;
}

// 押した瞬間
bool GamePadInput::TriggerButton(PadData button)
{
	bool preFlag = false;

	if (GamePadInput::GetInstance()->preJoyState_.Gamepad.wButtons & (WORD)button)
	{
		preFlag = true;
	}

	if (!preFlag && GamePadInput::GetInstance()->joyState_.Gamepad.wButtons & (WORD)button)
	{
		return true;
	}

	return false;
}

// 押しっぱなし
bool GamePadInput::PressButton(PadData button)
{
	if (GamePadInput::GetInstance()->joyState_.Gamepad.wButtons & (WORD)button)
	{
		return true;
	}
	return false;
}

// 離された瞬間
bool GamePadInput::ReleaseButton(PadData button)
{
	bool preFlag = false;

	// 前回の状態でボタンが押されていたかを確認
	if (GamePadInput::GetInstance()->preJoyState_.Gamepad.wButtons & (WORD)button)
	{
		preFlag = true;
	}

	// 現在の状態でボタンが押されていないことを確認
	if (preFlag && !(GamePadInput::GetInstance()->joyState_.Gamepad.wButtons & (WORD)button))
	{
		return true;
	}

	return false;
}

// Lスティック
Vector2 GamePadInput::GetLStick(const float& mode)
{
	return {
		GamePadInput::GetInstance()->joyState_.Gamepad.sThumbLX / mode,
		GamePadInput::GetInstance()->joyState_.Gamepad.sThumbLY / mode };
}

// Rスティック
Vector2 GamePadInput::GetRStick(const float& mode)
{
	return {
		GamePadInput::GetInstance()->joyState_.Gamepad.sThumbRX / mode,
		GamePadInput::GetInstance()->joyState_.Gamepad.sThumbRY / mode };
}

// -------------------------------------------------------------------------
// GamePadInput : ゲームパッド
// -------------------------------------------------------------------------

