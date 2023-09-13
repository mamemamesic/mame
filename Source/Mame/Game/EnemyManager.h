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
    static constexpr float lll = -1.75f;
    static constexpr float ll  = -1.5f;
    static constexpr float l   = -1.0f;
    static constexpr float c   =  0.0f;
    static constexpr float r   =  1.0f;
    static constexpr float rr  =  1.5f;
    static constexpr float rrr =  1.75f;

    static constexpr int ENEMY_SETS_INDEX_COUNT_ = 54;

     EnemySet enemySets_[ENEMY_SETS_INDEX_COUNT_] = {
        { (3.0f), EnemyType::Normal, { r,   0.25f, 50.0f }, 0,  3 }, //  1 �`���[�g���A����
        { (3.0f), EnemyType::Normal, { l,   0.25f, 50.0f }, 0,  3 }, //  2 �`���[�g���A����
        { (3.0f), EnemyType::Normal, { rrr, 0.25f, 50.0f }, 1,  3 }, //  3
        { (3.0f), EnemyType::Normal, { lll, 0.25f, 50.0f }, 1,  3 }, //  4
        { (3.0f), EnemyType::Normal, { rrr, 0.25f, 50.0f }, 1,  3 }, //  5

        { (1.0f), EnemyType::Normal, { r,   0.25f, 50.0f }, 1,  3 }, //  6
        { (2.0f), EnemyType::Normal, { l,   0.25f, 50.0f }, 4, 22 }, //  7 �|����Όオ�y�ɂȂ鋭�G��
        { (3.0f), EnemyType::Normal, { c,   0.25f, 50.0f }, 0,  5 }, //  8 �񕜖�
        { (0.0f), EnemyType::Normal, { rrr, 0.25f, 50.0f }, 2,  6 }, //  9 �����o��
        { (0.0f), EnemyType::Normal, { lll, 0.25f, 50.0f }, 2,  6 }, // 10 �����o��

        { (3.0f), EnemyType::Normal, { lll, 0.25f, 50.0f }, 2,  6 }, // 11
        { (3.0f), EnemyType::Normal, { c,   0.25f, 50.0f }, 0, 10 }, // 12 �񕜖�
        { (3.0f), EnemyType::Normal, { rrr, 0.25f, 50.0f }, 2, 10 }, // 13
        { (3.0f), EnemyType::Normal, { r,   0.25f, 50.0f }, 3, 15 }, // 14
        { (3.0f), EnemyType::Normal, { l,   0.25f, 50.0f }, 3, 15 }, // 15

        { (2.0f), EnemyType::Normal, { lll, 0.25f, 50.0f },   0, 150 }, // 16 ��(����)
        { (0.0f), EnemyType::Normal, { ll,  0.25f, 50.0f },   0, 150 }, // 17 ��(����)
        { (0.0f), EnemyType::Normal, { l,   0.25f, 50.0f },   0, 150 }, // 18 ��(����)
        { (0.0f), EnemyType::Normal, { -0.5f, 0.25f, 50.0f }, 0, 150 }, // 19 ��(����)
        { (0.0f), EnemyType::Normal, { c,     0.25f, 50.0f }, 0, 150 }, // 20 ��(����)
        { (0.0f), EnemyType::Normal, { 0.5f,  0.25f, 50.0f }, 0, 150 }, // 21 ��(����)
        { (0.0f), EnemyType::Normal, { r,   0.25f, 50.0f },   0,   5 }, // 22 ��(����)
        { (0.0f), EnemyType::Normal, { rr,  0.25f, 50.0f },   0,   5 }, // 23 ��(����)
        { (0.0f), EnemyType::Normal, { rrr, 0.25f, 50.0f },   0,   5 }, // 24 ��(����)

        { (2.0f), EnemyType::Normal, { rrr, 0.25f, 50.0f }, 4,  20 }, // 25 �K�i�o��
        { (0.5f), EnemyType::Normal, { rr,  0.25f, 50.0f }, 4,  20 }, // 26 �K�i�o��
        { (0.5f), EnemyType::Normal, { r,   0.25f, 50.0f }, 4,  20 }, // 27 �K�i�o��
        { (0.5f), EnemyType::Normal, { c,   0.25f, 50.0f }, 4,  20 }, // 28 �K�i�o��
        { (0.5f), EnemyType::Normal, { ll,  0.25f, 50.0f }, 0,  10 }, // 29
        { (0.5f), EnemyType::Normal, { lll, 0.25f, 50.0f }, 4,  20 }, // 30

        { (1.0f), EnemyType::Normal, { r,   0.25f, 50.0f }, 4,  20 }, // 31
        { (1.0f), EnemyType::Normal, { c,   0.25f, 50.0f }, 4,  20 }, // 32
        { (0.5f), EnemyType::Normal, { lll, 0.25f, 50.0f }, 2,  10 }, // 33
        { (1.0f), EnemyType::Normal, { rr,  0.25f, 50.0f }, 4,  20 }, // 34
        { (1.0f), EnemyType::Normal, { rrr, 0.25f, 50.0f }, 4,  20 }, // 35
        { (0.5f), EnemyType::Normal, { ll,  0.25f, 50.0f }, 2,  10 }, // 36

        { (1.0f), EnemyType::Normal, { lll, 0.25f, 50.0f },   0,  15 }, // 37 ��(����)
        { (0.0f), EnemyType::Normal, { ll,  0.25f, 50.0f },   0,  15 }, // 38 ��(����)
        { (0.0f), EnemyType::Normal, { l,   0.25f, 50.0f },   0,  15 }, // 39 ��(����)
        { (0.0f), EnemyType::Normal, { -0.5f, 0.25f, 50.0f }, 0, 200 }, // 40 ��(����)
        { (0.0f), EnemyType::Normal, { c,     0.25f, 50.0f }, 0, 200 }, // 41 ��(����)
        { (0.0f), EnemyType::Normal, { 0.5f,  0.25f, 50.0f }, 0, 200 }, // 42 ��(����)
        { (0.0f), EnemyType::Normal, { r,   0.25f, 50.0f },   0, 200 }, // 43 ��(����)
        { (0.0f), EnemyType::Normal, { rr,  0.25f, 50.0f },   0, 200 }, // 44 ��(����)
        { (0.0f), EnemyType::Normal, { rrr, 0.25f, 50.0f },   0, 200 }, // 45 ��(����)

        { (2.0f), EnemyType::Normal, { lll, 0.25f, 50.0f },   0, 200 }, // 46 ��(����)
        { (0.0f), EnemyType::Normal, { ll,  0.25f, 50.0f },   0, 200 }, // 47 ��(����)
        { (0.0f), EnemyType::Normal, { l,   0.25f, 50.0f },   0, 200 }, // 48 ��(����)
        { (0.0f), EnemyType::Normal, { -0.5f, 0.25f, 50.0f }, 0, 200 }, // 49 ��(����)
        { (0.0f), EnemyType::Normal, { c,     0.25f, 50.0f }, 0, 200 }, // 50 ��(����)
        { (0.0f), EnemyType::Normal, { 0.5f,  0.25f, 50.0f }, 0, 200 }, // 51 ��(����)
        { (0.0f), EnemyType::Normal, { r,   0.25f, 50.0f },   0,  15 }, // 52 ��(����)
        { (0.0f), EnemyType::Normal, { rr,  0.25f, 50.0f },   0,  15 }, // 53 ��(����)
        { (0.0f), EnemyType::Normal, { rrr, 0.25f, 50.0f },   0,  15 }, // 54 ��(����)
    };

public:
    float timer_ = 0.0f;
    int   currentEnemySetsIndex_ = 0;

private:
    std::vector<Enemy*> enemies_ = {};
    std::set<Enemy*>    removes_ = {};
};
