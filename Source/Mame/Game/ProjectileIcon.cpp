#include "ProjectileIcon.h"
#include "ProjectileIconManager.h"

int ProjectileIcon::nameNum_ = 0;

// コンストラクタ
ProjectileIcon::ProjectileIcon(ProjectileIconManager* manager)
    : manager_(manager)
{
    manager->Register(this);
}

// 初期化
void ProjectileIcon::Initialize()
{
    projectileManager_.Initialize();
}

// 終了化
void ProjectileIcon::Finalize()
{
    projectileManager_.Finalize();
}

// Updateの前に呼ばれる
void ProjectileIcon::Begin()
{
}

// 更新処理
void ProjectileIcon::Update(const float& elapsedTime)
{
    projectileManager_.Update(elapsedTime);
}

// Updateの後に呼ばれる
void ProjectileIcon::End()
{
}

// 描画処理
void ProjectileIcon::Render(const float& scale)
{
    model_->Render(scale);

    projectileManager_.Render(scale);
}

// ImGui用
void ProjectileIcon::DrawDebug()
{
#ifdef USE_IMGUI

    GetTransform()->DrawDebug();

    projectileManager_.DrawDebug();

#endif // USE_IMGUI
}

// 破棄
void ProjectileIcon::Destroy()
{
    manager_->Remove(this);
}
