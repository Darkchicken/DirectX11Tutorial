#pragma once

#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

#include "D3dClass.h"
#include "CameraClass.h"
#include "ModelClass.h"
#include "BitmapClass.h"
#include "LightShaderClass.h"
#include "LightClass.h"

#include "TextureShaderClass.h"

//Globals
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;
class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();

private:
	bool render(float);

private:
	D3DClass* m_D3D;

	CameraClass* m_Camera;
	ModelClass* m_Model;
	LightShaderClass* m_LightShader;
	LightClass* m_Light;

	BitmapClass* m_Bitmap;

	//For UI
	TextureShaderClass* m_TextureShader;

};

#endif

