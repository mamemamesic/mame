#include "Projectile.h"
#include "ProjectileManager.h"

#include <memory>
#include "../Graphics/Graphics.h"

int Projectile::nameNum = 0;

// �R���X�g���N�^
Projectile::Projectile(ProjectileManager* manager)
    :manager(manager)
{
    Graphics& graphics = Graphics::Instance();

    model = std::make_unique<Model>(graphics.GetDevice(), "./Resources/Model/Projectile/sqhere.fbx");

    manager->Register(this);
}

// ������
void Projectile::Initialize()
{
}

// �I����
void Projectile::Finalize()
{
}

// Update�̑O�ɌĂ΂��
void Projectile::Begin()
{
}

// �X�V����
void Projectile::Update(const float& elapsedTime)
{
}

// Update�̌�ɌĂ΂��
void Projectile::End()
{
}

// �`�揈��
void Projectile::Render(const float& scale)
{
    model->Render(scale);
}

// ImGui�p
void Projectile::DrawDebug()
{
#ifdef USE_IMGUI

    GetTransform()->DrawDebug();

#endif // USE_IMGUI
}

// �j��
void Projectile::Destroy()
{
    manager->Remove(this);
}
