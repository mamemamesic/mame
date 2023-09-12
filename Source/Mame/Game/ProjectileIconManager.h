#pragma once

#include "ProjectileIcon.h"
#include <vector>
#include <set>

class ProjectileIconManager
{
private:
    enum ColorTableLabel
    {
        Red,
        Yellow,
        Green,
        Blue,
        Purple,
        Black,
        IndexMax,
    };

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
    const int GetProjectileIconCount() const { return static_cast<int>(projectileIcons_.size()); } // �e�ې��擾

    ProjectileIcon* GetProjectileIcon(int index) { return projectileIcons_.at(index); } // �e�ێ擾

private:
    void AddProjectileSpeedAll();

public:
    static constexpr int PILE_UP_COUNT_MAX_ = 5; // �ςݏグ�ő吔

public:
    int pileUpCounter_  = 0; // �ςݏグ�����𐔂���
    int columnCounter_  = 0; // ��𐔂���

    int projectileIconRenderLimit_ = 25;

    DirectX::XMFLOAT3 projStraitePosition_ = {};
    DirectX::XMFLOAT3 projStraiteForward_  = {};

private:
    std::vector<ProjectileIcon*>  projectileIcons_ = {};
    std::set<ProjectileIcon*>     removes_ = {};

    DirectX::XMFLOAT4 colorTable_[ColorTableLabel::IndexMax] = {
        { 1,0,0,1 },
        { 1,1,0,1 },
        { 0,1,0,1 },
        { 0,0,1,1 },
        { 1,0,1,1 },
        { 0.1f, 0.1f, 0.1f, 1 },
    };
    int colorTableIndex_ = {};

    int colorChangeProjectileIconIndex_ = 0;

};