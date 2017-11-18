#pragma once
//includes
#include <d3d11.h>
#include <stdio.h>


class TextureArrayClass
{
private:
	struct TargaHeader
	{
		unsigned char data1[12];
		unsigned short width;
		unsigned short height;
		unsigned char bpp;
		unsigned char data2;
	};

public:
	TextureArrayClass();
	TextureArrayClass(const TextureArrayClass&);
	~TextureArrayClass();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, char*, char*);
	void Shutdown();

	ID3D11ShaderResourceView** GetTextureArray();

private:
	//Add all read functions here to load various formats
	bool LoadTarga(char*, int&, int&);

private:
	unsigned char* m_targaData;
	ID3D11Texture2D* m_texture;
	ID3D11ShaderResourceView* m_textures[2];
};

