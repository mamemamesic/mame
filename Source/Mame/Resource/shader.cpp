#include "shader.h"
#include "../Graphics/Camera.h"

#include <fstream>

#include "../Other/misc.h"

#include "skinned_mesh.h"

#include "../Graphics/Graphics.h"

HRESULT CreateVsFromCso(ID3D11Device* device, const char* cso_name, ID3D11VertexShader** vertex_shader,
    ID3D11InputLayout** input_layout, D3D11_INPUT_ELEMENT_DESC* input_element_desc, UINT num_elements)
{
    FILE* fp{ nullptr };
    fopen_s(&fp, cso_name, "rb");
    _ASSERT_EXPR_A(fp, "CSO File not found");

    fseek(fp, 0, SEEK_END);
    long cso_sz{ ftell(fp) };
    fseek(fp, 0, SEEK_SET);

    std::unique_ptr<unsigned char[]>cso_data{ std::make_unique<unsigned char[]>(cso_sz) };
    fread(cso_data.get(), cso_sz, 1, fp);
    fclose(fp);

    HRESULT hr{ S_OK };
    hr = device->CreateVertexShader(cso_data.get(), cso_sz, nullptr, vertex_shader);
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    if (input_layout)
    {
        hr = device->CreateInputLayout(input_element_desc, num_elements,
            cso_data.get(), cso_sz, input_layout);
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
    }
    
    return hr;
}

HRESULT CreatePsFromCso(ID3D11Device* device, const char* cso_name, ID3D11PixelShader** pixel_shader)
{
    FILE* fp{ nullptr };
    fopen_s(&fp, cso_name, "rb");
    _ASSERT_EXPR_A(fp, "CSO File not found");

    fseek(fp, 0, SEEK_END);
    long cso_sz{ ftell(fp) };
    fseek(fp, 0, SEEK_SET);

    std::unique_ptr<unsigned char[]>cso_data{ std::make_unique<unsigned char[]>(cso_sz) };
    fread(cso_data.get(), cso_sz, 1, fp);
    fclose(fp);

    HRESULT hr{ S_OK };
    hr = device->CreatePixelShader(cso_data.get(), cso_sz, nullptr, pixel_shader);
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    return hr;
}

Shader::Shader(ID3D11Device* device)
{
    // 定数バッファ
    {
        HRESULT hr{ S_OK };

        D3D11_BUFFER_DESC bufferDesc{};
        bufferDesc.ByteWidth = sizeof(SceneConstants);
        bufferDesc.Usage = D3D11_USAGE_DEFAULT;
        bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        bufferDesc.CPUAccessFlags = 0;
        bufferDesc.MiscFlags = 0;
        bufferDesc.StructureByteStride = 0;
        // SceneConstants
        hr = device->CreateBuffer(&bufferDesc, nullptr,
            sceneConstantBuffer[static_cast<UINT>(CONSTANT_BUFFER::SCENE_CONSTANT)].GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


        bufferDesc.ByteWidth = sizeof(CBParametric);
        // CBParametric
        hr = device->CreateBuffer(&bufferDesc, nullptr,
            sceneConstantBuffer[static_cast<UINT>(CONSTANT_BUFFER::CONSTANT_BUFFER_PARAMETRIC)].GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


        // POINT_LIGHT
        bufferDesc.ByteWidth = sizeof(LightConstants);
        // LightConstants
        hr = device->CreateBuffer(&bufferDesc, nullptr,
            sceneConstantBuffer[static_cast<UINT>(CONSTANT_BUFFER::LIGHT_CONSTANT)].GetAddressOf());

    }

    // ブレンドステート
    {
        HRESULT hr{ S_OK };

        D3D11_BLEND_DESC blend_desc{};
        // ピクセルをレンダーターゲットに設定するときに、
        //マルチサンプリング手法としてアルファ隊カバレッジを使用するかどうかを指定します
        blend_desc.AlphaToCoverageEnable = FALSE;
        // 同時レンダーターゲットで独立したブレンドを有効にするかどうかを設定します。
        // 独立したブレンドを有効にするには、TUREに設定します
        // FALSEに設定すると、RenderTarget[0]メンバーのみが使用されます。RenderTarget[1..7]は無視されます
        blend_desc.IndependentBlendEnable = FALSE;
        // D3D11_RENDER_TARGET_BLEND_DESC[8]
        blend_desc.RenderTarget[0].BlendEnable = TRUE;
        blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
        blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
        blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
        blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
        blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
        blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
        blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
        hr = device->CreateBlendState(&blend_desc, blendState.GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
    }

    // 深度ステンシルステート
    {
        HRESULT hr{ S_OK };

        D3D11_DEPTH_STENCIL_DESC depth_stencil_desc{};
        // 0 深度テスト：オン　深度ライト：オン
        depth_stencil_desc.DepthEnable = TRUE;	// 深度テストを有効にします
        depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;	// 深度データで変更できる深度ステンシルバッファーの一部を特定します
        depth_stencil_desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL; // 深度データと既存データを比較する関数
        hr = device->CreateDepthStencilState(&depth_stencil_desc, depthStencilState[0].GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

        // 1 深度テスト：オン　深度ライト：オフ
        depth_stencil_desc.DepthEnable = TRUE;
        depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
        hr = device->CreateDepthStencilState(&depth_stencil_desc, depthStencilState[1].GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

        // 2 深度テスト：オフ　深度ライト：オン
        depth_stencil_desc.DepthEnable = FALSE;
        depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        hr = device->CreateDepthStencilState(&depth_stencil_desc, depthStencilState[2].GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

        // 3 深度テスト：オフ　深度ライト：オフ
        depth_stencil_desc.DepthEnable = FALSE;
        depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
        hr = device->CreateDepthStencilState(&depth_stencil_desc, depthStencilState[3].GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
    }

    // ラスタライザーステート
    {
        HRESULT hr{ S_OK };

        D3D11_RASTERIZER_DESC rasterizer_desc{};
        rasterizer_desc.FillMode = D3D11_FILL_SOLID;    // レンダリング時に使用するフィルモード
        rasterizer_desc.CullMode = D3D11_CULL_BACK;     // 指定した方向に面する三角形が描画されないことを示す
        rasterizer_desc.FrontCounterClockwise = TRUE;  // 三角形が表向きかを決定する
        rasterizer_desc.DepthBias = 0;                  // 指定されたピクセルに追加された深度値。
        rasterizer_desc.DepthBiasClamp = 0;             // ピクセルの最大深度バイアス
        rasterizer_desc.SlopeScaledDepthBias = 0;       // 指定されたピクセルの傾きのスカラー
        rasterizer_desc.DepthClipEnable = TRUE;         // 距離に基づいてクリッピングを有効にします
        rasterizer_desc.ScissorEnable = FALSE;          // ハサミ四角形カリングを有効にします。
        rasterizer_desc.MultisampleEnable = FALSE;      // マルチサンプリングアンチエイリアシング(MSAA)レンダーターゲットで
                                                        // 四角形またはアルファ線のアンチエイリアシングアルゴリズムを使用するかどうかを指定します
        rasterizer_desc.AntialiasedLineEnable = FALSE;  // 回線アンチエイリアシングを有効にするかどうか

        hr = device->CreateRasterizerState(&rasterizer_desc, rasterizerState[0].GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

        rasterizer_desc.CullMode = D3D11_CULL_NONE;
        hr = device->CreateRasterizerState(&rasterizer_desc, rasterizerState[3].GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

        rasterizer_desc.FillMode = D3D11_FILL_WIREFRAME;
        rasterizer_desc.CullMode = D3D11_CULL_BACK;
        rasterizer_desc.AntialiasedLineEnable = TRUE;
        hr = device->CreateRasterizerState(&rasterizer_desc, rasterizerState[1].GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

        rasterizer_desc.FillMode = D3D11_FILL_WIREFRAME;
        rasterizer_desc.CullMode = D3D11_CULL_NONE;
        rasterizer_desc.AntialiasedLineEnable = TRUE;
        hr = device->CreateRasterizerState(&rasterizer_desc, rasterizerState[2].GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
    }

    // サンプラステート
    {
        HRESULT hr{ S_OK };

        D3D11_SAMPLER_DESC samplerDesc{};
        samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
        samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.MipLODBias = 0;
        samplerDesc.MaxAnisotropy = 16;
        samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
        samplerDesc.BorderColor[0] = 0;
        samplerDesc.BorderColor[1] = 0;
        samplerDesc.BorderColor[2] = 0;
        samplerDesc.BorderColor[3] = 0;
        samplerDesc.MinLOD = 0;
        samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
        hr = device->CreateSamplerState(&samplerDesc, samplerState[static_cast<size_t>(SAMPLER_STATE::POINT)].GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

        samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        hr = device->CreateSamplerState(&samplerDesc, samplerState[static_cast<size_t>(SAMPLER_STATE::LINEAR)].GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

        samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
        samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        hr = device->CreateSamplerState(&samplerDesc, samplerState[static_cast<size_t>(SAMPLER_STATE::ANISOTROPIC)].GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

        samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
        samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
        samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
        samplerDesc.BorderColor[0] = 0;
        samplerDesc.BorderColor[1] = 0;
        samplerDesc.BorderColor[2] = 0;
        samplerDesc.BorderColor[3] = 0;
        hr = device->CreateSamplerState(&samplerDesc, samplerState[static_cast<size_t>(SAMPLER_STATE::LINEAR_BORDER_BLACK)].GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

        samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
        samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
        samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
        samplerDesc.BorderColor[0] = 1;
        samplerDesc.BorderColor[1] = 1;
        samplerDesc.BorderColor[2] = 1;
        samplerDesc.BorderColor[3] = 1;
        hr = device->CreateSamplerState(&samplerDesc, samplerState[static_cast<size_t>(SAMPLER_STATE::LINEAR_BORDER_WHITE)].GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

        // SHADOW
        samplerDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
        samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
        samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
        samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
        samplerDesc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL; // D3D11_COMPARISON_LESS_EQUAL
        samplerDesc.BorderColor[0] = 1;
        samplerDesc.BorderColor[1] = 1;
        samplerDesc.BorderColor[2] = 1;
        samplerDesc.BorderColor[3] = 1;
        hr = device->CreateSamplerState(&samplerDesc, samplerState[static_cast<size_t>(SAMPLER_STATE::COMPARISON_LINEAR_BORDER_WHITE)].GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
    }

    // LightConstants
    {
        Graphics& graphics = Graphics::Instance();
#ifdef _DEBUG
        // POINT_LIGHT
        pointLightModel = std::make_unique<Model>(graphics.GetDevice(),
            "./Resources/Model/Collision/sqhere.fbx");
#endif// _DEBUG

        // DIRECTION_LIGHT
        {
            lightConstant.directionLight.direction = DirectX::XMFLOAT4(0.0f, -1.0f, 0.0f, 0.0f);
            lightConstant.directionLight.color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
        }

        // POINT_LIGHT
        {
#if POINT_LIGHT_ONE
            lightConstant.pointLight.position = DirectX::XMFLOAT4(0.0f, 1.5f, 3.0f, 0.0f);
            lightConstant.pointLight.color = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
            lightConstant.pointLight.range = 5.0f;
#else
            for (int i = 0; i < pointLightMax; ++i)
            {
                lightConstant.pointLight[i].position=DirectX::XMFLOAT4(0.0f, 1.5f, 3.0f, 0.0f);
                lightConstant.pointLight[i].color = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
                lightConstant.pointLight[i].range = 5.0f;
            }
#endif// POINT_LIGHT_ONE
        }

        // SPOT_LIGHT
        {
            lightConstant.spotLight.position = DirectX::XMFLOAT4(0.0f, 1.0f, 15.0f, 0.0f);
            lightConstant.spotLight.color = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
            lightConstant.spotLight.direction = DirectX::XMFLOAT3(1.0f, -1.0f, 1.0f);

            lightConstant.spotLight.range = 10.0f;

            // 正規化
            DirectX::XMVECTOR spotLightDirectionVec = DirectX::XMLoadFloat3(&lightConstant.spotLight.direction);
            spotLightDirectionVec = DirectX::XMVector3Normalize(spotLightDirectionVec);
            DirectX::XMStoreFloat3(&lightConstant.spotLight.direction, spotLightDirectionVec);

            lightConstant.spotLight.angle = DirectX::XMConvertToRadians(-5.0f);
        }

        // HEMISPHERE_LIGHT
        {
            // 地面色
            lightConstant.hemisphereLight.groundColor.x = 0.7f;
            lightConstant.hemisphereLight.groundColor.y = 0.5f;
            lightConstant.hemisphereLight.groundColor.z = 0.3f;

            // 天球色
            lightConstant.hemisphereLight.skyColor.x = 0.15f;
            lightConstant.hemisphereLight.skyColor.y = 0.7f;
            lightConstant.hemisphereLight.skyColor.z = 0.95f;

            // 地面の法線を設定
            lightConstant.hemisphereLight.groundNormal.x = 0.0f;
            lightConstant.hemisphereLight.groundNormal.y = 1.0f;
            lightConstant.hemisphereLight.groundNormal.z = 0.0f;
        }
    }
}

void Shader::Initialize()
{
    Camera& camera = Camera::Instance();
    camera.Initialize();
}

void Shader::Begin(ID3D11DeviceContext* deviceContext, const RenderContext& rc)
{
    Camera& camera = Camera::Instance();

    // サンプラーステート
    deviceContext->PSSetSamplers(0, 1, samplerState[static_cast<size_t>(SAMPLER_STATE::POINT)].GetAddressOf());
    deviceContext->PSSetSamplers(1, 1, samplerState[static_cast<size_t>(SAMPLER_STATE::LINEAR)].GetAddressOf());
    deviceContext->PSSetSamplers(2, 1, samplerState[static_cast<size_t>(SAMPLER_STATE::ANISOTROPIC)].GetAddressOf());
    deviceContext->PSSetSamplers(3, 1, samplerState[static_cast<size_t>(SAMPLER_STATE::LINEAR_BORDER_BLACK)].GetAddressOf());
    deviceContext->PSSetSamplers(4, 1, samplerState[static_cast<size_t>(SAMPLER_STATE::LINEAR_BORDER_WHITE)].GetAddressOf());

    deviceContext->OMSetDepthStencilState(depthStencilState[0].Get(), 1);

    deviceContext->OMSetBlendState(blendState.Get(), nullptr, 0xFFFFFFFF);

    deviceContext->RSSetState(rasterizerState[0].Get());

    // ビュープロジェクション変換行列を計算し、それを定数バッファにセットする
    camera.SetPerspectiveFov(deviceContext);
        

    SceneConstants scene{};
    DirectX::XMStoreFloat4x4(&scene.viewProjection, camera.GetViewMatrix() * camera.GetProjectionMatrix());
    
    scene.lightDirection = { view.position.x,view.position.y,view.position.z,view.position.w };
    scene.cameraPosition = { view.camera.x,view.camera.y,view.camera.z,view.camera.w };
    deviceContext->UpdateSubresource(sceneConstantBuffer[0].Get(), 0, 0, &scene, 0, 0);
    deviceContext->VSSetConstantBuffers(1, 1, sceneConstantBuffer[0].GetAddressOf());
    deviceContext->PSSetConstantBuffers(1, 1, sceneConstantBuffer[0].GetAddressOf());

    // POINT_LIGHT
    {
        deviceContext->UpdateSubresource(sceneConstantBuffer[static_cast<UINT>(CONSTANT_BUFFER::LIGHT_CONSTANT)].Get(), 0, 0, &lightConstant, 0, 0);
        deviceContext->VSSetConstantBuffers(5, 1, sceneConstantBuffer[static_cast<UINT>(CONSTANT_BUFFER::LIGHT_CONSTANT)].GetAddressOf());
        deviceContext->PSSetConstantBuffers(5, 1, sceneConstantBuffer[static_cast<UINT>(CONSTANT_BUFFER::LIGHT_CONSTANT)].GetAddressOf());

#ifdef _DEBUG
#if POINT_LIGHT_ONE
        DirectX::XMFLOAT3 ptPos = { lightConstant.pointLight.position.x, lightConstant.pointLight.position.y, lightConstant.pointLight.position.z };
        pointLightModel->GetTransform()->SetPosition(ptPos);
        pointLightModel->Render(0.1f);
#endif// POINT_LIGHT_ONE
#endif// _DEBUG
    }


#ifdef USE_IMGUI
    //DrawDebug();
#endif
}

void Shader::Begin(ID3D11DeviceContext* deviceContext, const RenderContext& rc, const SceneConstants& sceneConstant)
{
    Camera& camera = Camera::Instance();

    // サンプラーステート
    {
        deviceContext->PSSetSamplers(0, 1, samplerState[static_cast<size_t>(SAMPLER_STATE::POINT)].GetAddressOf());
        deviceContext->PSSetSamplers(1, 1, samplerState[static_cast<size_t>(SAMPLER_STATE::LINEAR)].GetAddressOf());
        deviceContext->PSSetSamplers(2, 1, samplerState[static_cast<size_t>(SAMPLER_STATE::ANISOTROPIC)].GetAddressOf());
        deviceContext->PSSetSamplers(3, 1, samplerState[static_cast<size_t>(SAMPLER_STATE::LINEAR_BORDER_BLACK)].GetAddressOf());
        deviceContext->PSSetSamplers(4, 1, samplerState[static_cast<size_t>(SAMPLER_STATE::LINEAR_BORDER_WHITE)].GetAddressOf());
        // SHADOW
        deviceContext->PSSetSamplers(5, 1, samplerState[static_cast<size_t>(SAMPLER_STATE::COMPARISON_LINEAR_BORDER_WHITE)].GetAddressOf());
    }

    deviceContext->OMSetDepthStencilState(depthStencilState[0].Get(), 1);

    deviceContext->OMSetBlendState(blendState.Get(), nullptr, 0xFFFFFFFF);

    deviceContext->RSSetState(rasterizerState[0].Get());

    // ビュープロジェクション変換行列を計算し、それを定数バッファにセットする
    camera.SetPerspectiveFov(deviceContext);


    SceneConstants data{};
    DirectX::XMStoreFloat4x4(&data.viewProjection, camera.GetViewMatrix() * camera.GetProjectionMatrix());

    data.lightDirection = sceneConstant.lightDirection;
    data.cameraPosition = sceneConstant.cameraPosition;
    data.lightViewProjection = sceneConstant.lightViewProjection;
    deviceContext->UpdateSubresource(sceneConstantBuffer[0].Get(), 0, 0, &data, 0, 0);
    deviceContext->VSSetConstantBuffers(1, 1, sceneConstantBuffer[0].GetAddressOf());
    deviceContext->PSSetConstantBuffers(1, 1, sceneConstantBuffer[0].GetAddressOf());

    // POINT_LIGHT
    {
        deviceContext->UpdateSubresource(sceneConstantBuffer[static_cast<UINT>(CONSTANT_BUFFER::LIGHT_CONSTANT)].Get(), 0, 0, &lightConstant, 0, 0);
        deviceContext->VSSetConstantBuffers(5, 1, sceneConstantBuffer[static_cast<UINT>(CONSTANT_BUFFER::LIGHT_CONSTANT)].GetAddressOf());
        deviceContext->PSSetConstantBuffers(5, 1, sceneConstantBuffer[static_cast<UINT>(CONSTANT_BUFFER::LIGHT_CONSTANT)].GetAddressOf());

#ifdef _DEBUG
#if POINT_LIGHT_ONE
        DirectX::XMFLOAT3 ptPos = { lightConstant.pointLight.position.x, lightConstant.pointLight.position.y, lightConstant.pointLight.position.z };
        pointLightModel->GetTransform()->SetPosition(ptPos);
        pointLightModel->Render(0.1f);
#endif// POINT_LIGHT_ONE
#endif// _DEBUG
    }

}

void Shader::SetState(ID3D11DeviceContext* dc, int RastarizeState, int DepthStencilState, int SamplerState)
{
    dc->PSSetSamplers(SamplerState, 1, samplerState[SamplerState].GetAddressOf());
    dc->RSSetState(rasterizerState[RastarizeState].Get());
    dc->OMSetDepthStencilState(depthStencilState[DepthStencilState].Get(), 1);
}

void Shader::Draw(ID3D11DeviceContext* dc, Model* model)
{

}

void Shader::End(ID3D11DeviceContext* dc)
{
}

void Shader::DrawDebug()
{
    Camera& camera = Camera::Instance();

    ImGui::Begin("view");

    if (ImGui::TreeNode("camera"))
    {
        ImGui::SliderFloat("wight", &view.viewWitdh,1.0f,10000.0f);
        ImGui::SliderFloat("height", &view.viewHeight, 1.0f, 10000.0f);
        ImGui::SliderFloat("nearZ", &view.nearZ, 1.0f, 10000.0f);
        ImGui::SliderFloat("farZ", &view.farZ, 1.0f, 10000.0f);

        ImGui::DragFloat4("positon", &view.position.x);
        ImGui::DragFloat4("camera", &view.camera.x);

        ImGui::TreePop();
    }
    
    camera.DrawDebug();

    ImGui::End();

    if (ImGui::Begin("Light"))
    {
        // DIRECTION_LIGHT
        if (ImGui::TreeNode("DirectionLight"))
        {
            ImGui::DragFloat4("directiono", &lightConstant.directionLight.direction.x);
            ImGui::ColorEdit4("color", &lightConstant.directionLight.color.x);
            ImGui::TreePop();
        }

        // POINT_LIGHT
#if POINT_LIGHT_ONE
        if (ImGui::TreeNode("PointLight"))
        {
            ImGui::DragFloat4("position", &lightConstant.pointLight.position.x);
            ImGui::ColorEdit4("color", &lightConstant.pointLight.color.x);
            ImGui::DragFloat("range", &lightConstant.pointLight.range);
            ImGui::TreePop();
        }
#else
        for (int i = 0; i < pointLightMax; ++i)
        {
            ImGui::Begin(("pointLight" + std::to_string(i)).c_str());
            ImGui::DragFloat4("position", &lightConstant.pointLight[i].position.x);
            ImGui::ColorEdit4("color", &lightConstant.pointLight[i].color.x);
            ImGui::DragFloat("range", &lightConstant.pointLight[i].range);
            ImGui::End();
        }
        //if (ImGui::TreeNode("PointLight"))
        //{
        //    for (int i = 0; i < pointLightMax; ++i)
        //    {
        //        //ImGui::BeginMenu(("pointLight" + std::to_string(i)).c_str());
        //        ImGui::TreeNode(("pointLight" + std::to_string(i)).c_str());

        //        ImGui::DragFloat4("position", &lightConstant.pointLight[i].position.x);
        //        ImGui::ColorEdit4("color", &lightConstant.pointLight[i].color.x);
        //        ImGui::DragFloat("range", &lightConstant.pointLight[i].range);

        //        ImGui::TreePop();
        //        //ImGui::EndMenu();
        //    }
        //    ImGui::TreePop();
        //}
#endif// POINT_LIGHT_ONE

        // SPOT_LIGHT
        if (ImGui::TreeNode("SpotLight"))
        {
            ImGui::DragFloat4("position", &lightConstant.spotLight.position.x);
            ImGui::ColorEdit4("color", &lightConstant.spotLight.color.x);
            ImGui::DragFloat("range", &lightConstant.spotLight.range);
            ImGui::DragFloat3("direction", &lightConstant.spotLight.direction.x);
            float angle = DirectX::XMConvertToDegrees(lightConstant.spotLight.angle);
            ImGui::DragFloat("angle", &angle);
            lightConstant.spotLight.angle = DirectX::XMConvertToRadians(angle);
            ImGui::TreePop();
        }

        // HEMISPHERE_LIGHT
        if (ImGui::TreeNode("HemisphereLight"))
        {
            ImGui::ColorEdit4("groundColor", &lightConstant.hemisphereLight.groundColor.x);
            ImGui::ColorEdit4("skyColor", &lightConstant.hemisphereLight.skyColor.x);
            ImGui::DragFloat4("groundNormal", &lightConstant.hemisphereLight.groundNormal.x);
            ImGui::TreePop();
        }

        ImGui::End();
    }
}
