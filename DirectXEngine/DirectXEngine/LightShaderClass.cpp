#include "LightShaderClass.h"



LightShaderClass::LightShaderClass()
{
	m_vertexShader = 0;
	m_pixelShader = 0;
	m_layout = 0;
	m_sampleState = 0;
	m_matrixBuffer = 0;

	m_lightBuffer = 0;
}

LightShaderClass::LightShaderClass(const LightShaderClass& other)
{
}

LightShaderClass::~LightShaderClass()
{
}

bool LightShaderClass::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;

	//Initialize the vertex and pixel shaders
	result = initializeShader(device, hwnd, L"../DirectXEngine/light.vs", L"../DirectXEngine/light.ps");
	if (!result)
	{
		return false;
	}
	return true;
}
void LightShaderClass::Shutdown()
{
	//Shutdown the vertex and pixel shaders as well as the related objects
	shutdownShader();

	return;
}
bool LightShaderClass::Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, 
	XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, XMFLOAT3 lightDirection, XMFLOAT4 diffuseColor)
{
	bool result;

	//Set the shader parameters that it will use for rendering
	result = setShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, texture, lightDirection, diffuseColor);
	if (!result)
	{
		return false;
	}
	
	//Now render the prepared buffers with the shader
	renderShader(deviceContext, indexCount);

	return true;
}


bool LightShaderClass::initializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*)
{

}
void LightShaderClass::shutdownShader()
{

}
void LightShaderClass::outputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*)
{

}

bool LightShaderClass::setShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*, XMFLOAT3, XMFLOAT4)
{

}
void LightShaderClass::renderShader(ID3D11DeviceContext*, int)
{

}
