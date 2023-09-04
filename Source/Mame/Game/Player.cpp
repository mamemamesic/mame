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

        const char* const fbxName = "./Resources/Model/Collision/sqhere.fbx";
        craSphere_.model_ = std::make_unique<Model>(graphics.GetDevice(), "./Resources/Model/Collision/sqhere.fbx");

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
    using DirectX::XMFLOAT3;
    using DirectX::XMFLOAT4;
    using DirectX::XMConvertToRadians;

    Transform* transform = GetTransform();

    Character::Update(elapsedTime); // �L�����N�^�[���ʂ̍X�V����

    Character::UpdateAnimation(elapsedTime); // �A�j���[�V�����X�V

    // �ړ�
    const GamePad& gamePad = Input::Instance().GetGamePad();
    {
        const float aLx = gamePad.GetAxisLX();
        const float aLy = gamePad.GetAxisLY();

        XMFLOAT3 pos = transform->GetPosition();
        constexpr float addPos = 3.0f;
        if (aLx != 0.0f) pos.x += ((aLx * addPos) * elapsedTime);
        if (aLy != 0.0f) pos.z += ((aLy * addPos) * elapsedTime);

        // �ʒu�X�V
        transform->SetPosition(pos);
    }

    // ��]�i�m�F�p�j
    {
        const float aRx = gamePad.GetAxisRX();
        const float aRy = gamePad.GetAxisRY();

        XMFLOAT4 rotation = transform->GetRotation();
        constexpr float addRotate = 90.0f;

        if (aRx != 0.0f)
        {
            const float addRotateY = XMConvertToRadians(aRx * addRotate);
            rotation.y += (addRotateY * elapsedTime);
        }
        if (aRy != 0.0f)
        {
            const float addRotateX = XMConvertToRadians(aRy * addRotate);
            rotation.x += (addRotateX * elapsedTime);
        }

        // ��]�l�X�V
        transform->SetRotation(rotation);
    }

    // �ߐڍU�����͏���
    if (InputCloseRangeAttack() == true) CloseRangeAttack();

    // �ߐڍU���X�V����
    UpdateCloseRangeAttack(elapsedTime);

}

// Update�̌�ɌĂ΂��
void Player::End()
{
}

// �`�揈��
void Player::Render(const float& elapsedTime, const float& scale)
{
    Character::Render(elapsedTime, scale);

    // �ߐڍU���p�̋��̕`��
#ifdef _DEBUG
    if (craSphere_.lifeTimer_ > 0.0f)
    {
        const float range = craSphere_.radius_ * 2.0f;
        craSphere_.model_->Render(range, 1);
    }
#endif // _DEBUG
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

    if (gamePad.GetButtonDown() & GamePad::BTN_A)
    {
        return true;
    }

    return false;
}

void Player::CloseRangeAttack()
{
    using DirectX::XMFLOAT3;

    const Transform* transform  = GetTransform();
    const XMFLOAT3   plPosition = transform->GetPosition();
    const XMFLOAT3   forward    = transform->CalcForward();

    const float radius = 0.25f;             // ���a
    const float length = (0.1f + radius);   // ���a�̑傫���ɂ���ċ��������������悤�ɂ���

    // �v���C���[�̑O��ւ�Ɉʒu��ݒ�
    const XMFLOAT3 spherePosition = {
        plPosition.x + (forward.x * length),
        plPosition.y + (forward.y * length) + offsetY,
        plPosition.z + (forward.z * length),
    };

    const float lifeTime = 0.5f;    // ��������

    // �ߐڍU���p�̋��̐���
    CreateCloseRangeAttackSphere(spherePosition, radius, lifeTime);
}

void Player::CreateCloseRangeAttackSphere(
    const DirectX::XMFLOAT3& pos,
    const float radius,
    const float lifeTime)
{
    craSphere_.model_->GetTransform()->SetPosition(pos);
    craSphere_.radius_    = radius;
    craSphere_.lifeTimer_ = lifeTime;
}

void Player::UpdateCloseRangeAttack(const float elapsedTime)
{
    if (craSphere_.lifeTimer_ <= 0.0f) return;

    craSphere_.lifeTimer_ -= elapsedTime;

    // ���̂ƓG�Ƃ̏Փˏ���
    //CollisionCRASphereVsEnemies();
}
