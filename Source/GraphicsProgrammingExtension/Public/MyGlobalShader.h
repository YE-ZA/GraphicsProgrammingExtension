// Used the most updated way to create global shaders.

#pragma once

#include "GlobalShader.h"
#include "ShaderParameters.h"
#include "ShaderParameterStruct.h"

class FMyGlobalVS : public FGlobalShader
{
    DECLARE_GLOBAL_SHADER(FMyGlobalVS);

public:
	FMyGlobalVS() = default;
	FMyGlobalVS(const ShaderMetaType::CompiledShaderInitializerType& Initializer)
		: FGlobalShader(Initializer) {}

    static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters)
	{
		return IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM5);
	}
};

class FMyGlobalPS : public FGlobalShader
{
    DECLARE_GLOBAL_SHADER(FMyGlobalPS);
	SHADER_USE_PARAMETER_STRUCT(FMyGlobalPS, FGlobalShader);

public:
    // SHADER_USE_PARAMETER_STRUCT will automatically declare the constructors.
	// Repeating the declaration will cause errors.

	static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters)
	{
		return IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM5);
	}

	static void ModifyCompilationEnvironment(const FGlobalShaderPermutationParameters& Parameters, FShaderCompilerEnvironment& OutEnvironment)
	{
		FGlobalShader::ModifyCompilationEnvironment(Parameters, OutEnvironment);
        // Equals to #define MY_DEFINE 1 in the shader file
        // In Shaders we can use
        // #if MY_DEFINE
        //     ...
        // #endif
        // to branch out based on different platforms.
		OutEnvironment.SetDefine(TEXT("MY_DEFINE"), 1);
	}

	BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
		// The second parameter must match the name defined in HLSL.
	    SHADER_PARAMETER(FVector3f, MyColor)
		SHADER_PARAMETER(float, MyAlpha)
	END_SHADER_PARAMETER_STRUCT()
};

void RenderMyGlobalShader(FRHICommandList& RHICmdList, ERHIFeatureLevel::Type FeatureLevel, const FVector3f& Color, float Alpha);
