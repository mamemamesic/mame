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
    static constexpr int ENEMY_SETS_INDEX_COUNT_ = 4;

    const EnemySet enemySets_[ENEMY_SETS_INDEX_COUNT_] = {
        { 3.0f,  EnemyType::Normal, { 0, 0.25f, 50.0f },  1 },
        { 6.0f,  EnemyType::Normal, { 0, 0.25f, 50.0f },  1 },
        { 9.0f,  EnemyType::Normal, { 0, 0.25f, 50.0f },  1 },
        { 12.0f, EnemyType::Normal, { 0, 0.25f, 50.0f },  2 },
    };

public:
    float timer_ = 0.0f;
    int   currentEnemySetsIndex_ = 0;

private:
    std::vector<Enemy*> enemies_ = {};
    std::set<Enemy*>    removes_ = {};
};
