#pragma once

#include <memory>

#include "Character.h"
#include "ProjectileManager.h"
#include "ProjectileIconManager.h"

class Player : public Character
{
public: // enum�֘A
    // �X�e�[�g
    enum class State
    {
        Idle,   // �ҋ@
        Walk,   // ���s
    };

private: // enum�֘A

    // �A�j���[�V����
    enum class Animation
    {
    };

    // �ߐڍU���p�̋��̍\����
    struct CloseRangeAttackSphere
    {
        DirectX::XMFLOAT3 position_ = {};
        float radius_    = 0.0f;
        float lifeTimer_ = 0.0f;
    };

public:
    Player();
    ~Player() override;

    void Initialize() override;                     // ������
    void Finalize();                                // �I����

    void Begin();                                   // ���t���[����ԍŏ��ɌĂ΂��
    void Update(const float& elapsedTime) override; // �X�V����
    void End();                                     // ���t���[����ԍŌ�ɌĂ΂��

    void Render(const float& elapsedTime, const float& scale) override; // �`�揈��

    void DrawDebug() override;  // ImGui�p

private:
    bool InputCloseRangeAttack();                           // �ߐڍU������
    void CreateCloseRangeAttackSphere();                    // �ߐڍU���p�̋��̐���
    void UpdateCloseRangeAttack(const float elapsedTime);   // �ߐڍU���X�V

    void CollisionCRASphereVsEnemies();                     // ���̂ƓG�Ƃ̏Փˏ���

private:
    CloseRangeAttackSphere craSphere_ = {}; // �ߐڍU���p�̋���
    float sphereRadius_     = 0.5f;
    float sphereLifeTime_   = 0.5f;

    float offsetY_          = 0.5f;

    ProjectileManager projectileManager_ = {};
    float launchTimer_       = 0.0f;
    float launchTime_        = 1.0f;

    ProjectileManager projectileIconManager_ = {};
    int   weaponCount_ = 0;
};

