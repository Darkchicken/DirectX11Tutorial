#include "ColorShaderClass.h"



ColorShaderClass::ColorShaderClass()
{
	m_vertexShader = 0;
	m_pixelShader = 0;
	m_layout = 0;
	m_matrixBuffer = 0;
}

ColorShaderClass::ColorShaderClass(const ColorShaderClass&)
{
}

ColorShaderClass::~ColorShaderClass()
{
}

bool ColorShaderClass::Initialize(ID3D11Device*, HWND)
{
	bool result;

	//Initialize the vertex and pixel shaders
	result = initializeShader();
}
void ColorShaderClass::Shutdown()
{

}
bool ColorShaderClass::Render(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX)
{

}

bool ColorShaderClass::initializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*)
{

}
void ColorShaderClass::shutdownShader()
{

}
void ColorShaderClass::outputShaderErrorMessage(ID3D10Blob, HWND, WCHAR*)
{

}

bool ColorShaderClass::setShaderParameters(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX)
{

}
void ColorShaderClass::renderShader(ID3D11DeviceContext*, int)
{

}
