#pragma once

#include "MyMath.h"

#include <map>
#include <iostream>


/* SaveValueクラス */
class SaveValue {

private:

	// コンストラクタ、デストラクタ
	SaveValue() = default;
	~SaveValue() = default;
	SaveValue(const SaveValue&) = delete;
	const SaveValue& operator=(const SaveValue&) = delete;

public:

	// インスタンスの取得
	static SaveValue* GetInstance() {
		static SaveValue instance;
		return &instance;
	}

	// 値を保存する
	static void Save(const std::string key, int value);
	static void Save(const std::string key, float value);
	
	// 値取り出す
	static int const GetIntValue(const std::string key);
	static float const GetFloatValue(const std::string key);

	// クリア
	static void Clear();

private:

	std::map<std::string, int> intValueMap_;
	std::map<std::string, float> floatValueMap_;

};

