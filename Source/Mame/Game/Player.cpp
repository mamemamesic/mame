#include "Player.h"

#include <memory>

#include "../Graphics/Graphics.h"
#include "../Graphics/DebugRenderer.h"
#include "../Input/Input.h"

#include "AbilityManager.h"
#include "EnemyManager.h"
#include "Collision.h"
#include "ProjectileStraite.h"

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
}

// 終了化
void Player::Finalize()
{
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

    // 弾丸更新処理
    projectileManager_.Update(elapsedTime);

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

    projectileManager_.Render(0.1f);

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
