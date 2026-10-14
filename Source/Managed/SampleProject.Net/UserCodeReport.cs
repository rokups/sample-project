using SampleProjectApi;
using Urho3D.Core;

namespace SampleProject.Net;

/// <summary>
/// Uses the C++ user code of the sample (<c>Source/Application/SampleProjectApi.h</c>) through its generated bindings.
/// </summary>
public static class UserCodeReport
{
    /// <summary>Describes a new native <c>PlayerController</c> and the default values of <c>GameSaveData</c>.</summary>
    public static string Describe(Context context)
    {
        using var player = new PlayerController(context);
        using var data = new GameSaveData();
        return $"User code: {player.GetTypeName()}, GameSaveData defaults: position {data.ActorPosition}, rotation {data.ActorRotation}. Plugin type: {typeof(global::Urho3D.SampleComponent).FullName}.";
    }
}
