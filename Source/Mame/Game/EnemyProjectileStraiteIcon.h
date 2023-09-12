#pragma once

#include "ProjectileIcon.h"

class EnemyProjectileStraiteIcon : public ProjectileIcon
{
public:
    EnemyProjectileStraiteIcon(ProjectileIconManager* manager);
    ~EnemyProjectileStraiteIcon() {}

    void Initialize()                       override;   // ������
    void Finalize()                         override {} // �I����
    void Begin()                            override {} // ���t���[����ԍŏ��ɌĂ΂��
    void Update(const float& elapsedTime)   override;
    void End()                              override {} // ���t���[����ԍŌ�ɌĂ΂��
    void Render(const float& scale)         override;   // �`�揈��
    void DrawDebug()                        override;   // ImGui�p
private:
    float launchTime_ = 1.0f;

};