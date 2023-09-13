#include "Player.h"

#include <memory>
#include <random>

#include "../Graphics/Graphics.h"
#include "../Graphics/DebugRenderer.h"
#include "../Input/Input.h"

#include "AbilityManager.h"
#include "EnemyManager.h"

#include "ProjectileStraiteIcon.h"
#include "EnemyProjectileStraiteIcon.h"
#include "ProjectileStraite.h"

#include "Collision.h"


// コンストラクタ
Player::Player()
{
    Graphics& graphics = Graphics::Instance();

    // モデル生成
    {
        model = std::make_unique<Model>(graphics.GetDevice(),
            "./Resources/Model/sikaku.fbx");
            //"./Resources/Model/sotai.fbx");
            //"./Resources/Model/sanaModel/mameoall.fbx");
            //"./Resources/Model/testModel/nico.fbx");

        shotRay_ = std::make_unique<Model>(graphics.GetDevice(),
            "./Resources/Model/sikaku.fbx");
    }

}

// デストラクタ
Player::~Player()
{
}

// 初期化
void Player::Initialize()
{
    Transform* transform = GetTransform();

    transform->SetPosition(DirectX::XMFLOAT3(0, 0.25f, 0));
    debugSqhereOffset.y += offsetY_;
    //model->color = DirectX::XMFLOAT4(0.5f, 0.5f, 1.0f, 1.0f);

    shotRay_->GetTransform()->SetScale(DirectX::XMFLOAT3(0.03f, 0.03f, 80.0f));
    shotRay_->color = { 1.0f, 0.3f, 0.0f, 0.8f };

    Character::Initialize();

    // 待機アニメーションに設定してる
    Character::PlayAnimation(0, true);

    new ProjectileStraiteIcon(&projectileIconManager_);

    projectileIconManager_.Initialize();
}

// 終了化
void Player::Finalize()
{
    projectileIconManager_.Finalize();
}

// Updateの前に呼ばれる
void Player::Begin()
{
}

// 更新処理
void Player::Update(const float& elapsedTime)
{
    using DirectX::XMFLOAT3;
    using DirectX::XMFLOAT4;
    using DirectX::XMConvertToRadians;

    Transform* transform = GetTransform();
    const GamePad& gamePad = Input::Instance().GetGamePad();

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

            // プレイヤーの爆発方向設定
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

        projectileIconManager_.Update(elapsedTime);

        return;
    }

    Character::Update(elapsedTime); // キャラクター共通の更新処理

    Character::UpdateAnimation(elapsedTime); // アニメーション更新

    // 射線位置更新
    {
        shotRay_->GetTransform()->SetPositionX(transform->GetPosition().x + 0.0075f);
        shotRay_->GetTransform()->SetPositionY(transform->GetPosition().y - 0.25f);
        shotRay_->GetTransform()->SetPositionZ(transform->GetPosition().z + 20.0f);
    }

    // 移動
    {
        const float aLx = gamePad.GetAxisLX();
        //const float aLy = gamePad.GetAxisLY();

        XMFLOAT3 pos = transform->GetPosition();
        constexpr float addPos = 3.0f;
        if (aLx != 0.0f) pos.x += ((aLx * addPos) * elapsedTime);
        //if (aLy != 0.0f) pos.z += ((aLy * addPos) * elapsedTime);

        constexpr float posLimitX = 1.875f;
        if (pos.x < -posLimitX) pos.x = -posLimitX;
        else if (pos.x > posLimitX) pos.x = posLimitX;

        // 位置更新
        transform->SetPosition(pos);
    }

    // 回転（確認用）
#if 0
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

        // 回転値更新
        transform->SetRotation(rotation);
    }
#endif

    {
        //// 近接攻撃入力処理
        //if (InputCloseRangeAttack() == true) CreateCloseRangeAttackSphere();

        //// 近接攻撃更新処理
        //UpdateCloseRangeAttack(elapsedTime);
    }

    // 弾丸アイコン更新処理
    {
#ifdef _DEBUG
        // 弾丸アイコン追加
        if (gamePad.GetButtonDown() & GamePad::BTN_A)
        {
            //EnemyManager& enemyManager = EnemyManager::Instance();
            //new EnemyProjectileStraiteIcon(&enemyManager.GetEnemy(0)->projectileIconManager_);

            new ProjectileStraiteIcon(&projectileIconManager_);
        }
#endif

        // 位置設定
        {
            int   pileUpCounter = 0;       // 重ねた数をカウントする
            int   columnCounter = 0;       // 列の数をカウントする
            float shiftLeft = 0.0f;    // すべての列を等しく左にずらす
            float shiftRight = 0.0f;    // それぞれの列を列数に比例して右にずらす

            const float moveSpeedX = 3.0f;
            const float moveSpeedY = 2.75f;
            const float moveSpeedZ = 30.0f;

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
                Transform* projIconTransform = projectileIcon->GetTransform();

                const XMFLOAT3 plPosition = GetTransform()->GetPosition();
                const float    plTop = (plPosition.y + 0.4f);

                constexpr float addPositionY = 0.2f;
                constexpr float addPositionX = (-0.1f);

                // Move Y
                {
                    const float targetPosY = {
                        plTop + (static_cast<float>(pileUpCounter) * addPositionY)
                    };
                    if (projIconTransform->GetPosition().y > targetPosY)
                    {
                        projIconTransform->AddPositionY(-moveSpeedY * elapsedTime);
                        if (projIconTransform->GetPosition().y < targetPosY)
                        {
                            projIconTransform->SetPositionY(targetPosY);
                        }
                    }
                    if (projIconTransform->GetPosition().y < targetPosY)
                    {
                        projIconTransform->AddPositionY(moveSpeedY * elapsedTime);
                        if (projIconTransform->GetPosition().y > targetPosY)
                        {
                            projIconTransform->SetPositionY(targetPosY);
                        }
                    }
                }

                const float columnCount = static_cast<float>(projectileIconManager_.columnCounter_);

#if 0 // 列がずれるタイミングの違い確認用
                if (projectileIconCount % 5 != 0)
#endif
                {
                    // 列を全体的に左にずらしてから個々の列を列数に比例して右にずらしていく
                    shiftLeft = (columnCount * addPositionX);
                    shiftRight = (static_cast<float>(columnCounter) * 0.1f);

                    projectileIcon->shitLeft_ = shiftLeft;
                    projectileIcon->shitRight_ = shiftRight;
                }

                // Move X
                {
                    const float targetPosX = {
                        plPosition.x + projectileIcon->offsetX_ +
                        projectileIcon->shitLeft_ + projectileIcon->shitRight_
                    };
                    if (projIconTransform->GetPosition().x > targetPosX)
                    {
                        projIconTransform->AddPositionX(-moveSpeedX * elapsedTime);
                        if (projIconTransform->GetPosition().x < targetPosX)
                        {
                            projIconTransform->SetPositionX(targetPosX);
                        }
                    }
                    if (projIconTransform->GetPosition().x < targetPosX)
                    {
                        projIconTransform->AddPositionX(moveSpeedX * elapsedTime);
                        if (projIconTransform->GetPosition().x > targetPosX)
                        {
                            projIconTransform->SetPositionX(targetPosX);
                        }
                    }
                }

                // Move Z
                {
                    const float targetPosZ = plPosition.z;
                    if (projIconTransform->GetPosition().z > targetPosZ)
                    {
                        projIconTransform->AddPositionZ(-moveSpeedZ * elapsedTime);
                        if (projIconTransform->GetPosition().z < targetPosZ)
                        {
                            projIconTransform->SetPositionZ(targetPosZ);
                        }
                    }
                    if (projIconTransform->GetPosition().z < targetPosZ)
                    {
                        projIconTransform->AddPositionZ(moveSpeedZ * elapsedTime);
                        if (projIconTransform->GetPosition().z > targetPosZ)
                        {
                            projIconTransform->SetPositionZ(targetPosZ);
                        }
                    }
                }

                ++pileUpCounter; // 積み上げカウント加算

                // 一定数積んだら列を分けて１から積み上げ直す
                if (pileUpCounter >= projectileIconManager_.PILE_UP_COUNT_MAX_)
                {
                    pileUpCounter = 0;      // 積み上げカウントをリセット
                    ++columnCounter;        // 列カウントを増やす
                }

            }
        }

        projectileIconManager_.Update(elapsedTime);
    }


    // 残り体力によって色を赤くしていく
    {
        const float floatHp = static_cast<float>(hp_);
        const float floatMaxHp = static_cast<float>(maxHp_);
        const float hpPercentage = (floatHp / floatMaxHp);
        const float setColorR = (hp_ == 1) ? 1.0f : (1.25f - hpPercentage);
        const float setColorG = (hp_ == 1) ? 0.0f : (1.25f - hpPercentage);
        const float setColorB = (hp_ == 1) ? 0.0f : (hpPercentage);
        model->color.x = setColorR;
        model->color.y = setColorG;
        model->color.z = setColorB;
    }

    if (invincibleTimer_ > 0.0f)
    {
        const int timer = static_cast<int>(invincibleTimer_ * 100.0f);
        model->color.w = (timer & 0x08) ? 0.4f : 0.95f;

        invincibleTimer_ -= elapsedTime; // 無敵時間減少
    }
    else
    {
        model->color.w = 0.95f;
    }

}

// Updateの後に呼ばれる
void Player::End()
{
}

// 描画処理
void Player::Render(const float& elapsedTime, const float& scale)
{
    using DirectX::XMFLOAT4;

    Character::Render(elapsedTime, scale);

    if (this->hp_ > 0) shotRay_->Render(scale);

    projectileIconManager_.Render(0.1f);

    // 近接攻撃用の球体描画
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

// ImGui用
void Player::DrawDebug()
{
#ifdef USE_IMGUI
    if (ImGui::BeginMenu("player"))
    {
        Character::DrawDebug();

        if (ImGui::TreeNode("shotRay_"))
        {
            shotRay_->DrawDebug();
            ImGui::TreePop();
        }

        //projectileIconManager_.DrawDebug();

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

    //const float length = (0.1f + radius);   // 半径の大きさによって距離が調整されるようにする
    const float length = 0.5f;

    // プレイヤーの前らへんに位置を設定
    const XMFLOAT3 spherePosition = {
        plPosition.x + (forward.x * length),
        plPosition.y + (forward.y * length) + offsetY_,
        plPosition.z + (forward.z * length),
    };

    // パラメータ代入
    craSphere_.position_ = spherePosition;
    //craSphere_.model_->GetTransform()->SetScaleFactor(XMFLOAT3(diameter, diameter, diameter));
    craSphere_.radius_ = sphereRadius_;
    craSphere_.lifeTimer_ = sphereLifeTime_;
}

void Player::UpdateCloseRangeAttack(const float elapsedTime)
{
    if (craSphere_.lifeTimer_ <= 0.0f) return;

    craSphere_.lifeTimer_ -= elapsedTime;

    // 近接攻撃と敵との衝突処理
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

        // 衝突した敵を消す
        if (isIntersect == true) enemyManager.Remove(enemy);

    }
}
