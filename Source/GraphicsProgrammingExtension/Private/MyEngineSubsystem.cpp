#include "MyEngineSubsystem.h"
#include "MySceneViewExtension.h"

void UMyEngineSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    UE_LOG(LogTemp, Log, TEXT("UMyEngineSubsystem initialized."));
	MySceneViewExtension = FSceneViewExtensions::NewExtension<FMySceneViewExtension>();
}

void UMyEngineSubsystem::Deinitialize()
{
    UE_LOG(LogTemp, Log, TEXT("UMyEngineSubsystem deinitialized."));
    
    // Safely clean up the scene view extension before releasing it.
	MySceneViewExtension->IsActiveThisFrameFunctions.Empty();
	// Check SceneViewExtensionContext.h for more details on this lambda function.
	FSceneViewExtensionIsActiveFunctor IsActiveFunctor;
	IsActiveFunctor.IsActiveFunction = [](const ISceneViewExtension* SceneViewExtension, const FSceneViewExtensionContext& Context)
	{
		return TOptional<bool>(false);
	};
	MySceneViewExtension->IsActiveThisFrameFunctions.Add(IsActiveFunctor);

	// Release the scene view extension.
	MySceneViewExtension.Reset();
}