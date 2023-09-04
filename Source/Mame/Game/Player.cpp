#include "Player.h"

#include "../Graphics/Graphics.h"
#include "../Input/Input.h"

#include "AbilityManager.h"
#include <memory>

// �R���X�g���N�^
Player::Player()
{
    Graphics& graphics = Graphics::Instance();

    // ���f������
    {
        model = std::make_unique<Model>(graphics.GetDevice(),
            "./Resources/Model/sotai.fbx");
            //"./Resources/Model/sanaModel/mameoall.fbx");
            //"./Resources/Model/testModel/nico.fbx");
    }
}

// �f�X�g���N�^
Player::~Player()
{
}

// ������
void Player::Initialize()
{
    Character::Initialize();

    // �ҋ@�A�j���[�V�����ɐݒ肵�Ă�
    Character::PlayAnimation(0, true);
}

// �I����
void Player::Finalize()
{
}

// Update�̑O�ɌĂ΂��
void Player::Begin()
{
}

// �X�V����
void Player::Update(const float& elapsedTime)
{
    Transform* transform = GetTransform();

    Character::Update(elapsedTime); // �L�����N�^�[���ʂ̍X�V����

    Character::UpdateAnimation(elapsedTime); // �A�j���[�V�����X�V

    const GamePad& gamePad = Input::Instance().GetGamePad();

    // �ړ�
    {
        const float aLx = gamePad.GetAxisLX();
        const float aLy = gamePad.GetAxisLY();

        DirectX::XMFLOAT3 pos = transform->GetPosition();
        if (aLx > 0.0f) pos.x += elapsedTime;
        if (aLx < 0.0f) pos.x -= elapsedTime;
        if (aLy > 0.0f) pos.z += elapsedTime;
        if (aLy < 0.0f) pos.z -= elapsedTime;

        // �ʒu�X�V
        transform->SetPosition(pos);
    }

    // �ߐڍU������
    if (InputCloseRangeAttack()) CloseRangeAttack();

}

// Update�̌�ɌĂ΂��
void Player::End()
{
}

// �`�揈��
void Player::Render(const float& elapsedTime, const float& scale)
{
    Character::Render(elapsedTime, scale);
}

// ImGui�p
void Player::DrawDebug()
{
#ifdef USE_IMGUI
    if (ImGui::BeginMenu("player"))
    {
        Character::DrawDebug();

        ImGui::EndMenu();
    }
#endif // USE_IMGUI
}


bool Player::InputCloseRangeAttack()
{
    const GamePad& gamePad = Input::Instance().GetGamePad();

    if (gamePad.GetButtonDown() & GamePad::BTN_START)
    {
        return true;
    }

    return false;
}

void Player::CloseRangeAttack()
{
    using DirectX::XMFLOAT3;

    const Transform* transform = GetTransform();
    const XMFLOAT3   forward   = transform->CalcForward();

    // �v���C���[�̑O��ւ�Ɉʒu��ݒ�
    XMFLOAT3 position = forward;
    position.z *= 1.5f;

    const float radius = 0.5f;      // ���a
    const float lifeTime = 0.1f;    // ��������

    CreateCloseRangeAttackSphere(position, radius, lifeTime); // �ߐڍU���p�̋��̐���
}

void Player::CreateCloseRangeAttackSphere(
    const DirectX::XMFLOAT3& pos,
    const float radius,
    const float lifeTime)
{
    craSphere_.position_  = pos;
    craSphere_.radius_    = radius;
    craSphere_.lifeTimer_ = lifeTime;
}

void Player::UpdateCRASphere(const float elapsedTime)
{
    if (craSphere_.lifeTimer_ <= 0.0f) return;

    craSphere_.lifeTimer_ -= elapsedTime;

    //CollisionCRASphereVsEnemies();
}
