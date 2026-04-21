// Followed this repository to create a scene view extension:
// https://github.com/A57R4L/SceneViewExtensionTemplate
// Also referenced the guide in SceneViewExtension.h

#pragma once

#include "SceneViewExtension.h"

class FMySceneViewExtension : public FSceneViewExtensionBase
{
public:
    FMySceneViewExtension(const FAutoRegister& AutoRegister);

    virtual void PostRenderBasePassDeferred_RenderThread(FRDGBuilder& GraphBuilder, FSceneView& InView, const FRenderTargetBindingSlots& RenderTargets, TRDGUniformBufferRef<FSceneTextureUniformParameters> SceneTextures) override;
};
