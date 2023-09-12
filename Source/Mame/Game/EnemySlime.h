#pragma once
#include "Enemy.h"

class EnemySlime : public Enemy
{
public:
    //EnemySlime() {};
    EnemySlime(EnemyManager* manager, DirectX::XMFLOAT3 enemy_set, int count);
    ~EnemySlime() override;

    void Initialize()                                           override; // ������
    void Finalize()                                             override; // �I����
    void Begin()                                                override; // ���t���[����ԍŏ��ɌĂ΂��
    void Update(const float& elapsedTime)                       override; // �X�V����
    void End()                                                  override; // ���t���[����ԍŌ�ɌĂ΂��
    void Render(const float& elapsedTime, const float& scale)   override; // �`�揈��
    void DrawDebug()                                            override; // �f�o�b�O�`��

private:
    float offsetY_ = 0.25f;
    float dist;
    DirectX::XMFLOAT3 pos_1;
    DirectX::XMFLOAT3 speed = { 0,0,-0.02f };
    int enemy_count;
    int state = 0;
    DirectX::XMFLOAT3 enemySet = { 0,0,10 };
   
};

