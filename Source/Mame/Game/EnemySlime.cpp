#include "EnemySlime.h"

#include <random>

#include "../Graphics/Graphics.h"
#include "EnemyManager.h"
#include "EnemyProjectileStraiteIcon.h"
#include "PlayerManager.h"
#include "Collision.h"

// コンストラクタ
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
    using DirectX::XMConvertToRadians;

    if (this->hp_ <= 0)
    {
        // 爆発方向設定
        if (false == projectileIconManager_.isSetBombDirection_)
        {
            constexpr float randMin = -1.0f;
            constexpr float randMax = 1.0f;
            std::random_device rd = {};
            std::default_random_engine eng(rd());
            std::uniform_real_distribution<> distr(randMin, randMax);

            float directionX = static_cast<float>(distr(eng));
            float directionZ = static_cast<float>(distr(eng));

            // 敵の爆発方向設定
            {
                this->bombDirection_ = { directionX, 0.5f, directionZ };
            }

            // 弾丸アイコンの爆発方向設定
            {
                const int projectileIconCount = projectileIconManager_.GetProjectileIconCount();
                for (int i = 0; i < projectileIconCount; ++i)
                {
                    const int projectileIconRenderLimit = projectileIconManager_.projectileIconRenderLimit_;
                    if (i >= projectileIconRenderLimit) break;

                    ProjectileIcon* projectileIcon = projectileIconManager_.GetProjectileIcon(i);

                    directionX = static_cast<float>(distr(eng));
                    directionZ = static_cast<float>(distr(eng));
                    projectileIcon->bombDirection_ = { directionX, 0.5f, directionZ };

                    // 存在する弾丸を消去
                    projectileIcon->projectileManager_.Clear();
                }
            }

            projectileIconManager_.isSetBombDirection_ = true;
        }

        // 爆発方向に飛ばす
        {
            constexpr float bombSpeed = 11.0f;
            XMFLOAT3 bomb = {
                bombSpeed * this->bombDirection_.x * elapsedTime,
                bombSpeed * this->bombDirection_.y * elapsedTime,
                bombSpeed * this->bombDirection_.z * elapsedTime,
            };
            this->GetTransform()->AddPosition(bomb);

            constexpr float addRotate = DirectX::XMConvertToRadians(180.0f);
            this->GetTransform()->AddRotationX(addRotate * elapsedTime);
            this->GetTransform()->AddRotationY(addRotate * elapsedTime);
            this->GetTransform()->AddRotationZ(addRotate * elapsedTime);

            const int projectileIconCount = projectileIconManager_.GetProjectileIconCount();
            for (int i = 0; i < projectileIconCount; ++i)
            {
                const int projectileIconRenderLimit = projectileIconManager_.projectileIconRenderLimit_;
                if (i >= projectileIconRenderLimit) break;

                ProjectileIcon* projectileIcon = projectileIconManager_.GetProjectileIcon(i);
                Transform* projIconTransform = projectileIcon->GetTransform();

                bomb = {
                    bombSpeed * projectileIcon->bombDirection_.x * elapsedTime,
                    bombSpeed * projectileIcon->bombDirection_.y * elapsedTime,
                    bombSpeed * projectileIcon->bombDirection_.z * elapsedTime,
                };
                projIconTransform->AddPosition(bomb);
            }
        }

        this->deathTimer_ += elapsedTime;
        if (deathTimer_ >= 4.0f)
        {
            EnemyManager::Instance().Remove(this);
            return;
        }

        return;
    }


    Enemy::Update(elapsedTime);

    Character::UpdateAnimation(elapsedTime);

    // 移動
    {
        XMFLOAT3 pos = GetTransform()->GetPosition();

        pos.z += speed.z * elapsedTime;

        GetTransform()->SetPosition(pos);
    }

    // 回転(Y軸固定)
    GetTransform()->SetRotationY(XMConvertToRadians(180.0f));


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

                const XMFLOAT3& parentPosition = GetTransform()->GetPosition();
                const float     parentTop = (parentPosition.y + 0.4f);

                constexpr float addPositionY = 0.2f;
                constexpr float addPositionX = (-0.1f);

                // Y位置設定
                projectileIconTransform->SetPositionY(
                    parentTop + (static_cast<float>(pileUpCounter) * addPositionY)
                );

                const float columnCount = static_cast<float>(projectileIconManager_.columnCounter_);

#if 0 // 列がずれるタイミングの違い確認用
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
                    parentPosition.x + projectileIcon->offsetX_ +
                    projectileIcon->shitLeft_ + projectileIcon->shitRight_
                );

                // Z
                projectileIconTransform->SetPositionZ(parentPosition.z);

                ++pileUpCounter; // 積み上げカウント加算

                // 一定数積んだら列を分けて１から積み上げ直す
                if (pileUpCounter >= projectileIconManager_.PILE_UP_COUNT_MAX_)
                {
                    pileUpCounter = 0;    // 積み上げカウントをリセット
                    ++columnCounter;        // 列カウントを増やす
                }

            }
        }

        const XMFLOAT3& position = GetTransform()->GetPosition();
        const XMFLOAT3& forward = GetTransform()->CalcForward();
        projectileIconManager_.projStraitePosition_ = position;
        projectileIconManager_.projStraiteForward_ = forward;
        projectileIconManager_.Update(elapsedTime);
    }


    CollisionEnemyVsPlayer();

    if (model->color.w < 1.0f)
    {
        model->color.w += (1.75f * elapsedTime);
        if (model->color.w > 1.0f) model->color.w = 1.0f;
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

void EnemySlime::CollisionEnemyVsPlayer()
{
    if (this->hp_ <= 0) return;

    const std::unique_ptr<Player>& player = PlayerManager::Instance().GetPlayer();
    if (player->hp_ <= 0) return;
    if (player->invincibleTimer_ > 0.0f) return;

    using DirectX::XMFLOAT3;

    const XMFLOAT3& enmPos = this->GetTransform()->GetPosition();
    const XMFLOAT3& plPos  = player->GetTransform()->GetPosition();

    XMFLOAT3 outPosition = {};
    if (Collision::IntersectSphereVsSphere(
        enmPos, this->radius_,
        plPos, player->radius_, &outPosition))
    {
        --player->hp_;
        player->invincibleTimer_ = player->setInvincibleTime_;

        --this->hp_;

        return;
    }

}

