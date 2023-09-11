#include "ProjectileStraite.h"

#include "EnemyManager.h"
#include "PlayerManager.h"
#include "ProjectileStraiteIcon.h"
#include "Collision.h"

// コンストラクタ
ProjectileStraite::ProjectileStraite(ProjectileManager* manager)
    :Projectile(manager)
{
    //lifeTimer = 10.0f;

    // ImGui名前設定
    SetName("ProjectileStraite" + std::to_string(nameNum++));
}

// 初期化
void ProjectileStraite::Initialize()
{
}

// 更新処理
void ProjectileStraite::Update(const float& elapsedTime)
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

    CollisionProjectileVsEnemies();
}

// 描画処理
void ProjectileStraite::Render(const float& scale)
{
    Projectile::Render(scale);
}

// ImGui用
void ProjectileStraite::DrawDebug()
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
void ProjectileStraite::Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position)
{
    this->direction = direction;
    GetTransform()->SetPosition(position);
}

void ProjectileStraite::CollisionProjectileVsEnemies()
{
    using DirectX::XMFLOAT3;

    EnemyManager& enemyManager = EnemyManager::Instance();
    const int enemyCount = enemyManager.GetEnemyCount();
    for (int i = 0; (i < enemyCount); ++i)
    {
        Enemy* enemy = enemyManager.GetEnemy(i);

        if (enemy->hp_ <= 0) return;

        const XMFLOAT3& projectilePos = GetTransform()->GetPosition();
        const XMFLOAT3& enemyPos = enemy->GetTransform()->GetPosition();

        XMFLOAT3 outPosition = {};
        if (Collision::IntersectSphereVsSphere(
            projectilePos, GetRadius(),
            enemyPos, enemy->radius_, &outPosition))
        {
            --enemy->hp_;
            if (enemy->hp_ <= 0)
            {
                // 敵の所持している弾丸アイコンの数だけプレイヤーの弾丸アイコンを追加
                const std::unique_ptr<Player>& player = PlayerManager::Instance().GetPlayer();
                const int enemyProjIconCount = enemy->projectileIconManager_.GetProjectileIconCount();
                for (int i = 0; i < enemyProjIconCount; ++i)
                {
                    new ProjectileStraiteIcon(&player->projectileIconManager_);
                }

                enemyManager.Remove(enemy);
            }

            this->Destroy(); // 弾丸消去

            break; // 弾が当たって消えたのでbreak終了
        }
    }
}
