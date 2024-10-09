#include "ParticleArray.h"



// 更新処理
void ParticleArray::Update()
{
	// isActiveがfalseなら削除
	particles_.remove_if([](unique_ptr<IParticle>& particle)
		{
			if (not particle->GetIsActive()) {
				return true;
			}
			return false;
		});

	// 削除チェックをスルー出来たら、更新処理
	for (auto& p : particles_) {

		p->Update();
	}
}


// 粒子配列に追加
void ParticleArray::push_back(unique_ptr<IParticle> addParticle)
{
	particles_.push_back(move(addParticle)); // 所有権を移動して追加
}

