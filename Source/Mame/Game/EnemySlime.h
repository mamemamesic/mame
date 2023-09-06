#pragma once
#include "Enemy.h"
#include "PlayerManager.h"


class EnemySlime : public Enemy
{
public:
    EnemySlime() {};
    EnemySlime(DirectX::XMFLOAT3 enemy_set);
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
    bool flg = false;
    DirectX::XMFLOAT3 speed;
    float time;
    int count = 3;
};

