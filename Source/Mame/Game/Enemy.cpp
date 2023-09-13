#include "Enemy.h"
#include "EnemyManager.h"


void Enemy::Initialize()
{
    Character::Initialize();
}

void Enemy::Update(const float& elapsedTime)
{
    // 残り体力によって色を赤くしていく
    {
        const float floatHp    = static_cast<float>(hp_);
        const float floatMaxHp = static_cast<float>(maxHp_);
        const float setColorGB = (floatHp / floatMaxHp);
        model->color.y = model->color.z = setColorGB;
    }

    Character::Update(elapsedTime);
}

void Enemy::Render(const float& elapsedTime, const float& scale)
{
    Character::Render(elapsedTime, scale);
}
