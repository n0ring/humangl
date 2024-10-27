#pragma once

#include "glm_inc.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "nrg_math.hpp"

// #define nrg glm

class GLM_utils
{
public:
	static void setGLMVector(nrg::vec3 &glm_vec, aiVector3D &assimp_vec)
	{
		glm_vec = nrg::vec3(assimp_vec.x, assimp_vec.y, assimp_vec.z);
	}

	static nrg::quat GetGLMQuat(const aiQuaternion &pOrientation)
	{
		return nrg::quat(pOrientation.w, pOrientation.x, pOrientation.y, pOrientation.z);
	}

	static nrg::mat4 convertMatrixToGLMFormat(const aiMatrix4x4 &from)
	{
		nrg::mat4 to;
		// the a,b,c,d in assimp is the row ; the 1,2,3,4 is the column
		to[0][0] = from.a1;
		to[1][0] = from.a2;
		to[2][0] = from.a3;
		to[3][0] = from.a4;
		to[0][1] = from.b1;
		to[1][1] = from.b2;
		to[2][1] = from.b3;
		to[3][1] = from.b4;
		to[0][2] = from.c1;
		to[1][2] = from.c2;
		to[2][2] = from.c3;
		to[3][2] = from.c4;
		to[0][3] = from.d1;
		to[1][3] = from.d2;
		to[2][3] = from.d3;
		to[3][3] = from.d4;
		return to;
	}

	// static void printMatrix(const nrg::mat4 &matrix)
	// {
	// 	for (int row = 0; row < 4; ++row)
	// 	{
	// 		for (int col = 0; col < 4; ++col)
	// 		{
	// 			std::cout << matrix[col][row] << ' '; // nrg::mat4 хранит данные в колонно-ориентированном виде
	// 		}
	// 		std::cout << std::endl;
	// 	}

	// 	std::cout << std::endl;
	// }
};