#pragma once

#include "ProjectileIcon.h"

class EnemyProjectileStraiteIcon : public ProjectileIcon
{
public:
    EnemyProjectileStraiteIcon(ProjectileIconManager* manager);
    ~EnemyProjectileStraiteIcon() {}

    void Initialize()                       override;   // 初期化
    void Finalize()                         override {} // 終了化
    void Begin()                            override {} // 毎フレーム一番最初に呼ばれる
    void Update(const float& elapsedTime)   override;
    void End()                              override {} // 毎フレーム一番最後に呼ばれる
    void Render(const float& scale)         override;   // 描画処理
    void DrawDebug()                        override;   // ImGui用
private:
    float launchTime_ = 1.0f;

};