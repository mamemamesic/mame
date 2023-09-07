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
    }

}

// デストラクタ
Player::~Player()
{
}

// 初期化
void Player::Initialize()
{
    GetTransform()->SetPosition(DirectX::XMFLOAT3(0, 0.25f, 0));
    debugSqhereOffset.y += offsetY_;

    Character::Initialize();

    // 待機アニメーションに設定してる
    Character::PlayAnimation(0, true);

    projectileIconManager_.Initialize();

    new ProjectileStraiteIcon(&projectileIconManager_);
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

    Character::Update(elapsedTime); // キャラクター共通の更新処理

    Character::UpdateAnimation(elapsedTime); // アニメーション更新

    // 移動
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

        // 位置更新
        transform->SetPosition(pos);
    }

    // 回転（確認用）
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

    //// 近接攻撃入力処理
    //if (InputCloseRangeAttack() == true) CreateCloseRangeAttackSphere();

    //// 近接攻撃更新処理
    //UpdateCloseRangeAttack(elapsedTime);

    // 仮
    if (gamePad.GetButtonDown() & GamePad::BTN_A)
    {
        // 弾丸アイコン追加
        new ProjectileStraiteIcon(&projectileIconManager_);
    }

    // 弾丸アイコン更新処理
    {
        // 位置設定
        int   pileUpCounter   = 0;       // 重ねた数をカウントする
        int   columnCounter   = 0;       // 列の数をカウントする
        float shiftLeft       = 0.0f;    // すべての列を等しく左にずらす
        float shitRight       = 0.0f;    // それぞれの列を列数に比例して右にずらす
        bool  isColumnChanged = false;
        const int projectileiconCount = projectileIconManager_.GetProjectileIconCount();
        for (int i = 0; i < projectileiconCount; ++i)
        {
            ProjectileIcon* projectileIcon          = projectileIconManager_.GetProjectileIcon(i);
            Transform*      projectileIconTransform = projectileIcon->GetTransform();

            const XMFLOAT3 plPosition = GetTransform()->GetPosition();
            const float    plTop      = (plPosition.y + 0.4f);

            constexpr float addPositionY = 0.2f;
            constexpr float addPositionX = (-0.1f);

            // Y位置設定
            projectileIconTransform->SetPositionY(plTop + (static_cast<float>(pileUpCounter) * addPositionY));

            const float columnCount         = static_cast<float>(projectileIconManager_.columnCounter_);
            const int   projectileIconCount = projectileIconManager_.GetProjectileIconCount();

#if 1 // 列がずれるタイミングの違い確認用
            if (projectileIconCount % 5 != 0)
#endif
            {
                shiftLeft = (columnCount * addPositionX);
                shitRight = (static_cast<float>(columnCounter) * 0.1f);

                projectileIcon->shitLeft_  = shiftLeft;
                projectileIcon->shitRight_ = shitRight;
            }

            // 列を全体的に左にずらしてから個々の列を列数に比例して右にずらしていく
            projectileIconTransform->SetPositionX(
                plPosition.x + projectileIcon->offsetX_ +
                projectileIcon->shitLeft_ + projectileIcon->shitRight_
            );

            ++pileUpCounter; // 積み上げカウント加算

            // 一定数積んだら列を分けて１から積み上げ直すA
            if (pileUpCounter >= projectileIconManager_.PILE_UP_COUNT_MAX_)
            {
                pileUpCounter   = 0;    // 積み上げカウントをリセット
                ++columnCounter;        // 列カウントを増やす
            }

        }

        projectileIconManager_.Update(elapsedTime);

#ifdef USE_IMGUI
        if (ImGui::Begin("Count"))
        {
            ImGui::InputInt("pileUpCounter", &pileUpCounter);
            ImGui::InputInt("columnCounter", &columnCounter);
            ImGui::InputFloat("shiftLeft", &shiftLeft);
            ImGui::InputFloat("shitRight", &shitRight);
            ImGui::Checkbox("isColumnChanged", &isColumnChanged);

            ImGui::End();
        }
#endif // USE_IMGUI
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

        projectileIconManager_.DrawDebug();

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
