#pragma once

#include "Character.h"
#include <memory>

class Player : public Character
{
public: // enum関連
    // ステート
    enum class State
    {
        Idle,   // 待機
        Walk,   // 歩行
    };

private: // enum関連

    // アニメーション
    enum class Animation
    {
    };

    // 近接攻撃用の球体構造体
    struct CloseRangeAttackSphere
    {
        std::unique_ptr<Model> model_ = {};  // モデル情報
        float radius_    = 0.0f;             // 半径
        float lifeTimer_ = 0.0f;             // 生存タイマー
    };

public:
    Player();
    ~Player() override;

    void Initialize() override;                     // 初期化
    void Finalize();                                // 終了化

    void Begin();                                   // 毎フレーム一番最初に呼ばれる
    void Update(const float& elapsedTime) override; // 更新処理
    void End();                                     // 毎フレーム一番最後に呼ばれる

    void Render(const float& elapsedTime, const float& scale) override; // 描画処理

    void DrawDebug() override;  // ImGui用

    bool InputCloseRangeAttack();   // 近接攻撃入力
    void CloseRangeAttack();        // 近接攻撃

    // 近接攻撃用の球体生成
    void CreateCloseRangeAttackSphere(
        const DirectX::XMFLOAT3& pos,
        const float radius,
        const float lifeTime
    );

    void UpdateCloseRangeAttack(const float elapsedTime);

private:
    CloseRangeAttackSphere craSphere_ = {}; // 近接攻撃用の球体

    float offsetY = 0.5f;

};

