cbuffer VariableBuffer : register(b0)
{
	matrix translation;
	matrix rotation;
	matrix scale;
	float4 color;
}

struct VertexInputType
{
	float3 position : POSITION;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

PixelInputType main(VertexInputType input) 
{
	PixelInputType output;

	output.position = float4(input.position, 1.f);

	output.position = mul(output.position, scale);
	output.position = mul(output.position, rotation);
	//output.position = mul(translation, output.position);

	output.color = color;

	return output;
}