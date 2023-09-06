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
    for (ProjectileIcon*& projectile : projectileIcons)
    {
        projectile->Initialize();
    }
}

// �I����
void ProjectileIconManager::Finalize()
{
    for (ProjectileIcon*& projectile : projectileIcons)
    {
        projectile->Finalize();
    }
}

// Update�̑O�ɌĂ΂��
void ProjectileIconManager::Begin()
{
    for (ProjectileIcon*& projectile : projectileIcons)
    {
        projectile->Begin();
    }
}

// �X�V����
void ProjectileIconManager::Update(const float& elapsedTime)
{
    // �X�V����
    for (ProjectileIcon*& projectile : projectileIcons)
    {
        projectile->Update(elapsedTime);
    }

    // �j������
    {
        for (ProjectileIcon* projectile : removes)
        {
            // vector����v�f���폜����ꍇ�̓C�e���[�^�[�ō폜
            std::vector<ProjectileIcon*>::iterator it = std::find(projectileIcons.begin(), projectileIcons.end(), projectile);

            // std::vector�ŊǗ�����Ă���v�f���폜����ɂ�erase()�֐����g�p����
            // (�j�����X�g�̃|�C���^����C�e���[�^�[���������Aerase�֐��ɓn��)
            if (it != projectileIcons.end())
            {
                projectileIcons.erase(it);
            }

            // �A�C�e���̔j��
            delete projectile;
        }
        // �j�����X�g���N���A
        removes.clear();
    }
}

// Update�̌�ɌĂ΂��
void ProjectileIconManager::End()
{
    for (ProjectileIcon*& projectile : projectileIcons)
    {
        projectile->End();
    }
}

// �`�揈��
void ProjectileIconManager::Render(const float& scale)
{
    for (ProjectileIcon*& projectile : projectileIcons)
    {
        projectile->Render(scale);
    }
}

// ImGui�p
void ProjectileIconManager::DrawDebug()
{
    for (ProjectileIcon*& projectile : projectileIcons)
    {
        projectile->DrawDebug();
    }
}

// �o�^
void ProjectileIconManager::Register(ProjectileIcon* projectile)
{
    projectileIcons.emplace_back(projectile);
}

// �폜
void ProjectileIconManager::Remove(ProjectileIcon* projectile)
{
    // �j�����X�g�ɒǉ�
    removes.insert(projectile);
}

// �S�폜
void ProjectileIconManager::Clear()
{
    for (ProjectileIcon*& projectile : projectileIcons)
    {
        delete projectile;
    }
    projectileIcons.clear();
    projectileIcons.shrink_to_fit();  // vector�̗]���ȃ��������J������֐�(C++11)
}
