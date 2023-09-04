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
    Transform* transform = GetTransform();

    Character::Update(elapsedTime); // キャラクター共通の更新処理

    Character::UpdateAnimation(elapsedTime); // アニメーション更新

    const GamePad& gamePad = Input::Instance().GetGamePad();

    // 移動
    {
        const float aLx = gamePad.GetAxisLX();
        const float aLy = gamePad.GetAxisLY();

        DirectX::XMFLOAT3 pos = transform->GetPosition();
        if (aLx > 0.0f) pos.x += elapsedTime;
        if (aLx < 0.0f) pos.x -= elapsedTime;
        if (aLy > 0.0f) pos.z += elapsedTime;
        if (aLy < 0.0f) pos.z -= elapsedTime;

        // 位置更新
        transform->SetPosition(pos);
    }

    // 近接攻撃処理
    if (InputCloseRangeAttack()) CloseRangeAttack();

}

// Updateの後に呼ばれる
void Player::End()
{
}

// 描画処理
void Player::Render(const float& elapsedTime, const float& scale)
{
    Character::Render(elapsedTime, scale);
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

    if (gamePad.GetButtonDown() & GamePad::BTN_START)
    {
        return true;
    }

    return false;
}

void Player::CloseRangeAttack()
{
    using DirectX::XMFLOAT3;

    const Transform* transform = GetTransform();
    const XMFLOAT3   forward   = transform->CalcForward();

    // プレイヤーの前らへんに位置を設定
    XMFLOAT3 position = forward;
    position.z *= 1.5f;

    const float radius = 0.5f;      // 半径
    const float lifeTime = 0.1f;    // 生存時間

    CreateCloseRangeAttackSphere(position, radius, lifeTime); // 近接攻撃用の球体生成
}

void Player::CreateCloseRangeAttackSphere(
    const DirectX::XMFLOAT3& pos,
    const float radius,
    const float lifeTime)
{
    craSphere_.position_  = pos;
    craSphere_.radius_    = radius;
    craSphere_.lifeTimer_ = lifeTime;
}

void Player::UpdateCRASphere(const float elapsedTime)
{
    if (craSphere_.lifeTimer_ <= 0.0f) return;

    craSphere_.lifeTimer_ -= elapsedTime;

    //CollisionCRASphereVsEnemies();
}
