#include "TextureArrayClass.h"



TextureArrayClass::TextureArrayClass()
{
	m_targaData = 0;
	m_texture = 0;
	m_textures[0] = 0;
	m_textures[1] = 0;
}

TextureArrayClass::TextureArrayClass(const TextureArrayClass& other)
{
}

TextureArrayClass::~TextureArrayClass()
{
}

bool TextureArrayClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, 
	char* fileName1, char* fileName2)
{
	bool result;
	int height, width;
	D3D11_TEXTURE2D_DESC textureDesc;
	HRESULT hResult;
	unsigned int rowPitch;
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;

	//Load the targa image data into memory
	result = LoadTarga(fileName1, height, width);
	if (!result)
	{
		return false;
	}

	//Setup the description of the texture
	textureDesc.Height = height;
	textureDesc.Width = width;
	textureDesc.MipLevels = 0;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	//Create the empty texture
	hResult = device->CreateTexture2D(&textureDesc, NULL, &m_texture);
	if (FAILED(hResult))
	{
		return false;
	}

	//Set the row pitch of the targa image data
	rowPitch = (width * 4) * sizeof(unsigned char);

	//Copy the targa image data into the texture
	deviceContext->UpdateSubresource(m_texture, 0, NULL, m_targaData, rowPitch, 0);

	//Setup the shader resource view description
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;

	//Create the shader resource view for the texture
	hResult = device->CreateShaderResourceView(m_texture, &srvDesc, &m_textures[0]);
	if (FAILED(hResult))
	{
		return false;
	}

	//Generate the mipmaps for this texture
	deviceContext->GenerateMips(m_textures[0]);

	//////////////
	//Texture 2//
	//////////////

	//Load the targa image data into memory
	result = LoadTarga(fileName2, height, width);
	if (!result)
	{
		return false;
	}

	textureDesc.Height = height;
	textureDesc.Width = width;

	//Create the empty texture
	hResult = device->CreateTexture2D(&textureDesc, NULL, &m_texture);
	if (FAILED(hResult))
	{
		return false;
	}

	//Set the row pitch of the targa image data
	rowPitch = (width * 4) * sizeof(unsigned char);

	//Copy the targa image data into the texture
	deviceContext->UpdateSubresource(m_texture, 0, NULL, m_targaData, rowPitch, 0);

	//Create the shader resource view for the texture
	hResult = device->CreateShaderResourceView(m_texture, &srvDesc, &m_textures[1]);
	if (FAILED(hResult))
	{
		return false;
	}

	//Generate the mipmaps for this texture
	deviceContext->GenerateMips(m_textures[1]);

	//Release the targa image data now that the image data has been loaded into the texture
	delete[] m_targaData;
	m_targaData = 0;

	return true;

}
void TextureArrayClass::Shutdown()
{
	//Release the texture resource view
	if (m_textures[0])
	{
		m_textures[0]->Release();
		m_textures[0] = 0;
	}
	if (m_textures[1])
	{
		m_textures[1]->Release();
		m_textures[1] = 0;
	}

	//Release the texture
	if (m_texture)
	{
		m_texture->Release();
		m_texture = 0;
	}

	//Release the targa data
	if (m_targaData)
	{
		delete[] m_targaData;
		m_targaData = 0;
	}

	return;
}

ID3D11ShaderResourceView** TextureArrayClass::GetTextureArray()
{
	return m_textures;
}

bool TextureArrayClass::LoadTarga(char* fileName, int& height, int& width)
{
	int error, bpp, imageSize, index, i, j, k;
	FILE* filePtr;
	unsigned int count;
	TargaHeader targaFileHeader;
	unsigned char* targaImage;

	//Open the targa file for reading in binary
	error = fopen_s(&filePtr, fileName, "rb");
	if (error != 0)
	{
		return false;
	}

	//Read in the file header
	count = (unsigned int)fread(&targaFileHeader, sizeof(TargaHeader), 1, filePtr);
	if (count != 1)
	{
		return false;
	}

	//Get the important information from the header
	height = (int)targaFileHeader.height;
	width = (int)targaFileHeader.width;
	bpp = (int)targaFileHeader.bpp;

	//Check that it is 32 bit and not 24 bit
	if (bpp != 32)
	{
		return false;
	}

	//Calculate the size of the 32 bit image data
	imageSize = width * height * 4;

	//Allocate the memory for the targa image data
	targaImage = new unsigned char[imageSize];
	if (!targaImage)
	{
		return false;
	}

	//Read in the targa image data
	count = (unsigned int)fread(targaImage, 1, imageSize, filePtr);
	if (count != imageSize)
	{
		return false;
	}

	//Close the file
	error = fclose(filePtr);
	if (error != 0)
	{
		return false;
	}

	//Allocate the memory for the targa destination data
	m_targaData = new unsigned char[imageSize];
	if (!m_targaData)
	{
		return false;
	}

	//Initialize the index into the targa destination data array
	index = 0;

	//Initialize the index into the targa image data
	k = (width * height * 4) - (width * 4);

	//Now copy the targa image data into the targa destination array in the correct order since the targa 
	//format is stored upside down
	for (j = 0; j < height; ++j)
	{
		for (i = 0; i < width; ++i)
		{
			m_targaData[index + 0] = targaImage[k + 2]; //Red
			m_targaData[index + 1] = targaImage[k + 1]; //Green
			m_targaData[index + 2] = targaImage[k + 0]; //Blue
			m_targaData[index + 3] = targaImage[k + 3]; //Alpha

														//Increment the indices into the targa data
			k += 4;
			index += 4;
		}
		//Set the targa image data index back to the preceding row at the beginning of the column since its reading it in upside down
		k -= (width * 8);
	}

	//Release the targa image data now that it was copied into the destination array
	delete[] targaImage;
	targaImage = 0;

	return true;
}