#include "Stage.h"
#include "../Graphics/Graphics.h"

// �R���X�g���N�^
Stage::Stage()
{
    Graphics& graphics = Graphics::Instance();

    model = std::make_unique<Model>(graphics.GetDevice(), "./Resources/Model/Stage/stage.fbx");
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
    GetTransform()->SetScaleFactor(10.0f);
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
