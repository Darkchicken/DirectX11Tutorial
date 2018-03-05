#pragma once

#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

#include "D3dClass.h"
#include "CameraClass.h"
#include "ModelClass.h"
#include "BitmapClass.h"
#include "LightShaderClass.h"
#include "LightClass.h"
#include "FrustrumClass.h"
#include "ModelListClass.h"

#include "TextureShaderClass.h"
#include "MultiTextureShaderClass.h"
#include "AlphaMapShaderClass.h"
#include "BumpMapShaderClass.h"
#include "SpecMapShaderClass.h"
#include "TextClass.h"

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
	bool Frame(int, int, float, float, float, int, int);
	bool Render();

private:
	D3DClass* m_D3D;

	CameraClass* m_Camera;
	ModelClass* m_Model;
	LightShaderClass* m_LightShader;
	LightClass* m_Light;
	ModelListClass* m_ModelList;
	FrustrumClass* m_Frustrum;

	BitmapClass* m_Bitmap;

	//For UI
	MultiTextureShaderClass* m_MultiTextureShader;
	TextureShaderClass* m_TextureShader;

	AlphaMapShaderClass* m_alphaMapShader;

	BumpMapShaderClass* m_bumpMapShader;

	SpecMapShaderClass* m_specMapShader;

	TextClass* m_Text;

	//Holds last moust pos
	float m_lastMouseX, m_lastMouseY;

};

#endif

