//#pragma once
//#include "GameObject.h"
//
//class IParticleModel
//{
//public:
//	virtual void Init();
//
//	virtual void Update();
//
//	virtual void Draw(Camera* camera);
//
//	// 新しいインスタンスを作成
//	template<typename T>
//	static std::unique_ptr<T> Create() {
//		std::unique_ptr<T> newParticle = std::make_unique<T>();
//		newParticle->Initialize();
//		return std::move(newParticle);
//	}
//
//	bool isAlive = false;
//protected:
//	std::unique_ptr<Model> model_{};
//
//	WorldTransform worldTransform_{};
//
//	float count{};
//};
//
