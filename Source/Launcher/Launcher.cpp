#include <Urho3D/Engine/EngineDefs.h>
#include <Urho3D/IO/FileSystem.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Plugins/PluginManager.h>

#include <Player/PlayerApplication.h>

#if SAMPLE_PROJECT_CSHARP
    #include <Urho3D/CSharp/DotNetHost.h>

    #include <filesystem>
    #include <string>
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
        const std::filesystem::path directory(GetSubsystem<FileSystem>()->GetProgramDir().c_str());

        DotNetHostOptions options;
        options.runtimeConfigPath = directory / (std::string(SHARPCPP_RBFX_PLUGIN_NAME) + ".runtimeconfig.json");
        std::string error;
        if (!host_.Initialize(options, error))
        {
            Fail(ea::string("Cannot start the .NET runtime: ") + error.c_str());
            return;
        }
        if (!host_.LoadPlugin(GetSubsystem<PluginManager>(), directory / (std::string(SHARPCPP_RBFX_PLUGIN_NAME) + ".dll"), error))
        {
            Fail(ea::string("Cannot load the C# plugin: ") + error.c_str());
            return;
        }
#endif
        PlayerApplication::Start();
    }

    void Stop() override
    {
        PlayerApplication::Stop();
#if SAMPLE_PROJECT_CSHARP
        host_.Close();
#endif
    }

#if SAMPLE_PROJECT_CSHARP
private:
    void Fail(const ea::string& message)
    {
        URHO3D_LOGERROR("{}", message);
        ErrorExit(message);
    }

    DotNetHost host_;
#endif
};

URHO3D_DEFINE_APPLICATION_MAIN(LauncherApplication);
