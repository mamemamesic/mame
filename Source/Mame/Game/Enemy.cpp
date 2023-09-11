#include "Enemy.h"
#include "EnemyManager.h"

Enemy::Enemy(EnemyManager* manager)
    :manager(manager)
{
    manager->Register(this);
}

void Enemy::Initialize()
{
    Character::Initialize();
}

void Enemy::Update(const float& elapsedTime)
{
    Character::Update(elapsedTime);
}

void Enemy::Render(const float& elapsedTime, const float& scale)
{
    Character::Render(elapsedTime, scale);
}

// ”jŠü
void Enemy::Destroy()
{
    manager->Remove(this);
}