#include "EnemyProjectileStraiteIcon.h"
#include "../Graphics/Graphics.h"

#include "ProjectileIconManager.h"
#include "PlayerManager.h"
#include "EnemyProjectileStraite.h"

// コンストラクタ
EnemyProjectileStraiteIcon::EnemyProjectileStraiteIcon(ProjectileIconManager* manager)
    :ProjectileIcon(manager)
{
    Graphics& graphics = Graphics::Instance();

    // ImGui名前設定
    SetName("ProjectileStraite" + std::to_string(nameNum_++));
}

// 初期化
void EnemyProjectileStraiteIcon::Initialize()
{
    ProjectileIcon::Initialize();

    launchTimer_ = launchTime_;
}

// 更新処理
void EnemyProjectileStraiteIcon::Update(const float& elapsedTime)
{
    using DirectX::XMFLOAT3;

    ProjectileIcon::Update(elapsedTime);

    launchTimer_ -= elapsedTime;
    if (launchTimer_ <= 0.0f)
    {
        constexpr float length = 0.3f;

        const XMFLOAT3& position = manager_->projStraitePosition_;
        const XMFLOAT3& forward  = manager_->projStraiteForward_;
        const DirectX::XMFLOAT3 spawnPosition = {
            position.x + (forward.x * length),
            position.y + (forward.y * length),
            position.z + (forward.z * length)
        };

        EnemyProjectileStraite* projectile = new EnemyProjectileStraite(&projectileManager_);
        projectile->speed_          = setSpeed_;
        projectile->lifeTimer_      = setLifeTime_;
        projectile->model->color    = { 1.0f, 0.1f, 0.1f, 1.0f };
        projectile->Launch(forward, spawnPosition);

        launchTimer_ = launchTime_;
    }
}

// 描画処理
void EnemyProjectileStraiteIcon::Render(const float& scale)
{
    ProjectileIcon::Render(scale);
}

// ImGui用
void EnemyProjectileStraiteIcon::DrawDebug()
{
#ifdef USE_IMGUI
    ProjectileIcon::DrawDebug();
    if (ImGui::BeginMenu(GetName()))
    {
        ImGui::EndMenu();
    }
#endif // USE_IMGUI
}
