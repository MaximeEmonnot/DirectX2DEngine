cbuffer VariableBuffer : register(b0)
{
	float2 offset;
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

	output.position.x += offset.x;
	output.position.y += offset.y;

	output.color = color;

	return output;
}