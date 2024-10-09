#include "Timer.h"



// スタート時に呼び出す処理
void Timer::Start(float setStart, float setEnd)
{
	this->nowFrame_ = setStart;
	this->endFrame_ = setEnd;

	this->isActive_ = true;
	this->isFinish_ = false;
}


// タイマーの更新処理
void Timer::Update()
{
	if (isFinish_) {
		isActive_ = false;
		return;
	}

	nowFrame_++;

	if (nowFrame_ >= endFrame_) {
		isActive_ = false;
		isFinish_ = true;
	}
}


// リセットみたいなやつ
void Timer::Clear()
{
	this->nowFrame_ = 0;
	this->endFrame_ = 0;

	this->isActive_ = false;
	this->isFinish_ = true;
}
