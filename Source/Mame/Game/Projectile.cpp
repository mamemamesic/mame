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
    using DirectX::XMFLOAT3;
    using DirectX::XMFLOAT4;

    model->Render(scale);

#ifdef _DEBUG
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    const XMFLOAT3 position = GetTransform()->GetPosition();
    const XMFLOAT4 color    = { 0,0,0,1 };
    debugRenderer->DrawSphere(position, radius_, color);

#endif // _DEBUG
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
