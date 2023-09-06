#pragma once

#include "Enemy.h"
#include <vector>
#include <set>

class EnemyManager
{
private:
    EnemyManager() {}
    ~EnemyManager() {}

public:
    static EnemyManager& Instance()
    {
        static EnemyManager instance;
        return instance;
    }

    void Initialize();
    void Finalize();
    void Begin();
    void Update(const float& elapsedTime);
    void End();
    void Render(const float elapsedTime, const float scale);
    void DrawDebug();

public:
    // 敵同士の衝突処理
    void CollisionEnemyVsEnemies();

    // エネミー登録
    void Register(Enemy* enemy);

    // エネミー削除
    void Remove(Enemy* enemy);

    // エネミー全削除
    void Clear();

    // エネミー数取得
    const int GetEnemyCount() const { return static_cast<int>(enemies_.size()); }

    // エネミー取得
    Enemy* GetEnemy(const int& index) { return enemies_.at(index); }

    // デバッグプリミティブ描画
    void DrawDebugPrimitive();

private:
    std::vector<Enemy*> enemies_ = {};
    std::set<Enemy*>    removes_ = {};
};
