#pragma once

#include "Projectile.h"

class ProjectileStraiteIcon : public Projectile
{
public:
    ProjectileStraiteIcon(ProjectileManager* manager);
    ~ProjectileStraiteIcon() {}

    void Initialize()                       override;   // ������
    void Finalize()                         override {} // �I����
    void Begin()                            override {} // ���t���[����ԍŏ��ɌĂ΂��
    void Update(const float& elapsedTime)   override;   // �X�V����
    void End()                              override {} // ���t���[����ԍŌ�ɌĂ΂��
    void Render(const float& scale)         override;   // �`�揈��
    void DrawDebug()                        override;   // ImGui�p
};

