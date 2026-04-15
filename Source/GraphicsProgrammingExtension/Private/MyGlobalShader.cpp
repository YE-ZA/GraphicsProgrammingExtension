#include "MyGlobalShader.h"

#include "ClearQuad.h"
#include "PipelineStateCache.h"
#include "RenderUtils.h"
#include "RHIStaticStates.h"

IMPLEMENT_GLOBAL_SHADER(FMyGlobalVS, "/Plugin/GraphicsProgrammingExtension/MyGlobalShader.usf", "MainVS", SF_Vertex);
IMPLEMENT_GLOBAL_SHADER(FMyGlobalPS, "/Plugin/GraphicsProgrammingExtension/MyGlobalShader.usf", "MainPS", SF_Pixel);

void RenderMyGlobalShader(FRHICommandList& RHICmdList, ERHIFeatureLevel::Type FeatureLevel, const FVector3f& Color, float Alpha)
{
	FGraphicsPipelineStateInitializer GraphicsPSOInit;
	RHICmdList.ApplyCachedRenderTargets(GraphicsPSOInit);

    // FM_Solid means fill the polygon, CM_None means no culling.
    GraphicsPSOInit.RasterizerState = TStaticRasterizerState<FM_Solid, CM_None>::GetRHI();
    // CW_RGBA means Color Write Mask = RGBA, write it for 8 times because Unreal supports maximum 8 multiple render targets(MRT).
	// GraphicsPSOInit.BlendState = TStaticBlendStateWriteMask<CW_RGBA, CW_RGBA, CW_RGBA, CW_RGBA, CW_RGBA, CW_RGBA, CW_RGBA, CW_RGBA>::GetRHI();
    // Alpha blending: out = src * srcA + dst * (1 - srcA).
    GraphicsPSOInit.BlendState = TStaticBlendState<CW_RGBA, BO_Add, BF_SourceAlpha, BF_InverseSourceAlpha, BO_Add, BF_One, BF_InverseSourceAlpha>::GetRHI();
    // false means depth test is disabled, CF_Always means always pass the depth test.
    GraphicsPSOInit.DepthStencilState = TStaticDepthStencilState<false, CF_Always>::GetRHI();

	FGlobalShaderMap* ShaderMap = GetGlobalShaderMap(FeatureLevel);
	TShaderMapRef<FMyGlobalVS> VertexShader(ShaderMap);
	TShaderMapRef<FMyGlobalPS> PixelShader(ShaderMap);

    GraphicsPSOInit.BoundShaderState.VertexDeclarationRHI = GetVertexDeclarationFVector4();
	GraphicsPSOInit.BoundShaderState.VertexShaderRHI = VertexShader.GetVertexShader();
	GraphicsPSOInit.BoundShaderState.PixelShaderRHI = PixelShader.GetPixelShader();
	GraphicsPSOInit.PrimitiveType = PT_TriangleStrip;

	// Above this line are all about setting up the graphics pipeline state(PSO), now we finally apply it.
    SetGraphicsPipelineState(RHICmdList, GraphicsPSOInit, 0);

	FMyGlobalPS::FParameters Parameters;
	Parameters.MyColor = Color;
	Parameters.MyAlpha = Alpha;
	SetShaderParameters(RHICmdList, PixelShader, PixelShader.GetPixelShader(), Parameters);

	// GClearVertexBuffer is a global vertex buffer that is used to draw a full screen quad to clear the screen or do some post processing.
    RHICmdList.SetStreamSource(0, GClearVertexBuffer.VertexBufferRHI, 0);
	// The three params are the starting index, the number of primitives, and the number of instances.
    RHICmdList.DrawPrimitive(0, 2, 1);
}
