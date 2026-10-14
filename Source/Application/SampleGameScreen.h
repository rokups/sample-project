#pragma once

#include <Urho3D/Engine/StateManager.h>
#include <Urho3D/Graphics/Viewport.h>
#include <Urho3D/Scene/LogicComponent.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/UI/SplashScreen.h>

#include "SampleProjectApi.h"

using namespace Urho3D;

class SampleProject;

URHO3D_GLOBAL_CONSTANT(ConstString Category_SampleProject{ "Component/SampleProject" });

/// Screen with actual game.
class SampleGameScreen : public ApplicationState
{
    URHO3D_OBJECT(SampleGameScreen, ApplicationState);

public:
    explicit SampleGameScreen(Context* context);
    ~SampleGameScreen() override;

    static void RegisterObject(Context* context);

    /// Implement ApplicationState.
    /// @{
    void Activate(StringVariantMap& bundle) override;
    void Update(float timeStep) override;
    void Deactivate() override;
    /// @}

private:
    /// Save game.
    void SaveGame();
    /// Load game.
    void LoadGame();
    /// Handle key down
    void HandleKeyDown(StringHash eventType, VariantMap& eventData);
    /// Init scene.
    void InitScene();
    /// Get current application instance.
    SampleProject* GetApp() const;

    /// Main scene of the game.
    SharedPtr<Scene> scene_;

    /// Actor and camera nodes.
    SharedPtr<Node> actorNode_;
    SharedPtr<Node> cameraNode_;

    /// Autosave timer.
    float autosaveTimer_{};
};
