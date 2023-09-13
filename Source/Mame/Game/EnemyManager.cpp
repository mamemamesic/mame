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

    // �j������
    // ��projcectile�͈̔�for������erase()����ƕs����������Ă��܂����߁A
    //   �X�V�������I�������ɔj�����X�g�ɐς܂ꂽ�I�u�W�F�N�g���폜����
    for (Enemy* enemy : removes_)
    {
        using EnemyIterator = std::vector<Enemy*>::iterator;

        // vector����v�f���폜����ꍇ�̓C�e���[�^�[�ō폜
        EnemyIterator it = std::find(enemies_.begin(), enemies_.end(), enemy);

        // std::vector�ŊǗ�����Ă���v�f���폜����ɂ�erase()�֐����g�p����
        // �i�j�����X�g�̃|�C���^����C�e���[�^�[���������Aerase�֐��ɓn���j
        if (it != enemies_.end())
        {
            enemies_.erase(it);
        }

        // �G�̔j������
        //safe_delete(enemy);
        if (enemy != nullptr)
        {
            delete (enemy);
            enemy = nullptr;
        }
    }
    // �j�����X�g���N���A
    removes_.clear();

    // �G���m�̏Փˏ���
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

        // a�ȍ~�̓G�Ɣ�����s���ia�ȑO�͂��łɔ���ς݂̂��߁j
        for (int b = a + 1; b < enemyCount; ++b)
        {
            Enemy* enemyB = enemyManager.GetEnemy(b);

            // �Փ˔���
            //DirectX::XMFLOAT3 outPosition = {};
            //if (Collision::IntersectSphereVsSphere(
            //    enemyA->GetPosition(), enemyA->GetRadius(),
            //    enemyB->GetPosition(), enemyB->GetRadius(),
            //    outPosition))
            //{
            //    // �����o����̈ʒu�ݒ�
            //    enemyB->SetPosition(outPosition);
            //}

            // �Փ˔���
            DirectX::XMFLOAT3 outPosition = {};
            if (!Collision::IntersectCylinderVsCylinder(
                enemyA->GetPosition(), enemyA->GetRadius(), enemyA->GetHeight(),
                enemyB->GetPosition(), enemyB->GetRadius(), enemyB->GetHeight(),
                outPosition))
            {
                continue;
            }

            // �����o����̈ʒu�ݒ�
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
    // �j�����X�g�ɒǉ�
    // �i������projectiles�̗v�f���폜���Ă��܂��Ɣ͈�for���ŕs����N�������ߔj�����X�g�ɒǉ�����j
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
