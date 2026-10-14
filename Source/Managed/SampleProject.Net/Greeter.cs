using Urho3D.Core;
using Urho3D.IO;
using Urho3D.Scene;

namespace SampleProject.Net;

/// <summary>Small C# component. It writes one log line when it starts.</summary>
public partial class Greeter : LogicComponent
{
    /// <summary>Text of the log line. It is the engine attribute "Message".</summary>
    [SerializeField]
    public string Message { get; set; } = "Hello from the C# plugin of the sample project.";

    public Greeter(Context context)
        : base(context)
    {
    }

    public override void Start()
    {
        base.Start();
        Log.Info(Message);
    }
}
