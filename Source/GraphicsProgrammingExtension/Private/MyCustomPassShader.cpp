#include "MyCustomPassShader.h"

#include "PipelineStateCache.h"
#include "RenderUtils.h"
#include "RHIStaticStates.h"

IMPLEMENT_GLOBAL_SHADER(FMyCustomPassVS, "/Plugin/GraphicsProgrammingExtension/MyCustomPassShader.usf", "MainVS", SF_Vertex);
IMPLEMENT_GLOBAL_SHADER(FMyCustomPassPS, "/Plugin/GraphicsProgrammingExtension/MyCustomPassShader.usf", "MainPS", SF_Pixel);

void RenderMyCustomPassShader(FRHICommandList& RHICmdList, ERHIFeatureLevel::Type FeatureLevel, FMyCustomPassPS::FParameters& PassParameters)
{
    FGraphicsPipelineStateInitializer GraphicsPSOInit;
    RHICmdList.ApplyCachedRenderTargets(GraphicsPSOInit);

    GraphicsPSOInit.RasterizerState = TStaticRasterizerState<FM_Solid, CM_None>::GetRHI();
    // Per-MRT color write masks. We only write to RT2 (MSR) and RT3 (BaseColor).
    // Check enum EColorWriteMask in RHIDefinitions.h for more details.
    GraphicsPSOInit.BlendState = TStaticBlendState<
        CW_NONE, BO_Add, BF_One, BF_Zero, BO_Add, BF_One, BF_Zero, // RT0 (unused)
        CW_NONE, BO_Add, BF_One, BF_Zero, BO_Add, BF_One, BF_Zero, // RT1 (unused)
        CW_RG, BO_Add, BF_One, BF_Zero, BO_Add, BF_One, BF_Zero,   // RT2 MSR: only RG, preserve BA
        CW_RGB                                                     // RT3 BaseColor: only RGB, preserve A
    >::GetRHI();
    GraphicsPSOInit.DepthStencilState = TStaticDepthStencilState<true, CF_GreaterEqual>::GetRHI();

    FGlobalShaderMap* const ShaderMap = GetGlobalShaderMap(FeatureLevel);
    TShaderMapRef<FMyCustomPassVS> VertexShader(ShaderMap);
    TShaderMapRef<FMyCustomPassPS> PixelShader(ShaderMap);

    GraphicsPSOInit.BoundShaderState.VertexDeclarationRHI = GetVertexDeclarationFVector4();
    GraphicsPSOInit.BoundShaderState.VertexShaderRHI = VertexShader.GetVertexShader();
    GraphicsPSOInit.BoundShaderState.PixelShaderRHI = PixelShader.GetPixelShader();
    GraphicsPSOInit.PrimitiveType = PT_TriangleList;

    SetGraphicsPipelineState(RHICmdList, GraphicsPSOInit, 0);

    SetShaderParameters(RHICmdList, PixelShader, PixelShader.GetPixelShader(), PassParameters);

    // Single fullscreen triangle (3 vertices).
    RHICmdList.DrawPrimitive(0, 1, 1);
}