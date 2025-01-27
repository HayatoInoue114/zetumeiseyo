// ChaceForStreightState.cpp
#include "ChaceForStreightState.h"

ChaceForStreightState::ChaceForStreightState(Blast* blast)
    : blast_(blast)
{
}

void ChaceForStreightState::Initialize()
{
    // 初期化処理
}

void ChaceForStreightState::Update(float deltaTime)
{
    // ChaceForStreight のロジックをここに移動
    if (blast_->IsTrace() && !blast_->IsBlast())
    {
        worldTransform.rotate = {};

        blast_->SetCount(blast_->GetCount() + 1);
        blast_->SetBlastCount(blast_->GetBlastCount() + 1);
        // 座標を移動させる（1フレーム分の移動量を足しこむ)
        Vector3 toPlayer = blast_->GetPlayer()->GetWorldPos() - worldTransform.translate;

        // ベクトルを正規化する
        toPlayer = Normalize(toPlayer);

        blast_->SetVelocity(SLerp(blast_->GetVelocity(), toPlayer, blast_->GetChaseIntensity()) * blast_->GetParam().speed);

        // Y軸周り角度(0y)
        worldTransform.rotate.y = std::atan2(blast_->GetVelocity().x, blast_->GetVelocity().z);

        blast_->SetVelocityXZ(std::sqrt(blast_->GetVelocity().x * blast_->GetVelocity().x + blast_->GetVelocity().z * blast_->GetVelocity().z));
        // X軸周り角度(0x)
        worldTransform.rotate.x = std::atan2(-blast_->GetVelocity().y, blast_->GetVelocityXZ());

        blast_->SetT(0);

        blast_->SetStreight(true);
    }
    if (blast_->IsStreight())
    {
        blast_->SetStreightCount(blast_->GetStreightCount() + 1);
        if (blast_->GetStreightCount() == 30)
        {
            blast_->SetStreightVel(blast_->GetVelocity() * 10);
        }
        if (blast_->GetStreightCount() >= 30)
        {
            worldTransform.translate = worldTransform.translate + blast_->GetStreightVel();
        }
        if (blast_->GetStreightCount() >= 60)
        {
            blast_->SetStreightCount(0);
            blast_->SetStreight(false);
        }
    }
}

void ChaceForStreightState::SetWorldTransform(const WorldTransform* transform)
{
    if (transform)
    {
        worldTransform = *transform;
    }
}

const WorldTransform* ChaceForStreightState::GetWorldTransform() const
{
    return &worldTransform;
}


