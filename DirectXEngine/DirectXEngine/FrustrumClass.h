#pragma once

//Includes
#include <directxmath.h>

using namespace DirectX;

class FrustrumClass
{
public:
	FrustrumClass();
	FrustrumClass(const FrustrumClass&);
	~FrustrumClass();

	void Initialize(float);

	void ConstructFrustrum(XMMATRIX,XMMATRIX);

	bool CheckPoint(float, float, float);
	bool CheckCube(float, float, float, float);
	bool CheckSphere(float, float, float, float);
	bool CheckRectangle(float, float, float, float, float, float);

private:
	float m_screenDepth;
	float m_planes[6][4];
};

