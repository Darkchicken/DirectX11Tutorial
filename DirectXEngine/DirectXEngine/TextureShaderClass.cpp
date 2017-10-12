#include "TextureShaderClass.h"



TextureShaderClass::TextureShaderClass()
{
	m_vertexShader = 0;
	m_pixelShader = 0;
	m_layout = 0;
	m_matrixBuffer = 0;
	m_sampleState = 0;
}

TextureShaderClass::TextureShaderClass(const TextureShaderClass& other)
{
}


TextureShaderClass::~TextureShaderClass()
{
}

bool TextureShaderClass::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;

	//Initialize the vertex and pixel shaders
	result = initializeShader(device, hwnd, L"../DirectXEngine/texture.vs", L"../DirectXEngine/texture.ps");
	if (!result)
	{
		return false;
	}

	return true;
}
void TextureShaderClass::Shutdown()
{
	//Shutdown the vertex and pixel shaders as well as the related objects
	shutdownShader();
	return;
}
bool TextureShaderClass::Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, 
	XMMATRIX viewMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture)
{
	bool result;

	//Set the shader parameters that it will use for rendering
	result = setShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, texture);
	if (!result)
	{
		return false;
	}

	//Now render the prepared buffers with the shader
	renderShader(deviceContext, indexCount);

	return true;
}


bool TextureShaderClass::initializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*)
{

}
void TextureShaderClass::shutdownShader()
{

}
void TextureShaderClass::outputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*)
{

}

bool TextureShaderClass::setShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*)
{

}
void TextureShaderClass::renderShader(ID3D11DeviceContext*, int)
{

}
