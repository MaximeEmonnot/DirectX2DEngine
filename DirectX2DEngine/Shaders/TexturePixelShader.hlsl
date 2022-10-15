Texture2D shaderTexture;
SamplerState SampleType;

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD0;
};

float4 main(PixelInputType input) : SV_TARGET
{
	return shaderTexture.Sample(SampleType, input.uv);
}