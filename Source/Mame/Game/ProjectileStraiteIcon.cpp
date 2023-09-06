#include "ProjectileStraiteIcon.h"
#include "ProjectileIconManager.h"

#include "../Graphics/Graphics.h"

// コンストラクタ
ProjectileStraiteIcon::ProjectileStraiteIcon(ProjectileIconManager* manager)
    :ProjectileIcon(manager)
{
    Graphics& graphics = Graphics::Instance();

    model_ = std::make_unique<Model>(graphics.GetDevice(), "./Resources/Model/Projectile/sqhere.fbx");

    // ImGui名前設定
    SetName("ProjectileStraite" + std::to_string(nameNum_++));
}

// 初期化
void ProjectileStraiteIcon::Initialize()
{

}

// 更新処理
void ProjectileStraiteIcon::Update(const float& elapsedTime)
{
    DirectX::XMFLOAT3 position = GetTransform()->GetPosition();

    GetTransform()->SetPosition(position);
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
