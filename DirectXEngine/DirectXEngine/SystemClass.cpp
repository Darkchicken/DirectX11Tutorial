#include "SystemClass.h"


SystemClass::SystemClass()
{
	m_input = 0;
	m_graphics = 0;
	m_sound = 0;
	m_fps = 0;
	m_cpu = 0;
	m_timer = 0;
	m_position = 0;
}

SystemClass::SystemClass(const SystemClass&)
{
}

SystemClass::~SystemClass()
{
}

bool SystemClass::Initialize()
{
	int screenWidth = 0, screenHeight = 0;
	bool result;

	//Initizlize the windows api
	InitializeWindows(screenWidth, screenHeight);

	//Create input object to handle user keyboard input
	m_input = new InputClass;
	if (!m_input)
	{
		return false;
	}

	//Initialize the input object
	result = m_input->Initialize(m_hinstance, m_hwnd, screenWidth, screenHeight);
	if (!result)
	{
		MessageBox(m_hwnd, L"Could not initialize the input object", L"Error", MB_OK);
		return false;
	}

	//Create the graphics object to handle rendering
	m_graphics = new GraphicsClass;
	if (!m_graphics)
	{
		return false;
	}

	//Initialize the graphics object
	result = m_graphics->Initialize(screenWidth, screenHeight, m_hwnd);
	if (!result)
	{
		return false;
	}

	//Create the sound object
	m_sound = new SoundClass;
	if (!m_sound)
	{
		return false;
	}

	//Initialize the sound object
	result = m_sound->Initialize(m_hwnd);
	if (!result)
	{
		MessageBox(m_hwnd, L"Could not initialize Direct Sound", L"Error", MB_OK);
		return false;
	}

	//Create the fps object
	m_fps = new FpsClass;
	if (!m_fps)
	{
		return false;
	}

	//Initialize the fps object
	m_fps->Initialize();

	//Create the cpu object
	m_cpu = new CpuClass;
	if (!m_cpu)
	{
		return false;
	}

	//Initialize the cpu object
	m_cpu->Initialize();

	//Create the timer object
	m_timer = new TimerClass;
	if (!m_timer)
	{
		return false;
	}

	//Initialize the timer object
	result = m_timer->Initialize();
	if (!result)
	{
		MessageBox(m_hwnd, L"Could not initialize the Timer object", L"Error", MB_OK);
		return false;
	}

	//Create the position object
	m_position = new PositionClass;
	if (!m_position)
	{
		return false;
	}

	return true;
}
void SystemClass::Shutdown()
{
	//Release the position object
	if (m_position)
	{
		delete m_position;
		m_position = 0;
	}

	//Release the timer object
	if (m_timer)
	{
		delete m_timer;
		m_timer = 0;
	}

	//Release the cpu object
	if (m_cpu)
	{
		m_cpu->Shutdown();
		delete m_cpu;
		m_cpu = 0;
	}

	//Release the fps object
	if (m_fps)
	{
		delete m_fps;
		m_fps = 0;
	}

	//Release the sound object
	if (m_sound)
	{
		m_sound->Shutdown();
		delete m_sound;
		m_sound = 0;
	}

	//Release the graphics object
	if (m_graphics)
	{
		m_graphics->Shutdown();
		delete m_graphics;
		m_graphics = 0;
	}

	//Release the input object
	if (m_input)
	{
		m_input->Shutdown();
		delete m_input;
		m_input = 0;
	}

	//Shutdown the window
	ShutdownWindows();

	return;
}
void SystemClass::Run()
{
	MSG msg;
	bool done, result;

	//Initialize the message structure
	ZeroMemory(&msg, sizeof(MSG));

	//Loop until there is a quit message from the window or user
	done = false;
	while (!done)
	{
		//Handle the windows messages
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		//if windows signals to end the application, exit out
		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			//Otherwise do frame processing. If frame processing fails then exit
			result = Frame();
			if (!result)
			{
				MessageBox(m_hwnd, L"Frame Processing Failed", L"Error", MB_OK);
				done = true;
			}
		}

		//Check if the user pressed escape and wants to quit
		if (m_input->IsEscapePressed() == true)
		{
			done = true;
		}
	}
	return;
}

LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	return DefWindowProc(hwnd, umsg, wparam, lparam);
}

bool SystemClass::Frame()
{
	bool keydown, result;
	int mouseX, mouseY;
	float rotationX, rotationY;

	//Update the system stats
	m_timer->Frame();
	m_fps->Frame();
	m_cpu->Frame();

	//Do the input frame processing
	result = m_input->Frame();
	if (!result)
	{
		return false;
	}

	//Set the frame time for calculating the updated position
	m_position->SetFrameTime(m_timer->GetTime());

	//Check if the left or right arrow key has been pressed, if so rotate the camera accordingly
	keydown = m_input->IsQPressed();
	m_position->TurnLeft(keydown);

	keydown = m_input->IsEPressed();
	m_position->TurnRight(keydown);

	//Get the location of the mouse from the input object
	m_input->GetMouseLocation(mouseX, mouseY);

	//Get the current view point rotation
	m_position->GetRotation(rotationX, rotationY);

	//Do the frame processing for the graphics object
	result = m_graphics->Frame(m_fps->GetFps(), m_cpu->GetCpuPercentage(), m_timer->GetTime(), rotationX, rotationY, m_input->IsForwardBackward(), m_input->IsLeftRight());
	if (!result)
	{
		return false;
	}

	//Finally render the graphics to the screen
	result = m_graphics->Render();
	if (!result)
	{
		return false;
	}

	return true;
}
void SystemClass::InitializeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;

	//Get an external pointer to this object
	ApplicationHandle = this;

	//Get the instance of this application
	m_hinstance = GetModuleHandle(NULL);

	//Give the application a name
	m_applicationName = L"Engine";

	//Setup the window class with default settings
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	//Register the window class
	RegisterClassEx(&wc);

	//Determine the resolution of the clients desktop screen
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	//Setup the screen settings depending on if it is running full screen or windowed mode
	if (FULL_SCREEN)
	{
		//If full screen, set the window to the maximum size of the users desktop and 32 bit
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		//Change the display settings to full screen
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		//Set the position of the window to the top left corner
		posX = posY = 0;
	}
	else
	{
		//If windowed, set to 800x600 resolution
		screenWidth = 800;
		screenHeight = 600;

		//Place the window in the middle of the screen
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	//Create the window with the screen settings and get a handle to it
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName, 
		WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_POPUP, posX, posY, screenWidth, screenHeight, 
		NULL, NULL, m_hinstance, NULL );

	//Bring the window up on the screen and set it as the main focus
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	//Hide the mouse cursor
	ShowCursor(false);

	return;
}
void SystemClass::ShutdownWindows()
{
	//Show the mouse cursor
	ShowCursor(true);

	//Fix the display settings if leaving full screen mode
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}
	
	//Remove the window
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	//Remove the application instance
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	//Release the pointer to this class
	ApplicationHandle = NULL;

	return;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		//Check if the window is being destroyed
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		//Check if the window is being closed
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}

		//All other messages pass to the message handler in the system class
		default:
		{
			return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
		}
	}
}

