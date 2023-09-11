#pragma once

#include "../Graphics/Model.h"
#include "../Game/ProjectileManager.h"

// �O���錾
class ProjectileIconManager;

class ProjectileIcon
{
public:
    ProjectileIcon(ProjectileIconManager* manager);
    virtual ~ProjectileIcon() {}

    virtual void Initialize();                      // ������
    virtual void Finalize();                        // �I����
    virtual void Begin();                           // ���t���[����ԍŏ��ɌĂ΂��
    virtual void Update(const float& elapsedTime);  // �X�V����
    virtual void End();                             // ���t���[����ԍŌ�ɌĂ΂��
    virtual void Render(const float& scale);        // �`�揈��
    virtual void DrawDebug();                       // ImGui�p

public:
    void Destroy(); // �j��

public: // �擾�E�ݒ�@�֘A
    Transform* GetTransform() { return model_->GetTransform(); }

    float GetRadius()const { return radius_; }   // ���a�擾
    void SetRadius(float r) { radius_ = r; }     // ���a�ݒ�

public:
    std::unique_ptr<Model> model_ = nullptr;
    ProjectileManager      projectileManager_   = {};

    float offsetX_      = 0.0f;
    float shitLeft_     = 0.0f;
    float shitRight_    = 0.0f;

    float setLifeTime_  = 4.0f;     // �e�ۂɐݒ�
    float setSpeed_     = 20.0f;    // �e�ۂɐݒ�

protected:
    float launchTimer_ = 0.0f;

private:
    ProjectileIconManager* manager_             = nullptr;

    float radius_      = 0.5f;

public: // --- ImGui�p --- //
    const char* GetName() const { return name_.c_str(); }
    void SetName(std::string n) { name_ = n; }
    static int nameNum_;

private: // --- ImGui�p --- //
    std::string name_ = {};
};