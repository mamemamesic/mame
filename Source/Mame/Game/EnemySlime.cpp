#include "EnemySlime.h"
#include "../Graphics/Graphics.h"

// コンストラクタ
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

    // ImGui名前設定
    SetName("EnemySlime" + std::to_string(nameNum++));
}

// デストラクタ
EnemySlime::~EnemySlime()
{
}

// 初期化
void EnemySlime::Initialize()
{
    debugSqhereOffset.y += offsetY_;
    GetTransform()->SetPosition(DirectX::XMFLOAT3(0, 0, 20));

    Enemy::Initialize();

    Character::PlayAnimation(0, true);
}

// 終了化
void EnemySlime::Finalize()
{
}

// Updateの前に呼ばれる
void EnemySlime::Begin()
{
}

// 更新処理
void EnemySlime::Update(const float& elapsedTime)
{
    Enemy::Update(elapsedTime);

    Character::UpdateAnimation(elapsedTime);

    //DirectX::XMFLOAT3 player_pos = PlayerManager::Instance().GetPlayer()->GetTransform()->GetPosition();
    DirectX::XMFLOAT3 pos = GetTransform()->GetPosition();

    /*DirectX::XMFLOAT3 vecPlayer = { player_pos.x - pos.x,player_pos.y - pos.y,player_pos.z - pos.z };
    dist = sqrtf(vecPlayer.x * vecPlayer.x + vecPlayer.y * vecPlayer.y + vecPlayer.z * vecPlayer.z);
    pos_1 = vecPlayer;*/

    time++;
    float elapsedFream = 60.0f * elapsedTime;
    //雑魚敵の設定
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
        if (enemy_count == 6)   //ボスの設定に移動
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
    case 2: //ボスの設定
        speed.x = 0.01;
        state = 4;
        break;
    case 3:
        speed.x = -0.01;
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
}

// Updateの後に呼ばれる
void EnemySlime::End()
{
}

// 描画処理
void EnemySlime::Render(const float& elapsedTime, const float& scale)
{
    Enemy::Render(elapsedTime, scale);
}

// ImGui用
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
