#include "EnemySlime.h"

#include "../Graphics/Graphics.h"
#include "EnemyManager.h"
#include "ProjectileStraiteIcon.h"

// �R���X�g���N�^
EnemySlime::EnemySlime(DirectX::XMFLOAT3 enemy_set,int count)
{
    Graphics& graphics = Graphics::Instance();

    model = std::make_unique<Model>(graphics.GetDevice(),
        //"./Resources/Model/sanaModel/mameoall.fbx");
        "./Resources/Model/testModel/plantune.fbx");
        //"./Resources/Model/testModel/latha.fbx");
        //"./Resources/Model/Collision/sphere.fbx");

    DirectX::XMFLOAT3 pos = GetTransform()->GetPosition();
    pos = enemy_set;

    enemy_count = count;

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
    debugSqhereOffset.y += offsetY_;
    GetTransform()->SetPosition(DirectX::XMFLOAT3(0, 0, 20));

    Enemy::Initialize();

    Character::PlayAnimation(0, true);

    new ProjectileStraiteIcon(&projectileIconManager_);

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

    Enemy::Update(elapsedTime);

    Character::UpdateAnimation(elapsedTime);

    //DirectX::XMFLOAT3 player_pos = PlayerManager::Instance().GetPlayer()->GetTransform()->GetPosition();
    DirectX::XMFLOAT3 pos = GetTransform()->GetPosition();

    /*DirectX::XMFLOAT3 vecPlayer = { player_pos.x - pos.x,player_pos.y - pos.y,player_pos.z - pos.z };
    dist = sqrtf(vecPlayer.x * vecPlayer.x + vecPlayer.y * vecPlayer.y + vecPlayer.z * vecPlayer.z);
    pos_1 = vecPlayer;*/

    time++;
    float elapsedFrame = 60.0f * elapsedTime;
    //�G���G�̐ݒ�
    switch (state) {
    case 0:
        /*speed.x = elapsedTime * 0.01f * vecPlayer.x / dist;
        speed.y = elapsedTime * 0.01f * vecPlayer.y / dist;
        speed.z = elapsedTime * 0.01f * vecPlayer.z / dist;*/
        //speed.z = -0.001f*elapsedFream;
        speed.z = this->speed.z * elapsedTime;
        state++;
        break;
    case 1:
        pos.z += speed.z;
        if (enemy_count == 6)   //�{�X�̐ݒ�Ɉړ�
        {
            if (pos.z <= 5)
            {
                speed.z = 0;
                pos.z += speed.z;
                state++;
                break;
            }
        }
        break;
    case 2: //�{�X�̐ݒ�
        speed.x = 0.01f;
        state = 4;
        break;
    case 3:
        speed.x = -0.01f;
        state = 4;
        break;
    case 4:
        pos.x += speed.x;
        if (pos.x > 4)state = 3;
        if (pos.x < -4)state = 2;

        break;
    }

    //PlayerManager::Instance().GetPlayer()->GetTransform()->SetPosition(player_pos);
    GetTransform()->SetPosition(pos);

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

                const XMFLOAT3 plPosition = GetTransform()->GetPosition();
                const float    plTop = (plPosition.y + 0.4f);

                constexpr float addPositionY = 0.2f;
                constexpr float addPositionX = (-0.1f);

                // Y�ʒu�ݒ�
                projectileIconTransform->SetPositionY(
                    plTop + (static_cast<float>(pileUpCounter) * addPositionY)
                );

                const float columnCount = static_cast<float>(projectileIconManager_.columnCounter_);

#if 1 // �񂪂����^�C�~���O�̈Ⴂ�m�F�p
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
                    plPosition.x + projectileIcon->offsetX_ +
                    projectileIcon->shitLeft_ + projectileIcon->shitRight_
                );

                ++pileUpCounter; // �ςݏグ�J�E���g���Z

                // ��萔�ς񂾂��𕪂��ĂP����ςݏグ����
                if (pileUpCounter >= projectileIconManager_.PILE_UP_COUNT_MAX_)
                {
                    pileUpCounter = 0;    // �ςݏグ�J�E���g�����Z�b�g
                    ++columnCounter;        // ��J�E���g�𑝂₷
                }

            }
        }

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
        ImGui::DragFloat("time", &time);
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


void EnemySlime::Death()
{
    //const int projIconCount = projectileIconManager_.GetProjectileIconCount();
    //for (int i = 0; i < projIconCount; ++i)
    //{
    //    projectileIconManager_.Register(new ProjectileIconStraite());
    //}

    //EnemyManager::Instance().Remove(this);
}
