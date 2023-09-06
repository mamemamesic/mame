#include "ProjectileStraiteIcon.h"

#include "../Graphics/Graphics.h"

#include "ItemManager.h"

// �R���X�g���N�^
ProjectileStraiteIcon::ProjectileStraiteIcon(ProjectileManager* manager)
    :Projectile(manager)
{
    Graphics& graphics = Graphics::Instance();

    model = std::make_unique<Model>(graphics.GetDevice(), "./Resources/Model/Projectile/sqhere.fbx");

    // ImGui���O�ݒ�
    SetName("ProjectileStraite" + std::to_string(nameNum++));
}

// ������
void ProjectileStraiteIcon::Initialize()
{
}

// �X�V����
void ProjectileStraiteIcon::Update(const float& elapsedTime)
{
    DirectX::XMFLOAT3 position = GetTransform()->GetPosition();

    GetTransform()->SetPosition(position);
}

// �`�揈��
void ProjectileStraiteIcon::Render(const float& scale)
{
    Projectile::Render(scale);
}

// ImGui�p
void ProjectileStraiteIcon::DrawDebug()
{
#ifdef USE_IMGUI
    Projectile::DrawDebug();
    if (ImGui::BeginMenu(GetName()))
    {

        ImGui::EndMenu();
    }
#endif // USE_IMGUI
}
