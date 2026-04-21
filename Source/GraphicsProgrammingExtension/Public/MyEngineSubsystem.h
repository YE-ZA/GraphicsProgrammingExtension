// Subsystem to keep MySceneViewExtension alive.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/EngineSubsystem.h"
#include "MyEngineSubsystem.generated.h"

class FMySceneViewExtension;

UCLASS()
class UMyEngineSubsystem : public UEngineSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

private:
	TSharedPtr<FMySceneViewExtension, ESPMode::ThreadSafe> MySceneViewExtension;
};