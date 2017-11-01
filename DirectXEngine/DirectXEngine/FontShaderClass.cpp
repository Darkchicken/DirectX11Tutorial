#include "FontShaderClass.h"



FontShaderClass::FontShaderClass()
{
	m_vertexShader = 0;
	m_pixelShader = 0;
	m_layout = 0;
	m_constantBuffer = 0;
	m_sampleState = 0;
	m_pixelBuffer = 0;
}

FontShaderClass::FontShaderClass(const FontShaderClass& other)
{
}

FontShaderClass::~FontShaderClass()
{
}

bool FontShaderClass::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;

	//Initialize the vertex and pixel shaders
	result = initializeShader(device, hwnd, L"../DirectXEngine/font.vs", L"../DirectXEngine/font.ps");
	if (!result)
	{
		return false;
	}

	return true;

}
void FontShaderClass::Shutdown()
{
	//Shutdown the vertex and pixel shaders as well as the related objects

	shutdownShader();

	return;
}
bool FontShaderClass::Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, 
	XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, XMFLOAT4 pixelColor)
{
	bool result;

	//Set the shader parameters that it will use for rendering
	result = setShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, texture, pixelColor);
	if (!result)
	{
		return false;
	}

	//Now render the prepared buffers with the shader
	renderShader(deviceContext, indexCount);

	return true;
}


bool FontShaderClass::initializeShader(ID3D11Device*, HWND hwnd, WCHAR*, WCHAR*)
{

}
void FontShaderClass::shutdownShader()
{

}
void FontShaderClass::outputShaderErrorMessage(ID3D10Blob*, HWND, char*)
{

}

bool FontShaderClass::setShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*, XMFLOAT4)
{

}
void FontShaderClass::renderShader(ID3D11DeviceContext*, int)
{

}
