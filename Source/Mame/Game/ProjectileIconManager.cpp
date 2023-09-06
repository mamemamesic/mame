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
    for (ProjectileIcon*& projectileIcon : projectileIcons)
    {
        projectileIcon->Initialize();
    }
}

// 終了化
void ProjectileIconManager::Finalize()
{
    for (ProjectileIcon*& projectileIcon : projectileIcons)
    {
        projectileIcon->Finalize();
    }
}

// Updateの前に呼ばれる
void ProjectileIconManager::Begin()
{
    for (ProjectileIcon*& projectileIcon : projectileIcons)
    {
        projectileIcon->Begin();
    }
}

// 更新処理
void ProjectileIconManager::Update(const float& elapsedTime)
{
    // 更新処理
    for (ProjectileIcon*& projectile : projectileIcons)
    {
        projectile->Update(elapsedTime);
    }

    // 破棄処理
    {
        for (ProjectileIcon* projectileIcon : removes)
        {
            // vectorから要素を削除する場合はイテレーターで削除
            std::vector<ProjectileIcon*>::iterator it = std::find(projectileIcons.begin(), projectileIcons.end(), projectileIcon);

            // std::vectorで管理されている要素を削除するにはerase()関数を使用する
            // (破棄リストのポインタからイテレーターを検索し、erase関数に渡す)
            if (it != projectileIcons.end())
            {
                projectileIcons.erase(it);
            }

            // アイテムの破棄
            delete projectileIcon;
        }
        // 破棄リストをクリア
        removes.clear();
    }
}

// Updateの後に呼ばれる
void ProjectileIconManager::End()
{
    for (ProjectileIcon*& projectileIcon : projectileIcons)
    {
        projectileIcon->End();
    }
}

// 描画処理
void ProjectileIconManager::Render(const float& scale)
{
    for (ProjectileIcon*& projectileIcon : projectileIcons)
    {
        projectileIcon->Render(scale);
    }
}

// ImGui用
void ProjectileIconManager::DrawDebug()
{
    for (ProjectileIcon*& projectileIcon : projectileIcons)
    {
        projectileIcon->DrawDebug();
    }
}

// 登録
void ProjectileIconManager::Register(ProjectileIcon* projectile)
{
    projectileIcons.emplace_back(projectile);
}

// 削除
void ProjectileIconManager::Remove(ProjectileIcon* projectile)
{
    // 破棄リストに追加
    removes.insert(projectile);
}

// 全削除
void ProjectileIconManager::Clear()
{
    for (ProjectileIcon*& projectileIcon : projectileIcons)
    {
        if (projectileIcon)
        {
            delete projectileIcon;
            projectileIcon = nullptr;
        }
    }
    projectileIcons.clear();
    projectileIcons.shrink_to_fit();  // vectorの余分なメモリを開放する関数(C++11)
}
