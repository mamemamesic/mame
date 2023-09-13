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

    static constexpr int ENEMY_SETS_INDEX_COUNT_ = 68;

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
        { (0.0f), EnemyType::Normal, { rr,  0.25f, 50.0f },   1,   5 }, // 23 ��(����)
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
        { (0.0f), EnemyType::Normal, { ll,  0.25f, 50.0f },   1,  15 }, // 38 ��(����)
        { (0.0f), EnemyType::Normal, { l,   0.25f, 50.0f },   0,  15 }, // 39 ��(����)
        { (0.0f), EnemyType::Normal, { -0.5f, 0.25f, 50.0f }, 0, 200 }, // 40 ��(����)
        { (0.0f), EnemyType::Normal, { c,     0.25f, 50.0f }, 0, 200 }, // 41 ��(����)
        { (0.0f), EnemyType::Normal, { 0.5f,  0.25f, 50.0f }, 0, 200 }, // 42 ��(����)
        { (0.0f), EnemyType::Normal, { r,   0.25f, 50.0f },   0, 200 }, // 43 ��(����)
        { (0.0f), EnemyType::Normal, { rr,  0.25f, 50.0f },   0, 200 }, // 44 ��(����)
        { (0.0f), EnemyType::Normal, { rrr, 0.25f, 50.0f },   0, 200 }, // 45 ��(����)

        { (2.0f), EnemyType::Normal, { lll, 0.25f, 50.0f },   1, 250 }, // 46 ��(����)
        { (0.0f), EnemyType::Normal, { ll,  0.25f, 50.0f },   0, 250 }, // 47 ��(����)
        { (0.0f), EnemyType::Normal, { l,   0.25f, 50.0f },   1, 250 }, // 48 ��(����)
        { (0.0f), EnemyType::Normal, { -0.5f, 0.25f, 50.0f }, 0, 250 }, // 49 ��(����)
        { (0.0f), EnemyType::Normal, { c,     0.25f, 50.0f }, 1, 250 }, // 50 ��(����)
        { (0.0f), EnemyType::Normal, { 0.5f,  0.25f, 50.0f }, 0, 250 }, // 51 ��(����)
        { (0.0f), EnemyType::Normal, { r,   0.25f, 50.0f },   1,  15 }, // 52 ��(����)
        { (0.0f), EnemyType::Normal, { rr,  0.25f, 50.0f },   2,  15 }, // 53 ��(����)
        { (0.0f), EnemyType::Normal, { rrr, 0.25f, 50.0f },   1,  15 }, // 54 ��(����)

        { (2.0f), EnemyType::Normal, { lll, 0.25f, 50.0f },   0, 300 }, // 55 ��(����)
        { (0.0f), EnemyType::Normal, { ll,  0.25f, 50.0f },   2, 300 }, // 56 ��(����)
        { (0.0f), EnemyType::Normal, { l,   0.25f, 50.0f },   0, 300 }, // 57 ��(����)
        { (0.0f), EnemyType::Normal, { -0.5f, 0.25f, 50.0f }, 2, 25  }, // 58 ��(����)
        { (0.0f), EnemyType::Normal, { c,     0.25f, 50.0f }, 3, 25  }, // 59 ��(����)
        { (0.0f), EnemyType::Normal, { 0.5f,  0.25f, 50.0f }, 2, 25  }, // 60 ��(����)
        { (0.0f), EnemyType::Normal, { r,   0.25f, 50.0f },   0, 300 }, // 61 ��(����)
        { (0.0f), EnemyType::Normal, { rr,  0.25f, 50.0f },   2, 300 }, // 62 ��(����)
        { (0.0f), EnemyType::Normal, { rrr, 0.25f, 50.0f },   0, 300 }, // 63 ��(����)

        { (7.0f), EnemyType::Normal, { lll, 0.25f, 50.0f }, 25 * 2, 1000 }, // 64 �{�X(����)
        { (0.0f), EnemyType::Normal, { l,   0.25f, 50.0f }, 25 * 3, 1000 }, // 65 �{�X(����)
        { (0.0f), EnemyType::Normal, { c,   0.25f, 50.0f }, 25 * 4, 1000 }, // 66 �{�X(����)
        { (0.0f), EnemyType::Normal, { r,   0.25f, 50.0f }, 25 * 5, 1000 }, // 67 �{�X(����)
        { (0.0f), EnemyType::Normal, { rrr, 0.25f, 50.0f }, 25 * 6, 1000 }, // 68 �{�X(����)
    };

public:
    float timer_ = 0.0f;
    int   currentEnemySetsIndex_ = 0;

private:
    std::vector<Enemy*> enemies_ = {};
    std::set<Enemy*>    removes_ = {};
};
