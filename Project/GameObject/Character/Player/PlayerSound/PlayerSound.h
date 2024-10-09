#pragma once

#include "GameObject.h"

#include <map>
#include <memory>


// Audioクラスの前方宣言
class Audio;

/* PlayerSoundクラス */
class PlayerSound {

public:

	// コンストラクタ、デストラクタ
	PlayerSound() {};
	~PlayerSound() {};

	// 初期化処理
	void Initialize();

	// 再生
	void Play(const std::string key, float vol = 1.0f, bool loop = false);

	// 停止
	void Stop(const std::string key);


private:

	// Audioクラス
	Audio* audio_ = nullptr;

	// マップで管理
	std::map<string, uint32_t> soundMap_;
};

