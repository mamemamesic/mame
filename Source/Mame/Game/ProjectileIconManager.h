#pragma once

#include "ProjectileIcon.h"
#include <vector>
#include <set>

class ProjectileIconManager
{
public:
    ProjectileIconManager();
    ~ProjectileIconManager();

    void Initialize();                                          // ������
    void Finalize();                                            // �I����
    void Begin();                                               // ���t���[����ԍŏ��ɌĂ΂��
    void Update(const float& elapsedTime);                      // �X�V����
    void End();                                                 // ���t���[����ԍŌ�ɌĂ΂��
    void Render(const float& scale);  // �`�揈��
    void DrawDebug();                                           // ImGui�p

public:
    void Register(ProjectileIcon* projectile);  // �o�^
    void Remove(ProjectileIcon* projectile);    // �폜
    void Clear();                           // �S�폜

public: // �擾�E�ݒ�@�֘A
    const int GetProjectileIconCount() const { return static_cast<int>(projectileIcons.size()); } // �e�ې��擾

    ProjectileIcon* GetProjectileIcon(int index) { return projectileIcons.at(index); } // �e�ێ擾

private:
    std::vector<ProjectileIcon*>  projectileIcons = {};
    std::set<ProjectileIcon*>     removes = {};
};