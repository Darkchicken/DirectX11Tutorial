#include "MultiTextureShaderClass.h"



MultiTextureShaderClass::MultiTextureShaderClass()
{
	m_vertexShader = 0;
	m_pixelShader = 0;
	m_layout = 0;
	m_matrixBuffer = 0;
	m_sampleState = 0;
}

MultiTextureShaderClass::MultiTextureShaderClass(const MultiTextureShaderClass& other)
{
}

MultiTextureShaderClass::~MultiTextureShaderClass()
{
}

bool MultiTextureShaderClass::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;

	//Initialize the vertex and pixel shaders
	result = initializeShader(device, hwnd, L"../DirectXEngine/mulltitexture.vs", L"../DirectXEngine/multitexture.ps");
	if (!result)
	{
		return false;
	}

	return true;
}
void MultiTextureShaderClass::Shutdown()
{
	//Shutdown the vertex and pixel shaders as well as the related objects
	shutdownShader();

	return;
}
bool MultiTextureShaderClass::Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, 
	XMMATRIX viewMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView** textureArray)
{
	bool result;

	//Set the shader parameters that it will use for rendering
	result = setShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, textureArray);
	if (!result)
	{
		return false;
	}

	//Now render the prepared buffers with the shader
	renderShader(deviceContext, indexCount);

	return true;
}


bool MultiTextureShaderClass::initializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_SAMPLER_DESC samplerDesc;

	//Inititalize the pointers this function will use to null
	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

	//Compile the vertex shader code
	result = D3DCompileFromFile(vsFilename, NULL, NULL, "MultiTextureVertexShader", "vs_5_0",
		D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertexShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		//If the shader failed to compile it should have written something to the error message
		if (errorMessage)
		{
			outputShaderErrorMessage(errorMessage, hwnd, vsFilename);
		}
		//If there was nothing in the error message then it simply could not find the shader file itself
		else
		{
			MessageBox(hwnd, vsFilename, L"Missing Shader File", MB_OK);
		}
		return false;
	}
}
void MultiTextureShaderClass::shutdownShader()
{

}
void MultiTextureShaderClass::outputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*)
{

}

bool MultiTextureShaderClass::setShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView**)
{

}
void MultiTextureShaderClass::renderShader(ID3D11DeviceContext*, int)
{

}
