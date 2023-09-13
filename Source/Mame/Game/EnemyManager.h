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
        int               spawnProjIconCount_ = 0;                  // ��������e�ۃA�C�R���̐�
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
    // �G���m�̏Փˏ���
    void CollisionEnemyVsEnemies();

    // �G�l�~�[�o�^
    void Register(Enemy* enemy);

    // �G�l�~�[�폜
    void Remove(Enemy* enemy);

    // �G�l�~�[�S�폜
    void Clear();

    // �G�l�~�[���擾
    const int GetEnemyCount() const { return static_cast<int>(enemies_.size()); }

    // �G�l�~�[�擾
    Enemy* GetEnemy(const int& index) { return enemies_.at(index); }

    // �f�o�b�O�v���~�e�B�u�`��
    void DrawDebugPrimitive();

public:
    static constexpr int ENEMY_SETS_INDEX_COUNT_ = 13;

     EnemySet enemySets_[ENEMY_SETS_INDEX_COUNT_] = {
        { (3.0f *  1.0f), EnemyType::Normal, {  1.0f, 0.25f, 50.0f }, 0, 3 }, // 1
        { (3.0f *  2.0f), EnemyType::Normal, { -1.0f, 0.25f, 50.0f }, 0, 3 }, // 2
        { (3.0f *  3.0f), EnemyType::Normal, {  2.0f, 0.25f, 50.0f }, 1, 3 }, // 3
        { (3.0f *  4.0f), EnemyType::Normal, { -2.0f, 0.25f, 50.0f }, 1, 3 }, // 4
        { (3.0f *  5.0f), EnemyType::Normal, {  2.0f, 0.25f, 50.0f }, 1, 3 }, // 5
        { (3.0f *  6.0f), EnemyType::Normal, {  1.0f, 0.25f, 50.0f }, 1, 3 }, // 6
        { (3.0f *  7.0f), EnemyType::Normal, { -1.0f, 0.25f, 50.0f }, 4, 8 }, // 7
        { (3.0f *  8.0f), EnemyType::Normal, {  2.0f, 0.25f, 50.0f }, 2, 6 }, // 8
        { (3.0f *  9.0f), EnemyType::Normal, {  1.0f, 0.25f, 50.0f }, 2, 6 }, // 9
        { (3.0f * 10.0f), EnemyType::Normal, { -2.0f, 0.25f, 50.0f }, 2, 6 }, // 10

        { (3.0f * 11.0f), EnemyType::Normal, { -2.0f, 0.25f, 50.0f }, 2, 6 }, // 11
        { (3.0f * 12.0f), EnemyType::Normal, { -2.0f, 0.25f, 50.0f }, 2, 6 }, // 12
        { (3.0f * 13.0f), EnemyType::Normal, { -2.0f, 0.25f, 50.0f }, 2, 6 }, // 13

    };

public:
    float timer_ = 0.0f;
    int   currentEnemySetsIndex_ = 0;

private:
    std::vector<Enemy*> enemies_ = {};
    std::set<Enemy*>    removes_ = {};
};
