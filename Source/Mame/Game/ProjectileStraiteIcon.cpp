#include "ProjectileStraiteIcon.h"
#include "ProjectileIconManager.h"

#include "../Graphics/Graphics.h"

// �R���X�g���N�^
ProjectileStraiteIcon::ProjectileStraiteIcon(ProjectileIconManager* manager)
    :ProjectileIcon(manager)
{
    Graphics& graphics = Graphics::Instance();

    model_ = std::make_unique<Model>(graphics.GetDevice(), "./Resources/Model/Projectile/sqhere.fbx");

    // ImGui���O�ݒ�
    SetName("ProjectileStraite" + std::to_string(nameNum_++));
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
    ProjectileIcon::Render(scale);
}

// ImGui�p
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
