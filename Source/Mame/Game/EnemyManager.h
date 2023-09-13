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
    static constexpr float lll = -1.75f;
    static constexpr float ll  = -1.5f;
    static constexpr float l   = -1.0f;
    static constexpr float c   =  0.0f;
    static constexpr float r   =  1.0f;
    static constexpr float rr  =  1.5f;
    static constexpr float rrr =  1.75f;

    static constexpr int ENEMY_SETS_INDEX_COUNT_ = 54;

     EnemySet enemySets_[ENEMY_SETS_INDEX_COUNT_] = {
        { (3.0f), EnemyType::Normal, { r,   0.25f, 50.0f }, 0,  3 }, //  1 チュートリアル役
        { (3.0f), EnemyType::Normal, { l,   0.25f, 50.0f }, 0,  3 }, //  2 チュートリアル役
        { (3.0f), EnemyType::Normal, { rrr, 0.25f, 50.0f }, 1,  3 }, //  3
        { (3.0f), EnemyType::Normal, { lll, 0.25f, 50.0f }, 1,  3 }, //  4
        { (3.0f), EnemyType::Normal, { rrr, 0.25f, 50.0f }, 1,  3 }, //  5

        { (1.0f), EnemyType::Normal, { r,   0.25f, 50.0f }, 1,  3 }, //  6
        { (2.0f), EnemyType::Normal, { l,   0.25f, 50.0f }, 4, 22 }, //  7 倒せれば後が楽になる強敵役
        { (3.0f), EnemyType::Normal, { c,   0.25f, 50.0f }, 0,  5 }, //  8 回復役
        { (0.0f), EnemyType::Normal, { rrr, 0.25f, 50.0f }, 2,  6 }, //  9 同時出現
        { (0.0f), EnemyType::Normal, { lll, 0.25f, 50.0f }, 2,  6 }, // 10 同時出現

        { (3.0f), EnemyType::Normal, { lll, 0.25f, 50.0f }, 2,  6 }, // 11
        { (3.0f), EnemyType::Normal, { c,   0.25f, 50.0f }, 0, 10 }, // 12 回復役
        { (3.0f), EnemyType::Normal, { rrr, 0.25f, 50.0f }, 2, 10 }, // 13
        { (3.0f), EnemyType::Normal, { r,   0.25f, 50.0f }, 3, 15 }, // 14
        { (3.0f), EnemyType::Normal, { l,   0.25f, 50.0f }, 3, 15 }, // 15

        { (2.0f), EnemyType::Normal, { lll, 0.25f, 50.0f },   0, 150 }, // 16 壁(同時)
        { (0.0f), EnemyType::Normal, { ll,  0.25f, 50.0f },   0, 150 }, // 17 壁(同時)
        { (0.0f), EnemyType::Normal, { l,   0.25f, 50.0f },   0, 150 }, // 18 壁(同時)
        { (0.0f), EnemyType::Normal, { -0.5f, 0.25f, 50.0f }, 0, 150 }, // 19 壁(同時)
        { (0.0f), EnemyType::Normal, { c,     0.25f, 50.0f }, 0, 150 }, // 20 壁(同時)
        { (0.0f), EnemyType::Normal, { 0.5f,  0.25f, 50.0f }, 0, 150 }, // 21 壁(同時)
        { (0.0f), EnemyType::Normal, { r,   0.25f, 50.0f },   0,   5 }, // 22 壁(同時)
        { (0.0f), EnemyType::Normal, { rr,  0.25f, 50.0f },   0,   5 }, // 23 壁(同時)
        { (0.0f), EnemyType::Normal, { rrr, 0.25f, 50.0f },   0,   5 }, // 24 壁(同時)

        { (2.0f), EnemyType::Normal, { rrr, 0.25f, 50.0f }, 4,  20 }, // 25 階段出現
        { (0.5f), EnemyType::Normal, { rr,  0.25f, 50.0f }, 4,  20 }, // 26 階段出現
        { (0.5f), EnemyType::Normal, { r,   0.25f, 50.0f }, 4,  20 }, // 27 階段出現
        { (0.5f), EnemyType::Normal, { c,   0.25f, 50.0f }, 4,  20 }, // 28 階段出現
        { (0.5f), EnemyType::Normal, { ll,  0.25f, 50.0f }, 0,  10 }, // 29
        { (0.5f), EnemyType::Normal, { lll, 0.25f, 50.0f }, 4,  20 }, // 30

        { (1.0f), EnemyType::Normal, { r,   0.25f, 50.0f }, 4,  20 }, // 31
        { (1.0f), EnemyType::Normal, { c,   0.25f, 50.0f }, 4,  20 }, // 32
        { (0.5f), EnemyType::Normal, { lll, 0.25f, 50.0f }, 2,  10 }, // 33
        { (1.0f), EnemyType::Normal, { rr,  0.25f, 50.0f }, 4,  20 }, // 34
        { (1.0f), EnemyType::Normal, { rrr, 0.25f, 50.0f }, 4,  20 }, // 35
        { (0.5f), EnemyType::Normal, { ll,  0.25f, 50.0f }, 2,  10 }, // 36

        { (1.0f), EnemyType::Normal, { lll, 0.25f, 50.0f },   0,  15 }, // 37 壁(同時)
        { (0.0f), EnemyType::Normal, { ll,  0.25f, 50.0f },   0,  15 }, // 38 壁(同時)
        { (0.0f), EnemyType::Normal, { l,   0.25f, 50.0f },   0,  15 }, // 39 壁(同時)
        { (0.0f), EnemyType::Normal, { -0.5f, 0.25f, 50.0f }, 0, 200 }, // 40 壁(同時)
        { (0.0f), EnemyType::Normal, { c,     0.25f, 50.0f }, 0, 200 }, // 41 壁(同時)
        { (0.0f), EnemyType::Normal, { 0.5f,  0.25f, 50.0f }, 0, 200 }, // 42 壁(同時)
        { (0.0f), EnemyType::Normal, { r,   0.25f, 50.0f },   0, 200 }, // 43 壁(同時)
        { (0.0f), EnemyType::Normal, { rr,  0.25f, 50.0f },   0, 200 }, // 44 壁(同時)
        { (0.0f), EnemyType::Normal, { rrr, 0.25f, 50.0f },   0, 200 }, // 45 壁(同時)

        { (2.0f), EnemyType::Normal, { lll, 0.25f, 50.0f },   0, 200 }, // 46 壁(同時)
        { (0.0f), EnemyType::Normal, { ll,  0.25f, 50.0f },   0, 200 }, // 47 壁(同時)
        { (0.0f), EnemyType::Normal, { l,   0.25f, 50.0f },   0, 200 }, // 48 壁(同時)
        { (0.0f), EnemyType::Normal, { -0.5f, 0.25f, 50.0f }, 0, 200 }, // 49 壁(同時)
        { (0.0f), EnemyType::Normal, { c,     0.25f, 50.0f }, 0, 200 }, // 50 壁(同時)
        { (0.0f), EnemyType::Normal, { 0.5f,  0.25f, 50.0f }, 0, 200 }, // 51 壁(同時)
        { (0.0f), EnemyType::Normal, { r,   0.25f, 50.0f },   0,  15 }, // 52 壁(同時)
        { (0.0f), EnemyType::Normal, { rr,  0.25f, 50.0f },   0,  15 }, // 53 壁(同時)
        { (0.0f), EnemyType::Normal, { rrr, 0.25f, 50.0f },   0,  15 }, // 54 壁(同時)
    };

public:
    float timer_ = 0.0f;
    int   currentEnemySetsIndex_ = 0;

private:
    std::vector<Enemy*> enemies_ = {};
    std::set<Enemy*>    removes_ = {};
};
