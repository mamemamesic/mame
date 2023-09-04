#include "Player.h"

#include "../Graphics/Graphics.h"
#include "../Input/Input.h"

#include "AbilityManager.h"
#include <memory>

// コンストラクタ
Player::Player()
{
    Graphics& graphics = Graphics::Instance();

    // モデル生成
    {
        model = std::make_unique<Model>(graphics.GetDevice(),
            "./Resources/Model/sotai.fbx");
            //"./Resources/Model/sanaModel/mameoall.fbx");
            //"./Resources/Model/testModel/nico.fbx");

        const char* const fbxName = "./Resources/Model/Collision/sqhere.fbx";
        craSphere_.model_ = std::make_unique<Model>(graphics.GetDevice(), "./Resources/Model/Collision/sqhere.fbx");

    }
}

// デストラクタ
Player::~Player()
{
}

// 初期化
void Player::Initialize()
{
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
        const float aLy = gamePad.GetAxisLY();

        XMFLOAT3 pos = transform->GetPosition();
        constexpr float addPos = 3.0f;
        if (aLx != 0.0f) pos.x += ((aLx * addPos) * elapsedTime);
        if (aLy != 0.0f) pos.z += ((aLy * addPos) * elapsedTime);

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

    // 近接攻撃入力処理
    if (InputCloseRangeAttack() == true) CloseRangeAttack();

    // 近接攻撃更新処理
    UpdateCloseRangeAttack(elapsedTime);

}

// Updateの後に呼ばれる
void Player::End()
{
}

// 描画処理
void Player::Render(const float& elapsedTime, const float& scale)
{
    Character::Render(elapsedTime, scale);

    // 近接攻撃用の球体描画
#ifdef _DEBUG
    if (craSphere_.lifeTimer_ > 0.0f)
    {
        const float range = craSphere_.radius_ * 2.0f;
        craSphere_.model_->Render(range, 1);
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

        ImGui::EndMenu();
    }
#endif // USE_IMGUI
}


bool Player::InputCloseRangeAttack()
{
    const GamePad& gamePad = Input::Instance().GetGamePad();

    if (gamePad.GetButtonDown() & GamePad::BTN_A)
    {
        return true;
    }

    return false;
}

void Player::CloseRangeAttack()
{
    using DirectX::XMFLOAT3;

    const Transform* transform  = GetTransform();
    const XMFLOAT3   plPosition = transform->GetPosition();
    const XMFLOAT3   forward    = transform->CalcForward();

    const float radius = 0.25f;             // 半径
    const float length = (0.1f + radius);   // 半径の大きさによって距離が調整されるようにする

    // プレイヤーの前らへんに位置を設定
    const XMFLOAT3 spherePosition = {
        plPosition.x + (forward.x * length),
        plPosition.y + (forward.y * length) + offsetY,
        plPosition.z + (forward.z * length),
    };

    const float lifeTime = 0.5f;    // 生存時間

    // 近接攻撃用の球体生成
    CreateCloseRangeAttackSphere(spherePosition, radius, lifeTime);
}

void Player::CreateCloseRangeAttackSphere(
    const DirectX::XMFLOAT3& pos,
    const float radius,
    const float lifeTime)
{
    craSphere_.model_->GetTransform()->SetPosition(pos);
    craSphere_.radius_    = radius;
    craSphere_.lifeTimer_ = lifeTime;
}

void Player::UpdateCloseRangeAttack(const float elapsedTime)
{
    if (craSphere_.lifeTimer_ <= 0.0f) return;

    craSphere_.lifeTimer_ -= elapsedTime;

    // 球体と敵との衝突処理
    //CollisionCRASphereVsEnemies();
}
