#pragma once
#include <math.h>
#include <iostream>

/// <summary>
/// 2次元ベクトル
/// </summary>
struct Vector2 final {

	// 静的定数メンバ変数
	static const Vector2 zero;
	static const Vector2 one;
	static const Vector2 oneX;
	static const Vector2 oneY;

	// メンバ変数
	float x, y;

	// デフォルトコンストラクタ
	Vector2() : x(0.0f), y(0.0f) {};
	// パラメータ付きコンストラクタ
	Vector2(float x, float y) : x(x), y(y) {};

	// == 演算子のオーバーロード　
	inline constexpr bool operator==(const Vector2& rhs) noexcept {
		return x == rhs.x && y == rhs.y;
	}

	// != 演算子のオーバーロード 
	inline constexpr bool operator!=(const Vector2& rhs) noexcept { 
		return !(*this == rhs); 
	}
};



/// <summary>
/// 3次元ベクトル
/// </summary>
struct Vector3 final {

	// 静的定数メンバ変数
	static const Vector3 zero;
	static const Vector3 one;
	static const Vector3 oneX;
	static const Vector3 oneY;
	static const Vector3 oneZ;

	// メンバ変数
	float x, y, z;

	// デフォルトコンストラクタ
	Vector3() : x(0.0f), y(0.0f), z(0.0f) {};
	// パラメータ付きコンストラクタ
	Vector3(float x, float y, float z) : x(x), y(y), z(z){};

	// == 演算子のオーバーロード
	inline constexpr bool operator==(const Vector3& rhs) noexcept {
		return x == rhs.x && y == rhs.y && z == rhs.z;
	}

	// != 演算子のオーバーロード 
	inline constexpr bool operator!=(const Vector3& rhs) noexcept {
		return !(*this == rhs);
	}

	// 外積を計算するメソッド
	Vector3 Cross(const Vector3& other) const {
		return Vector3(
			y * other.z - z * other.y,
			z * other.x - x * other.z,
			x * other.y - y * other.x
		);
	}

	// ノーマライズを行うメソッド
	Vector3 Normalize() const {
		float length = sqrt(x * x + y * y + z * z);
		if (length != 0) {
			return Vector3(x / length, y / length, z / length);
		}
		return Vector3(0, 0, 0); // ゼロベクトルの場合はそのまま返す
	}

	// ドット積の計算
	float Dot(const Vector3& other) const {
		return x * other.x + y * other.y + z * other.z;
	}

	// ストリーム演算子のオーバーロード
	friend std::ostream& operator<<(std::ostream& os, const Vector3& vec) {
		os << "Vector3(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
		return os;
	}
	// LengthSquaredの実装
	float LengthSquared() const {
		return (x * x) + (y * y) + (z * z);
	}
};



/// <summary>
/// 4次元ベクトル
/// </summary>
struct Vector4 final {

	// 静的定数メンバ変数
	static const Vector4 zero;
	static const Vector4 one;
	static const Vector4 oneX;
	static const Vector4 oneY;
	static const Vector4 oneZ;
	static const Vector4 oneW;

	// メンバ変数
	float x, y, z, w;

	// デフォルトコンストラクタ
	Vector4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {};
	// パラメータ付きコンストラクタ
	Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {};

	// == 演算子のオーバーロード　
	inline constexpr bool operator==(const Vector4& rhs) noexcept {
		return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w;
	}

	// != 演算子のオーバーロード 
	inline constexpr bool operator!=(const Vector4& rhs) noexcept {
		return !(*this == rhs);
	}
};
