// Followed this tutorial to create a plugin:
// https://medium.com/@manning.w27/advanced-graphics-programming-in-unreal-part-1-10488f2e17dd
// Also referenced the structure of NanoGS:
// https://github.com/TimChen1383/NanoGaussianSplatting

// Copyright Epic Games, Inc. All Rights Reserved.

#include "GraphicsProgrammingExtension.h"
#include "Interfaces/IPluginManager.h"
#include "RenderGraphBuilder.h"
#include "RenderGraphUtils.h"

#include "MyGlobalShader.h"

#define LOCTEXT_NAMESPACE "FGraphicsProgrammingExtensionModule"

//----------------------------------------------------------------------
// Console Variables for testing the graphics programming extension
//----------------------------------------------------------------------

static TAutoConsoleVariable<int32> CVarMyGlobalShaderTest(
	TEXT("r.MyGlobalShaderTest"),
	0,
	TEXT("0: off. 1/2/3: draw overlay with R/G/B channel enabled (alpha=1)."),
	ECVF_RenderThreadSafe
);

namespace
{
	FDelegateHandle GOverlayDelegateHandle;

	// FPostOpaqueRenderParameters is defined in RendererInterface.h.
	// It will be temporarily constructed every frame if the delegate is bound, check SceneRendering.cpp.
	// Both PostOpaqueRenderDelegate and OverlayRenderDelegate use this class to pass parameters to the callback function.
	void OnOverlayRender(FPostOpaqueRenderParameters& Parameters)
	{
		const int32 TestValue = CVarMyGlobalShaderTest.GetValueOnRenderThread();
		if (TestValue == 0)
		{
			return;
		}

		if (!Parameters.GraphBuilder || !Parameters.ColorTexture)
		{
			return;
		}

		FRDGBuilder& GraphBuilder = *Parameters.GraphBuilder;

		FRenderTargetParameters* PassParameters = GraphBuilder.AllocParameters<FRenderTargetParameters>();
		// Bind the Parameters.ColorTexture(Scene Color) to RenderTargets[0] that Unreal will draw to viewport by default.
		// ELoad means load the content of the ColorTexture, EClear means clear the ColorTexture to black.
		// Funny enough changing to RenderTargets[1] or Parameters.DepthTexture/NormalTexture will crash the engine.
		PassParameters->RenderTargets[0] = FRenderTargetBinding(Parameters.ColorTexture, ERenderTargetLoadAction::ELoad);

		const FIntRect ViewRect = Parameters.ViewportRect;
		const ERHIFeatureLevel::Type FeatureLevel = GMaxRHIFeatureLevel;

		const FVector3f Color(
			TestValue == 1 ? 1.0f : 0.0f,
			TestValue == 2 ? 1.0f : 0.0f,
			TestValue == 3 ? 1.0f : 0.0f
		);

		const float Alpha = TestValue / 3.0f;

		GraphBuilder.AddPass(
			RDG_EVENT_NAME("GraphicsProgrammingExtension_MyGlobalShaderOverlay"),
			PassParameters,
			ERDGPassFlags::Raster,
			[ViewRect, FeatureLevel, Color, Alpha](FRHICommandList& RHICmdList)
			{
				RHICmdList.SetViewport(
					(float)ViewRect.Min.X, (float)ViewRect.Min.Y, 0.0f,
					(float)ViewRect.Max.X, (float)ViewRect.Max.Y, 1.0f);

				RenderMyGlobalShader(RHICmdList, FeatureLevel, Color, Alpha);
			});
	}
}

void FGraphicsProgrammingExtensionModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	// Tell the engine where to find those custom shaders.
    FString ShaderDirectory = FPaths::Combine(IPluginManager::Get().FindPlugin(TEXT("GraphicsProgrammingExtension"))->GetBaseDir(), TEXT("Shaders"));
    AddShaderSourceDirectoryMapping("/Plugin/GraphicsProgrammingExtension", ShaderDirectory);

	// Register the overlay render delegate to render the MyGlobalShader.
	IRendererModule& RendererModule = FModuleManager::LoadModuleChecked<IRendererModule>(TEXT("Renderer"));
	GOverlayDelegateHandle = RendererModule.RegisterOverlayRenderDelegate(FPostOpaqueRenderDelegate::CreateStatic(&OnOverlayRender));
}

void FGraphicsProgrammingExtensionModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	// Unregister the overlay render delegate.
	if (GOverlayDelegateHandle.IsValid() && FModuleManager::Get().IsModuleLoaded(TEXT("Renderer")))
	{
		IRendererModule& RendererModule = FModuleManager::GetModuleChecked<IRendererModule>(TEXT("Renderer"));
		RendererModule.RemoveOverlayRenderDelegate(GOverlayDelegateHandle);
		GOverlayDelegateHandle.Reset();
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FGraphicsProgrammingExtensionModule, GraphicsProgrammingExtension)