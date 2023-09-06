#include "Stage.h"
#include "../Graphics/Graphics.h"

// コンストラクタ
Stage::Stage()
{
    Graphics& graphics = Graphics::Instance();

    //model = std::make_unique<Model>(graphics.GetDevice(), "./Resources/Model/Stage/plane.fbx");
    model = std::make_unique<Model>(graphics.GetDevice(), "./Resources/Model/stage.fbx");
}

Stage::~Stage()
{
}

void Stage::Initialize()
{
    GetTransform()->SetScale(DirectX::XMFLOAT3(6.0f, 1.0f, 200.0f));
    //GetTransform()->SetScaleFactor(100.0f);
    GetTransform()->SetScaleFactor(1.0f);
}

void Stage::Finalize()
{
}

void Stage::Begin()
{
}

void Stage::Update(const float& elapsedTime)
{
}

void Stage::End()
{
}

void Stage::Render(const float& elapsedTime, const float& scale)
{
    model->Render(scale);
}

void Stage::DrawDebug()
{
#ifdef USE_IMGUI
    if (ImGui::BeginMenu("Stage"))
    {
        GetTransform()->DrawDebug();
        ImGui::EndMenu();
    }
#endif // USE_IMGUI
}
