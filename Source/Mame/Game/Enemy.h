#pragma once
#include "Character.h"

class EnemyManager;

class Enemy : public Character
{
public:
    Enemy(EnemyManager* manager);
    virtual ~Enemy() {};

    virtual void Initialize();                                          // ������
    virtual void Finalize() = 0;                                        // �I����
    virtual void Begin() = 0;                                           // ���t���[����ԍŏ��ɌĂ΂��
    virtual void Update(const float& elapsedTime);                      // �X�V����
    virtual void End() = 0;                                             // ���t���[����ԍŌ�ɌĂ΂��
    virtual void Render(const float& elapsedTime, const float& scale);  // �`�揈��
    virtual void DrawDebug() = 0;                                       // �f�o�b�O�`��

public:
    void Destroy(); // �j��
private:
    EnemyManager* manager = nullptr;
};

