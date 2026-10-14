#pragma once

// Types of the sample that are bound to C# (see Source/Launcher/CMakeLists.txt). This header has only the includes that
// these types need, because the binding generator also reads everything else that a header includes or declares.

#include <Urho3D/Urho3D.h>
#include <Urho3D/IO/Archive.h>
#include <Urho3D/Math/Quaternion.h>
#include <Urho3D/Math/Vector3.h>
#include <Urho3D/Scene/LogicComponent.h>

using namespace Urho3D;

// The bindings library links App.Main, so these types need an export on Windows with a shared engine.
#ifdef URHO3D_STATIC
    #define SAMPLE_PROJECT_API
#else
    #if App_Main_EXPORT
        #define SAMPLE_PROJECT_API URHO3D_EXPORT_API
    #else
        #define SAMPLE_PROJECT_API URHO3D_IMPORT_API
    #endif
#endif

namespace SampleProjectApi
{

/// Player controller class.
class SAMPLE_PROJECT_API PlayerController : public LogicComponent
{
    URHO3D_OBJECT(PlayerController, LogicComponent);

public:
    explicit PlayerController(Context* context);

    /// Register object factory and attributes.
    static void RegisterObject(Context* context);

    void FixedUpdate(float timeStep) override;

private:
    float jumpInterval_{ 0.3f };
    float jumpCooldown_{};
};

/// Save file.
struct SAMPLE_PROJECT_API GameSaveData
{
    Vector3 actorPosition_;
    Quaternion actorRotation_;

    void SerializeInBlock(Archive& archive);
};

} // namespace SampleProjectApi
