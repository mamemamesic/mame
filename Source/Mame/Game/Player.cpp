#include "Player.h"

#include <memory>

#include "../Graphics/Graphics.h"
#include "../Graphics/DebugRenderer.h"
#include "../Input/Input.h"

#include "AbilityManager.h"
#include "EnemyManager.h"
#include "Collision.h"
#include "ProjectileStraite.h"
#include "ProjectileStraiteIcon.h"

// �R���X�g���N�^
Player::Player()
{
    Graphics& graphics = Graphics::Instance();

    // ���f������
    {
        model = std::make_unique<Model>(graphics.GetDevice(),
            "./Resources/Model/sikaku.fbx");
            //"./Resources/Model/sotai.fbx");
            //"./Resources/Model/sanaModel/mameoall.fbx");
            //"./Resources/Model/testModel/nico.fbx");
    }

}

// �f�X�g���N�^
Player::~Player()
{
}

// ������
void Player::Initialize()
{
    GetTransform()->SetPosition(DirectX::XMFLOAT3(0, 0.25f, 0));
    debugSqhereOffset.y += offsetY_;

    Character::Initialize();

    // �ҋ@�A�j���[�V�����ɐݒ肵�Ă�
    Character::PlayAnimation(0, true);

    projectileManager_.Initialize();
    projectileIconManager_.Initialize();

    new ProjectileStraiteIcon(&projectileIconManager_);
}

// �I����
void Player::Finalize()
{
    projectileManager_.Finalize();
    projectileIconManager_.Finalize();
}

// Update�̑O�ɌĂ΂��
void Player::Begin()
{
}

// �X�V����
void Player::Update(const float& elapsedTime)
{
    using DirectX::XMFLOAT3;
    using DirectX::XMFLOAT4;
    using DirectX::XMConvertToRadians;

    Transform* transform = GetTransform();

    Character::Update(elapsedTime); // �L�����N�^�[���ʂ̍X�V����

    Character::UpdateAnimation(elapsedTime); // �A�j���[�V�����X�V

    // �ړ�
    const GamePad& gamePad = Input::Instance().GetGamePad();
    {
        const float aLx = gamePad.GetAxisLX();
        //const float aLy = gamePad.GetAxisLY();

        XMFLOAT3 pos = transform->GetPosition();
        constexpr float addPos = 3.0f;
        if (aLx != 0.0f) pos.x += ((aLx * addPos) * elapsedTime);
        //if (aLy != 0.0f) pos.z += ((aLy * addPos) * elapsedTime);

        constexpr float posLimitX = 1.875f;
        if      (pos.x < -posLimitX) pos.x = -posLimitX;
        else if (pos.x >  posLimitX) pos.x =  posLimitX;

        // �ʒu�X�V
        transform->SetPosition(pos);
    }

    // ��]�i�m�F�p�j
    {
        const float aRx = gamePad.GetAxisRX();
        const float aRy = gamePad.GetAxisRY();

        XMFLOAT4 rotation = transform->GetRotation();
        constexpr float addRotate = 90.0f;

        if (aRx != 0.0f)
        {
            const float addRotateY = XMConvertToRadians(aRx * addRotate);
            rotation.y += (addRotateY * elapsedTime);
        }
        if (aRy != 0.0f)
        {
            const float addRotateX = XMConvertToRadians(aRy * addRotate);
            rotation.x += (addRotateX * elapsedTime);
        }

        // ��]�l�X�V
        transform->SetRotation(rotation);
    }

    //// �ߐڍU�����͏���
    //if (InputCloseRangeAttack() == true) CreateCloseRangeAttackSphere();

    //// �ߐڍU���X�V����
    //UpdateCloseRangeAttack(elapsedTime);

    launchTimer_ -= elapsedTime;
    if (launchTimer_ <= 0.0f)
    {
        const DirectX::XMFLOAT3 position = GetTransform()->GetPosition();
        const DirectX::XMFLOAT3 forward  = GetTransform()->CalcForward();

        const float length = 0.3f;

        const DirectX::XMFLOAT3 spawnPosition = {
            position.x + (forward.x * length),
            position.y + (forward.y * length),
            position.z + (forward.z * length)
        };

        ProjectileStraite* projectile = new ProjectileStraite(&projectileManager_);
        projectile->Launch(forward, spawnPosition);

        launchTimer_ = launchTime_;
    }

    // �e�ۍX�V����
    projectileManager_.Update(elapsedTime);


    // �e�ۃA�C�R���X�V����
    {
        // �ʒu�ݒ�
        int pileUpCounter = 0; // �d�˂������J�E���g����
        int columnCounter = 0; // ��̐����J�E���g����
        const int projectileiconCount = projectileIconManager_.GetProjectileIconCount();
        for (int i = 0; i < projectileiconCount; ++i)
        {
            ProjectileIcon* projectileIcon          = projectileIconManager_.GetProjectileIcon(i);
            Transform*      projectileIconTransform = projectileIcon->GetTransform();

            const XMFLOAT3 plPosition = GetTransform()->GetPosition();
            const float    plTop      = (plPosition.y + 0.4f);

            constexpr float addPositionY = 0.2f;
            constexpr float addPositionX = (-0.1f);

            // ��萔�ς񂾂��𕪂��ĂP����ςݏグ����
            if (pileUpCounter >= projectileIconManager_.PILE_UP_COUNT_MAX_)
            {
                pileUpCounter = 0; // �ςݏグ�J�E���g�����Z�b�g
                ++columnCounter;   // ��J�E���g�𑝂₷
            }

            // Y�ʒu�ݒ�
            projectileIconTransform->SetPositionY(plTop + (static_cast<float>(pileUpCounter) * addPositionY));

            const float columnCount = static_cast<float>(projectileIconManager_.columnCounter_);
            const float shiftLeft = (columnCount * addPositionX);               // ���ׂĂ̗�𓙂������ɂ��炷
            const float shitRight = (static_cast<float>(columnCounter) * 0.1f); // ���ꂼ��̗��񐔂ɔ�Ⴕ�ĉE�ɂ��炷

            // �܂����S�̓I�ɍ��ɂ��炵�Ă���X�̗��񐔂ɔ�Ⴕ�ĉE�ɂ��炷
            projectileIconTransform->SetPositionX(
                (plPosition.x + projectileIcon->offsetX_) + shiftLeft + shitRight
            );

            ++pileUpCounter; // �ςݏグ�J�E���g���Z
        }

        projectileIconManager_.Update(elapsedTime);
    }

    // ��
    if (gamePad.GetButtonDown() & GamePad::BTN_A)
    {
        new ProjectileStraiteIcon(&projectileIconManager_);
    }

}

// Update�̌�ɌĂ΂��
void Player::End()
{
}

// �`�揈��
void Player::Render(const float& elapsedTime, const float& scale)
{
    using DirectX::XMFLOAT4;

    Character::Render(elapsedTime, scale);

    projectileManager_.Render(0.1f);

    projectileIconManager_.Render(0.1f);

    // �ߐڍU���p�̋��̕`��
#ifdef _DEBUG
    if (craSphere_.lifeTimer_ > 0.0f)
    {
        //craSphere_.model_->Render(1.0f, 1);
        DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();
        const XMFLOAT4 color = { 1,1,1,1 };
        debugRenderer->DrawSphere(
            craSphere_.position_, craSphere_.radius_, color
        );
    }
#endif // _DEBUG
}

// ImGui�p
void Player::DrawDebug()
{
#ifdef USE_IMGUI
    if (ImGui::BeginMenu("player"))
    {
        Character::DrawDebug();

        float range = GetRange();
        ImGui::DragFloat("range", &range);
        SetRange(range);

        ImGui::EndMenu();
    }
#endif // USE_IMGUI
}


bool Player::InputCloseRangeAttack()
{
    if (craSphere_.lifeTimer_ > 0.0f) return false;

    const GamePad& gamePad = Input::Instance().GetGamePad();

    if (gamePad.GetButtonDown() & GamePad::BTN_A)
    {
        return true;
    }

    return false;
}

void Player::CreateCloseRangeAttackSphere()
{
    using DirectX::XMFLOAT3;

    const Transform* transform  = GetTransform();
    const XMFLOAT3   plPosition = transform->GetPosition();
    const XMFLOAT3   forward    = transform->CalcForward();

    //const float length = (0.1f + radius);   // ���a�̑傫���ɂ���ċ��������������悤�ɂ���
    const float length = 0.5f;

    // �v���C���[�̑O��ւ�Ɉʒu��ݒ�
    const XMFLOAT3 spherePosition = {
        plPosition.x + (forward.x * length),
        plPosition.y + (forward.y * length) + offsetY_,
        plPosition.z + (forward.z * length),
    };

    // �p�����[�^���
    craSphere_.position_ = spherePosition;
    //craSphere_.model_->GetTransform()->SetScaleFactor(XMFLOAT3(diameter, diameter, diameter));
    craSphere_.radius_ = sphereRadius_;
    craSphere_.lifeTimer_ = sphereLifeTime_;
}

void Player::UpdateCloseRangeAttack(const float elapsedTime)
{
    if (craSphere_.lifeTimer_ <= 0.0f) return;

    craSphere_.lifeTimer_ -= elapsedTime;

    // �ߐڍU���ƓG�Ƃ̏Փˏ���
    CollisionCRASphereVsEnemies();
}

void Player::CollisionCRASphereVsEnemies()
{
    if (craSphere_.lifeTimer_ <= 0.0f) return;

    using DirectX::XMFLOAT3;

    EnemyManager& enemyManager = EnemyManager::Instance();
    const int enemyCount = enemyManager.GetEnemyCount();
    for (int i = 0; i < enemyCount; ++i)
    {
        Enemy* enemy = enemyManager.GetEnemy(i);
        const XMFLOAT3 enemyPosition  = enemy->GetTransform()->GetPosition();
        const float    enemyRadius    = enemy->radius_;

        XMFLOAT3 outPosition = {};
        const bool isIntersect = {
            Collision::IntersectSphereVsSphere(
            craSphere_.position_, craSphere_.radius_,
            enemyPosition, enemyRadius,
            &outPosition
            )
        };

        // �Փ˂����G������
        if (isIntersect == true) enemyManager.Remove(enemy);

    }
}
