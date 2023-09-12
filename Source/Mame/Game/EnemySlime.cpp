#include "EnemySlime.h"

#include "../Graphics/Graphics.h"
#include "EnemyManager.h"
#include "EnemyProjectileStraiteIcon.h"

// �R���X�g���N�^
EnemySlime::EnemySlime(/*DirectX::XMFLOAT3 enemy_set,int count*/)
{
    Graphics& graphics = Graphics::Instance();

    model = std::make_unique<Model>(graphics.GetDevice(),
        //"./Resources/Model/sanaModel/mameoall.fbx");
        //"./Resources/Model/testModel/plantune.fbx");
        //"./Resources/Model/testModel/latha.fbx");
        //"./Resources/Model/Collision/sphere.fbx");
        "./Resources/Model/sikaku.fbx");

    DirectX::XMFLOAT3 pos = GetTransform()->GetPosition();
    //pos = enemy_set;

    //enemy_count = count;

    GetTransform()->SetPosition(pos);

    // ImGui���O�ݒ�
    SetName("EnemySlime" + std::to_string(nameNum++));
}

// �f�X�g���N�^
EnemySlime::~EnemySlime()
{
}

// ������
void EnemySlime::Initialize()
{
    using DirectX::XMConvertToRadians;

    debugSqhereOffset.y += offsetY_;
    GetTransform()->SetPosition(DirectX::XMFLOAT3(0, 0.25f, 20));
    GetTransform()->SetRotation(DirectX::XMFLOAT4(0, XMConvertToRadians(180.0f), 0, 0));
    model->color = DirectX::XMFLOAT4(1.0f, 0.25f, 0.25f, 1.0f);

    Enemy::Initialize();

    Character::PlayAnimation(0, true);

    //for (int i = 0; i < 19; ++i) new EnemyProjectileStraiteIcon(&projectileIconManager_);
    //new EnemyProjectileStraiteIcon(&projectileIconManager_);

    projectileIconManager_.Initialize();
}

// �I����
void EnemySlime::Finalize()
{
    projectileIconManager_.Finalize();
}

// Update�̑O�ɌĂ΂��
void EnemySlime::Begin()
{
}

// �X�V����
void EnemySlime::Update(const float& elapsedTime)
{
    using DirectX::XMFLOAT3;
    using DirectX::XMConvertToRadians;

    Enemy::Update(elapsedTime);

    Character::UpdateAnimation(elapsedTime);

    // �ړ�
    {
        XMFLOAT3 pos = GetTransform()->GetPosition();

        constexpr float addSpeedZ = (-5.0f);
        pos.z += addSpeedZ * elapsedTime;

        GetTransform()->SetPosition(pos);
    }

    // ��](Y���Œ�)
    GetTransform()->SetRotationY(XMConvertToRadians(180.0f));


    // �e�ۃA�C�R���X�V����
    {
        // �ʒu�ݒ�
        {
            int   pileUpCounter = 0;       // �d�˂������J�E���g����
            int   columnCounter = 0;       // ��̐����J�E���g����
            float shiftLeft = 0.0f;    // ���ׂĂ̗�𓙂������ɂ��炷
            float shiftRight = 0.0f;    // ���ꂼ��̗��񐔂ɔ�Ⴕ�ĉE�ɂ��炷

#ifdef USE_IMGUI
            int projectileIconCount = projectileIconManager_.GetProjectileIconCount();
#else
            const int projectileIconCount = projectileIconManager_.GetProjectileIconCount();
#endif
            for (int i = 0; i < projectileIconCount; ++i)
            {
                const int projectileIconRenderLimit = projectileIconManager_.projectileIconRenderLimit_;
                if (i >= projectileIconRenderLimit) break;

                ProjectileIcon* projectileIcon = projectileIconManager_.GetProjectileIcon(i);
                Transform* projectileIconTransform = projectileIcon->GetTransform();

                const XMFLOAT3& parentPosition = GetTransform()->GetPosition();
                const float     parentTop = (parentPosition.y + 0.4f);

                constexpr float addPositionY = 0.2f;
                constexpr float addPositionX = (-0.1f);

                // Y�ʒu�ݒ�
                projectileIconTransform->SetPositionY(
                    parentTop + (static_cast<float>(pileUpCounter) * addPositionY)
                );

                const float columnCount = static_cast<float>(projectileIconManager_.columnCounter_);

#if 0 // �񂪂����^�C�~���O�̈Ⴂ�m�F�p
                if (projectileIconCount % 5 != 0)
#endif
                {
                    shiftLeft = (columnCount * addPositionX);
                    shiftRight = (static_cast<float>(columnCounter) * 0.1f);

                    projectileIcon->shitLeft_ = shiftLeft;
                    projectileIcon->shitRight_ = shiftRight;
                }

                // ���S�̓I�ɍ��ɂ��炵�Ă���X�̗��񐔂ɔ�Ⴕ�ĉE�ɂ��炵�Ă���
                projectileIconTransform->SetPositionX(
                    parentPosition.x + projectileIcon->offsetX_ +
                    projectileIcon->shitLeft_ + projectileIcon->shitRight_
                );

                // Z
                projectileIconTransform->SetPositionZ(parentPosition.z);

                ++pileUpCounter; // �ςݏグ�J�E���g���Z

                // ��萔�ς񂾂��𕪂��ĂP����ςݏグ����
                if (pileUpCounter >= projectileIconManager_.PILE_UP_COUNT_MAX_)
                {
                    pileUpCounter = 0;    // �ςݏグ�J�E���g�����Z�b�g
                    ++columnCounter;        // ��J�E���g�𑝂₷
                }

            }
        }

        const XMFLOAT3& position = GetTransform()->GetPosition();
        const XMFLOAT3& forward  = GetTransform()->CalcForward();
        projectileIconManager_.projStraitePosition_ = position;
        projectileIconManager_.projStraiteForward_ = forward;
        projectileIconManager_.Update(elapsedTime);
    }
}

// Update�̌�ɌĂ΂��
void EnemySlime::End()
{
}

// �`�揈��
void EnemySlime::Render(const float& elapsedTime, const float& scale)
{
    Enemy::Render(elapsedTime, scale);

    projectileIconManager_.Render(0.1f);

}

// ImGui�p
void EnemySlime::DrawDebug()
{
#ifdef USE_IMGUI
    if (ImGui::BeginMenu(GetName()))
    {
        Character::DrawDebug();

        projectileIconManager_.DrawDebug();

        float range = GetRange();
        ImGui::DragFloat("range", &range);
        ImGui::DragFloat("dist", &dist);
        ImGui::DragFloat("speed_x", &speed.x);
        ImGui::DragFloat("speed_y", &speed.y);
        ImGui::DragFloat("speed_z", &speed.z);
        ImGui::DragFloat("pos_x", &pos_1.x);
        ImGui::DragFloat("pos_y", &pos_1.y);
        ImGui::DragFloat("pos_z", &pos_1.z);
        SetRange(range);

        ImGui::EndMenu();
    }
#endif // USE_IMGUI
}

