#pragma once

#include "Projectile.h"

class EnemyProjectileStraite : public Projectile
{
public:
    EnemyProjectileStraite(ProjectileManager* manager);
    ~EnemyProjectileStraite() {}

    void Initialize()                                           override;   // ������
    void Finalize()                                             override {} // �I����
    void Begin()                                                override {} // ���t���[����ԍŏ��ɌĂ΂��
    void Update(const float& elapsedTime)                       override;   // �X�V����
    void End()                                                  override {} // ���t���[����ԍŌ�ɌĂ΂��
    void Render(const float& scale)   override;   // �`�揈��
    void DrawDebug()                                            override;   // ImGui�p

public:
    // ����
    void Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position);

private:
    void CollisionProjectileVsPlayer();

private:
    DirectX::XMFLOAT3 direction = {};    // ����
    //float speed = 1.0f;            // ���x
    //float lifeTimer_ = 1.0f;         // ����
};