#include "SaveValue.h"



void SaveValue::Save(const std::string key, int value)
{
	int intValue = GetIntValue(key);
	if (intValue) { return; }
	SaveValue::GetInstance()->intValueMap_[key] = value;
}


void SaveValue::Save(const std::string key, float value)
{
	float floatValue = GetFloatValue(key);
	if (floatValue) { return; }
	SaveValue::GetInstance()->floatValueMap_[key] = value;
}


int const SaveValue::GetIntValue(const std::string key)
{
	auto value = SaveValue::GetInstance()->intValueMap_.find(key);
	if (value != SaveValue::GetInstance()->intValueMap_.end()) {
		return value->second;
	}
	return 0;
}


float const SaveValue::GetFloatValue(const std::string key)
{
	auto value = SaveValue::GetInstance()->floatValueMap_.find(key);
	if (value != SaveValue::GetInstance()->floatValueMap_.end()) {
		return value->second;
	}
	return 0.0f;
}


void SaveValue::Clear()
{
	SaveValue::GetInstance()->intValueMap_.clear();
	SaveValue::GetInstance()->floatValueMap_.clear();
}
