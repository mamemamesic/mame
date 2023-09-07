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
    projectileManager_.Initialize();
}

// �I����
void ProjectileIcon::Finalize()
{
    projectileManager_.Finalize();
}

// Update�̑O�ɌĂ΂��
void ProjectileIcon::Begin()
{
}

// �X�V����
void ProjectileIcon::Update(const float& elapsedTime)
{
    projectileManager_.Update(elapsedTime);
}

// Update�̌�ɌĂ΂��
void ProjectileIcon::End()
{
}

// �`�揈��
void ProjectileIcon::Render(const float& scale)
{
    model_->Render(scale);

    projectileManager_.Render(scale);
}

// ImGui�p
void ProjectileIcon::DrawDebug()
{
#ifdef USE_IMGUI

    GetTransform()->DrawDebug();

    projectileManager_.DrawDebug();

#endif // USE_IMGUI
}

// �j��
void ProjectileIcon::Destroy()
{
    manager_->Remove(this);
}
