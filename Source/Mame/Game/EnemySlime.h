#pragma once
#include "Enemy.h"

#include "PlayerManager.h"

class EnemySlime : public Enemy
{
public:
    EnemySlime();
    ~EnemySlime() override;

    void Initialize()                                           override; // ������
    void Finalize()                                             override; // �I����
    void Begin()                                                override; // ���t���[����ԍŏ��ɌĂ΂��
    void Update(const float& elapsedTime)                       override; // �X�V����
    void End()                                                  override; // ���t���[����ԍŌ�ɌĂ΂��
    void Render(const float& elapsedTime, const float& scale)   override; // �`�揈��
    void DrawDebug()                                            override; // �f�o�b�O�`��

private:
    float dist;
    bool flg = false;
    float time;
    DirectX::XMFLOAT3 speed{ 0,0,0 };
    DirectX::XMFLOAT3 pos_1;
};

