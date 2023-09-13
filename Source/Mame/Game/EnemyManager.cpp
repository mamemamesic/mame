#include "EnemyManager.h"
#include "Collision.h"

void EnemyManager::Initialize()
{
    for (Enemy*& enemy : enemies_) enemy->Initialize();

    currentEnemySetsIndex_ = 0;
    timer_                 = 0.0f;
}

void EnemyManager::Finalize()
{
    EnemyManager::Clear();
}

void EnemyManager::Begin()
{
    for (Enemy*& enemy : enemies_) enemy->Begin();
}

void EnemyManager::Update(const float& elapsedTime)
{
    for (Enemy*& enemy : enemies_) enemy->Update(elapsedTime);

    // 破棄処理
    // ※projcectileの範囲for文中でerase()すると不具合が発生してしまうため、
    //   更新処理が終わった後に破棄リストに積まれたオブジェクトを削除する
    for (Enemy* enemy : removes_)
    {
        using EnemyIterator = std::vector<Enemy*>::iterator;

        // vectorから要素を削除する場合はイテレーターで削除
        EnemyIterator it = std::find(enemies_.begin(), enemies_.end(), enemy);

        // std::vectorで管理されている要素を削除するにはerase()関数を使用する
        // （破棄リストのポインタからイテレーターを検索し、erase関数に渡す）
        if (it != enemies_.end())
        {
            enemies_.erase(it);
        }

        // 敵の破棄処理
        //safe_delete(enemy);
        if (enemy != nullptr)
        {
            delete (enemy);
            enemy = nullptr;
        }
    }
    // 破棄リストをクリア
    removes_.clear();

    // 敵同士の衝突処理
    EnemyManager::CollisionEnemyVsEnemies();
}

void EnemyManager::End()
{
    for (Enemy*& enemy : enemies_) enemy->End();
}

void EnemyManager::Render(const float elapsedTime, const float scale)
{
    for (Enemy*& enemy : enemies_) enemy->Render(elapsedTime, scale);
}

void EnemyManager::DrawDebug()
{
    for (Enemy*& enemy : enemies_) enemy->DrawDebug();
}

void EnemyManager::CollisionEnemyVsEnemies()
{
#if 0
    EnemyManager& enemyManager = EnemyManager::Instance();

    int enemyCount = enemyManager.GetEnemyCount();
    for (int a = 0; a < enemyCount; ++a)
    {
        Enemy* enemyA = enemyManager.GetEnemy(a);

        // a以降の敵と判定を行う（a以前はすでに判定済みのため）
        for (int b = a + 1; b < enemyCount; ++b)
        {
            Enemy* enemyB = enemyManager.GetEnemy(b);

            // 衝突判定
            //DirectX::XMFLOAT3 outPosition = {};
            //if (Collision::IntersectSphereVsSphere(
            //    enemyA->GetPosition(), enemyA->GetRadius(),
            //    enemyB->GetPosition(), enemyB->GetRadius(),
            //    outPosition))
            //{
            //    // 押し出し後の位置設定
            //    enemyB->SetPosition(outPosition);
            //}

            // 衝突判定
            DirectX::XMFLOAT3 outPosition = {};
            if (!Collision::IntersectCylinderVsCylinder(
                enemyA->GetPosition(), enemyA->GetRadius(), enemyA->GetHeight(),
                enemyB->GetPosition(), enemyB->GetRadius(), enemyB->GetHeight(),
                outPosition))
            {
                continue;
            }

            // 押し出し後の位置設定
            enemyB->SetPosition(outPosition);
        }
    }
#endif
}

void EnemyManager::Register(Enemy* enemy)
{
    enemies_.emplace_back(enemy);
}

void EnemyManager::Remove(Enemy* enemy)
{
    // 破棄リストに追加
    // （※直接projectilesの要素を削除してしまうと範囲for文で不具合を起こすため破棄リストに追加する）
    removes_.insert(enemy);
}

void EnemyManager::Clear()
{
    for (Enemy*& enemy : enemies_)
    {
        //safe_delete(enemy);
        if (enemy != nullptr)
        {
            delete (enemy);
            enemy = nullptr;
        }
    }
    enemies_.clear();
}


void EnemyManager::DrawDebugPrimitive()
{
    //for (Enemy*& enemy : enemies_) enemy->DrawDebugPrimitive();
}
