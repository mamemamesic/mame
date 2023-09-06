#pragma once

#include <memory>

#include "Character.h"
#include "ProjectileManager.h"
#include "ProjectileIconManager.h"

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
        DirectX::XMFLOAT3 position_ = {};
        float radius_    = 0.0f;
        float lifeTimer_ = 0.0f;
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

private:
    bool InputCloseRangeAttack();                           // 近接攻撃入力
    void CreateCloseRangeAttackSphere();                    // 近接攻撃用の球体生成
    void UpdateCloseRangeAttack(const float elapsedTime);   // 近接攻撃更新

    void CollisionCRASphereVsEnemies();                     // 球体と敵との衝突処理

private:
    CloseRangeAttackSphere craSphere_ = {}; // 近接攻撃用の球体
    float sphereRadius_     = 0.5f;
    float sphereLifeTime_   = 0.5f;

    float offsetY_          = 0.5f;

    ProjectileManager projectileManager_ = {};
    float launchTimer_       = 0.0f;
    float launchTime_        = 1.0f;

    ProjectileManager projectileIconManager_ = {};
    int   weaponCount_ = 0;
};

