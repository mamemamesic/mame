#include "EnemySlime.h"
#include "../Graphics/Graphics.h"


// �R���X�g���N�^
EnemySlime::EnemySlime()
{
    Graphics& graphics = Graphics::Instance();
    
    model = std::make_unique<Model>(graphics.GetDevice(),
        //"./Resources/Model/sanaModel/mameoall.fbx");
        "./Resources/Model/testModel/plantune.fbx");
        //"./Resources/Model/testModel/latha.fbx");
        //"./Resources/Model/Collision/sphere.fbx");

    DirectX::XMFLOAT3 pos = GetTransform()->GetPosition();
    pos = { 4,0,10 };


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
    Enemy::Initialize();

    Character::PlayAnimation(0, true);
}

// �I����
void EnemySlime::Finalize()
{
}

// Update�̑O�ɌĂ΂��
void EnemySlime::Begin()
{
}

// �X�V����
void EnemySlime::Update(const float& elapsedTime)
{
    Enemy::Update(elapsedTime);

    Character::UpdateAnimation(elapsedTime);

    DirectX::XMFLOAT3 player_pos = PlayerManager::Instance().GetPlayer()->GetTransform()->GetPosition();
    DirectX::XMFLOAT3 pos = GetTransform()->GetPosition();
    
    DirectX::XMFLOAT3 vecPlayer = { player_pos.x - pos.x,player_pos.y - pos.y,player_pos.z - pos.z };
    dist = sqrtf(vecPlayer.x * vecPlayer.x + vecPlayer.y * vecPlayer.y + vecPlayer.z * vecPlayer.z);
    pos_1 = vecPlayer;
    
    
    //time += elapsedTime;
    switch (flg) {
    case false:
        speed.x = elapsedTime * 0.01f * vecPlayer.x / dist;
        speed.y = elapsedTime * 0.01f * vecPlayer.y / dist;
        speed.z = elapsedTime * 0.01f * vecPlayer.z / dist;
        flg=true;
    case true:
     /*   if (time > 5) {
            flg = false;
            time = 0;
            break;
        }*/
        pos.x += speed.x;
        pos.y += speed.y;
        pos.z += speed.z;
        break;
    }

    PlayerManager::Instance().GetPlayer()->GetTransform()->SetPosition(player_pos);
    GetTransform()->SetPosition(pos);



}

// Update�̌�ɌĂ΂��
void EnemySlime::End()
{
}

// �`�揈��
void EnemySlime::Render(const float& elapsedTime, const float& scale)
{
    Enemy::Render(elapsedTime, scale);
}

// ImGui�p
void EnemySlime::DrawDebug()
{
#ifdef USE_IMGUI
    if (ImGui::BeginMenu(GetName()))
    {
        Character::DrawDebug();

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
