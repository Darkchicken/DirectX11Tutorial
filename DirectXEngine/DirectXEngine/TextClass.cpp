#include "TextClass.h"



TextClass::TextClass()
{
}

TextClass::TextClass(const TextClass& other)
{
}


TextClass::~TextClass()
{
}

bool TextClass::Initialize(ID3D11Device*, ID3D11DeviceContext*, HWND, int, int, XMMATRIX)
{

}
void TextClass::Shutdown()
{

}
bool TextClass::Render(ID3D11DeviceContext*, XMMATRIX, XMMATRIX)
{

}


bool TextClass::initializeSentence(SentenceType**, int, ID3D11Device*)
{

}
bool TextClass::updateSentence(SentenceType*, char*, int, int, float, float, float, ID3D11DeviceContext*)
{

}
void TextClass::releaseSentence(SentenceType**)
{

}
bool TextClass::renderSentence(ID3D11DeviceContext*, SentenceType*, XMMATRIX, XMMATRIX)
{

}
