#include "PlayerSound.h"
#include "Audio/Audio.h"


// 初期化処理
void PlayerSound::Initialize()
{
	// オーディオのインスタンス
	audio_ = Audio::GetInstance();


	// 射撃時のSE
	soundMap_["shooting"] = Audio::LoadSound("SE/Player/shooting.wav");
	soundMap_["dash"] = Audio::LoadSound("SE/Player/dash.wav");
	soundMap_["damage"] = Audio::LoadSound("SE/Player/collisionAtt.wav");

}


// 再生
void PlayerSound::Play(const std::string key, float vol, bool loop)
{
	audio_->PlayOnSound(soundMap_[key], loop, vol);
}


// 停止
void PlayerSound::Stop(const std::string key)
{
	audio_->StopOnSound(soundMap_[key]);
}

