#include "ProjectileStraiteIcon.h"
#include "ProjectileIconManager.h"

#include "../Graphics/Graphics.h"
#include "../Game/PlayerManager.h"
#include "../Game/ProjectileStraite.h"

// コンストラクタ
ProjectileStraiteIcon::ProjectileStraiteIcon(ProjectileIconManager* manager)
    :ProjectileIcon(manager)
{
    Graphics& graphics = Graphics::Instance();

    // ImGui名前設定
    SetName("ProjectileStraite" + std::to_string(nameNum_++));
}

// 初期化
void ProjectileStraiteIcon::Initialize()
{
    ProjectileIcon::Initialize();

    launchTimer_ = launchTime_;
}

// 更新処理
void ProjectileStraiteIcon::Update(const float& elapsedTime)
{
    ProjectileIcon::Update(elapsedTime);

    launchTimer_ -= elapsedTime;
    if (launchTimer_ <= 0.0f)
    {
        PlayerManager& playerManager = PlayerManager::Instance();
        const DirectX::XMFLOAT3 position = playerManager.GetPlayer()->GetTransform()->GetPosition();
        const DirectX::XMFLOAT3 forward  = playerManager.GetPlayer()->GetTransform()->CalcForward();

        constexpr float length = 0.3f;

        const DirectX::XMFLOAT3 spawnPosition = {
            position.x + (forward.x * length),
            position.y + (forward.y * length),
            position.z + (forward.z * length)
        };

        ProjectileStraite* projectile = new ProjectileStraite(&projectileManager_);
        projectile->speed_       = setSpeed_;
        projectile->lifeTimer_   = setLifeTime_;
        projectile->model->color = model_->color;
        projectile->Launch(forward, spawnPosition);

        launchTimer_ = launchTime_;
    }
}

// 描画処理
void ProjectileStraiteIcon::Render(const float& scale)
{
    ProjectileIcon::Render(scale);
}

// ImGui用
void ProjectileStraiteIcon::DrawDebug()
{
#ifdef USE_IMGUI
    ProjectileIcon::DrawDebug();
    if (ImGui::BeginMenu(GetName()))
    {
        ImGui::EndMenu();
    }
#endif // USE_IMGUI
}
