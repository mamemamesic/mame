#include "ProjectileStraite.h"

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
