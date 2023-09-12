#include "EnemyProjectileStraite.h"

#include "PlayerManager.h"
#include "ProjectileStraiteIcon.h"
#include "Collision.h"

// コンストラクタ
EnemyProjectileStraite::EnemyProjectileStraite(ProjectileManager* manager)
    :Projectile(manager)
{
    //lifeTimer = 10.0f;

    // ImGui名前設定
    SetName("ProjectileStraite" + std::to_string(nameNum++));
}

// 初期化
void EnemyProjectileStraite::Initialize()
{
}

// 更新処理
void EnemyProjectileStraite::Update(const float& elapsedTime)
{
    //// 寿命処理
    //lifeTimer_ -= elapsedTime;
    //if (lifeTimer_ <= 0.0f) Destroy();

    DirectX::XMFLOAT3 position = GetTransform()->GetPosition();

    if (position.z >= 80.0f) Destroy();

    // 移動
    {
        float speed = this->speed_ * elapsedTime;
        position.x += direction.x * speed;
        position.y += direction.y * speed;
        position.z += direction.z * speed;
    }

    GetTransform()->SetPosition(position);

    CollisionProjectileVsPlayer();
}

// 描画処理
void EnemyProjectileStraite::Render(const float& scale)
{
    Projectile::Render(scale);
}

// ImGui用
void EnemyProjectileStraite::DrawDebug()
{
#ifdef USE_IMGUI
    Projectile::DrawDebug();
    if (ImGui::BeginMenu(GetName()))
    {

        ImGui::EndMenu();
    }
#endif // USE_IMGUI
}

// 発射
void EnemyProjectileStraite::Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position)
{
    this->direction = direction;
    GetTransform()->SetPosition(position);
}

void EnemyProjectileStraite::CollisionProjectileVsPlayer()
{
    if (this->hp_ <= 0) return;

    using DirectX::XMFLOAT3;

    std::unique_ptr<Player>& player = PlayerManager::Instance().GetPlayer();

    if (player->hp_ <= 0) return;

    const XMFLOAT3& projPos = GetTransform()->GetPosition();
    const XMFLOAT3& plPos = player->GetTransform()->GetPosition();

    XMFLOAT3 outPosition = {};
    if (Collision::IntersectSphereVsSphere(
        projPos, GetRadius(),
        plPos, player->radius_, &outPosition))
    {
        --player->hp_;

        --this->hp_;
        if (this->hp_ <= 0) { this->Destroy(); } // 弾丸消去

    }
}
