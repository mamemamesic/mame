#include "Projectile.h"
#include "ProjectileManager.h"

#include <memory>
#include "../Graphics/Graphics.h"

int Projectile::nameNum = 0;

// コンストラクタ
Projectile::Projectile(ProjectileManager* manager)
    :manager(manager)
{
    Graphics& graphics = Graphics::Instance();

    model = std::make_unique<Model>(graphics.GetDevice(), "./Resources/Model/Projectile/sqhere.fbx");

    manager->Register(this);
}

// 初期化
void Projectile::Initialize()
{
}

// 終了化
void Projectile::Finalize()
{
}

// Updateの前に呼ばれる
void Projectile::Begin()
{
}

// 更新処理
void Projectile::Update(const float& elapsedTime)
{
}

// Updateの後に呼ばれる
void Projectile::End()
{
}

// 描画処理
void Projectile::Render(const float& scale)
{
    model->Render(scale);
}

// ImGui用
void Projectile::DrawDebug()
{
#ifdef USE_IMGUI

    GetTransform()->DrawDebug();

#endif // USE_IMGUI
}

// 破棄
void Projectile::Destroy()
{
    manager->Remove(this);
}
