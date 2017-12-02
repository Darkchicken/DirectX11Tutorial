//Globals

cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

//Typedefs
struct VertexInputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
};

//Vertex shader
PixelInputType BumpMapVertexShader(VertexInputType input)
{
	PixelInputType output;
	
	// change the position of the vertex against the world, view, and projection matrices
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);
	
	//Store the texture coordinates for the pixel shader
	output.tex = input.tex;
	
	//calculate the nomal vector against the world matrix only, and then normalize the final value
	output.normal = mul(input.normal, (float3x3)worldMatrix);
	output.normal = normalize(output.normal);
	
	//calculate the tangent vector against the world matrix only, and then normalize the final value
	output.tangent = mul(input.tangent, (float3x3)worldMatrix);
	output.tangent = normalize(output.tangent);
	
	//calculate the binormal vector against the world matrix only, and then normalize the final value
	output.binormal = mul(input.binormal, (float3x3) worldMatrix);
	output.binormal = normalize(output.binormal);
	
	return output;
}