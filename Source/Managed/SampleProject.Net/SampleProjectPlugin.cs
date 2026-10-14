using Urho3D.Core;
using Urho3D.IO;
using Urho3D.Plugins;

namespace SampleProject.Net;

/// <summary>
/// The managed plugin of the sample project. The C++ launcher loads it with <c>DotNetHost::LoadPlugin</c>.
/// <see cref="ManagedPlugin"/> registers <see cref="Greeter"/> on load. The plugin also uses the bound C++ types
/// <see cref="SampleProjectApi.PlayerController"/> and <see cref="SampleProjectApi.GameSaveData"/> and logs a description of them.
/// </summary>
public partial class SampleProjectPlugin : ManagedPlugin
{
    public SampleProjectPlugin(Context context)
        : base(context)
    {
    }

    protected override void Load()
    {
        base.Load();
        Log.Info("C# plugin SampleProject.Net is loaded.");

        // The C++ plugin App.Main registers the factory of PlayerController. A second registration from here
        // would fail with a duplicate reflection error, so the plugin only uses the bound types.
        Log.Info(UserCodeReport.Describe(GetContext()));
    }
}
