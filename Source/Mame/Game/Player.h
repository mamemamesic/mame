#pragma once

#include "Character.h"
#include <memory>

class Player : public Character
{
public: // enum�֘A
    // �X�e�[�g
    enum class State
    {
        Idle,   // �ҋ@
        Walk,   // ���s
    };

private: // enum�֘A

    // �A�j���[�V����
    enum class Animation
    {
    };

    // �ߐڍU���p�̋��̍\����
    struct CloseRangeAttackSphere
    {
        std::unique_ptr<Model> model_ = {};  // ���f�����
        float radius_    = 0.0f;             // ���a
        float lifeTimer_ = 0.0f;             // �����^�C�}�[
    };

public:
    Player();
    ~Player() override;

    void Initialize() override;                     // ������
    void Finalize();                                // �I����

    void Begin();                                   // ���t���[����ԍŏ��ɌĂ΂��
    void Update(const float& elapsedTime) override; // �X�V����
    void End();                                     // ���t���[����ԍŌ�ɌĂ΂��

    void Render(const float& elapsedTime, const float& scale) override; // �`�揈��

    void DrawDebug() override;  // ImGui�p

    bool InputCloseRangeAttack();   // �ߐڍU������
    void CloseRangeAttack();        // �ߐڍU��

    // �ߐڍU���p�̋��̐���
    void CreateCloseRangeAttackSphere(
        const DirectX::XMFLOAT3& pos,
        const float radius,
        const float lifeTime
    );

    void UpdateCloseRangeAttack(const float elapsedTime);

private:
    CloseRangeAttackSphere craSphere_ = {}; // �ߐڍU���p�̋���

    float offsetY = 0.5f;

};

