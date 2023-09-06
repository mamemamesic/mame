#include "SceneGame.h"

#include "../Graphics/Graphics.h"
#include "../Graphics/EffectManager.h"
#include "../Graphics/Camera.h"

#include "../Input/Input.h"

#include "../Other/misc.h"

#include "../Game/PlayerManager.h"
#include "../Game/EnemyManager.h"
#include "../Game/EnemySlime.h"

#include "SceneManager.h"
#include "SceneLoading.h"
#include "SceneTitle.h"


// リソース生成
void SceneGame::CreateResource()
{
    Graphics& graphics = Graphics::Instance();

    // SkyBox
    {
#if SKY_BOX
        skyBoxSprite = std::make_shared<Sprite>(graphics.GetDevice(),
            L"./Resources/Image/SkyBox/Data/beautiful_sky.jpg");

        skyBox = std::make_unique<SkyBox>(graphics.GetDevice(), skyBoxSprite);
#endif// SKY_BOX
    }

    // SHADOW
    shadow.shadowMap = std::make_unique<ShadowMap>(graphics.GetDevice(),
        shadow.shadowMapWidth, shadow.shadowMapHeight);
    {
        HRESULT hr{ S_OK };
        D3D11_BUFFER_DESC desc{};
        desc.ByteWidth = sizeof(Shader::SceneConstants);
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        desc.CPUAccessFlags = 0;
        desc.MiscFlags = 0;
        desc.StructureByteStride = 0;
        hr = graphics.GetDevice()->CreateBuffer(&desc, nullptr, SceneConstantBuffer.GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
    }
}

// 初期化
void SceneGame::Initialize()
{
    PlayerManager& playerManager = PlayerManager::Instance();
    EnemyManager&  enemyManager  = EnemyManager::Instance();

    // 生成
    {
        // プレイヤー生成
        playerManager.GetPlayer() = std::make_unique<Player>();

        // 敵生成
        enemyManager.Register(new EnemySlime());
    }

    // 初期化
    {
        // カメラ初期化
        Camera::Instance().Initialize();

        // プレイヤー初期化
        playerManager.Initialize();

        // 敵初期化
        enemyManager.Initialize();
    }

}

// 終了化
void SceneGame::Finalize()
{
    PlayerManager& playerManager = PlayerManager::Instance();
    EnemyManager&  enemyManager  = EnemyManager::Instance();

    playerManager.Finalize();

    enemyManager.Finalize();
}

// Updateの前に呼び出される
void SceneGame::Begin()
{
    PlayerManager& playerManager = PlayerManager::Instance();
    EnemyManager&  enemyManager  = EnemyManager::Instance();

    playerManager.Begin();

    enemyManager.Begin();

}

// 更新処理
void SceneGame::Update(const float& elapsedTime)
{
    PlayerManager& playerManager = PlayerManager::Instance();
    EnemyManager&  enemyManager  = EnemyManager::Instance();

    const GamePad& gamePad = Input::Instance().GetGamePad();

    // タイトル遷移
    //if (gamePad.GetButtonDown() & GamePad::BTN_A)
    //{
    //    Mame::Scene::SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTitle));
    //    return;
    //}

    // 更新
    {

#ifdef _DEBUG
        // Debug用カメラ
        if (gamePad.GetButtonDown() & GamePad::BTN_X)isDebugCamera = isDebugCamera ? false : true;
        if (isDebugCamera)
        {
            int posX = 1980 / 2;
            int posY = 1080 / 2;

            POINT pos;
            GetCursorPos(&pos);

            DirectX::XMFLOAT2 nowPosition{ static_cast<float>(pos.x),static_cast<float>(pos.y) };
            DirectX::XMFLOAT2 oldPosition{ static_cast<float>(posX),static_cast<float>(posY) };
            DirectX::XMVECTOR nowVector = DirectX::XMLoadFloat2(&nowPosition);
            DirectX::XMVECTOR oldVector = DirectX::XMLoadFloat2(&oldPosition);
            DirectX::XMVECTOR moveVector = DirectX::XMVectorSubtract(nowVector, oldVector);
            DirectX::XMFLOAT2 moveVectorFloat2;
            DirectX::XMStoreFloat2(&moveVectorFloat2, moveVector);

            Camera::Instance().UpdateDebug(elapsedTime, moveVectorFloat2);

            SetCursorPos(posX, posY);
        }
        else
#else
        Camera::Instance().Update(); // カメラ更新
#endif // _DEBUG

        playerManager.Update(elapsedTime); // プレイヤー更新

        enemyManager.Update(elapsedTime);

        EffectManager::Instance().Update(elapsedTime); // エフェクト更新処理
    }

}

// Updateの後に呼び出される
void SceneGame::End()
{
    PlayerManager& playerManager = PlayerManager::Instance();
    EnemyManager&  enemyManager  = EnemyManager::Instance();

    playerManager.End();
    enemyManager.End();
}

// 描画処理
void SceneGame::Render(const float& elapsedTime)
{
    Graphics& graphics = Graphics::Instance();
    PlayerManager& playerManager = PlayerManager::Instance();
    EnemyManager&  enemyManager  = EnemyManager::Instance();

    // 描画の初期設定
    {
        ID3D11DeviceContext* deviceContext = graphics.GetDeviceContext();
        ID3D11RenderTargetView* renderTargetView = graphics.GetRenderTargetView();
        ID3D11DepthStencilView* depthStencilView = graphics.GetDepthStencilView();

        FLOAT color[]{ 0.2f, 0.2f, 1.0f, 1.0f };
        deviceContext->ClearRenderTargetView(renderTargetView, color);
        deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
        deviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

#if SKY_BOX
        skyBox->Render(graphics.GetDeviceContext(),
            Camera::Instance().GetViewMatrix(), Camera::Instance().GetProjectionMatrix());
#endif// SKY_BOX

        // SHADOW
        Shader::SceneConstants sceneConstant{};
        sceneConstant.lightDirection = graphics.GetShader()->view.position;
        sceneConstant.cameraPosition = graphics.GetShader()->view.camera;

        // SHADOW : make shadow map
        {
            const float aspectRatio = shadow.shadowMap->viewport.Width / shadow.shadowMap->viewport.Height;
            DirectX::XMVECTOR F =
            {
                DirectX::XMLoadFloat4(&shadow.lightViewFocus)
            };
            DirectX::XMVECTOR E =
            {
                DirectX::XMVectorSubtract(F,
                DirectX::XMVectorScale(
                    DirectX::XMVector3Normalize(
                        DirectX::XMLoadFloat4(&graphics.GetShader()->view.position)),shadow.lightViewDistance))
            };
            DirectX::XMVECTOR U =
            {
                DirectX::XMVectorSet(0.0f,1.0f,0.0f,0.0f)
            };
            DirectX::XMMATRIX V =
            {
                DirectX::XMMatrixLookAtLH(E,F,U)
            };
            DirectX::XMMATRIX P =
            {
                DirectX::XMMatrixOrthographicLH(shadow.lightViewSize * aspectRatio,
                shadow.lightViewSize,shadow.lightViewNearZ,shadow.lightViewFarZ)
            };

            DirectX::XMStoreFloat4x4(&sceneConstant.viewProjection, V * P);
            sceneConstant.lightViewProjection = sceneConstant.viewProjection;
            deviceContext->UpdateSubresource(SceneConstantBuffer.Get(), 0, 0, &sceneConstant, 0, 0);
            deviceContext->VSSetConstantBuffers(1, 1, SceneConstantBuffer.GetAddressOf());

            shadow.shadowMap->Clear(deviceContext, 1.0f);
            shadow.shadowMap->Activate(deviceContext);

            // SHADOW : 影つけたいモデルはここにRenderする
            {
                //PlayerManager::Instance().Render(elapsedTime, playerScaleFactor);
                //
                //enemySlime[0]->Render(elapsedTime, enemyScaleFactor);
                //enemySlime[1]->Render(elapsedTime, enemyScaleFactor);
            }

            shadow.shadowMap->Deactivete(deviceContext);
        }

        // カメラ関係
        RenderContext rc;
        rc.lightDirection = { 0.0f, -1.0f, 0.0f, 0.0f };

        Shader* shader = graphics.GetShader();
        shader->Begin(graphics.GetDeviceContext(), rc);
    }

    // MODEL_RENDER
    // 描画したいものは個々に書く
    {
        constexpr float playerScaleFactor = 0.01f;
        playerManager.Render(elapsedTime, playerScaleFactor);

        constexpr float enemyScaleFactor = 0.001f;
        enemyManager.Render(elapsedTime, enemyScaleFactor);
    }

    // 3Dエフェクト描画
    {
        Camera& camera = Camera::Instance();
        DirectX::XMFLOAT4X4 view, projection;
        DirectX::XMStoreFloat4x4(&view, camera.GetViewMatrix());
        DirectX::XMStoreFloat4x4(&projection, camera.GetProjectionMatrix());

        EffectManager::Instance().Render(view, projection);
    }

    // 3Dデバッグ描画
    {
        Camera& camera = Camera::Instance();
        DirectX::XMFLOAT4X4 view, projection;
        DirectX::XMStoreFloat4x4(&view, camera.GetViewMatrix());
        DirectX::XMStoreFloat4x4(&projection, camera.GetProjectionMatrix());

        // デバッグレンダラ描画実行
        graphics.GetDebugRenderer()->Render(graphics.GetDeviceContext(), view, projection);
    }

}

// debug用
void SceneGame::DrawDebug()
{
    Camera& camera = Camera::Instance();
    PlayerManager& playerManager = PlayerManager::Instance();
    EnemyManager&  enemyManager  = EnemyManager::Instance();
    // カメラ
    camera.DrawDebug();

    // プレイヤー
    playerManager.DrawDebug();

    // 敵
    enemyManager.DrawDebug();
}