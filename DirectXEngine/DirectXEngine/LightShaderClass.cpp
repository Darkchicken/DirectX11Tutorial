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


bool LightShaderClass::initializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[3];
	unsigned int numElements;
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_BUFFER_DESC lightBufferDesc;

	//Initialize the pointers this function will use to null
	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

	//compile the vertex shader code
	result = D3DCompileFromFile(vsFilename, NULL, NULL, "LightVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 
		0, &vertexShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		//If the shader failed to compile, it should have written something to the error message
		if (errorMessage)
		{
			outputShaderErrorMessage(errorMessage, hwnd, vsFilename);
		}
		//If there was nothing in the error message, it simply could not find the shader file itself
		else
		{
			MessageBox(hwnd, vsFilename, L"Missing Shader File", MB_OK);
		}
		return false;
	}

	//compile the pixel shader code
	result = D3DCompileFromFile(psFilename, NULL, NULL, "LightPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 
		0, & pixelShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		//If the shader failed to compile, it should have written something to the error message
		if (errorMessage)
		{
			outputShaderErrorMessage(errorMessage, hwnd, psFilename);
		}
		//If there was nothing in the error message, it simply could not find the shader file itself
		else
		{
			MessageBox(hwnd, psFilename, L"Missing Shader File", MB_OK);
		}
		return false;
	}

	//Create the vertex shader from the buffer
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(),vertexShaderBuffer->GetBufferSize(),NULL, &m_vertexShader);
	if (FAILED(result))
	{
		return false;
	}

	//Create the pixel shader from the buffer
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
	if (FAILED(result))
	{
		return false;
	}

	//Create the vertex input layout description
	//This setup needs to match the VertexType structure in the ModelClass and in the shaders
	polygonLayout[0].SemanticName = "POSITION";
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
