#include "ModelListClass.h"



ModelListClass::ModelListClass()
{
	m_modelInfoList = 0;
}

ModelListClass::ModelListClass(const ModelListClass& other)
{
}


ModelListClass::~ModelListClass()
{
}

bool ModelListClass::Initialize(int numModels)
{
	float red, green, blue;

	//Store the number of models
	m_modelCount = numModels;

	//Create a list array of the model information
	m_modelInfoList = new ModelInfoType[m_modelCount];
	if (!m_modelInfoList)
	{
		return false;
	}

	//Seed the random generator with the current time
	srand((unsigned int)time(NULL));

	//Go through all the models and randomly generate the model color and position
	for (int i = 0; i < m_modelCount; ++i)
	{
		//Generate a random color for the model
		red = (float)rand() / RAND_MAX;
		green = (float)rand() / RAND_MAX;
		blue = (float)rand() / RAND_MAX;

		m_modelInfoList[i].color = XMFLOAT4(red, green, blue, 1.0f);

		//Generate a random position in front of the viewer for the mode
		m_modelInfoList[i].positionX = (((float)rand() - (float)rand()) / RAND_MAX) * 10.0f;
		m_modelInfoList[i].positionY = (((float)rand() - (float)rand()) / RAND_MAX) * 10.0f;
		m_modelInfoList[i].positionZ = ((((float)rand() - (float)rand()) / RAND_MAX) * 10.0f) + 5.0f;
	}

	return true;
}
void ModelListClass::Shutdown()
{
	//Release the model information list
	if (m_modelInfoList)
	{
		delete[] m_modelInfoList;
		m_modelInfoList = 0;
	}

	return;
}

int ModelListClass::GetModelCount()
{
	return m_modelCount;
}
void ModelListClass::GetData(int index, float& positionX, float& positionY, float& positionZ, XMFLOAT4& color)
{
	positionX = m_modelInfoList[index].positionX;
	positionY = m_modelInfoList[index].positionY;
	positionZ = m_modelInfoList[index].positionZ;

	color = m_modelInfoList[index].color;

	return;
}
