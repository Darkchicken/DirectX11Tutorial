// MayaModelConverter.cpp : Defines the entry point for the console application.
//


#include <iostream>
#include <fstream>

//Typedefs
typedef struct
{
	float x, y, z;
}VertexType;

typedef struct
{
	int vindex1, vindex2, vindex3;
	int tindex1, tindex2, tindex3;
	int nindex1, nindex2, nindex3;
}FaceType;

//Function prototypes
void GetModelFilename(char*);
bool ReadFileCounts(char*, int&, int&, int&, int&);
bool LoadDataStructures(char*, int, int, int, int);

int main()
{
	bool result;
	char filename[256];
	int vertexCount, textureCount, normalCount, faceCount;
	char garbage;

	//Read in the name of the model file
	GetModelFilename(filename);

	//Read in the number of vertices, texture coordinates, normals, and faces so that the data structures
	//can be initialized with the exact sizes needed
	result = ReadFileCounts(filename, vertexCount, textureCount, normalCount, faceCount);
	if (!result)
	{
		return -1;
	}

	//Display the counts to the screen for information purposes
	std::cout << std::endl;
	std::cout << "Vertices: " << vertexCount << std::endl;
	std::cout << "UVs: " << textureCount << std::endl;
	std::cout << "Normals: " << normalCount << std::endl;
	std::cout << "Faces: " << faceCount << std::endl;

	//Now read the data from the file into the data structures and then output it into our model format
	result = LoadDataStructures(filename, vertexCount, textureCount, normalCount, faceCount);
	if (!result)
	{
		return -1;
	}

	//Notify the user the model has been converted
	std::cout << "/nFile has been converted." << std::endl;
	std::cout << "Do you wish to exit? (y/n)";
	std::cin >> garbage;

	return 0;
}

void GetModelFilename(char* filename)
{
	bool done;
	std::ifstream fin;

	//Loop until we have a filename
	done = false;
	while (!done)
	{
		//Ask the user for the filename
		std::cout << "Enter model filename:";

		//Read in the filename
		std::cin >> filename;

		//Attempt to open file
		fin.open(filename);

		if (fin.good())
		{
			//If the file exists and there are no problems then exit since we have the filename
			std::cout << "File " << filename << " found." << std::endl << std::endl;
			done = true;
		}
		else
		{
			//If the file does not exist or there was a problem opening it then notify the user and repeat the process
			fin.clear();
			std::cout << std::endl;
			std::cout << "File " << filename << " could not be opened." << std::endl << std::endl;
		}
	}

	return;
}
bool ReadFileCounts(char* filename, int& vertexCount, int& textureCount, int& normalCount, int& faceCount)
{
	std::ifstream fin;
	char input;

	//Initialize the counts
	vertexCount = 0;
	textureCount = 0;
	normalCount = 0;
	faceCount = 0;

	//Open the file
	fin.open(filename);

	//Check if it was successful opening the file
	if (fin.fail() == true)
	{
		return false;
	}

	//Read from the file and continue to read until the end of the file is reached
	fin.get(input);
	while (!fin.eof())
	{
		//If the line starts with 'v' then count either the vertex, the texture coordinates, or the normal vector
		if (input == 'v')
		{
			fin.get(input);
			if (input == ' ') { vertexCount++; }
			if (input == 't') { textureCount++; }
			if (input == 'n') { normalCount++; }
		}

		//If the line starts with 'f' then increment the face count
		if (input == 'f')
		{
			fin.get(input);
			if (input == ' ') { faceCount++; }
		}

		//Otherwise read in the remainder of the line
		while (input != '\n')
		{
			fin.get(input);
		}

		//Start reading the beginning of the next line
		fin.get(input);
	}

	//Close the file
	fin.close();

	return true;
}
bool LoadDataStructures(char*filename, int vertexCount, int textureCount, int normalCount, int faceCount)
{
	VertexType *vertices, *texCoords, *normals;
	FaceType *faces;
	std::ifstream fin;
	int vertexIndex, texCoordIndex, normalIndex, faceIndex, vIndex, tIndex, nIndex;
	char input, input2;
	std::ofstream fout;

	//Initialize the four data structures;
	vertices = new VertexType[vertexCount];
	if (!vertices)
	{
		return false;
	}
	texCoords = new VertexType[textureCount];
	if (!texCoords)
	{
		return false;
	}
	normals = new VertexType[normalCount];
	if (!normals)
	{
		return false;
	}
	faces = new FaceType[faceCount];
	if (!faces)
	{
		return false;
	}

	//Initialize the indices
	vertexIndex = 0;
	texCoordIndex = 0;
	normalIndex = 0;
	faceIndex = 0;

	//Open the file
	fin.open(filename);

	//Check if it was successful in opening the file
	if (fin.fail() == true)
	{
		return false;
	}

	//Read in the vertices, texture coordinates and normals into the data structures
	//IMPORTANT: Also convert to left hand coordinate system since Maya uses a right hand system
	fin.get(input);
	while (!fin.eof())
	{
		if (input == 'v')
		{
			fin.get(input);

			//Read in the vertices
			if (input == ' ')
			{
				fin >> vertices[vertexIndex].x >> vertices[vertexIndex].y >> vertices[vertexIndex].z;

				//Invert the z vertex to change to left hand coordinate system
				vertices[vertexIndex].z = vertices[vertexIndex].z * -1.0f;
				vertexIndex++;
			}

			//Read in the texture UV coordinates
			if (input == 't')
			{
				fin >> texCoords[textureCount].x >> texCoords[textureCount].y;

				//Invert the V texture coordinate to left hand coordinate system
				texCoords[textureCount].y = 1.0f - texCoords[textureCount].y;
				textureCount++;
			}

			//Read in the normals
			if (input == 'n')
			{
				fin >> normals[normalIndex].x >> normals[normalIndex].y >> normals[normalIndex].z;

				//Invert the z normal to change to left hand coordinate system
				normals[normalIndex].z = normals[normalIndex].z * -1.0f;
				normalIndex++;
			}
		}

		//Read in the faces
		if (input == 'f')
		{
			fin.get(input);
			if (input == ' ')
			{
				//Read the face data in backwards to convert it to a left hand system from a right hand system
				fin >> faces[faceIndex].vindex3 >> input2 >> faces[faceIndex].tindex3 >> input2 >> faces[faceIndex].nindex3
					>> faces[faceIndex].vindex2 >> input2 >> faces[faceIndex].tindex2 >> input2 >> faces[faceIndex].nindex2
					>> faces[faceIndex].vindex1 >> input2 >> faces[faceIndex].tindex1 >> input2 >> faces[faceIndex].nindex1;
				faceIndex++;

			}
		}

		//Read in the remainder of the line
		while (input != '\n')
		{
			fin.get(input);
		}

		//Start reading the beginning of the next line
		fin.get(input);
	}
	//Close the file
	fin.close();

	//Open the output file
	fout.open("model.txt");

	//Write out the file header that our model format uses
	fout << "Vertex Count: " << (faceCount * 3) << std::endl;
	fout << std::endl;
	fout << "Data:" << std::endl;
	fout << std::endl;

	//Now loop through all the faces and output the vertices for each face
	for (int i = 0; i < faceIndex; ++i)
	{
		vIndex = faces[i].vindex1 - 1;
		tIndex = faces[i].tindex1 - 1;
		nIndex = faces[i].nindex1 - 1;

		fout << vertices[vIndex].x << ' ' << vertices[vIndex].y << ' ' << vertices[vIndex].z << ' '
			<< texCoords[tIndex].x << ' ' << texCoords[tIndex].y << ' '
			<< normals[nIndex].x << ' ' << normals[nIndex].y << ' ' << normals[nIndex].z << std::endl;

		vIndex = faces[i].vindex2 - 1;
		tIndex = faces[i].tindex2 - 1;
		nIndex = faces[i].nindex2 - 1;


		fout << vertices[vIndex].x << ' ' << vertices[vIndex].y << ' ' << vertices[vIndex].z << ' '
			<< texCoords[tIndex].x << ' ' << texCoords[tIndex].y << ' '
			<< normals[nIndex].x << ' ' << normals[nIndex].y << ' ' << normals[nIndex].z << std::endl;

		vIndex = faces[i].vindex3 - 1;
		tIndex = faces[i].tindex3 - 1;
		nIndex = faces[i].nindex3 - 1;

		fout << vertices[vIndex].x << ' ' << vertices[vIndex].y << ' ' << vertices[vIndex].z << ' '
			<< texCoords[tIndex].x << ' ' << texCoords[tIndex].y << ' '
			<< normals[nIndex].x << ' ' << normals[nIndex].y << ' ' << normals[nIndex].z << std::endl;

	}

	//Close the output file
	fout.close();

	//Release the four data structures

	if (vertices)
	{
		delete[] vertices;
		vertices = 0;
	}
	if (texCoords)
	{
		delete[] texCoords;
		texCoords = 0;
	}
	if (normals)
	{
		delete[] normals;
		normals = 0;
	}
	if (faces)
	{
		delete[] faces;
		faces = 0;
	}
	return true;
}
