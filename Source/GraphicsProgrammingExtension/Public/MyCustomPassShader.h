#pragma once

#include "GlobalShader.h"
#include "ShaderParameters.h"
#include "ShaderParameterStruct.h"

class FMyCustomPassVS : public FGlobalShader
{
    DECLARE_GLOBAL_SHADER(FMyCustomPassVS);

public:
    FMyCustomPassVS() = default;
    FMyCustomPassVS(const ShaderMetaType::CompiledShaderInitializerType& Initializer)
        : FGlobalShader(Initializer) {}

    static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters)
    {
        return IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM5);
    }
};

class FMyCustomPassPS : public FGlobalShader
{
    DECLARE_GLOBAL_SHADER(FMyCustomPassPS);
    SHADER_USE_PARAMETER_STRUCT(FMyCustomPassPS, FGlobalShader);

public:
    static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters)
    {
        return IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM5);
    }

    BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
        SHADER_PARAMETER_STRUCT_REF(FViewUniformShaderParameters, View)
        SHADER_PARAMETER(FVector3f, MyColor)
        RENDER_TARGET_BINDING_SLOTS()
    END_SHADER_PARAMETER_STRUCT()
};

void RenderMyCustomPassShader(FRHICommandList& RHICmdList, ERHIFeatureLevel::Type FeatureLevel, FMyCustomPassPS::FParameters& PassParameters);
