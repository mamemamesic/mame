#pragma once

#include "Enemy.h"
#include <vector>
#include <set>

class EnemyManager
{
public:
    enum class EnemyType
    {
        Normal,
        Boss,
    };

    struct EnemySet
    {
        float             spawnTime_          = 0.0f;
        EnemyType         enemyType_          = EnemyType::Normal;
        DirectX::XMFLOAT3 spawnPosition_      = { 0,0,0 };
        int               spawnProjIconCount_ = 0;                  // 生成する弾丸アイコンの数
        int               hp_                 = 0;
    };

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

public:
    static constexpr int ENEMY_SETS_INDEX_COUNT_ = 24;

     EnemySet enemySets_[ENEMY_SETS_INDEX_COUNT_] = {
        { (3.0f), EnemyType::Normal, {  1.0f, 0.25f, 50.0f }, 0,  3 }, // 1 チュートリアル役
        { (3.0f), EnemyType::Normal, { -1.0f, 0.25f, 50.0f }, 0,  3 }, // 2 チュートリアル役
        { (3.0f), EnemyType::Normal, {  2.0f, 0.25f, 50.0f }, 1,  3 }, // 3
        { (3.0f), EnemyType::Normal, { -2.0f, 0.25f, 50.0f }, 1,  3 }, // 4
        { (3.0f), EnemyType::Normal, {  2.0f, 0.25f, 50.0f }, 1,  3 }, // 5

        { (3.0f), EnemyType::Normal, {  1.0f, 0.25f, 50.0f }, 1,  3 }, // 6
        { (3.0f), EnemyType::Normal, { -1.0f, 0.25f, 50.0f }, 4, 22 }, // 7 倒せれば後が楽になる強敵役
        { (3.0f), EnemyType::Normal, {  2.0f, 0.25f, 50.0f }, 0,  5 }, // 8 回復役
        { (3.0f), EnemyType::Normal, {  1.0f, 0.25f, 50.0f }, 2,  6 }, // 9
        { (3.0f), EnemyType::Normal, { -2.0f, 0.25f, 50.0f }, 2,  6 }, // 10

        { (3.0f), EnemyType::Normal, { -2.0f, 0.25f, 50.0f }, 2,  6 }, // 11
        { (3.0f), EnemyType::Normal, {  0.0f, 0.25f, 50.0f }, 0, 10 }, // 12 回復役
        { (3.0f), EnemyType::Normal, {  2.0f, 0.25f, 50.0f }, 2,  6 }, // 13
        { (3.0f), EnemyType::Normal, {  1.0f, 0.25f, 50.0f }, 3,  9 }, // 14
        { (3.0f), EnemyType::Normal, { -1.0f, 0.25f, 50.0f }, 3,  9 }, // 15

        { (3.0f), EnemyType::Normal, { -2.0f, 0.25f, 50.0f }, 0,  30 }, // 16 壁
        { (0.0f), EnemyType::Normal, { -1.5f, 0.25f, 50.0f }, 0,  30 }, // 17 壁
        { (0.0f), EnemyType::Normal, { -1.0f, 0.25f, 50.0f }, 0,  30 }, // 18 壁
        { (0.0f), EnemyType::Normal, { -0.5f, 0.25f, 50.0f }, 0,  30 }, // 19 壁
        { (0.0f), EnemyType::Normal, {  0.0f, 0.25f, 50.0f }, 0,  30 }, // 20 壁
        { (0.0f), EnemyType::Normal, {  0.5f, 0.25f, 50.0f }, 0,  30 }, // 21 壁
        { (0.0f), EnemyType::Normal, {  1.0f, 0.25f, 50.0f }, 0,  30 }, // 22 壁
        { (0.0f), EnemyType::Normal, {  1.5f, 0.25f, 50.0f }, 1,  15 }, // 23 壁
        { (0.0f), EnemyType::Normal, {  2.0f, 0.25f, 50.0f }, 0,  30 }, // 24 壁

    };

public:
    float timer_ = 0.0f;
    int   currentEnemySetsIndex_ = 0;

private:
    std::vector<Enemy*> enemies_ = {};
    std::set<Enemy*>    removes_ = {};
};
