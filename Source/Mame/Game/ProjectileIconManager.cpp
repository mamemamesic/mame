#include "ProjectileIconManager.h"

// コンストラクタ
ProjectileIconManager::ProjectileIconManager()
{
}

// デストラクタ
ProjectileIconManager::~ProjectileIconManager()
{
    Clear();
}

// 初期化
void ProjectileIconManager::Initialize()
{
    for (ProjectileIcon*& projectileIcon : projectileIcons_)
    {
        projectileIcon->Initialize();
    }
}

// 終了化
void ProjectileIconManager::Finalize()
{
    for (ProjectileIcon*& projectileIcon : projectileIcons_)
    {
        projectileIcon->Finalize();
    }
}

// Updateの前に呼ばれる
void ProjectileIconManager::Begin()
{
    for (ProjectileIcon*& projectileIcon : projectileIcons_)
    {
        projectileIcon->Begin();
    }
}

// 更新処理
void ProjectileIconManager::Update(const float& elapsedTime)
{
    // 更新処理
    for (ProjectileIcon*& projectile : projectileIcons_)
    {
        projectile->Update(elapsedTime);
    }

    // 破棄処理
    {
        for (ProjectileIcon* projectileIcon : removes_)
        {
            // vectorから要素を削除する場合はイテレーターで削除
            std::vector<ProjectileIcon*>::iterator it = std::find(projectileIcons_.begin(), projectileIcons_.end(), projectileIcon);

            // std::vectorで管理されている要素を削除するにはerase()関数を使用する
            // (破棄リストのポインタからイテレーターを検索し、erase関数に渡す)
            if (it != projectileIcons_.end())
            {
                projectileIcons_.erase(it);
            }

            // アイテムの破棄
            delete projectileIcon;
        }
        // 破棄リストをクリア
        removes_.clear();
    }
}

// Updateの後に呼ばれる
void ProjectileIconManager::End()
{
    for (ProjectileIcon*& projectileIcon : projectileIcons_)
    {
        projectileIcon->End();
    }
}

// 描画処理
void ProjectileIconManager::Render(const float& scale)
{
    for (ProjectileIcon*& projectileIcon : projectileIcons_)
    {
        projectileIcon->Render(scale);
    }
}

// ImGui用
void ProjectileIconManager::DrawDebug()
{
    for (ProjectileIcon*& projectileIcon : projectileIcons_)
    {
        projectileIcon->DrawDebug();
    }
}

// 登録
void ProjectileIconManager::Register(ProjectileIcon* projectileIcon)
{
    projectileIcon->offsetX_ = (0.1f * columnCounter_); // 生成位置を列に応じて右にずらしていく
    projectileIcons_.emplace_back(projectileIcon);

    // 縦にある程度積んだら分列させて左にずらすようにする
    ++pileUpCounter_;
    if (pileUpCounter_ >= PILE_UP_COUNT_MAX_)
    {
        ++columnCounter_;   // 列を加算

        AddProjectileSpeedAll();

        pileUpCounter_ = 0; // 積み上げカウントをリセット
    }

}

// 削除
void ProjectileIconManager::Remove(ProjectileIcon* projectile)
{
    // 破棄リストに追加
    removes_.insert(projectile);
}

// 全削除
void ProjectileIconManager::Clear()
{
    for (ProjectileIcon*& projectileIcon : projectileIcons_)
    {
        if (projectileIcon)
        {
            delete projectileIcon;
            projectileIcon = nullptr;
        }
    }
    projectileIcons_.clear();
    projectileIcons_.shrink_to_fit();  // vectorの余分なメモリを開放する関数(C++11)
}


void ProjectileIconManager::AddProjectileSpeedAll()
{
    // 弾丸アイコン取得
    for (ProjectileIcon*& projectileIcon : projectileIcons_)
    {
        // 速度上昇・寿命タイマー減少
        constexpr float addSpeed         = 10.0f;
        constexpr float multiplyLifeTime = 0.7f;
        projectileIcon->setSpeed_    += addSpeed;
        projectileIcon->setLifeTime_ *= multiplyLifeTime;
    }
}
