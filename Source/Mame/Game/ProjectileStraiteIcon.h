#pragma once

#include "ProjectileIcon.h"

class ProjectileStraiteIcon : public ProjectileIcon
{
public:
    ProjectileStraiteIcon(ProjectileIconManager* manager);
    ~ProjectileStraiteIcon() {}

    void Initialize()                       override;   // ������
    void Finalize()                         override {} // �I����
    void Begin()                            override {} // ���t���[����ԍŏ��ɌĂ΂��
    void Update(const float& elapsedTime)   override;   // �X�V����
    void End()                              override {} // ���t���[����ԍŌ�ɌĂ΂��
    void Render(const float& scale)         override;   // �`�揈��
    void DrawDebug()                        override;   // ImGui�p
};

