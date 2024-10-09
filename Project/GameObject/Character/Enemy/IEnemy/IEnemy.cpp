#include "IEnemy.h"
#include <GameManager.h>

void IEnemy::SpawnMove() {
	if (worldTransform_.translate.y <= 0) {
		worldTransform_.translate.y += 0.1f;
		//worldTransform_.rotate.y += 0.1f;
		worldTransform_.UpdateMatrix();
	}
	else {
		isSpawn = true;
	}
}

void IEnemy::TitleMove()
{
	
	flame_++;

	Scope scope = { 420,720 };

	int jumpFlame = (int)RandomGenerator::getRandom(scope);

	if (flame_ % 120 == 0 && !isAnime_) {
		isAnime_ = true;
	}

	/*if (animationTime_ == 1.0f) {
		isAnime_ = false;
	}*/

	if (isAnime_) {
		// ジャンプの高さ
		float jumpHeight = 1.5f;

		// ジャンプの時間
		float jumpDuration = 1.0f;

		// 現在の時間
		float currentTime = 1.0f / 60.0f;

		animationTime_ += currentTime;

		if (animationTime_ == currentTime) {
			originalPos_ = worldTransform_.translate;
			originalScale_ = worldTransform_.scale;
		}

		worldTransform_.translate = Jump(originalPos_, jumpHeight, jumpDuration, animationTime_);
		worldTransform_.scale = JumpScale(originalScale_, jumpHeight, jumpDuration, animationTime_);
		Shake(originalPos_, jumpHeight, jumpDuration, animationTime_);

		//worldTransform_.rotate.y += 0.1f;
	}
	/*else {
		animationTime_ = 0;
		worldTransform_.scale = { (float)1,(float)1,(float)1 };
	}*/

	if (animationTime_ >= 1.0f) {
		if (worldTransform_.translate.y >= -50.0f) {
			worldTransform_.translate.y -= 0.1f;
			//worldTransform_.rotate.y += 0.1f;
			worldTransform_.UpdateMatrix();
			isAnime_ = false;
		}
	}

	if (worldTransform_.translate.y <= -50.0f) {
		isAlive_ = false;
	}

	worldTransform_.UpdateMatrix();
}

void IEnemy::SelectMove()
{
	flame_++;

	Scope scope = { 420,720 };

	int jumpFlame = (int)RandomGenerator::getRandom(scope);

	if (flame_ % 120 == 0 && !isAnime_) {
		isAnime_ = true;
		animationTime_ = 0.0f;  // リセット
	}

	if (isAnime_) {
		// ジャンプの高さ
		float jumpHeight = 1.5f;

		// ジャンプの時間
		float jumpDuration = 1.0f;

		// 現在の時間
		float currentTime = 1.0f / 60.0f;

		animationTime_ += currentTime;

		if (animationTime_ == currentTime) {
			originalPos_ = worldTransform_.translate;
			originalScale_ = worldTransform_.scale;
		}

		// ジャンプの移動とスケールを更新
		worldTransform_.translate = Jump(originalPos_, jumpHeight, jumpDuration, animationTime_);
		worldTransform_.scale = JumpScale(originalScale_, jumpHeight, jumpDuration, animationTime_);
		

		// 回転も更新
		worldTransform_.rotate.y += 0.1f;
	}

	// ジャンプループが終わったとき
	if (animationTime_ >= 1.0f) {
		worldTransform_.scale = originalScale_;  // スケールを元に戻す

		// 任意の処理、例えば位置を少し下げるなど
		if (worldTransform_.translate.y >= -50.0f) {
			worldTransform_.translate.y -= 0.1f;
			worldTransform_.rotate.y += 0.1f;
			worldTransform_.UpdateMatrix();
			isAnime_ = false;
		}
	}

	// 行列を更新
	worldTransform_.UpdateMatrix();
}

void IEnemy::FeedAnimetion()
{
	worldTransform_.rotate.y += 0.05f;
	if (worldTransform_.scale.x >= 0.0f) {
		worldTransform_.scale.x -= 0.01f;
		worldTransform_.scale.y -= 0.01f;
		worldTransform_.scale.z -= 0.01f;
	}
	else {
		isFeedAnimetionEnd_ = true;
	}
}

Vector3 IEnemy::FloatingAnimation(float time, float amplitude, float frequency) {
	Vector3 initialPos{};

	float offsetY = amplitude * sinf(frequency * time);

	// Y座標を上下動させる
	return { 0, initialPos.y + offsetY, 0 };
}


Vector3 IEnemy::Jump(const Vector3& initialPosition, float jumpHeight, float jumpDuration, float currentTime) {
	// ジャンプの高さが0以下なら初期位置を返す
	if (jumpHeight <= 0) {
		return initialPosition;
	}

	// 現在のジャンプ時間を[0, 1]の範囲に正規化する
	float normalizedTime = (float)min(currentTime / jumpDuration, 1.0f);

	Scope scope = { -0.1f,0.1f };
	ScopeVec3 vec3 = { scope,scope,scope };



	// ジャンプの進行度をイージング関数で計算する
	float jumpProgress;
	if (normalizedTime < 0.5f) {
		jumpProgress = jumpHeight * (float)Ease::OutQuint(normalizedTime * 2.0f);
		//worldTransform_.translate += RandomGenerator::getRandom(vec3);
	}
	else {
		jumpProgress = jumpHeight * (1.0f - (float)Ease::OutQuint((normalizedTime - 0.5f) * 2.0f));
	}

	// 初期位置からの垂直方向の距離を計算し、それを元の位置に加える
	Vector3 finalPosition = initialPosition;
	finalPosition.y += jumpProgress;

	// ジャンプが終了したら元の位置に戻る
	if (currentTime >= jumpDuration) {
		finalPosition.y = initialPosition.y;
	}

	return finalPosition;
}

void IEnemy::Shake(const Vector3& initialPosition, float jumpHeight, float jumpDuration, float currentTime) {

	// 現在のジャンプ時間を[0, 1]の範囲に正規化する
	float normalizedTime = (float)min(currentTime / jumpDuration, 1.0f);

	Scope scope = { -0.2f,0.2f };
	ScopeVec3 vec3 = { scope,scope,scope };

	// ジャンプの進行度をイージング関数で計算する
	float jumpProgress{};
	if (normalizedTime < 0.5f && flame_ % 5 == 0) {
		worldTransform_.translate += RandomGenerator::getRandom(vec3);
		//worldTransform_.rotate += RandomGenerator::getRandom(vec3);
	}
}

//Vector3 IEnemy::JumpScale(const Vector3& initialScale, float jumpHeight, float jumpDuration, float currentTime) {
//	// ジャンプの高さが0以下なら初期位置を返す
//	if (jumpHeight <= 0) {
//		return initialScale;
//	}
//
//	// ジャンプ後のスケールを計算
//	Vector3 finalScale = initialScale;
//	if (currentTime < jumpDuration) {
//		finalScale += calculateJumpScale(currentTime * 1.1f, jumpDuration, initialScale);
//	}
//	else {
//		finalScale += calculateLandingScale(currentTime, jumpDuration, initialScale);
//	}
//
//	// ジャンプが終了したら元の位置に戻る
//	if (currentTime >= jumpDuration) {
//		finalScale = initialScale;
//	}
//
//	return finalScale;
//}

// ジャンプのスケールを計算する関数
Vector3 IEnemy::JumpScale(const Vector3& initialScale, float jumpHeight, float jumpDuration, float currentTime) {
	Vector3 finalScale = initialScale;

	// ジャンプの最高到達点を計算
	float peakTime = jumpDuration * 0.5f;
	float peakHeight = jumpHeight;

	if (currentTime < peakTime) {
		// 上昇中のスケールを計算
		finalScale = calculateJumpScale(initialScale, currentTime, peakTime, jumpHeight);
	}
	else if (currentTime < jumpDuration) {
		// 下降中のスケールを計算（着地直前に横方向にスケールを伸ばす）
		finalScale = calculateLandingScale(currentTime - peakTime, jumpDuration - peakTime, initialScale, peakHeight);
	}

	// ジャンプ終了後、徐々に元のスケールに戻す
	if (currentTime >= jumpDuration) {
		float normalizedTime = (currentTime - jumpDuration) / (jumpDuration * 0.5f);
		normalizedTime = min(normalizedTime, 1.0f);

		finalScale = Lerp(finalScale, initialScale, Ease::OutQuart(normalizedTime));
	}

	return finalScale;
}

// 上昇中のスケールを計算する関数
Vector3 IEnemy::calculateJumpScale(const Vector3& initialScale, float currentTime, float peakTime, float jumpHeight) {
	float normalizedTime = min(currentTime / peakTime, 1.0f);

	// 縦方向のスケールを計算
	float scaleMultiplier = (1.0f - normalizedTime) * (jumpHeight / 2.0f);

	// 横方向のスケールは変更なし
	return { initialScale.x, initialScale.y + scaleMultiplier, initialScale.z };
}

// 下降中のスケールを計算する関数
Vector3 IEnemy::calculateLandingScale(float currentTime, float landingDuration, const Vector3& initialScale, float peakHeight) {
	// 遅延時間を設定
	float delayBeforeStretch = landingDuration * 0.25f; // 下降の25%の位置でスケールを変更開始
	float adjustedTime = currentTime - delayBeforeStretch;

	// 遅延が適用されるときのみスケールを変更
	float normalizedTime = max(adjustedTime / (landingDuration - delayBeforeStretch), 0.0f);

	// イージング関数を使ってスケールの変化をスムーズに
	float scaleMultiplier = Ease::OutElastic(normalizedTime) * (peakHeight / 2.0f);

	// 横方向のスケールを引き伸ばした後に元に戻るプロセスを追加
	float returnTime = normalizedTime - 1.0f;
	float returnMultiplier = (returnTime >= 0.0f) ? Ease::InElastic(returnTime) * (peakHeight / 2.0f) : 0.0f;

	// スケールの変更
	return {
		initialScale.x + scaleMultiplier - returnMultiplier,
		initialScale.y - scaleMultiplier,
		initialScale.z + scaleMultiplier - returnMultiplier
	};
}




float IEnemy::CalculateAngle(const Vector3& current, const Vector3& target) {
	// 目標位置と現在位置の差を計算
	float deltaX = target.x - current.x;
	float deltaZ = target.z - current.z;

	// atan2関数を使用して角度を計算
	return std::atan2(deltaX, deltaZ);
}