#include "FrustrumClass.h"

FrustrumClass::FrustrumClass()
{
}

FrustrumClass::FrustrumClass(const FrustrumClass& other)
{
}

FrustrumClass::~FrustrumClass()
{
}

void FrustrumClass::Initialize(float screenDepth)
{
	m_screenDepth = screenDepth;
	return;
}

void FrustrumClass::ConstructFrustrum(XMMATRIX projectionMatrix, XMMATRIX viewMatrix)
{
	XMFLOAT4X4 pMatrix, matrix;
	float zMinimum, r, length;
	XMMATRIX finalMatrix;

	
	//Convert the projection matrix into a 4x4 float type
	XMStoreFloat4x4(&pMatrix, projectionMatrix);

	//Calculate the minimum z distance in the frustrum
	zMinimum = -pMatrix._43 / pMatrix._33;
	r = m_screenDepth / (m_screenDepth - zMinimum);

	//Load the updated values back into the projection matrix
	pMatrix._33 = r;
	pMatrix._43 = -r * zMinimum;
	projectionMatrix = XMLoadFloat4x4(&pMatrix);

	//Create the final frustrum matrix from the view matrix and updated projection matrix
	finalMatrix = XMMatrixMultiply(viewMatrix, projectionMatrix);

	//Convert the final matrix into a 4x4 float type
	XMStoreFloat4x4(&matrix, finalMatrix);

	//calculate near plane of frustrum
	m_planes[0][0] = matrix._14 + matrix._13;
	m_planes[0][1] = matrix._24 + matrix._23;
	m_planes[0][2] = matrix._34 + matrix._33;
	m_planes[0][3] = matrix._44 + matrix._43;

	//Normalize the near plane
	length = sqrt((m_planes[0][0] * m_planes[0][0]) + (m_planes[0][1] * m_planes[0][1]) + (m_planes[0][2] * m_planes[0][2]));
	m_planes[0][0] /= length;
	m_planes[0][1] /= length;
	m_planes[0][2] /= length;
	m_planes[0][3] /= length;

	//calculate far plane of frustrum
	m_planes[1][0] = matrix._14 - matrix._13;
	m_planes[1][1] = matrix._24 - matrix._23;
	m_planes[1][2] = matrix._34 - matrix._33;
	m_planes[1][3] = matrix._44 - matrix._43;

	//Normalize the far plane
	length = sqrt((m_planes[1][0] * m_planes[1][0]) + (m_planes[1][1] * m_planes[1][1]) + (m_planes[1][2] * m_planes[1][2]));
	m_planes[1][0] /= length;
	m_planes[1][1] /= length;
	m_planes[1][2] /= length;
	m_planes[1][3] /= length;

	//calculate left plane of frustrum
	m_planes[2][0] = matrix._14 + matrix._11;
	m_planes[2][1] = matrix._24 + matrix._21;
	m_planes[2][2] = matrix._34 + matrix._31;
	m_planes[2][3] = matrix._44 + matrix._41;

	//Normalize the left plane
	length = sqrt((m_planes[2][0] * m_planes[2][0]) + (m_planes[2][1] * m_planes[2][1]) + (m_planes[2][2] * m_planes[2][2]));
	m_planes[2][0] /= length;
	m_planes[2][1] /= length;
	m_planes[2][2] /= length;
	m_planes[2][3] /= length;

	//calculate right plane of frustrum
	m_planes[3][0] = matrix._14 - matrix._11;
	m_planes[3][1] = matrix._24 - matrix._21;
	m_planes[3][2] = matrix._34 - matrix._31;
	m_planes[3][3] = matrix._44 - matrix._41;

	//Normalize the right plane
	length = sqrt((m_planes[3][0] * m_planes[3][0]) + (m_planes[3][1] * m_planes[3][1]) + (m_planes[3][2] * m_planes[3][2]));
	m_planes[3][0] /= length;
	m_planes[3][1] /= length;
	m_planes[3][2] /= length;
	m_planes[3][3] /= length;

	//calculate top plane of frustrum
	m_planes[4][0] = matrix._14 - matrix._12;
	m_planes[4][1] = matrix._24 - matrix._22;
	m_planes[4][2] = matrix._34 - matrix._32;
	m_planes[4][3] = matrix._44 - matrix._42;

	//Normalize the top plane
	length = sqrt((m_planes[4][0] * m_planes[4][0]) + (m_planes[4][1] * m_planes[4][1]) + (m_planes[4][2] * m_planes[4][2]));
	m_planes[4][0] /= length;
	m_planes[4][1] /= length;
	m_planes[4][2] /= length;
	m_planes[4][3] /= length;

	//calculate bottom plane of frustrum
	m_planes[5][0] = matrix._14 + matrix._12;
	m_planes[5][1] = matrix._24 + matrix._22;
	m_planes[5][2] = matrix._34 + matrix._32;
	m_planes[5][3] = matrix._44 + matrix._42;

	//Normalize the bottom plane
	length = sqrt((m_planes[5][0] * m_planes[5][0]) + (m_planes[5][1] * m_planes[5][1]) + (m_planes[5][2] * m_planes[5][2]));
	m_planes[5][0] /= length;
	m_planes[5][1] /= length;
	m_planes[5][2] /= length;
	m_planes[5][3] /= length;

	return;
}

bool FrustrumClass::CheckPoint(float x, float y, float z)
{
	float dotProduct;

	//Check each of the six planes to make sure the point is inside all of them and hence inside the frustrum
	for (int i = 0; i < 6; ++i)
	{
		//Calculate the dot product of the plane and the 3D point
		dotProduct = (m_planes[i][0]*x) + (m_planes[i][1] * y) + (m_planes[i][2] * z) + (m_planes[i][3] * 1.0f);

		//Determine if the point is on the correct side of the current plane, exit out if it is not
		if (dotProduct <= 0.0f)
		{
			return false;
		}
	}
	return true;
}
bool FrustrumClass::CheckCube(float xCenter, float yCenter, float zCenter, float radius)
{
	float dotProduct;

	//Check each of the siz planes to see if the cube is inside the frustrum
	for (int i = 0; i < 6; ++i)
	{
		//Check all eight points of the cube to see if they all reside within the frustrum
		dotProduct = (m_planes[i][0] * (xCenter - radius)) + (m_planes[i][1] * (yCenter - radius)) + (m_planes[i][2] * (zCenter - radius)) + (m_planes[i][3] * 1.0f);
		if(dotProduct > 0.0f)
		{
			continue;
		}

		dotProduct = (m_planes[i][0] * (xCenter + radius)) + (m_planes[i][1] * (yCenter - radius)) + (m_planes[i][2] * (zCenter - radius)) + (m_planes[i][3] * 1.0f);
		if (dotProduct > 0.0f)
		{
			continue;
		}

		dotProduct = (m_planes[i][0] * (xCenter - radius)) + (m_planes[i][1] * (yCenter + radius)) + (m_planes[i][2] * (zCenter - radius)) + (m_planes[i][3] * 1.0f);
		if (dotProduct > 0.0f)
		{
			continue;
		}

		dotProduct = (m_planes[i][0] * (xCenter + radius)) + (m_planes[i][1] * (yCenter + radius)) + (m_planes[i][2] * (zCenter - radius)) + (m_planes[i][3] * 1.0f);
		if (dotProduct > 0.0f)
		{
			continue;
		}

		dotProduct = (m_planes[i][0] * (xCenter - radius)) + (m_planes[i][1] * (yCenter - radius)) + (m_planes[i][2] * (zCenter + radius)) + (m_planes[i][3] * 1.0f);
		if (dotProduct > 0.0f)
		{
			continue;
		}

		dotProduct = (m_planes[i][0] * (xCenter + radius)) + (m_planes[i][1] * (yCenter - radius)) + (m_planes[i][2] * (zCenter + radius)) + (m_planes[i][3] * 1.0f);
		if (dotProduct > 0.0f)
		{
			continue;
		}

		dotProduct = (m_planes[i][0] * (xCenter - radius)) + (m_planes[i][1] * (yCenter + radius)) + (m_planes[i][2] * (zCenter + radius)) + (m_planes[i][3] * 1.0f);
		if (dotProduct > 0.0f)
		{
			continue;
		}

		dotProduct = (m_planes[i][0] * (xCenter + radius)) + (m_planes[i][1] * (yCenter + radius)) + (m_planes[i][2] * (zCenter + radius)) + (m_planes[i][3] * 1.0f);
		if (dotProduct > 0.0f)
		{
			continue;
		}

		return false;
	}
	return true;
}
bool FrustrumClass::CheckSphere(float xCenter, float yCenter, float zCenter, float radius)
{
	float dotProduct;

	//Check the six planes to see if the sphere is inside them or not
	for (int i = 0; i < 6; ++i)
	{
		dotProduct = ((m_planes[i][0] * xCenter) + (m_planes[i][1] * yCenter) + (m_planes[i][2] * zCenter) + (m_planes[i][3] * 1.0f));
		if (dotProduct <= -radius)
		{
			return false;
		}
	}
	return true;
}
bool FrustrumClass::CheckRectangle(float xCenter, float yCenter, float zCenter, float xSize, float ySize, float zSize)
{
	float dotProduct;

	//Check each of the siz planes to see if the rectangle is inside the frustrum
	for (int i = 0; i < 6; ++i)
	{
		//Check all eight points of the cube to see if they all reside within the frustrum
		dotProduct = (m_planes[i][0] * (xCenter - xSize)) + (m_planes[i][1] * (yCenter - ySize)) + (m_planes[i][2] * (zCenter - zSize)) + (m_planes[i][3] * 1.0f);
		if (dotProduct > 0.0f)
		{
			continue;
		}

		dotProduct = (m_planes[i][0] * (xCenter + xSize)) + (m_planes[i][1] * (yCenter - ySize)) + (m_planes[i][2] * (zCenter - zSize)) + (m_planes[i][3] * 1.0f);
		if (dotProduct > 0.0f)
		{
			continue;
		}

		dotProduct = (m_planes[i][0] * (xCenter - xSize)) + (m_planes[i][1] * (yCenter + ySize)) + (m_planes[i][2] * (zCenter - zSize)) + (m_planes[i][3] * 1.0f);
		if (dotProduct > 0.0f)
		{
			continue;
		}

		dotProduct = (m_planes[i][0] * (xCenter + xSize)) + (m_planes[i][1] * (yCenter + ySize)) + (m_planes[i][2] * (zCenter - zSize)) + (m_planes[i][3] * 1.0f);
		if (dotProduct > 0.0f)
		{
			continue;
		}

		dotProduct = (m_planes[i][0] * (xCenter - xSize)) + (m_planes[i][1] * (yCenter - ySize)) + (m_planes[i][2] * (zCenter + zSize)) + (m_planes[i][3] * 1.0f);
		if (dotProduct > 0.0f)
		{
			continue;
		}

		dotProduct = (m_planes[i][0] * (xCenter + xSize)) + (m_planes[i][1] * (yCenter - ySize)) + (m_planes[i][2] * (zCenter + zSize)) + (m_planes[i][3] * 1.0f);
		if (dotProduct > 0.0f)
		{
			continue;
		}

		dotProduct = (m_planes[i][0] * (xCenter - xSize)) + (m_planes[i][1] * (yCenter + ySize)) + (m_planes[i][2] * (zCenter + zSize)) + (m_planes[i][3] * 1.0f);
		if (dotProduct > 0.0f)
		{
			continue;
		}

		dotProduct = (m_planes[i][0] * (xCenter + xSize)) + (m_planes[i][1] * (yCenter + ySize)) + (m_planes[i][2] * (zCenter + zSize)) + (m_planes[i][3] * 1.0f);
		if (dotProduct > 0.0f)
		{
			continue;
		}

		return false;
	}
	return true;
}
