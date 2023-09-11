#include "EnemySlime.h"

#include "../Graphics/Graphics.h"
#include "EnemyManager.h"
#include "ProjectileStraiteIcon.h"

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

    new ProjectileStraiteIcon(&projectileIconManager_);

    projectileIconManager_.Initialize();
}

// 終了化
void EnemySlime::Finalize()
{
    projectileIconManager_.Finalize();
}

// Updateの前に呼ばれる
void EnemySlime::Begin()
{
}

// 更新処理
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

    // 弾丸アイコン更新処理
    {
        // 位置設定
        {
            int   pileUpCounter = 0;       // 重ねた数をカウントする
            int   columnCounter = 0;       // 列の数をカウントする
            float shiftLeft = 0.0f;    // すべての列を等しく左にずらす
            float shiftRight = 0.0f;    // それぞれの列を列数に比例して右にずらす

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

                // Y位置設定
                projectileIconTransform->SetPositionY(
                    plTop + (static_cast<float>(pileUpCounter) * addPositionY)
                );

                const float columnCount = static_cast<float>(projectileIconManager_.columnCounter_);

#if 1 // 列がずれるタイミングの違い確認用
                if (projectileIconCount % 5 != 0)
#endif
                {
                    shiftLeft = (columnCount * addPositionX);
                    shiftRight = (static_cast<float>(columnCounter) * 0.1f);

                    projectileIcon->shitLeft_ = shiftLeft;
                    projectileIcon->shitRight_ = shiftRight;
                }

                // 列を全体的に左にずらしてから個々の列を列数に比例して右にずらしていく
                projectileIconTransform->SetPositionX(
                    plPosition.x + projectileIcon->offsetX_ +
                    projectileIcon->shitLeft_ + projectileIcon->shitRight_
                );

                ++pileUpCounter; // 積み上げカウント加算

                // 一定数積んだら列を分けて１から積み上げ直す
                if (pileUpCounter >= projectileIconManager_.PILE_UP_COUNT_MAX_)
                {
                    pileUpCounter = 0;    // 積み上げカウントをリセット
                    ++columnCounter;        // 列カウントを増やす
                }

            }
        }

        projectileIconManager_.Update(elapsedTime);
    }
}

// Updateの後に呼ばれる
void EnemySlime::End()
{
}

// 描画処理
void EnemySlime::Render(const float& elapsedTime, const float& scale)
{
    Enemy::Render(elapsedTime, scale);

    projectileIconManager_.Render(0.1f);

}

// ImGui用
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
