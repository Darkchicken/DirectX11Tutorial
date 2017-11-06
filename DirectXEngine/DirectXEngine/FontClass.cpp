#include "FontClass.h"



FontClass::FontClass()
{
	m_font = 0;
	m_texture = 0;
}

FontClass::FontClass(const FontClass& other)
{
}


FontClass::~FontClass()
{
}

bool FontClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* fontFilename, 
	char* textureFilename, float fontHeight, int spaceSize)
{
	bool result;

	//Store the height of the font
	m_fontHeight = fontHeight;
	//Store the size of spaces in pixel size
	m_spaceSize = spaceSize;

	//Load in the text file containing the font data
	result = loadFontData(fontFilename);
	if (!result)
	{
		return false;
	}

	//Load the texture that has the font characters on it
	result = loadTexture(device, deviceContext, textureFilename);
	if (!result)
	{
		return false;
	}

	return true;
}
void FontClass::Shutdown()
{
	//Release the font texture
	releaseTexture();

	//Release the font data
	releaseFontData();

	return;
}

ID3D11ShaderResourceView* FontClass::GetTexture()
{
	return m_texture->GetTexture();
}

void FontClass::BuildVertexArray(void* vertices, char* sentence, float drawX, float drawY)
{
	VertexType* vertexPtr;
	int numLetters, index, letter;

	//Coerce the input vertices into a VertexType structure
	vertexPtr = (VertexType*)vertices;

	//Get the number of letters in the sentence
	numLetters = (int)strlen(sentence);

	//Initialize the index to the vertex array
	index = 0;

	//Draw each letter onto a quad
	for (int i = 0; i < numLetters; ++i)
	{
		letter = ((int)sentence[i]) - 32;

		//If the letter is a space then just move over three pixels
		if (letter == 0)
		{
			drawX = drawX + (float)m_spaceSize;
		}
		else
		{
			//First triangle in quad
			vertexPtr[index].position = XMFLOAT3(drawX, drawY, 0.0f); //Top left
			vertexPtr[index].texture = XMFLOAT2(m_font[letter].left, 0.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3((drawX +m_font[letter].size), (drawY - m_fontHeight), 0.0f); //Bottom right
			vertexPtr[index].texture = XMFLOAT2(m_font[letter].right, 1.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3(drawX, (drawY - m_fontHeight), 0.0f); //Bottom left
			vertexPtr[index].texture = XMFLOAT2(m_font[letter].left, 1.0f);
			index++;

			//Second triangle in quead
			vertexPtr[index].position = XMFLOAT3(drawX, drawY, 0.0f); //Top left
			vertexPtr[index].texture = XMFLOAT2(m_font[letter].left, 0.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3((drawX + m_font[letter].size), drawY, 0.0f); //Top right
			vertexPtr[index].texture = XMFLOAT2(m_font[letter].right, 0.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3((drawX + m_font[letter].size), (drawY - m_fontHeight), 0.0f); //Bottom right
			vertexPtr[index].texture = XMFLOAT2(m_font[letter].right, 1.0f);
			index++;

			//Update the x location for drawing by the size of the letter and one pixel
			drawX = drawX + m_font[letter].size + 1.0f;
		}
	}

	return;
}


bool FontClass::loadFontData(char* filename)
{
	std::ifstream fin;
	char temp;
	const int charCount = 95;

	//Create the font spacing buffer
	m_font = new FontType[charCount];
	if (!m_font)
	{
		return false;
	}

	//Read in the font size and spacing between chars
	fin.open(filename);
	if (fin.fail())
	{
		return false;
	}

	//Read in the 95 used ascii characters for text
	for (int i = 0; i < charCount; ++i)
	{
		fin.get(temp);
		while (temp != ' ')
		{
			fin.get(temp);
		}
		fin.get(temp);
		while (temp != ' ')
		{
			fin.get(temp);
		}
		fin >> m_font[i].left;
		fin >> m_font[i].right;
		fin >> m_font[i].size;
	}

	//Close the file
	fin.close();

	return true;
}
void FontClass::releaseFontData()
{
	//Release the font data array
	if (m_font)
	{
		delete[] m_font;
		m_font = 0;
	}

	return;
}
bool FontClass::loadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename)
{
	bool result;

	//Create the texture object
	m_texture = new TextureClass;
	if (!m_texture)
	{
		return false;
	}

	//Initialize the texture object
	result = m_texture->Initialize(device, deviceContext, filename);
	if (!result)
	{
		return false;
	}

	return true;
}
void FontClass::releaseTexture()
{
	//Release the texture object
	if (m_texture)
	{
		m_texture->Shutdown();
		delete m_texture;
		m_texture = 0;
	}

	return;
}
