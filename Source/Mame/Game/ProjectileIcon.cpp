#include "ProjectileIcon.h"
#include "ProjectileIconManager.h"

int ProjectileIcon::nameNum_ = 0;

// �R���X�g���N�^
ProjectileIcon::ProjectileIcon(ProjectileIconManager* manager)
    : manager_(manager)
{
    manager->Register(this);
}

// ������
void ProjectileIcon::Initialize()
{
}

// �I����
void ProjectileIcon::Finalize()
{
}

// Update�̑O�ɌĂ΂��
void ProjectileIcon::Begin()
{
}

// �X�V����
void ProjectileIcon::Update(const float& elapsedTime)
{
}

// Update�̌�ɌĂ΂��
void ProjectileIcon::End()
{
}

// �`�揈��
void ProjectileIcon::Render(const float& scale)
{
    model_->Render(scale);
}

// ImGui�p
void ProjectileIcon::DrawDebug()
{
#ifdef USE_IMGUI

    GetTransform()->DrawDebug();

#endif // USE_IMGUI
}

// �j��
void ProjectileIcon::Destroy()
{
    manager_->Remove(this);
}
