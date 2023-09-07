#pragma once

#include "../Graphics/Model.h"
#include "../Game/ProjectileManager.h"

// 前方宣言
class ProjectileIconManager;

class ProjectileIcon
{
public:
    ProjectileIcon(ProjectileIconManager* manager);
    virtual ~ProjectileIcon() {}

    virtual void Initialize();                      // 初期化
    virtual void Finalize();                        // 終了化
    virtual void Begin();                           // 毎フレーム一番最初に呼ばれる
    virtual void Update(const float& elapsedTime);  // 更新処理
    virtual void End();                             // 毎フレーム一番最後に呼ばれる
    virtual void Render(const float& scale);        // 描画処理
    virtual void DrawDebug();                       // ImGui用

public:
    void Destroy(); // 破棄

public: // 取得・設定　関連
    Transform* GetTransform() { return model_->GetTransform(); }

    float GetRadius()const { return radius_; }   // 半径取得
    void SetRadius(float r) { radius_ = r; }     // 半径設定

public:
    std::unique_ptr<Model> model_ = nullptr;
    ProjectileManager      projectileManager_   = {};

    float offsetX_      = 0.0f;
    float shitLeft_     = 0.0f;
    float shitRight_    = 0.0f;

    float setLifeTime_  = 4.0f;     // 弾丸に設定
    float setSpeed_     = 20.0f;    // 弾丸に設定

protected:
    float launchTimer_ = 0.0f;

private:
    ProjectileIconManager* manager_             = nullptr;

    float radius_      = 0.5f;

public: // --- ImGui用 --- //
    const char* GetName() const { return name_.c_str(); }
    void SetName(std::string n) { name_ = n; }
    static int nameNum_;

private: // --- ImGui用 --- //
    std::string name_ = {};
};