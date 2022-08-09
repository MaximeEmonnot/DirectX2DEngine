cbuffer VariableBuffer : register(b0)
{
	matrix translation;
	matrix rotation;
	matrix scale;
}

struct VertexInputType
{
	float3 position : POSITION;
	float2 uv : TEXCOORD0;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD0;
};

PixelInputType main( VertexInputType input )
{
	PixelInputType output;

	output.position = float4(input.position, 1.f);

	output.position = mul(output.position, scale);

	output.uv = input.uv;

	return output;
}