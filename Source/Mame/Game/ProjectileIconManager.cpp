#include "ProjectileIconManager.h"

// �R���X�g���N�^
ProjectileIconManager::ProjectileIconManager()
{
}

// �f�X�g���N�^
ProjectileIconManager::~ProjectileIconManager()
{
    Clear();
}

// ������
void ProjectileIconManager::Initialize()
{
    for (ProjectileIcon*& projectileIcon : projectileIcons_)
    {
        projectileIcon->Initialize();
    }
}

// �I����
void ProjectileIconManager::Finalize()
{
    for (ProjectileIcon*& projectileIcon : projectileIcons_)
    {
        projectileIcon->Finalize();
    }
}

// Update�̑O�ɌĂ΂��
void ProjectileIconManager::Begin()
{
    for (ProjectileIcon*& projectileIcon : projectileIcons_)
    {
        projectileIcon->Begin();
    }
}

// �X�V����
void ProjectileIconManager::Update(const float& elapsedTime)
{
    // �X�V����
    for (ProjectileIcon*& projectile : projectileIcons_)
    {
        projectile->Update(elapsedTime);
    }

    // �j������
    {
        for (ProjectileIcon* projectileIcon : removes_)
        {
            // vector����v�f���폜����ꍇ�̓C�e���[�^�[�ō폜
            std::vector<ProjectileIcon*>::iterator it = std::find(projectileIcons_.begin(), projectileIcons_.end(), projectileIcon);

            // std::vector�ŊǗ�����Ă���v�f���폜����ɂ�erase()�֐����g�p����
            // (�j�����X�g�̃|�C���^����C�e���[�^�[���������Aerase�֐��ɓn��)
            if (it != projectileIcons_.end())
            {
                projectileIcons_.erase(it);
            }

            // �A�C�e���̔j��
            delete projectileIcon;
        }
        // �j�����X�g���N���A
        removes_.clear();
    }
}

// Update�̌�ɌĂ΂��
void ProjectileIconManager::End()
{
    for (ProjectileIcon*& projectileIcon : projectileIcons_)
    {
        projectileIcon->End();
    }
}

// �`�揈��
void ProjectileIconManager::Render(const float& scale)
{
    for (ProjectileIcon*& projectileIcon : projectileIcons_)
    {
        projectileIcon->Render(scale);
    }
}

// ImGui�p
void ProjectileIconManager::DrawDebug()
{
    for (ProjectileIcon*& projectileIcon : projectileIcons_)
    {
        projectileIcon->DrawDebug();
    }
}

// �o�^
void ProjectileIconManager::Register(ProjectileIcon* projectileIcon)
{
    projectileIcon->offsetX_ = (0.1f * columnCounter_); // �����ʒu���ɉ����ĉE�ɂ��炵�Ă���
    projectileIcons_.emplace_back(projectileIcon);

    // �c�ɂ�����x�ς񂾂番�񂳂��č��ɂ��炷�悤�ɂ���
    ++pileUpCounter_;
    if (pileUpCounter_ >= PILE_UP_COUNT_MAX_)
    {
        ++columnCounter_;   // ������Z

        AddProjectileSpeedAll();

        pileUpCounter_ = 0; // �ςݏグ�J�E���g�����Z�b�g
    }

}

// �폜
void ProjectileIconManager::Remove(ProjectileIcon* projectile)
{
    // �j�����X�g�ɒǉ�
    removes_.insert(projectile);
}

// �S�폜
void ProjectileIconManager::Clear()
{
    for (ProjectileIcon*& projectileIcon : projectileIcons_)
    {
        if (projectileIcon)
        {
            delete projectileIcon;
            projectileIcon = nullptr;
        }
    }
    projectileIcons_.clear();
    projectileIcons_.shrink_to_fit();  // vector�̗]���ȃ��������J������֐�(C++11)
}


void ProjectileIconManager::AddProjectileSpeedAll()
{
    // �e�ۃA�C�R���擾
    for (ProjectileIcon*& projectileIcon : projectileIcons_)
    {
        // ���x�㏸�E�����^�C�}�[����
        constexpr float addSpeed         = 10.0f;
        constexpr float multiplyLifeTime = 0.7f;
        projectileIcon->setSpeed_    += addSpeed;
        projectileIcon->setLifeTime_ *= multiplyLifeTime;
    }
}
