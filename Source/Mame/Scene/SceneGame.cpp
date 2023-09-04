#include "SceneGame.h"

#include "../Graphics/Graphics.h"
#include "../Input/Input.h"

#include "SceneManager.h"
#include "SceneLoading.h"
#include "SceneTitle.h"

#include "../Graphics/Camera.h"
#include "../Game/PlayerManager.h"

void SceneGame::CreateResource()
{
}

// ������
void SceneGame::Initialize()
{
    PlayerManager& playerManager = PlayerManager::Instance();

    // ����
    {
        // �v���C���[����
        playerManager.GetPlayer() = std::make_unique<Player>();
    }

    // ������
    {
        // �J����������
        Camera::Instance().Initialize();

        // �v���C���[������
        playerManager.Initialize();
    }

}

// �I����
void SceneGame::Finalize()
{
    PlayerManager::Instance().Finalize();
}

// Update�̑O�ɌĂяo�����
void SceneGame::Begin()
{
    PlayerManager::Instance().Begin();
}

// �X�V����
void SceneGame::Update(const float& elapsedTime)
{
    const GamePad& gamePad = Input::Instance().GetGamePad();

    if (gamePad.GetButtonDown() & GamePad::BTN_A)
    {
        Mame::Scene::SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTitle));
        return;
    }

    // �X�V
    {
        Camera::Instance().Update(); // �J�����X�V

        PlayerManager::Instance().Update(elapsedTime); // �v���C���[�X�V
    }
}

// Update�̌�ɌĂяo�����
void SceneGame::End()
{
    PlayerManager::Instance().End();
}

// �`�揈��
void SceneGame::Render(const float& elapsedTime)
{
    Graphics& graphics = Graphics::Instance();
    ID3D11DeviceContext*    immediate_context  = graphics.GetDeviceContext();
    ID3D11RenderTargetView* render_target_view = graphics.GetRenderTargetView();
    ID3D11DepthStencilView* depth_stencil_view = graphics.GetDepthStencilView();

    FLOAT color[]{ 0.2f, 0.2f, 1.0f, 1.0f };
    immediate_context->ClearRenderTargetView(render_target_view, color);
    immediate_context->ClearDepthStencilView(depth_stencil_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    immediate_context->OMSetRenderTargets(1, &render_target_view, depth_stencil_view);

    RenderContext rc = {};
    rc.lightDirection = { 0.0f, -1.0f, 0.0f, 0.0f };

    Shader* shader = graphics.GetShader();
    shader->Begin(graphics.GetDeviceContext(), rc);

    constexpr float playerScaleFactor = 0.01f;
    PlayerManager::Instance().Render(elapsedTime, playerScaleFactor);
}

// debug�p
void SceneGame::DrawDebug()
{
    // �J����
    Camera::Instance().DrawDebug();

    PlayerManager::Instance().DrawDebug();
}