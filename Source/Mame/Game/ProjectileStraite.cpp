#include "ProjectileStraite.h"

// �R���X�g���N�^
ProjectileStraite::ProjectileStraite(ProjectileManager* manager)
    :Projectile(manager)
{
    //lifeTimer = 10.0f;

    // ImGui���O�ݒ�
    SetName("ProjectileStraite" + std::to_string(nameNum++));
}

// ������
void ProjectileStraite::Initialize()
{
}

// �X�V����
void ProjectileStraite::Update(const float& elapsedTime)
{
    //// ��������
    //lifeTimer_ -= elapsedTime;
    //if (lifeTimer_ <= 0.0f) Destroy();

    DirectX::XMFLOAT3 position = GetTransform()->GetPosition();

    if (position.z >= 80.0f) Destroy();

    // �ړ�
    {
        float speed = this->speed_ * elapsedTime;
        position.x += direction.x * speed;
        position.y += direction.y * speed;
        position.z += direction.z * speed;
    }

    GetTransform()->SetPosition(position);
}

// �`�揈��
void ProjectileStraite::Render(const float& scale)
{
    Projectile::Render(scale);
}

// ImGui�p
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

// ����
void ProjectileStraite::Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position)
{
    this->direction = direction;
    GetTransform()->SetPosition(position);
}
