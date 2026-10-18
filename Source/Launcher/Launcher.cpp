#include <Urho3D/Engine/EngineDefs.h>
#include <Urho3D/IO/FileSystem.h>

#include <Player/PlayerApplication.h>

#if SAMPLE_PROJECT_CSHARP
    #include <Urho3D/CSharp/DotNetHost.h>
#endif

using namespace Urho3D;

class LauncherApplication : public PlayerApplication
{
public:
    using PlayerApplication::PlayerApplication;

    void Setup() override
    {
        PlayerApplication::Setup();

#if MOBILE
        engineParameters_[EP_ORIENTATIONS] = "Portrait";
        engineParameters_[EP_RESOURCE_PREFIX_PATHS] = ";..;../..";
#else
        auto fs = context_->GetSubsystem<FileSystem>();
        engineParameters_[EP_RESOURCE_PREFIX_PATHS] = fs->GetProgramDir() + ";" + fs->GetCurrentDir() + "../";
#endif

        auto plugins = LinkedPlugins::GetLinkedPlugins();
#if SAMPLE_PROJECT_CSHARP
        // PlayerApplication::Start keeps only the plugins from EP_PLUGINS, so the managed plugin must be in the list.
        plugins.push_back(SHARPCPP_RBFX_PLUGIN_NAME);
#endif
        engineParameters_[EP_PLUGINS] = ea::string::joined(plugins, ";");
        LinkedPlugins::RegisterStaticPlugins();
    }

    void Start() override
    {
#if SAMPLE_PROJECT_CSHARP
        // The engine creates the plugin manager before Start().
        host_ = MakeShared<DotNetHost>(context_);
        if (!host_->Start(SHARPCPP_RBFX_PLUGIN_NAME))
        {
            ErrorExit(host_->GetLastError());
            return;
        }
#endif
        PlayerApplication::Start();
    }

    void Stop() override
    {
        PlayerApplication::Stop();
#if SAMPLE_PROJECT_CSHARP
        if (host_)
            host_->Stop();
#endif
    }

#if SAMPLE_PROJECT_CSHARP
private:
    SharedPtr<DotNetHost> host_;
#endif
};

URHO3D_DEFINE_APPLICATION_MAIN(LauncherApplication);
