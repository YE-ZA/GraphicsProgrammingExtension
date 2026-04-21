#include "MySceneViewExtension.h"
#include "MyCustomPassShader.h"
#include "RenderGraphBuilder.h"

extern TAutoConsoleVariable<int32> CVarMySceneViewExtensionTest;

FMySceneViewExtension::FMySceneViewExtension(const FAutoRegister& AutoRegister) : FSceneViewExtensionBase(AutoRegister)
{
    UE_LOG(LogTemp, Log, TEXT("FMySceneViewExtension registered."));
}

void FMySceneViewExtension::PostRenderBasePassDeferred_RenderThread(FRDGBuilder& GraphBuilder, FSceneView& InView, const FRenderTargetBindingSlots& RenderTargets, TRDGUniformBufferRef<FSceneTextureUniformParameters> SceneTextures)
{
    const int32 TestValue = CVarMySceneViewExtensionTest.GetValueOnRenderThread();
    if (TestValue == 0)
    {
        return;
    }

    const FIntRect ViewRect = InView.UnscaledViewRect;
    const ERHIFeatureLevel::Type FeatureLevel = InView.GetFeatureLevel();

    // Allocate & fill the RDG pass parameters. Reuse the GBuffer MRT / depth already bound by the base pass.
    FMyCustomPassPS::FParameters* PassParameters = GraphBuilder.AllocParameters<FMyCustomPassPS::FParameters>();
    PassParameters->View = InView.ViewUniformBuffer;
    PassParameters->MyColor = FVector3f(0.0f, 1.0f, 1.0f);
    PassParameters->RenderTargets = RenderTargets;

    // We write SV_Depth, so we need depth-write access on the depth/stencil target.
    if (FRDGTextureRef DepthTexture = PassParameters->RenderTargets.DepthStencil.GetTexture())
    {
        PassParameters->RenderTargets.DepthStencil = FDepthStencilBinding(
            DepthTexture,
            ERenderTargetLoadAction::ELoad,
            ERenderTargetLoadAction::ELoad,
            FExclusiveDepthStencil::DepthWrite_StencilWrite);
    }

    GraphBuilder.AddPass(
        RDG_EVENT_NAME("MyCustomPass"),
        PassParameters,
        ERDGPassFlags::Raster,
        [ViewRect, FeatureLevel, PassParameters](FRHICommandList& RHICmdList)
        {
            RHICmdList.SetViewport(
                (float)ViewRect.Min.X, (float)ViewRect.Min.Y, 0.0f,
                (float)ViewRect.Max.X, (float)ViewRect.Max.Y, 1.0f);

            RenderMyCustomPassShader(RHICmdList, FeatureLevel, *PassParameters);
        });
}
