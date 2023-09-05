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

private:
    std::vector<Enemy*> enemies_ = {};
    std::set<Enemy*>    removes_ = {};
};
