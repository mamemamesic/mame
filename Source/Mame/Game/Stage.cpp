#include "Stage.h"
#include "../Graphics/Graphics.h"

// コンストラクタ
Stage::Stage()
{
    Graphics& graphics = Graphics::Instance();

    //model = std::make_unique<Model>(graphics.GetDevice(), "./Resources/Model/Stage/plane.fbx");
    model = std::make_unique<Model>(graphics.GetDevice(), "./Resources/Model/stage.fbx");
}

Stage::Stage(const char* filename)
{
    Graphics& graphics = Graphics::Instance();

    model = std::make_unique<Model>(graphics.GetDevice(), filename);
}

Stage::~Stage()
{
}

void Stage::Initialize()
{
    GetTransform()->SetPosition(DirectX::XMFLOAT3(0.0f, 0.0f, 25.0f));
    GetTransform()->SetScale(DirectX::XMFLOAT3(5.0f, 1.0f, 55.0f));
    //GetTransform()->SetScaleFactor(100.0f);
    GetTransform()->SetScaleFactor(1.0f);
    model->color = { 0.55f, 0.55f, 0.55f, 1.0f };
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

void Stage::Render(const float& scale)
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
