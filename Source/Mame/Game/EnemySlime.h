#pragma once
#include "Enemy.h"
#include "PlayerManager.h"


class EnemySlime : public Enemy
{
public:
    EnemySlime() {};
    EnemySlime(DirectX::XMFLOAT3 enemy_set);
    ~EnemySlime() override;

    void Initialize()                                           override; // 初期化
    void Finalize()                                             override; // 終了化
    void Begin()                                                override; // 毎フレーム一番最初に呼ばれる
    void Update(const float& elapsedTime)                       override; // 更新処理
    void End()                                                  override; // 毎フレーム一番最後に呼ばれる
    void Render(const float& elapsedTime, const float& scale)   override; // 描画処理
    void DrawDebug()                                            override; // デバッグ描画

private:
    float offsetY_ = 0.25f;
    float dist;
    DirectX::XMFLOAT3 pos_1;
    bool flg = false;
    DirectX::XMFLOAT3 speed;
    float time;
    int count = 3;
};

