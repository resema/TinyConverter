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

	// initialize the counts
	vertexCount = 0;
	textureCount = 0;
	normalCount = 0;
	faceCount = 0;

	// open the file 
	fin.open(filename);

	// check if it was successful in opening the file
	if (fin.fail() == true)
	{
		return false;
	}

	// read from the file and continue to read until the end of the file is reached
	fin.get(input);
	while (!fin.eof())
	{
		// if the line starts with an 'v' then count either the vertex, the texture or the normal
		if (input == 'v')
		{
			fin.get(input);
			if (input == ' ') { vertexCount++; }
			if (input == 't') { textureCount++; }
			if (input == 'n') { normalCount++; }
		}

		// if the line starts with 'f' then increment the face count
		if (input == 'f')
		{
			fin.get(input);
			if (input == ' ') { faceCount++; }
		}

		// other wise read in the remainder of the line
		while (input != '\n')
		{
			fin.get(input);
		}

		// start reading the beginning of the next line
		fin.get(input);
	}
	
	// close the file
	fin.close();

	return true;
}

bool LoadDataStructures(char* filename, int vertexCount, int textureCount, int normalCount, int faceCount)
{
	VertexType *vertices, *texcoords, *normals;
	FaceType *faces;
	std::ifstream fin;
	int vertexIndex, texcoordIndex, normalIndex, faceIndex, vIndex, tIndex, nIndex;
	char input, input2;
	std::ofstream fout;

	// initialize the four data structures
	vertices = new VertexType[vertexCount];
	if (!vertices)
	{
		return false;
	}

	texcoords = new VertexType[textureCount];
	if (!texcoords)
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

	// initialize the indexes
	vertexIndex = 0;
	texcoordIndex = 0;
	normalIndex = 0;
	faceIndex = 0;

	// open the file
	fin.open(filename);

	// check if it was successful in opening the file
	if (fin.fail() == true)
	{
		return false;
	}

	// read in the vertices, texture coord, and normals into the data structures
	//  important: convert to LH coord system
	fin.get(input);
	while (!fin.eof())
	{
		if (input == 'v')
		{
			fin.get(input);

			// read in the verties
			if (input == ' ')
			{
				fin >> vertices[vertexIndex].x >> vertices[vertexIndex].y >> vertices[vertexIndex].z;

				// invert the Z vertex to change to left hand system
				vertices[vertexIndex].z = vertices[vertexIndex].z * -1.f;
				vertexIndex++;
			}

			// read in the texture uv coord
			if (input == 't')
			{
				fin >> texcoords[texcoordIndex].x >> texcoords[texcoordIndex].y;

				// invert the V texture coord to LHS
				texcoords[texcoordIndex].y = 1.f - texcoords[texcoordIndex].y;
				texcoordIndex++;
			}

			// read in the normals
			if (input == 'n')
			{
				fin >> normals[normalIndex].x >> normals[normalIndex].y >> normals[normalIndex].z;

				// invert the Z normal to change to LHS
				normals[normalIndex].z = normals[normalIndex].z * -1.f;
				normalIndex++;
			}
		}

		// read in the face
		if (input == 'f')
		{
			fin.get(input);
			if (input == ' ')
			{
				// read the face data in backwards to convert it to a LHS
				fin >> faces[faceIndex].vIndex3 >> input2 >> faces[faceIndex].tIndex3 >> input2 >> faces[faceIndex].nIndex3
					>> faces[faceIndex].vIndex2 >> input2 >> faces[faceIndex].tIndex2 >> input2 >> faces[faceIndex].nIndex2
					>> faces[faceIndex].vIndex1 >> input2 >> faces[faceIndex].tIndex1 >> input2 >> faces[faceIndex].nIndex1;
				faceIndex++;
			}
		}

		// read in the remainder of the line
		while (input != '\n')
		{
			fin.get(input);
		}

		// start reading the beginning of the next line
		fin.get(input);
	}

	// close the file
	fin.close();

	// open the output file
	fout.open("model.txt");

	// write out the file header that our model format uses
	fout << "Vertex Count: " << (faceCount * 3) << std::endl;
	fout << std::endl;
	fout << "Data: " << std::endl;
	fout << std::endl;

	// now loop through all the faces and output the three vertices for each face
	for (int i = 0; i < faceIndex; i++)
	{
		vIndex = faces[i].vIndex1 - 1;
		tIndex = faces[i].tIndex1 - 1;
		nIndex = faces[i].nIndex1 - 1;

		fout << vertices[vIndex].x << ' ' << vertices[vIndex].y << ' ' << vertices[vIndex].z << ' '
			<< texcoords[tIndex].x << ' ' << texcoords[tIndex].y << ' '
			<< normals[nIndex].x << ' ' << normals[nIndex].y << ' ' << normals[nIndex].z << std::endl;

		vIndex = faces[i].vIndex2 - 1;
		tIndex = faces[i].tIndex2 - 1;
		nIndex = faces[i].nIndex2 - 1;

		fout << vertices[vIndex].x << ' ' << vertices[vIndex].y << ' ' << vertices[vIndex].z << ' '
			<< texcoords[tIndex].x << ' ' << texcoords[tIndex].y << ' '
			<< normals[nIndex].x << ' ' << normals[nIndex].y << ' ' << normals[nIndex].z << std::endl;

		vIndex = faces[i].vIndex3 - 1;
		tIndex = faces[i].tIndex3 - 1;
		nIndex = faces[i].nIndex3 - 1;

		fout << vertices[vIndex].x << ' ' << vertices[vIndex].y << ' ' << vertices[vIndex].z << ' '
			<< texcoords[tIndex].x << ' ' << texcoords[tIndex].y << ' '
			<< normals[nIndex].x << ' ' << normals[nIndex].y << ' ' << normals[nIndex].z << std::endl;
	}

	// close the output file
	fout.close();

	// release the four data structures
	if (vertices)
	{
		delete[] vertices;
		vertices = nullptr;
	}
	if (texcoords)
	{
		delete[] texcoords;
		texcoords = nullptr;
	}
	if (normals)
	{
		delete[] normals;
		normals = nullptr;
	}
	if (faces)
	{
		delete[] faces;
		faces = nullptr;
	}

	return true;
}