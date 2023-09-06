#pragma once

#include "ProjectileIcon.h"
#include <vector>
#include <set>

class ProjectileIconManager
{
public:
    ProjectileIconManager();
    ~ProjectileIconManager();

    void Initialize();                                          // 初期化
    void Finalize();                                            // 終了化
    void Begin();                                               // 毎フレーム一番最初に呼ばれる
    void Update(const float& elapsedTime);                      // 更新処理
    void End();                                                 // 毎フレーム一番最後に呼ばれる
    void Render(const float& scale);  // 描画処理
    void DrawDebug();                                           // ImGui用

public:
    void Register(ProjectileIcon* projectileIcon);  // 登録
    void Remove(ProjectileIcon* projectile);    // 削除
    void Clear();                           // 全削除

public: // 取得・設定　関連
    const int GetProjectileIconCount() const { return static_cast<int>(projectileIcons.size()); } // 弾丸数取得

    ProjectileIcon* GetProjectileIcon(int index) { return projectileIcons.at(index); } // 弾丸取得

public:
    static constexpr int PILE_UP_COUNT_MAX_ = 5; // 積み上げ最大数

public:
    int pileUpCounter_  = 0; // 積み上げた数を数える
    int columnCounter_  = 0; // 列を数える

private:
    std::vector<ProjectileIcon*>  projectileIcons = {};
    std::set<ProjectileIcon*>     removes = {};

};