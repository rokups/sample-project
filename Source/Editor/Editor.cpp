#include "CreateRandomBox.h"

#include <Editor/EditorApplication.h>

#if SAMPLE_PROJECT_CSHARP
#include "RbfxCSharpEditorHost.h"
#endif

using namespace Urho3D;

class CustomEditorApplication : public EditorApplication
{
public:
    using EditorApplication::EditorApplication;

    /// Implement EditorApplication.
    /// @{
    void Setup() override
    {
        editorPluginManager_->AddPlugin("SampleProject.CreateRandomBox", &Plugin_CreateRandomBox);

        EditorApplication::Setup();
        LinkedPlugins::RegisterStaticPlugins();
    }

#if SAMPLE_PROJECT_CSHARP
    void Start() override
    {
        // Start the .NET runtime before the editor creates its plugin manager.
        ea::string error;
        if (!dotNet_.Start(context_, SHARPCPP_RBFX_EDITOR_HOST_NAME, error))
        {
            ErrorExit(error);
            return;
        }
        EditorApplication::Start();
        dotNet_.OnEditorStarted();
    }

    void Stop() override
    {
        EditorApplication::Stop();
        dotNet_.Stop();
    }
#endif
    /// @}

#if SAMPLE_PROJECT_CSHARP
private:
    RbfxEditorDotNetHost dotNet_;
#endif
};

URHO3D_DEFINE_APPLICATION_MAIN(CustomEditorApplication);
