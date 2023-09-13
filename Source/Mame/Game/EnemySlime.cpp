#include "EnemySlime.h"
#include "EnemyManager.h"
#include "../Graphics/Graphics.h"

#include <random>
//乱数　変数
constexpr int Int_MIN = 0;
constexpr float Int_MAX = 3;

// コンストラクタ
EnemySlime::EnemySlime(EnemyManager* manager, DirectX::XMFLOAT3 enemy_set,int count)
    :Enemy(manager)
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

    std::random_device rd;                                      //たまにspeedが速い敵が出現する
    std::default_random_engine eng(rd());                       //
    std::uniform_real_distribution<> distr(Int_MIN, Int_MAX);   //
    random = distr(eng);                                        //
    if (random == 0) speed.z = -0.04f;                          //

    if (enemy_count != 6 && random == 1) flg = true;            //横に移動しながら前にくる敵
    

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
    //GetTransform()->SetPosition(DirectX::XMFLOAT3(0, 0, 20));
   
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

    
    //雑魚敵の設定
    switch (state) {
    case 0:
        speed.z = this->speed.z;
        if (flg)speed.x = 0.01f;
        state++;
        break;
    case 1:
        pos.z += speed.z;

        if (enemy_count == 6)   //ボスの設定に移動(z固定x往復)
        {
            if (pos.z <= 5)
            {
                speed.z = 0;
                state++;
                break;
            }
        }
        pos.x += speed.x;
        if(pos.x > 2)state = 3;
        if (pos.x < -2)state = 2;
        break;
    case 2: //ボスの設定
        speed.x = 0.01;
        if (flg) { state = 1; break; }
        state = 4;
        break;
    case 3:
        speed.x = -0.01;
        if (flg) { state = 1; break; }
        state = 4;
        break;
    case 4:
        pos.x += speed.x;
        if (pos.x > 2)state = 3;
        if (pos.x < -2)state = 2;

        break;
    }

    if (pos.z <= -10)
    {
        Destroy();  //破棄
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
        ImGui::DragFloat("speed_x", &speed.x);
        ImGui::DragFloat("speed_y", &speed.y);
        ImGui::DragFloat("speed_z", &speed.z);
        ImGui::DragFloat("pos_x", &pos_1.x);
        ImGui::DragFloat("pos_y", &pos_1.y);
        ImGui::DragFloat("pos_z", &pos_1.z);
        ImGui::InputInt("random", &random);
        SetRange(range);

        ImGui::EndMenu();
    }
#endif // USE_IMGUI
}
