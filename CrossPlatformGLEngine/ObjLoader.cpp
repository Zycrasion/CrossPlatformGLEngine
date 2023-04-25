#include "ObjLoader.hpp"
using namespace std;

float *ParseVector(const char *line, int size = 3)
{
	float *vec = new float[size]{};

	int index = 0;
	bool negate = false;
	std::string str;
	float number = 0;

	bool ReadingNumber = false;

	while (line[0] != '\0')
	{
		if (!ReadingNumber && isdigit((int)line[0]))
			ReadingNumber = true;

		if (line[0] == '-')
		{
			negate = true;
		}

		if (isdigit((int)line[0]) || line[0] == '.')
		{
			str.push_back((char)line[0]);
		}
		else if (ReadingNumber && !isdigit((int)line[0]))
		{
			ReadingNumber = false;
			vec[index++] = std::stof(str) * (negate ? -1 : 1);
			str = "";
			negate = false;
		}
		line++;
	}
	vec[index++] = std::stof(str) * (negate ? -1 : 1);
	return vec;
}

std::vector<unsigned int> ParseFaces(const char *line)
{

	std::vector<unsigned int> indices = {};
	line++;
	while (line[0] != '\0')
	{
		if (line[0] == ' ')
		{
			line++;
			continue;
		}

		if (isdigit(line[0]))
		{
			std::string str;
			str.push_back(line[0]);
			line++;
			while (!(line[0] == '/' || line[0] == '\0' || line[0] == ' '))
			{
				str.push_back(line[0]);
				line++;
			}
			indices.push_back(stoi(str));
		}
		else
		{
			line++;
		}
	}

	std::vector<unsigned int> elements = {};

	for (int i = 0; i < indices.size(); i += 3)
	{
		elements.push_back(indices[i]);
	}

	return indices;
}

Mesh *LoadObj(const char *filename)
{

	string lineContents;

	ifstream ObjectFile(filename);

	vector<float> vertices;
	vector<float> UV;
	vector<float> normals;
	vector<unsigned int> indices;

	while (getline(ObjectFile, lineContents))
	{
		if (lineContents.starts_with("#"))
			continue;

		if (lineContents.starts_with("vt"))
		{
			float *vec = ParseVector(lineContents.c_str(), 2);
			UV.push_back(vec[0]);
			UV.push_back(vec[1]);
		}
		else if (lineContents.starts_with("vn"))
		{

			float *vec = ParseVector(lineContents.c_str());
			for (int i = 0; i < 3; i++)
				normals.push_back(vec[i]);
		}
		else if (lineContents.starts_with("v"))
		{
			float *vec = ParseVector(lineContents.c_str());
			for (int i = 0; i < 3; i++)
				vertices.push_back(vec[i]);
		}
		else if (lineContents.starts_with("f"))
		{
			std::vector<unsigned int> faces = ParseFaces(lineContents.c_str());
			for (int i = 0; i < faces.size(); i++)
			{
				indices.push_back(faces[i]);
			}
		}
	}
	float *uv = new float[2 * indices.size()];
	float *norms = new float[3 * indices.size()];
	float *verts = new float[3 * indices.size()];

	for (int i = 0; i < indices.size(); i += 3)
	{
		int index = indices[i] - 1;
		verts[i + 0] = vertices[(index * 3) + 0];
		verts[i + 1] = vertices[(index * 3) + 1];
		verts[i + 2] = vertices[(index * 3) + 2];

		int textureIndex = indices[i + 1] - 1;
		uv[(i / 3) * 2 + 0] = UV[(textureIndex * 2) + 0];
		uv[(i / 3) * 2 + 1] = UV[(textureIndex * 2) + 1];

		int normalIndex = indices[i + 2] - 1;
		norms[i + 0] = normals[(normalIndex) + 0];
		norms[i + 1] = normals[(normalIndex) + 1];
		norms[i + 2] = normals[(normalIndex) + 2];
	}
	Mesh *mesh = new Mesh(verts, indices.size() / 3, NULL, NULL, uv, norms);

	return mesh;
}