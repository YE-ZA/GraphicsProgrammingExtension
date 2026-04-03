// Followed this tutorial to create global shaders:
// https://dev.epicgames.com/documentation/en-us/unreal-engine/adding-global-shaders-to-unreal-engine
// Notice: many macros are outdated from the tutorial, but I will keep them for reference.

#pragma once

#include "GlobalShader.h"
#include "ShaderParameters.h"

class FMyGlobalVS : public FGlobalShader
{
	// OUTDATED
    // DECLARE_EXPORT_SHADER_TYPE(FMyGlobalVS, Global);
    // CURRENTLY USED
    DECLARE_GLOBAL_SHADER(FMyGlobalVS);

public:
	FMyGlobalVS() = default;
	FMyGlobalVS(const ShaderMetaType::CompiledShaderInitializerType& Initializer)
		: FGlobalShader(Initializer) {}

	// OUTDATED
    // static bool ShouldCache(EShaderPlatform Platform)
    // CURRENTLY USED - more flexible considering different platforms, feature levels, shader permutations, etc.
    static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters)
	{
		return true;
	}
};

class FMyGlobalPS : public FGlobalShader
{
	// OUTDATED
    // DECLARE_EXPORT_SHADER_TYPE(FMyGlobalPS, Global);
    // CURRENTLY USED
    DECLARE_GLOBAL_SHADER(FMyGlobalPS);

public:
	FMyGlobalPS() = default;
	FMyGlobalPS(const ShaderMetaType::CompiledShaderInitializerType& Initializer)
		: FGlobalShader(Initializer)
	{
		// SPF_Mandatory means the parameter is mandatory, if not set, the shader will not compile.
		MyColorParameter.Bind(Initializer.ParameterMap, TEXT("MyColor"), SPF_Mandatory);
		MyAlphaParameter.Bind(Initializer.ParameterMap, TEXT("MyAlpha"), SPF_Mandatory);
	}

	static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters)
	{
		return true;
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

	// Has to named as SetParameters with the first parameter is FRHIBatchedShaderParameters&, to be able to bind with SetShaderParametersLegacyPS.
	void SetParameters(FRHIBatchedShaderParameters& BatchedParameters, const FVector3f& Color, float Alpha)
	{
		SetShaderValue(BatchedParameters, MyColorParameter, Color);
		SetShaderValue(BatchedParameters, MyAlphaParameter, Alpha);
	}

protected:
	LAYOUT_FIELD(FShaderParameter, MyColorParameter);
	LAYOUT_FIELD(FShaderParameter, MyAlphaParameter);
};

void RenderMyGlobalShader(FRHICommandList& RHICmdList, ERHIFeatureLevel::Type FeatureLevel, const FVector3f& Color, float Alpha);
