#pragma once

#include "BaseScene.h"

#include "../Graphics/SkyBox.h"
#include "../Graphics/ShadowMap.h"
#include "../Resource/sprite.h"

#include "../Game/Stage.h"
#include <memory>

//#define SKY_BOX 1


class SceneGame : public Mame::Scene::BaseScene
{
public:
    SceneGame() {}
    ~SceneGame()override {}

    void CreateResource()override;                  // リソース生成
    void Initialize()   override;                   // 初期化
    void Finalize()     override;                   // 終了化
    void Begin()        override;                   // 毎フレーム一番最初に呼ばれる
    void Update(const float& elapsedTime) override; // 更新処理
    void End()          override;                   // 毎フレーム一番最後に呼ばれる
    void Render(const float& elapsedTime) override; // 描画処理

    void DrawDebug()    override;

public:
    enum class SAMPLER_STATE { POINT, LINEAR, ANISOTROPIC, LINEAR_BORDER_BLACK, LINEAR_BORDER_WHITE };
    Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerStates[5];

    enum class DEPTH_STATE { ZT_ON_ZW_ON, ZT_ON_ZW_OFF, ZT_OFF_ZW_ON, ZT_OFF_ZW_OFF };
    Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilStates[4];

    enum class BLEND_STATE { NONE, ALPHA, ADD, MULTIPLY };
    Microsoft::WRL::ComPtr<ID3D11BlendState> blendStates[4];

    enum class RASTER_STATE { SOLID, WIREFRAME, CULL_NONE, WIREFRAME_CULL_NONE };
    Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerStates[4];

    void SetStates();

private:
    bool isDebugCamera = false;

private:
#if SKY_BOX
    std::shared_ptr<Sprite> skyBoxSprite;
    std::unique_ptr<SkyBox> skyBox;
#endif// SKY_BOX

    // SHADOW
    Microsoft::WRL::ComPtr<ID3D11Buffer> SceneConstantBuffer;

    // SHADOW
    struct Shadow
    {
        const uint32_t shadowMapWidth = 2048;
        const uint32_t shadowMapHeight = 2048;
        std::unique_ptr<ShadowMap> shadowMap;
        DirectX::XMFLOAT4 lightViewFocus{ 0,0,0,1 };
        float lightViewDistance = 10.0f;
        float lightViewSize = 12.0f;
        float lightViewNearZ = 2.0f;
        float lightViewFarZ = 18.0f;
    } shadow;

private:
    std::unique_ptr<Stage> stage_;

    DirectX::XMFLOAT4 lightDirection_ = { 0, 1.0f, 0, 0 };
};

