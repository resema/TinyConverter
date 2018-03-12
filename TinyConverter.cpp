#include <iostream>
#include <fstream>

typedef struct
{
	float x, y, z;
} VertexType;

typedef struct
{
	int vIndex1, vIndex2, vIndex3;
	int tIndex1, tIndex2, tIndex3;
	int nIndex1, nIndex2, nIndex3;
} FaceType;

void GetModelFilename(char*);
bool ReadFileCounts(char*, int&, int&, int&, int&);
bool LoadDataStructures(char*, int, int, int, int);

int main()
{
	bool result;
	char filename[256];
	int vertexCount, textureCount, normalCount, faceCount;
	char garbage;

	// read in the name of the model file
	GetModelFilename(filename);

	// read in the numer of vertices, tex coords, normals, and faces
	result = ReadFileCounts(filename, vertexCount, textureCount, normalCount, faceCount);
	if (!result)
	{
		return -1;
	}

	// display the counts to the screen for information purposes
	std::cout << std::endl;
	std::cout << "Vertices: " << vertexCount << std::endl;
	std::cout << "UVs:      " << textureCount << std::endl;
	std::cout << "Normals:  " << normalCount << std::endl;
	std::cout << "Faces:    " << faceCount << std::endl;

	// now read the data from the file into the data structures and then output in our model format
	result = LoadDataStructures(filename, vertexCount, textureCount, normalCount, faceCount);
	if (!result)
	{
		return -1;
	}

	// notify the user the model has been converted
	std::cout << "\nFile has been converted." << std::endl;
	std::cout << "\nDo you wish to exit (y/n)?";
	std::cin >> garbage;

    return 0;
}

void GetModelFilename(char* filename)
{
	bool done;
	std::ifstream fin;

	// loop until filename found
	done = false;
	while (!done)
	{
		// ask the user for the filename
		std::cout << "Enter model filename: ";

		// read in the filename
		std::cin >> filename;

		// attempt to open the file
		fin.open(filename);

		if (fin.good())
		{
			// if the file exists and there are no other problems exit loop
			done = true;
		}
		else
		{
			// if the file does no exist or there are issues repeat the process
			fin.clear();
			std::cout << std::endl;
			std::cout << "File " << filename << " could not be openend." << std::endl << std::endl;
		}
	}

	return;
}

bool ReadFileCounts(char* filename, int& vertexCount, int& textureCount, int& normalCount, int& faceCount)
{
	std::ifstream fin;
	char input;
}