#pragma once

#include "BaseScene.h"

#include "../Graphics/SkyBox.h"
#include "../Graphics/ShadowMap.h"
#include "../Resource/sprite.h"

#define SKY_BOX 1


class SceneGame : public Mame::Scene::BaseScene
{
public:
    SceneGame() {}
    ~SceneGame()override {}

    void CreateResource()override;                  // ���\�[�X����
    void Initialize()   override;                   // ������
    void Finalize()     override;                   // �I����
    void Begin()        override;                   // ���t���[����ԍŏ��ɌĂ΂��
    void Update(const float& elapsedTime) override; // �X�V����
    void End()          override;                   // ���t���[����ԍŌ�ɌĂ΂��
    void Render(const float& elapsedTime) override; // �`�揈��

    void DrawDebug()    override;

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
};

