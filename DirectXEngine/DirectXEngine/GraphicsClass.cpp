#include "GraphicsClass.h"



GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	m_Model = 0;
	m_LightShader = 0;
	m_Light = 0;
	m_Bitmap = 0;
	m_Text = 0;
}

GraphicsClass::GraphicsClass(const GraphicsClass& other)
{

}

GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;
	XMMATRIX baseViewMatrix;

	//Create the direct3d object
	m_D3D = new D3DClass;
	if (!m_D3D)
	{
		return false;
	}

	//Initialize the direct3d object
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	//Create the camera object
	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}

	//Initialize a base view matrix with the camera for 2D user interface rendering
	m_Camera->SetPosition(0.0f, 0.0f, -1.0f);
	m_Camera->Render();
	m_Camera->GetViewMatrix(baseViewMatrix);

	//Set the initial position of the camera
	//m_Camera->SetPosition(0.0f, 0.0f, -25.0f);

	//Create the text object
	m_Text = new TextClass;
	if (!m_Text)
	{
		return false;
	}

	//Initialize the text object
	result = m_Text->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, baseViewMatrix);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the text object", L"Error", MB_OK);
		return false;
	}

	//Create the model object
	m_Model = new ModelClass;
	if (!m_Model)
	{
		return false;
	}

	//Initialize the model object
	result = m_Model->Initialize(m_D3D->GetDevice(),m_D3D->GetDeviceContext(),"../DirectXEngine/data/model.txt", "../DirectXEngine/data/stone01.tga");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object", L"Error", MB_OK);
		return false;
	}

	//Create the light shader object
	m_LightShader = new LightShaderClass;
	if (!m_LightShader)
	{
		return false;
	}

	//Initialize the light shader object
	result = m_LightShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object", L"Error", MB_OK);
		return false;
	}

	//Create the light object
	m_Light = new LightClass;
	if (!m_Light)
	{
		return false;
	}

	//Initialize the light object
	m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(0.0f, 0.0f, 1.0f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(32.0f);

	//Create the light shader object
	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader)
	{
		return false;
	}

	//Initialize the light shader object
	result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object", L"Error", MB_OK);
		return false;
	}

	//Create the bitmap object
	m_Bitmap = new BitmapClass;
	if (!m_Bitmap)
	{
		return false;
	}

	//Initialize the bitmap object
	result = m_Bitmap->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), screenWidth, screenHeight, "../DirectXEngine/data/stone01.tga", 256, 256);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object", L"Error", MB_OK);
		return false;
	}

	return true;
}
void GraphicsClass::Shutdown()
{
	//Release the text object
	if(m_Text)
	{
		m_Text->Shutdown();
		delete m_Text;
		m_Text = 0;
	}

	//Release the bitmap object
	if (m_Bitmap)
	{
		m_Bitmap->Shutdown();
		delete m_Bitmap;
		m_Bitmap = 0;
	}

	//Release the light object
	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	//Release the light shader object
	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}

	//Release the light shader object
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}


	//Release the model object
	if (m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}

	//Release the camera object 
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	//Release the direct3d object
	if (m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}
	return;
}
bool GraphicsClass::Frame(int mouseX, int mouseY)
{
	bool result;
	

	//Set the location of the mouse
	result = m_Text->SetMousePosition(mouseX, mouseY, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	//Set the position of the camera
	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);


	
	
	return true;
}

bool GraphicsClass::Render()
{
	XMMATRIX viewMatrix, projectionMatrix, worldMatrix, orthoMatrix;
	bool result;

	//Clear the buffers to begin the scene
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	//Generate the view matrix based on the camera's position
	m_Camera->Render();

	//Get the world, view, and projection matrices from the camera and D3D objects
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);

	//Rotate the world matrix by the rotation value so that the triangle will spin
	//worldMatrix = XMMatrixRotationY(rotation);

	//Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing
	m_Model->Render(m_D3D->GetDeviceContext());

	//Render the model using the light shader
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(),
		worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTexture() , m_Light->GetDirection(),m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
	if (!result)
	{
		return false;
	}

	//Turn off the z buffer to begin all 2D rendering
	m_D3D->TurnZBufferOff();

	//Put the bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing
	result = m_Bitmap->Render(m_D3D->GetDeviceContext(), 0, 0);
	if (!result)
	{
		return false;
	}

	//TODO if your view matrix is changing you will need to create a default one for 2D rendering and use it instead of the regular view matrix. In this tutorial using the regular view matrix is fine as the camera in this tutorial is stationary.
	//Maybe make a new light just for UI

	
	//Render the bitmap with the light shader
	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Bitmap->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, m_Bitmap->GetTexture());
	if (!result)
	{
		return false;
	}

	//Turn on the alpha blending before rendering the text
	m_D3D->TurnOnAlphaBlending();

	//Render the text strings
	result = m_Text->Render(m_D3D->GetDeviceContext(), worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}

	m_D3D->TurnOffAlphaBlending();

	//Turn the z buffer back on now that all 2D rendering has completed
	m_D3D->TurnZBufferOn();

	//Present the rendered scene to the screen
	m_D3D->EndScene();
	return true;
}
