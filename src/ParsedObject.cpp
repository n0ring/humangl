#include <cstdlib>
#include <ctime>
#include "ParsedObject.hpp"

/*
parse v in vector
parse vt in vector
faces in vectors of struct {IofV, IofVt}

*/

unsigned int getFace(std::string &word)
{
	return static_cast<unsigned int>(std::stoi(word) - 1);
}


void ParsedObject::split(std::string &line, std::vector<std::string> &words, char div)
{
	size_t start = 0, end = 0;
	words.clear();
	if (line.empty())
		return;
	while (end < line.size())
	{
		while (start < line.size() && line[start] == div)
			start++;
		end = start;
		while (end < line.size() && line[end] != div)
			end++;
		if (end - start > 0)
			words.push_back(line.substr(start, end - start));
		start = end;
	}
}


bool isLineValid(std::vector<std::string> &words)
{
	if (words.empty())
		return false;
	std::string &token = words[0];
	return token == "v" || token == "f" || token == "vt" || token == "vn" || token == "usemtl" || token == "mtllib";
}

bool ParsedObject::addVertex(std::vector<std::string> &words, std::vector<Pos> &vertices, ModelState &modelState)
{
	if (words.size() < kValidNumVertex)
		return false;
	try
	{
		tmpPos.x = std::stof(words[kIdxXData]);
		tmpPos.y = std::stof(words[kIdxYData]);
		tmpPos.z = std::stof(words[kIdxZData]);
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';
		for (auto &w : words)
			std::cout << w << " ";
		std::cout << std::endl
				  << std::endl;
		return false;
	}
	vertices.push_back(tmpPos);
	modelState.centerOffsetMax.x = std::max(modelState.centerOffsetMax.x, tmpPos.x);
	modelState.centerOffsetMin.x = std::min(modelState.centerOffsetMin.x, tmpPos.x);

	modelState.centerOffsetMax.y = std::max(modelState.centerOffsetMax.y, tmpPos.y);
	modelState.centerOffsetMin.y = std::min(modelState.centerOffsetMin.y, tmpPos.y);
	
	modelState.centerOffsetMax.z = std::max(modelState.centerOffsetMax.z, tmpPos.z);
	modelState.centerOffsetMin.z = std::min(modelState.centerOffsetMin.z, tmpPos.z);
	return true;
}

bool ParsedObject::addNormals(std::vector<std::string> &words, std::vector<Pos> &normals)
{
	if (words.size() != kValidNumVertex)
		return false;
	try
	{
		tmpPos.x = std::stof(words[kIdxXData]);
		tmpPos.y = std::stof(words[kIdxYData]);
		tmpPos.z = std::stof(words[kIdxZData]);
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';
		for (auto &w : words)
			std::cout << w << " ";
		std::cout << std::endl
				  << std::endl;
		return false;
	}
	normals.push_back(tmpPos);
	return true;
}

bool ParsedObject::addTextureCoord(std::vector<std::string> &words, std::vector<textPos> &text_coord)
{
	if (words.size() != kValidNumTexCoord)
		return false;
	try
	{
		tmpTexPos.x = std::stof(words[kIdxXData]);
		tmpTexPos.y = std::stof(words[kIdxYData]);
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';
		for (auto &w : words)
			std::cout << w << " ";
		std::cout << std::endl
				  << std::endl;
		return false;
	}
	text_coord.push_back(tmpTexPos);
	return true;
}

bool ParsedObject::addComplexFace(std::vector<std::string> &words, std::vector<face> &faces)
{
	int countFaces = 0;

	if (words.size() != kValidNumFaceTriangle && words.size() != kValidNumFaceSquare)
	{
		std::cout << "incorrect file. Use only triangles please: " << words.size() << std::endl;
		return false;
	}
	for (auto &word : words)
	{
		split(word, m_faceWords, '/');
		try
		{
			if (m_faceWords.size() == 2 || m_faceWords.size() == 3)
			{
				if (m_faceWords.size() > 1) // n/n
				{
					m_tmpFaces[countFaces].vtx = getFace(m_faceWords[0]);
					m_tmpFaces[countFaces].txt = getFace(m_faceWords[1]);
				}
				if (m_faceWords.size() > 2) // n/n/n
					m_tmpFaces[countFaces].nrm = getFace(m_faceWords[2]);
				countFaces++;
			}
		}
		catch (const std::exception &e)
		{
			std::cerr << e.what() << '\n';
			std::cout << "Error with face: " << word << std::endl;
			return false;
		}
	}
	if (countFaces == kTriangleCount) // no need to mix faces
	{
		for (int i = 0; i < 3; i++)
			faces.push_back(m_tmpFaces[i]);
	}
	else if (countFaces == kSquareCount) // need to mix in triangle
	{
		if (m_tmpFaces.size() < 4)
			std::cout << "error with faces\n";
		faces.push_back(m_tmpFaces[0]);
		faces.push_back(m_tmpFaces[1]);
		faces.push_back(m_tmpFaces[2]);
		faces.push_back(m_tmpFaces[0]);
		faces.push_back(m_tmpFaces[2]);
		faces.push_back(m_tmpFaces[3]);
	}
	return true;
}

bool ParsedObject::addSimpleFace(std::vector<std::string> &words, std::vector<face> &faces)
{
	if (words.size() == kValidNumFaceTriangle)
	{
		for (int i = 1; i < words.size(); i++)
		{
			m_tmpFace.vtx = getFace(words[i]);
			faces.push_back(m_tmpFace);
		}
	}
	else if (words.size() == kValidNumFaceSquare) // -1 for f in line
	{
		m_tmpFace.vtx = getFace(words[1]);
		faces.push_back(m_tmpFace);
		m_tmpFace.vtx = getFace(words[2]);
		faces.push_back(m_tmpFace);
		m_tmpFace.vtx = getFace(words[3]);
		faces.push_back(m_tmpFace);
		m_tmpFace.vtx = getFace(words[1]);
		faces.push_back(m_tmpFace);
		m_tmpFace.vtx = getFace(words[3]);
		faces.push_back(m_tmpFace);
		m_tmpFace.vtx = getFace(words[4]);
		faces.push_back(m_tmpFace);
	}
	else 
	{
		std::cout << "Incorect number of faces\n";
		return false;
	}
	return true;
}

void ParsedObject::parseObjFile(ModelState &modelState)
{
	std::string line;
	std::ifstream file(m_objFileName);
	std::vector<std::string> words;
	std::vector<Pos> vertices;
	std::vector<textPos> text_coord;
	std::vector<face> faces;
	std::vector<Pos> normals;
	vertex tmpVtx;

	float countVert = 0;
	float mtlCount = 0;
	m_curMtlIdx = 0.0f;
	m_tmpFace.mtl = m_curMtlIdx;
	tmpVtx.mtl = m_curMtlIdx;

	int debug_count = 0;
	if (file.is_open() == false)
	{
		std::cout << "File error: " << m_objFileName << std::endl;
		return;
	}
	while (getline(file, line))
	{
		if (line.empty())
			continue;
		if (line.back() == '\r')
			line.pop_back();
		split(line, words, ' ');
		if (isLineValid(words))
		{
			if (words[kIdxTypeData] == kVertexData) // VERTEX
			{
				m_ParseStatus = addVertex(words, vertices, modelState);
				countVert++;
			}
			else if (words[kIdxTypeData] == kTextureCoordData) // TEXTURES
			{
				m_ParseStatus = addTextureCoord(words, text_coord);
			}
			else if (words[kIdxTypeData] == kFaceData) // FACES
			{
				if (line.find('/') != std::string::npos) // v/t/n
				{
					m_ParseStatus = addComplexFace(words, faces);
				}
				else
				{
					m_ParseStatus = addSimpleFace(words, faces);
				}
			}
			else if (words[kIdxTypeData] == kNormalData) // NORMALS
			{
				if (!modelState.hasNormals)
					modelState.hasNormals = true;
				m_ParseStatus = addNormals(words, normals);
			}
			else if (words[kIdxTypeData] == kMtlData && words.size() == 2)
			{
				if (m_materialMap.count(words[kIdxMaterial]))
					m_curMtlIdx = m_materialMap[words[kIdxMaterial]];
				else
				{
					mtlCount++;
					m_materialMap[words[kIdxMaterial]] = mtlCount;
				}
				m_curMtlIdx = m_materialMap[words[kIdxMaterial]];
				m_tmpFace.mtl = m_curMtlIdx; // for addSimpleFace
				tmpTexPos.mtl = m_curMtlIdx; // for addTextureCoord
				tmpPos.mtl = m_curMtlIdx; // for normals and vertex 
				for (auto& el : m_tmpFaces)
					el.mtl = m_curMtlIdx;
			}
			else if (words[kIdxTypeData] == kMtlFileData)
			{
				if (words.size() == 2)
				{
					m_mtlFileName = words[kIdxMtlFileName];
					m_ParseStatus = true;
				}
				else
				{
					std::cout << "mtllib should have 1 argument, but has " << words.size() - 1 << std::endl;;
					m_ParseStatus = false;
				}
			}
			if (m_ParseStatus == false)
			{
				std::cout << "Bad line: " << line << std::endl;
				return;
			}
		}
	}

	// float centerOffsetX = (modelState.centerOffsetMin.x + modelState.centerOffsetMax.x) / 2.0f;
	// float centerOffsetY = (modelState.centerOffsetMin.y + modelState.centerOffsetMax.y) / 2.0f;
	// float centerOffsetZ = (modelState.centerOffsetMin.z + modelState.centerOffsetMax.z) / 2.0f;
	// for (auto& v : vertices)
	// {
	// 	v.x -= centerOffsetX;
	// 	v.y -= centerOffsetY;
	// 	v.z -= centerOffsetZ;
	// }
	for (auto f : faces)
	{
		tmpVtx.x = vertices[f.vtx].x;
		tmpVtx.y = vertices[f.vtx].y;
		tmpVtx.z = vertices[f.vtx].z;

		if (!text_coord.empty()) // textures
		{
			tmpVtx.u = text_coord[f.txt].x;
			tmpVtx.v = text_coord[f.txt].y;
		}
		else
		{
			float theta = atan2(vertices[f.vtx].z, vertices[f.vtx].x);
			float phi =
				acos(vertices[f.vtx].y / sqrt(vertices[f.vtx].x * vertices[f.vtx].x + vertices[f.vtx].y * vertices[f.vtx].y + vertices[f.vtx].z * vertices[f.vtx].z));
			tmpVtx.u = (theta + M_PI) / (2.0f * M_PI);
			tmpVtx.v = phi / M_PI;
		}
		tmpVtx.vnx = normals.empty() ? 0 : normals[f.nrm].x;
		tmpVtx.vny = normals.empty() ? 0 : normals[f.nrm].y;
		tmpVtx.vnz = normals.empty() ? 0 : normals[f.nrm].z;
		if (f.mtl) // if face hasn't any material look on other data
			tmpVtx.mtl = f.mtl;
		else if (vertices[f.vtx].mtl)
			tmpVtx.mtl = vertices[f.vtx].mtl;
		else if (text_coord[f.txt].mtl)
			tmpVtx.mtl = text_coord[f.txt].mtl;
		else if (!normals.empty() && normals[f.nrm].mtl)
			tmpVtx.mtl = normals[f.nrm].mtl;
		else 
			tmpVtx.mtl = 0;
		m_positions.push_back(tmpVtx);
	}

	// if (modelState.centerOffset.x < 0 && modelState.centerOffset.y < 0)
	// {
	// 	modelState.centerOffset = modelState.centerOffset * -1.0f;

	// }

	m_materials.resize(mtlCount + 1); // at least one for default
	m_varibles["materials_size"] = std::to_string(static_cast<int>(mtlCount + 1)); // for shader to change in text
}

void ParsedObject::generateIndeces()
{
	std::unordered_map<vertex, int, vertex_hash> mp; // [vtx, idx]
	for (int i = 0; i < m_positions.size(); i++)
	{
		if (mp.count(m_positions[i]))
		{
			m_indices.push_back(mp[m_positions[i]]);
		}
		else
		{
			mp[m_positions[i]] = i;
			m_indices.push_back(i);
		}
	}
}

void ParsedObject::generateNormals()
{
	for (int i = 2; i < m_positions.size(); i += 3)
	{
		// sides						x						y				z
		nrg::vec3 v0 = nrg::vec3(m_positions[i - 2].x, m_positions[i - 2].y, m_positions[i - 2].z);
		nrg::vec3 v1 = nrg::vec3(m_positions[i - 1].x, m_positions[i - 1].y, m_positions[i - 1].z);
		nrg::vec3 v2 = nrg::vec3(m_positions[i].x, m_positions[i].y, m_positions[i].z);

		nrg::vec3 edge1 = v1 - v0;
		nrg::vec3 edge2 = v2 - v0;
		nrg::vec3 normal = nrg::normalize(nrg::cross(edge1, edge2));
		m_positions[i - 2].vnx = m_positions[i - 1].vnx = m_positions[i].vnx = normal.x;
		m_positions[i - 2].vny = m_positions[i - 1].vny = m_positions[i].vny = normal.y;
		m_positions[i - 2].vnz = m_positions[i - 1].vnz = m_positions[i].vnz = normal.z;
	}
}

bool ParsedObject::addDataVec3(std::vector<std::string> &words, nrg::vec3 &v)
{
	if (words.size() == 4)
	{
		v.x = std::stof(words[1]);
		v.y = std::stof(words[2]);
		v.z = std::stof(words[3]);
	}
	else
	{
		std::cout << "Error with add data to " << words[0] << std::endl;
		return false;
	}
	return true;
}

bool ParsedObject::addDataF(std::vector<std::string> &words, float& f)
{
	if (words.size() == 2)
	{
		try
		{
			f = std::stof(words[1]);
			/* code */
		}
		catch(const std::exception& e)
		{
			std::cout << words[0] << " " << words[1] << std::endl;
			std::cerr << e.what() << '\n';
		}
		
	}
	else
	{
		std::cout << "Error with add data to " << words[0] << std::endl;
		return false;
	}
	return true;
}

std::string getPathToFile(std::string& filePath)
{
	int i = filePath.size() - 1;
	while (i >= 0 && filePath[i] != '/')
		i--;

	return filePath.substr(0, i + 1);
}

bool ParsedObject::parseMtlFile()
{
	std::string line;
	std::vector<std::string> words;
	int curMtlIdx = 0; // default
	m_materials[0].Ns = 100.0f;
	m_materials[0].ka = nrg::vec3(0.5f, 0.5f, 0.31f);
	m_materials[0].kd = nrg::vec3(1.0f, 0.5f, 0.31f);
	m_materials[0].ks = nrg::vec3(0.5f, 0.5f, 0.5f);
	m_materials[0].ke = nrg::vec3(0.0f, 0.0f, 0.0f);
	m_materials[0].ni = 1.45f;
	m_materials[0].d = 1.0f;
	m_materials[0].illum = 2;

	if (m_mtlFileName.empty())
	{
		std::cout << "no mtl file name in " << m_objFileName << std::endl;
		return false;
	}
	m_mtlFileName = getPathToFile(m_objFileName) + m_mtlFileName;
	std::ifstream file(m_mtlFileName);
	if (file.is_open() == false)
	{
		std::cout << "error with open file: " << m_mtlFileName << std::endl;
		return false;
	}
	std::cout << "mtl file: " << m_mtlFileName << std::endl;
	m_ParseStatus = true;
	while (getline(file, line))
	{
		if (line.empty())
			continue;
		if (line.back() == '\r')
			line.pop_back();
		split(line, words, ' ');
		if (!words.empty())
		{
			if ((words[kIdxTypeData] == kMtlNewMtlData))
			{
				if (words.size() == 2 && m_materialMap.count(words[kIdxMtlFileName]))
					curMtlIdx = m_materialMap[words[kIdxMtlFileName]];
				else
				{
					std::cout << "Material not exist from line: " << line << std::endl;
					for (auto& [material, idx] : m_materialMap)
						std::cout << material << " " << idx << std::endl;
					m_ParseStatus = false;
				}
			}
			else if ((words[kIdxTypeData] == kMtlNsData) && curMtlIdx)
			{
				m_ParseStatus = addDataF(words, m_materials[curMtlIdx].Ns);
			}
			else if ((words[kIdxTypeData] == kMtlKaData) && curMtlIdx)
			{
				m_ParseStatus = addDataVec3(words, m_materials[curMtlIdx].ka);
			}
			else if ((words[kIdxTypeData] == kMtlKdData) && curMtlIdx)
			{
				m_ParseStatus = addDataVec3(words, m_materials[curMtlIdx].kd);
			}
			else if ((words[kIdxTypeData] == kMtlKsData) && curMtlIdx)
			{
				m_ParseStatus = addDataVec3(words, m_materials[curMtlIdx].ks);
			}
			else if ((words[kIdxTypeData] == kMtlKeData) && curMtlIdx)
			{
				m_ParseStatus = addDataVec3(words, m_materials[curMtlIdx].ke);
			}
			else if ((words[kIdxTypeData] == kMtlNiData) && curMtlIdx)
			{
				m_ParseStatus = addDataF(words, m_materials[curMtlIdx].ni);
			}
			else if ((words[kIdxTypeData] == kMtlDData) && curMtlIdx)
			{
				m_ParseStatus = addDataF(words, m_materials[curMtlIdx].d);
			}
			else if ((words[kIdxTypeData] == kMtlIlData) && curMtlIdx)
			{
				m_ParseStatus = addDataF(words, m_materials[curMtlIdx].illum);
			}
			if (m_ParseStatus == false)
				return false;
		}
	}
	return true;
}

ParsedObject::ParsedObject(std::string objFileName, ModelState &modelState) : m_objFileName(objFileName), m_tmpFaces(4)
{
	parseObjFile(modelState);
	if (m_ParseStatus)
	{
		if (modelState.hasNormals == false)
		{
			generateNormals();
			modelState.hasNormals = true;
		}
		generateIndeces();
		if (parseMtlFile() == false)
		{
			m_materials.resize(1);
			m_ParseStatus = true;
			for (auto& pos : m_positions)
				pos.mtl = 0;
		}
		m_materials[0].Ns = 96.0f;
		m_materials[0].ka = nrg::vec3(0.0f, 0.0f, 0.0f);
		m_materials[0].kd = nrg::vec3(0.64f, 0.64f, 0.64f);
		m_materials[0].ks = nrg::vec3(0.5f, 0.5f, 0.5f);
		m_materials[0].ke = nrg::vec3(0.0f, 0.0f, 0.0f);
		m_materials[0].ni = 1.0f;
		m_materials[0].d = 1.0f;
		m_materials[0].illum = 2;
		std::cout << "Log: Parse done for file: " << m_objFileName << ". Verticies: " << m_positions.size() << " indeces: " << m_indices.size() << std::endl;
	}
}

void ParsedObject::clearData()
{
	m_positions.clear();
	m_materials.clear();
	m_indices.clear();
}


/*
i need to know for each fragpos which material use
1. add all data to every vertex as one more positions (which is stupid)
2. use some kind of indeces as arr/map for every material
2.1 use struct material {}
2.2 on parse shader change var to set arrays of materials

map [materialname, idx]
to m_positions (vertex) add idx float for material idx

*/