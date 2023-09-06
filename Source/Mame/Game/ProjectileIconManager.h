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
    void Register(ProjectileIcon* projectileIcon);  // �o�^
    void Remove(ProjectileIcon* projectile);    // �폜
    void Clear();                           // �S�폜

public: // �擾�E�ݒ�@�֘A
    const int GetProjectileIconCount() const { return static_cast<int>(projectileIcons.size()); } // �e�ې��擾

    ProjectileIcon* GetProjectileIcon(int index) { return projectileIcons.at(index); } // �e�ێ擾

public:
    static constexpr int PILE_UP_COUNT_MAX_ = 5; // �ςݏグ�ő吔

public:
    int pileUpCounter_  = 0; // �ςݏグ�����𐔂���
    int columnCounter_  = 0; // ��𐔂���

private:
    std::vector<ProjectileIcon*>  projectileIcons = {};
    std::set<ProjectileIcon*>     removes = {};

};