#pragma once
#include "Enemy.h"

#include "PlayerManager.h"

class EnemySlime : public Enemy
{
public:
    EnemySlime();
    ~EnemySlime() override;

    void Initialize()                                           override; // 初期化
    void Finalize()                                             override; // 終了化
    void Begin()                                                override; // 毎フレーム一番最初に呼ばれる
    void Update(const float& elapsedTime)                       override; // 更新処理
    void End()                                                  override; // 毎フレーム一番最後に呼ばれる
    void Render(const float& elapsedTime, const float& scale)   override; // 描画処理
    void DrawDebug()                                            override; // デバッグ描画

private:
    float dist;
    bool flg = false;
    float time;
    DirectX::XMFLOAT3 speed{ 0,0,0 };
    DirectX::XMFLOAT3 pos_1;
};

