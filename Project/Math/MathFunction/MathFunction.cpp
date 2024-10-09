#include "MathFunction.h"



/// -------------------------------------------------------------------------
/// ログ
/// -------------------------------------------------------------------------
// string->wstring
std::wstring ConverString(const std::string& str) {
	if (str.empty())
	{
		return std::wstring();
	}

	auto sizeNeeded = MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(&str[0]), static_cast<int>(str.size()), NULL, 0);
	if (sizeNeeded == 0)
	{
		return std::wstring();
	}
	std::wstring result(sizeNeeded, 0);
	MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(&str[0]), static_cast<int>(str.size()), &result[0], sizeNeeded);
	return result;
}
// wstring->string
std::string ConvertString(const std::wstring& str) {
	if (str.empty())
	{
		return std::string();
	}

	auto sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, str.data(), static_cast<int>(str.size()), NULL, 0, NULL, NULL);
	if (sizeNeeded == 0)
	{
		return std::string();
	}
	std::string result(sizeNeeded, 0);
	WideCharToMultiByte(CP_UTF8, 0, str.data(), static_cast<int>(str.size()), result.data(), sizeNeeded, NULL, NULL);
	return result;
}
// wstring->string
void Log(const std::string& message) {
	OutputDebugStringA(message.c_str());
}




/// -------------------------------------------------------------------------
/// float
/// -------------------------------------------------------------------------
// ラープ
float Lerp(const float& start, const float& end, float t) {
	return start + (end - start) * t;
}

// クランプ
float Clamp(const float& value, const float& minValue, const float& maxValue) {
	return max(minValue, min(value, maxValue));
}

// 0に近づくほど1になり、1や-1になるほど0を返す関数
float APOneAsZeroCloser(float value) {
	return exp(-pow(value, 2.0f));
}




/// -------------------------------------------------------------------------
/// 2次元ベクトル
/// -------------------------------------------------------------------------
// 内積
float Dot(const Vector2& v1, const Vector2& v2) {
	return (v1.x * v2.x) + (v1.y * v2.y);
}

// 外積
float Cross(const Vector2& v1, const Vector2& v2) {
	return (v1.x * v2.y) - (v1.y * v2.x);
}

// 長さ
float Length(const Vector2& v) {
	return std::sqrt(Dot(v, v));
}

// 絶対値
Vector2 Absolute(const Vector2& v) {
	return { std::abs(v.x), std::abs(v.y) };
}

// 正規化
Vector2 Normalize(const Vector2& v) {
	return v * (1.0f / Length(v));
}

// 正射影ベクトル
Vector2 Project(const Vector2& v1, const Vector2& v2) {
	return Dot(v1, Normalize(v2)) * Normalize(v2);
}

// 線形補間
Vector2 Lerp(const Vector2& start, const Vector2& end, const float t) {
	return start + t * (end - start);
}

// Vector3 -> Vector2 への変換 今はそのまま対する値を送ってるだけだけど今後はもっと変換処理作っていく
Vector2 ConvertVector(const Vector3& v, const ViewProjection& view) {
	Vector3 worldPos = v;
	Matrix4x4 matViewProjectionViewPort = view.matView * view.matProjection * view.matViewPort;
	worldPos = TransformByMatrix(worldPos, matViewProjectionViewPort);
	return { worldPos.x, worldPos.y };
}

// クランプ
Vector2 Clamp(const Vector2& value, const Vector2& minValue, const Vector2& maxValue)
{
	return {
		max(minValue.x, min(value.x, maxValue.x)),
		max(minValue.y, min(value.y, maxValue.y)),
	};
}




/// -------------------------------------------------------------------------
/// 3次元ベクトル
/// -------------------------------------------------------------------------
// 内積
float Dot(const Vector3& v1, const Vector3& v2) {
	return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
}

// 外積
Vector3 Cross(const Vector3& v1, const Vector3& v2) {
	return { (v1.y * v2.z) - (v1.z * v2.y), (v1.z * v2.x) - (v1.x * v2.z), (v1.x * v2.y) - (v1.y * v2.x) };
}

// 長さ
float Length(const Vector3& v) {
	return std::sqrt(Dot(v, v));
}

// 絶対値
Vector3 Absolute(const Vector3& v) {
	return { std::abs(v.x), std::abs(v.y), std::abs(v.z) };
}

// 正規化
Vector3 Normalize(const Vector3& v) {
	return v * (1.0f / Length(v));
}

// 正射影ベクトル
Vector3 Project(const Vector3& v1, const Vector3& v2) {
	return Dot(v1, Normalize(v2)) * Normalize(v2);
}

// 線形補間
Vector3 Lerp(const Vector3& start, const Vector3& end, const float t) {
	return start + t * (end - start);
}

// 球面線形補間
Vector3 SLerp(const Vector3& start, const Vector3& end, const float t) {
	float dot = Dot(start, end);
	float theta = std::acos(dot) * t;
	Vector3 relative = { end.x - start.x * dot, end.y - start.y * dot, end.z - start.z * dot };
	relative = Normalize(relative);
	Vector3 result = {
		start.x * std::cosf(theta) + relative.x * std::sinf(theta),
		start.y * std::cosf(theta) + relative.y * std::sinf(theta),
		start.z * std::cosf(theta) + relative.z * std::sinf(theta),
	};
	return result;
}

// 最近接線
Vector3 ClosestPoint(const Vector3& p, const Segment& s) {
	float length = Length(p);
	Vector3 normalize = Normalize(p);
	float dist = Dot((p - s.origin), normalize);
	dist = std::clamp(dist, 0.0f, length);
	Vector3 proj = dist * normalize;
	return s.origin + proj;
}

// 法線ベクトル
Vector3 Perpendicular(const Vector3& v) {
	if (v.x != 0.0f || v.y != 0.0f) {
		return { -v.y, v.x, 0.0f };
	}
	return { 0.0f, -v.z, v.y };
}

// 座標変換
Vector3 TransformByMatrix(const Vector3 v, const Matrix4x4 m)
{
	Vector3 result{};

	result.x = v.x * m.m[0][0] + v.y * m.m[1][0] + v.z *
		m.m[2][0] + 1.0f * m.m[3][0];

	result.y = v.x * m.m[0][1] + v.y * m.m[1][1] + v.z *
		m.m[2][1] + 1.0f * m.m[3][1];

	result.z = v.x * m.m[0][2] + v.y * m.m[1][2] + v.z *
		m.m[2][2] + 1.0f * m.m[3][2];

	float w = v.x * m.m[0][3] + v.y * m.m[1][3] + v.z *
		m.m[2][3] + 1.0f * m.m[3][3];

	assert(w != 0.0f);

	result.x /= w;
	result.y /= w;
	result.z /= w;

	return  result;
}

// ベクトル変換
Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m) {
	Vector3 result{};

	result.x = v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0];

	result.y = v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1];

	result.z = v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2];

	return result;
}

// Vector2 -> Vector3 への変換
//Vector3 ConvertVector(const Vector2& v) {
//	return { v.x, v.y, 0.0f };
//}

// Vector2をそのままVector3に入れる
Vector3 CreateVector3FromVector2(const Vector2& v) {
	return { v.x, v.y, 1.0f };
}

// CatmullRom補間
Vector3 CatmullRomInterpolation(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3, float t) {

	//float s = 0.5f;
	//float t2 = t * t;
	//float t3 = t * t * t;
	//Vector3 hr1 = (-p0 + (3.0f * p1) - (3.0f * p2) + p3) * t3;
	//Vector3 hr2 = ((2.0f * p0) - (5.0f * p1) + (4.0f * p2) - p3) * t2;
	//Vector3 hr3 = ((-p0 + p2) * t) + (2.0f + p1);
	//return (s * (hr1 + hr2 + hr3));

	float t2 = t * t;
	float t3 = t * t * t;
	return Vector3(
		//x
		0.5f * ((-p0.x + 3 * p1.x - 3 * p2.x + p3.x) * t3 +
			(2 * p0.x - 5 * p1.x + 4 * p2.x - p3.x) * t2 + (-p0.x + p2.x) * t + 2 * p1.x),
		//y
		0.5f * ((-p0.y + 3 * p1.y - 3 * p2.y + p3.y) * t3 +
			(2 * p0.y - 5 * p1.y + 4 * p2.y - p3.y) * t2 + (-p0.y + p2.y) * t + 2 * p1.y),
		//z
		0.5f * ((-p0.z + 3 * p1.z - 3 * p2.z + p3.z) * t3 +
			(2 * p0.z - 5 * p1.z + 4 * p2.z - p3.z) * t2 + (-p0.z + p2.z) * t + 2 * p1.z)
	);
}

// CatmullRomスプライン曲線上の座標を得る
Vector3 CatmullRomPosition(const std::vector<Vector3>& points, uint32_t index, float t) {

	const uint32_t kIndex = uint32_t(points.size() - 1);

	int index0 = ((index - 1) + kIndex) % kIndex;
	int index1 = index;
	int index2 = (index + 1) % kIndex;
	int index3 = (index + 2) % kIndex;

	Vector3 p0 = points[index0];
	Vector3 p1 = points[index1];
	Vector3 p2 = points[index2];
	Vector3 p3 = points[index3];
	
	return CatmullRomInterpolation(p0, p1, p2, p3, t);
}




/// -------------------------------------------------------------------------
/// 3x3行列
/// -------------------------------------------------------------------------




/// -------------------------------------------------------------------------
/// 4x4行列
/// -------------------------------------------------------------------------
// 逆行列
Matrix4x4 Inverse(const Matrix4x4& m) {
	Matrix4x4 result{};

	float as =
		m.m[0][0] * m.m[1][1] * m.m[2][2] * m.m[3][3] +
		m.m[0][0] * m.m[1][2] * m.m[2][3] * m.m[3][1] +
		m.m[0][0] * m.m[1][3] * m.m[2][1] * m.m[3][2] -

		m.m[0][0] * m.m[1][3] * m.m[2][2] * m.m[3][1] -
		m.m[0][0] * m.m[1][2] * m.m[2][1] * m.m[3][3] -
		m.m[0][0] * m.m[1][1] * m.m[2][3] * m.m[3][2] -

		m.m[0][1] * m.m[1][0] * m.m[2][2] * m.m[3][3] -
		m.m[0][2] * m.m[1][0] * m.m[2][3] * m.m[3][1] -
		m.m[0][3] * m.m[1][0] * m.m[2][1] * m.m[3][2] +

		m.m[0][3] * m.m[1][0] * m.m[2][2] * m.m[3][1] +
		m.m[0][2] * m.m[1][0] * m.m[2][1] * m.m[3][3] +
		m.m[0][1] * m.m[1][0] * m.m[2][3] * m.m[3][2] +

		m.m[0][1] * m.m[1][2] * m.m[2][0] * m.m[3][3] +
		m.m[0][2] * m.m[1][3] * m.m[2][0] * m.m[3][1] +
		m.m[0][3] * m.m[1][1] * m.m[2][0] * m.m[3][2] -

		m.m[0][3] * m.m[1][2] * m.m[2][0] * m.m[3][1] -
		m.m[0][2] * m.m[1][1] * m.m[2][0] * m.m[3][3] -
		m.m[0][1] * m.m[1][3] * m.m[2][0] * m.m[3][2] -

		m.m[0][1] * m.m[1][2] * m.m[2][3] * m.m[3][0] -
		m.m[0][2] * m.m[1][3] * m.m[2][1] * m.m[3][0] -
		m.m[0][3] * m.m[1][1] * m.m[2][2] * m.m[3][0] +

		m.m[0][3] * m.m[1][2] * m.m[2][1] * m.m[3][0] +
		m.m[0][2] * m.m[1][1] * m.m[2][3] * m.m[3][0] +
		m.m[0][1] * m.m[1][3] * m.m[2][2] * m.m[3][0];

	assert(as != 0.0f);
	float determinantRecp = 1.0f / as;

	// 一行目
	result.m[0][0] = (m.m[1][1] * m.m[2][2] * m.m[3][3] + m.m[1][2] * m.m[2][3] * m.m[3][1] +
		m.m[1][3] * m.m[2][1] * m.m[3][2] - m.m[1][3] * m.m[2][2] * m.m[3][1] -
		m.m[1][2] * m.m[2][1] * m.m[3][3] - m.m[1][1] * m.m[2][3] * m.m[3][2]) *
		determinantRecp;

	result.m[0][1] = (-m.m[0][1] * m.m[2][2] * m.m[3][3] - m.m[0][2] * m.m[2][3] * m.m[3][1] -
		m.m[0][3] * m.m[2][1] * m.m[3][2] + m.m[0][3] * m.m[2][2] * m.m[3][1] +
		m.m[0][2] * m.m[2][1] * m.m[3][3] + m.m[0][1] * m.m[2][3] * m.m[3][2]) *
		determinantRecp;

	result.m[0][2] = (
		m.m[0][1] * m.m[1][2] * m.m[3][3] + m.m[0][2] * m.m[1][3] * m.m[3][1] +
		m.m[0][3] * m.m[1][1] * m.m[3][2] - m.m[0][3] * m.m[1][2] * m.m[3][1] -
		m.m[0][2] * m.m[1][1] * m.m[3][3] - m.m[0][1] * m.m[1][3] * m.m[3][2]) *
		determinantRecp;

	result.m[0][3] = (-m.m[0][1] * m.m[1][2] * m.m[2][3] - m.m[0][2] * m.m[1][3] * m.m[2][1] -
		m.m[0][3] * m.m[1][1] * m.m[2][2] + m.m[0][3] * m.m[1][2] * m.m[2][1] +
		m.m[0][2] * m.m[1][1] * m.m[2][3] + m.m[0][1] * m.m[1][3] * m.m[2][2]) *
		determinantRecp;


	// 二行目
	result.m[1][0] = (-m.m[1][0] * m.m[2][2] * m.m[3][3] - m.m[1][2] * m.m[2][3] * m.m[3][0] -
		m.m[1][3] * m.m[2][0] * m.m[3][2] + m.m[1][3] * m.m[2][2] * m.m[3][0] +
		m.m[1][2] * m.m[2][0] * m.m[3][3] + m.m[1][0] * m.m[2][3] * m.m[3][2]) *
		determinantRecp;

	result.m[1][1] = (
		m.m[0][0] * m.m[2][2] * m.m[3][3] + m.m[0][2] * m.m[2][3] * m.m[3][0] +
		m.m[0][3] * m.m[2][0] * m.m[3][2] - m.m[0][3] * m.m[2][2] * m.m[3][0] -
		m.m[0][2] * m.m[2][0] * m.m[3][3] - m.m[0][0] * m.m[2][3] * m.m[3][2]) *
		determinantRecp;

	result.m[1][2] = (-m.m[0][0] * m.m[1][2] * m.m[3][3] - m.m[0][2] * m.m[1][3] * m.m[3][0] -
		m.m[0][3] * m.m[1][0] * m.m[3][2] + m.m[0][3] * m.m[1][2] * m.m[3][0] +
		m.m[0][2] * m.m[1][0] * m.m[3][3] + m.m[0][0] * m.m[1][3] * m.m[3][2]) *
		determinantRecp;

	result.m[1][3] = (
		m.m[0][0] * m.m[1][2] * m.m[2][3] + m.m[0][2] * m.m[1][3] * m.m[2][0] +
		m.m[0][3] * m.m[1][0] * m.m[2][2] - m.m[0][3] * m.m[1][2] * m.m[2][0] -
		m.m[0][2] * m.m[1][0] * m.m[2][3] - m.m[0][0] * m.m[1][3] * m.m[2][2]) *
		determinantRecp;


	// 三行目
	result.m[2][0] = (
		m.m[1][0] * m.m[2][1] * m.m[3][3] + m.m[1][1] * m.m[2][3] * m.m[3][0] +
		m.m[1][3] * m.m[2][0] * m.m[3][1] - m.m[1][3] * m.m[2][1] * m.m[3][0] -
		m.m[1][1] * m.m[2][0] * m.m[3][3] - m.m[1][0] * m.m[2][3] * m.m[3][1]) *
		determinantRecp;

	result.m[2][1] = (-m.m[0][0] * m.m[2][1] * m.m[3][3] - m.m[0][1] * m.m[2][3] * m.m[3][0] -
		m.m[0][3] * m.m[2][0] * m.m[3][1] + m.m[0][3] * m.m[2][1] * m.m[3][0] +
		m.m[0][1] * m.m[2][0] * m.m[3][3] + m.m[0][0] * m.m[2][3] * m.m[3][1]) *
		determinantRecp;

	result.m[2][2] = (
		m.m[0][0] * m.m[1][1] * m.m[3][3] + m.m[0][1] * m.m[1][3] * m.m[3][0] +
		m.m[0][3] * m.m[1][0] * m.m[3][1] - m.m[0][3] * m.m[1][1] * m.m[3][0] -
		m.m[0][1] * m.m[1][0] * m.m[3][3] - m.m[0][0] * m.m[1][3] * m.m[3][1]) *
		determinantRecp;

	result.m[2][3] = (-m.m[0][0] * m.m[1][1] * m.m[2][3] - m.m[0][1] * m.m[1][3] * m.m[2][0] -
		m.m[0][3] * m.m[1][0] * m.m[2][1] + m.m[0][3] * m.m[1][1] * m.m[2][0] +
		m.m[0][1] * m.m[1][0] * m.m[2][3] + m.m[0][0] * m.m[1][3] * m.m[2][1]) *
		determinantRecp;


	// 四行目
	result.m[3][0] = (-m.m[1][0] * m.m[2][1] * m.m[3][2] - m.m[1][1] * m.m[2][2] * m.m[3][0] -
		m.m[1][2] * m.m[2][0] * m.m[3][1] + m.m[1][2] * m.m[2][1] * m.m[3][0] +
		m.m[1][1] * m.m[2][0] * m.m[3][2] + m.m[1][0] * m.m[2][2] * m.m[3][1]) *
		determinantRecp;

	result.m[3][1] = (

		m.m[0][0] * m.m[2][1] * m.m[3][2] + m.m[0][1] * m.m[2][2] * m.m[3][0] +
		m.m[0][2] * m.m[2][0] * m.m[3][1] - m.m[0][2] * m.m[2][1] * m.m[3][0] -
		m.m[0][1] * m.m[2][0] * m.m[3][2] - m.m[0][0] * m.m[2][2] * m.m[3][1]) *
		determinantRecp;

	result.m[3][2] = (-m.m[0][0] * m.m[1][1] * m.m[3][2] - m.m[0][1] * m.m[1][2] * m.m[3][0] -
		m.m[0][2] * m.m[1][0] * m.m[3][1] + m.m[0][2] * m.m[1][1] * m.m[3][0] +
		m.m[0][1] * m.m[1][0] * m.m[3][2] + m.m[0][0] * m.m[1][2] * m.m[3][1]) *
		determinantRecp;

	result.m[3][3] = (

		m.m[0][0] * m.m[1][1] * m.m[2][2] + m.m[0][1] * m.m[1][2] * m.m[2][0] +
		m.m[0][2] * m.m[1][0] * m.m[2][1] - m.m[0][2] * m.m[1][1] * m.m[2][0] -
		m.m[0][1] * m.m[1][0] * m.m[2][2] - m.m[0][0] * m.m[1][2] * m.m[2][1]) *
		determinantRecp;

	return result;
}

// 転置行列
Matrix4x4 Transpose(const Matrix4x4& m) {
	Matrix4x4 result{};

	result.m[0][0] = m.m[0][0];
	result.m[0][1] = m.m[1][0];
	result.m[0][2] = m.m[2][0];
	result.m[0][3] = m.m[3][0];

	result.m[1][0] = m.m[0][1];
	result.m[1][1] = m.m[1][1];
	result.m[1][2] = m.m[2][1];
	result.m[1][3] = m.m[3][1];

	result.m[2][0] = m.m[0][2];
	result.m[2][1] = m.m[1][2];
	result.m[2][2] = m.m[2][2];
	result.m[2][3] = m.m[3][2];

	result.m[3][0] = m.m[0][3];
	result.m[3][1] = m.m[1][3];
	result.m[3][2] = m.m[2][3];
	result.m[3][3] = m.m[3][3];

	return result;
}

// 平行移動行列
Matrix4x4 MakeTranslateMatrix(const Vector3 translate) {
	Matrix4x4 result{};

	result.m[0][0] = 1.0f;
	result.m[0][1] = 0.0f;
	result.m[0][2] = 0.0f;
	result.m[0][3] = 0.0f;

	result.m[1][0] = 0.0f;
	result.m[1][1] = 1.0f;
	result.m[1][2] = 0.0f;
	result.m[1][3] = 0.0f;

	result.m[2][0] = 0.0f;
	result.m[2][1] = 0.0f;
	result.m[2][2] = 1.0f;
	result.m[2][3] = 0.0f;

	result.m[3][0] = translate.x;
	result.m[3][1] = translate.y;
	result.m[3][2] = translate.z;
	result.m[3][3] = 1.0f;


	return result;
}

// 拡大縮小行列
Matrix4x4 MakeScaleMatrix(const Vector3 scale) {
	Matrix4x4 result{};

	result.m[0][0] = scale.x;
	result.m[0][1] = 0.0f;
	result.m[0][2] = 0.0f;
	result.m[0][3] = 0.0f;

	result.m[1][0] = 0.0f;
	result.m[1][1] = scale.y;
	result.m[1][2] = 0.0f;
	result.m[1][3] = 0.0f;

	result.m[2][0] = 0.0f;
	result.m[2][1] = 0.0f;
	result.m[2][2] = scale.z;
	result.m[2][3] = 0.0f;

	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;


	return result;
}

// 回転行列(X,Y,Z)
Matrix4x4 MakeRotateXMatrix(float radian) {
	Matrix4x4 result{};

	result.m[0][0] = 1;
	result.m[0][1] = 0;
	result.m[0][2] = 0;
	result.m[0][3] = 0;

	result.m[1][0] = 0;
	result.m[1][1] = std::cos(radian);
	result.m[1][2] = std::sin(radian);
	result.m[1][3] = 0;

	result.m[2][0] = 0;
	result.m[2][1] = -std::sin(radian);
	result.m[2][2] = std::cos(radian);
	result.m[2][3] = 0;

	result.m[3][0] = 0;
	result.m[3][1] = 0;
	result.m[3][2] = 0;
	result.m[3][3] = 1;

	return result;
}
Matrix4x4 MakeRotateYMatrix(float radian) {
	Matrix4x4 result{};

	result.m[0][0] = std::cos(radian);
	result.m[0][1] = 0;
	result.m[0][2] = -std::sin(radian);
	result.m[0][3] = 0;

	result.m[1][0] = 0;
	result.m[1][1] = 1;
	result.m[1][2] = 0;
	result.m[1][3] = 0;

	result.m[2][0] = std::sin(radian);
	result.m[2][1] = 0;
	result.m[2][2] = std::cos(radian);
	result.m[2][3] = 0;

	result.m[3][0] = 0;
	result.m[3][1] = 0;
	result.m[3][2] = 0;
	result.m[3][3] = 1;

	return result;
}
Matrix4x4 MakeRotateZMatrix(float radian) {
	Matrix4x4 result{};

	result.m[0][0] = std::cos(radian);
	result.m[0][1] = std::sin(radian);
	result.m[0][2] = 0;
	result.m[0][3] = 0;

	result.m[1][0] = -std::sin(radian);
	result.m[1][1] = std::cos(radian);
	result.m[1][2] = 0;
	result.m[1][3] = 0;

	result.m[2][0] = 0;
	result.m[2][1] = 0;
	result.m[2][2] = 1;
	result.m[2][3] = 0;

	result.m[3][0] = 0;
	result.m[3][1] = 0;
	result.m[3][2] = 0;
	result.m[3][3] = 1;

	return result;
}

// 回転行列(all)
Matrix4x4 MakeRotateXYZMatrix(Vector3 rotate) {
	Matrix4x4 result{};

	Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotate.x);
	Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotate.y);
	Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotate.z);

	result = rotateXMatrix * (rotateYMatrix * rotateZMatrix);

	return result;
}

// 3次元アフィン変換行列 (W = SRT)
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
	Matrix4x4 result{};

	// 拡大縮小行列
	Matrix4x4 scaleMatrix;
	scaleMatrix = MakeScaleMatrix(scale);

	// 回転行列(all)
	Matrix4x4 rotateMatrix;
	rotateMatrix = MakeRotateXYZMatrix(rotate);

	// 平行移動行列
	Matrix4x4 translateMatrix;
	translateMatrix = MakeTranslateMatrix(translate);

	result = scaleMatrix * (rotateMatrix * translateMatrix);

	return result;
}

// 透視投影行列
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip) {
	Matrix4x4 result{};

	result.m[0][0] = (1 / aspectRatio) * 1 / std::tan(fovY / 2);
	result.m[0][1] = 0.0f;
	result.m[0][2] = 0.0f;
	result.m[0][3] = 0.0f;

	result.m[1][0] = 0.0f;
	result.m[1][1] = 1 / std::tan(fovY / 2);
	result.m[1][2] = 0.0f;
	result.m[1][3] = 0.0f;

	result.m[2][0] = 0.0f;
	result.m[2][1] = 0.0f;
	result.m[2][2] = farClip / (farClip - nearClip);
	result.m[2][3] = 1.0f;

	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = -nearClip * farClip / (farClip - nearClip);
	result.m[3][3] = 0.0f;

	return result;
}

// 正射影行列
Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip) {

	float width = right - left;
	float height = top - bottom;
	float zLength = farClip - nearClip;

	Matrix4x4 result{};

	result.m[0][0] = 2 / width;
	result.m[0][1] = 0.0f;
	result.m[0][2] = 0.0f;
	result.m[0][3] = 0.0f;

	result.m[1][0] = 0.0f;
	result.m[1][1] = 2 / height;
	result.m[1][2] = 0.0f;
	result.m[1][3] = 0.0f;

	result.m[2][0] = 0.0f;
	result.m[2][1] = 0.0f;
	result.m[2][2] = 1 / zLength;
	result.m[2][3] = 0.0f;

	result.m[3][0] = (left + right) / (left - right);
	result.m[3][1] = (top + bottom) / (bottom - top);
	result.m[3][2] = nearClip / (nearClip - farClip);
	result.m[3][3] = 1;

	return result;
}

// ビューポート変換行列
Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth) {
	Matrix4x4 result{};

	result.m[0][0] = width / 2;
	result.m[0][1] = 0.0f;
	result.m[0][2] = 0.0f;
	result.m[0][3] = 0.0f;

	result.m[1][0] = 0.0f;
	result.m[1][1] = -height / 2;
	result.m[1][2] = 0.0f;
	result.m[1][3] = 0.0f;

	result.m[2][0] = 0.0f;
	result.m[2][1] = 0.0f;
	result.m[2][2] = maxDepth - minDepth;
	result.m[2][3] = 0.0f;

	result.m[3][0] = left + (width / 2);
	result.m[3][1] = top + (height / 2);
	result.m[3][2] = minDepth;
	result.m[3][3] = 1.0f;

	return result;
}

// 任意軸回転行列
Matrix4x4 MakeRotateAxisAngle(const Vector3& axis, float angle) {
	float cosTheta = cosf(angle);
	float sinTheta = sinf(angle);

	Matrix4x4 result{};
	result = Matrix4x4::identity;


	result.m[0][0] = (axis.x * axis.x) * (1 - cosTheta) + cosTheta;
	result.m[0][1] = (axis.x * axis.y) * (1 - cosTheta) - axis.z * sinTheta;
	result.m[0][2] = (axis.x * axis.z) * (1 - cosTheta) + axis.y * sinTheta;

	result.m[1][0] = (axis.x * axis.y) * (1 - cosTheta) + axis.z * sinTheta;
	result.m[1][1] = (axis.y * axis.y) * (1 - cosTheta) + cosTheta;
	result.m[1][2] = (axis.y * axis.z) * (1 - cosTheta) - axis.x * sinTheta;

	result.m[2][0] = (axis.x * axis.z) * (1 - cosTheta) - axis.y * sinTheta;
	result.m[2][1] = (axis.y * axis.z) * (1 - cosTheta) + axis.x * sinTheta;
	result.m[2][2] = (axis.z * axis.z) * (1 - cosTheta) + cosTheta;


	return result;
}
Matrix4x4 MakeRotateAxisAngle(const Vector3& axis, float cos, float sin) {
	Matrix4x4 result{};
	result = Matrix4x4::identity;


	result.m[0][0] = axis.x * axis.x * (1 - cos) + cos;
	result.m[0][1] = axis.x * axis.y * (1 - cos) - axis.z * sin;
	result.m[0][2] = axis.x * axis.z * (1 - cos) + axis.y * sin;

	result.m[1][0] = axis.x * axis.y * (1 - cos) + axis.z * sin;
	result.m[1][1] = axis.y * axis.y * (1 - cos) + cos;
	result.m[1][2] = axis.y * axis.z * (1 - cos) - axis.x * sin;

	result.m[2][0] = axis.x * axis.z * (1 - cos) - axis.y * sin;
	result.m[2][1] = axis.y * axis.z * (1 - cos) + axis.x * sin;
	result.m[2][2] = axis.z * axis.z * (1 - cos) + cos;
	return result;
}

// ある方向からある方向への回転
Matrix4x4 DirectionToDirection(const Vector3& from, const Vector3& to) {

	Vector3 fromVector = Normalize(from);
	Vector3 toVector = Normalize(to);
	Vector3 n = Normalize(Cross(fromVector, toVector));

	float cos = Dot(fromVector, toVector);
	float sin = Length(Cross(fromVector, toVector));


	if (from.x == -to.x && from.y == -to.y && from.z == -to.z) {

		if (from.x != 0.0f || from.y != 0.0f) {

			n = { from.y, -from.x, 0.0f };
		}
		else if (from.x != 0.0f || from.z != 0.0f) {

			n = { from.z, 0.0f, -from.x };
		}
	}


	Matrix4x4 result = MakeRotateAxisAngle(n, sin, cos);


	return result;
}




/// -------------------------------------------------------------------------
/// クォータニオン
/// -------------------------------------------------------------------------
// 内積
float Dot(const Quaternion& q1, const Quaternion& q2) {
	return (q1.x * q2.x) + (q1.y * q2.y) + (q1.z * q2.z) + (q1.w * q2.w);
}

// 長さ
float Length(const Quaternion p) {
	return  std::sqrt(Dot(p, p));
}

// 共役Quaternionを返す
Quaternion Conjugate(const Quaternion& q) {
	return { q.w , -q.x, -q.y, -q.z };
}

// Quaternionのnormを返す
float Norm(const Quaternion& q) {
	return std::sqrt(float(pow(q.w, 2) + pow(q.x, 2) + pow(q.y, 2) + (q.z, 2)));
}

// 正規化したQuaternionを返す
Quaternion Normalize(const Quaternion& q) {
	float length = Length(q);
	return q / length;
}

// 逆Quatenionを返す
Quaternion Inverse(const Quaternion& q) {
	Quaternion result = Conjugate(q);
	float norm = float(pow(Norm(q), 2));
	return result / norm;
}

// 球面線形補間
Quaternion Slerp(const Quaternion& q1, const Quaternion& q2, float t) {
	Quaternion q0 = q1;
	float dot = Dot(q1, q2);
	if (dot < 0.0f) {
		q0 = -q0;
		dot = -dot;
	}
	constexpr float EPSILON = 0.0001f;
	if (dot >= 1.0f - EPSILON) {
		return (1.0f - t) * q0 + t * q1;
	}
	float theta = std::acos(dot);
	float sinTheta = 1.0f / std::sin(theta);
	float scale1 = std::sin((1.0f - t) * theta) * sinTheta;
	float scale2 = std::sin(t * theta) * sinTheta;
	return (q0 * scale1) + (q2 * scale2);
}

// 任意軸回転を表すQuaternionの生成
Quaternion	MakeRotateAxisAngleQuatenion(const Vector3& axis, float angle) {
	float halfAngle = angle * 0.5f;
	float sinHalfAngle = sinf(halfAngle);
	float cosHalfAngle = cosf(halfAngle);
	Quaternion hr = {
		axis.x * sinHalfAngle,
		axis.y * sinHalfAngle,
		axis.z * sinHalfAngle,
		cosHalfAngle };

	return hr;
}

// ベクトルをQuaternionで回転させた結果のベクトルを求める
Vector3 RotateVector(const Vector3& v, const Quaternion& q) {
	Quaternion vecQ = { 0.0f, v.x, v.y, v.z };
	Quaternion conQ = Conjugate(q);
	Quaternion rotV = q * (vecQ * conQ);
	return { rotV.x, rotV.y, rotV.z };
}

// Quaternionから回転行列を求める
Matrix4x4 MakeRotateMatrix(const Quaternion& q) {
	Quaternion powQ = { float(pow(q.w, 2)), float(pow(q.x, 2)), float(pow(q.y, 2)), float(pow(q.z, 2)) };
	Matrix4x4 hr = Matrix4x4::identity;
	hr.m[0][0] = powQ.w + powQ.x - powQ.y - powQ.z;
	hr.m[0][1] = 2 * ((q.x * q.y) + (q.w * q.z));
	hr.m[0][2] = 2 * ((q.x * q.z) - (q.w * q.y));

	hr.m[1][0] = 2 * ((q.x * q.y) - (q.w * q.z));
	hr.m[1][1] = powQ.w - powQ.x + powQ.y - powQ.z;
	hr.m[1][2] = 2 * ((q.y * q.z) + (q.w * q.x));

	hr.m[2][0] = 2 * ((q.x * q.z) + (q.w * q.y));
	hr.m[2][1] = 2 * ((q.y * q.z) - (q.w * q.x));
	hr.m[2][2] = powQ.w - powQ.x - powQ.y + powQ.z;
	return hr;
}

