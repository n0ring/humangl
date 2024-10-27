#include "nrg_math.hpp"

namespace nrg
{
	mat4 operator*(mat4 m1, mat4 m2)
	{
		vec4 SrcA0 = m1[0];
		vec4 SrcA1 = m1[1];
		vec4 SrcA2 = m1[2];
		vec4 SrcA3 = m1[3];

		vec4 SrcB0 = m2[0];
		vec4 SrcB1 = m2[1];
		vec4 SrcB2 = m2[2];
		vec4 SrcB3 = m2[3];

		mat4 Result;
		Result[0] = SrcA0 * SrcB0[0] + SrcA1 * SrcB0[1] + SrcA2 * SrcB0[2] + SrcA3 * SrcB0[3];
		Result[1] = SrcA0 * SrcB1[0] + SrcA1 * SrcB1[1] + SrcA2 * SrcB1[2] + SrcA3 * SrcB1[3];
		Result[2] = SrcA0 * SrcB2[0] + SrcA1 * SrcB2[1] + SrcA2 * SrcB2[2] + SrcA3 * SrcB2[3];
		Result[3] = SrcA0 * SrcB3[0] + SrcA1 * SrcB3[1] + SrcA2 * SrcB3[2] + SrcA3 * SrcB3[3];
		return Result;
	}

	vec4 operator*(mat4 m, vec4 v)
	{
		vec4 result;
		result.x = m[0][0] * v.x + m[1][0] * v.y + m[2][0] * v.z + m[3][0] * v.w;
		result.y = m[0][1] * v.x + m[1][1] * v.y + m[2][1] * v.z + m[3][1] * v.w;
		result.z = m[0][2] * v.x + m[1][2] * v.y + m[2][2] * v.z + m[3][2] * v.w;
		result.w = m[0][3] * v.x + m[1][3] * v.y + m[2][3] * v.z + m[3][3] * v.w;
		return result;
	}

	vec3 cross(vec3 a, vec3 b)
	{
		return vec3(
			a.y * b.z - a.z * b.y,
			a.z * b.x - a.x * b.z,
			a.x * b.y - a.y * b.x
		);
	}

	mat4 translate(mat4 m, vec3 v)
	{
		mat4 Result(m);
		Result[3] = m[0] * v[0] + m[1] * v[1] + m[2] * v[2] + m[3];
		return Result;
	}

	mat4 scale(mat4 m, vec3 v)
	{
		mat4 Result;
		Result[0] = m[0] * v[0];
		Result[1] = m[1] * v[1];
		Result[2] = m[2] * v[2];
		Result[3] = m[3];
		return Result;
	}

	float radians(float degrees)
	{
		return degrees * static_cast<float>(0.01745329251994329576923690768489);
	}

	vec3 dot(vec3 a, vec3 b) { return a * b; }


	vec3 normalize(vec3 vec)
	{
		float length = std::sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
    	if (length != 0.0) 
		{
			vec[0] /= length;
			vec[1] /= length;
			vec[2] /= length;
    	}
		return vec;
	}

mat4 rotate(mat4 m, float angle, vec3 v)
{
    float const a = angle;
    float const c = cos(a);
    float const s = sin(a);
    vec3 axis(normalize(v));
    vec3 temp(axis * (1.0f - c));
    mat4 Rotate(1.0f);

    Rotate[0][0] = c + temp[0] * axis[0];
    Rotate[0][1] = temp[0] * axis[1] + axis[2] * s;
    Rotate[0][2] = temp[0] * axis[2] - axis[1] * s;

    Rotate[1][0] = temp[1] * axis[0] - s * axis[2];
    Rotate[1][1] = c + temp[1] * axis[1];
    Rotate[1][2] = temp[1] * axis[2] + s * axis[0];

    Rotate[2][0] = temp[2] * axis[0] + s * axis[1];
    Rotate[2][1] = temp[2] * axis[1] - s * axis[0];
    Rotate[2][2] = c + temp[2] * axis[2];

    mat4 Result;
    Result[0] = m[0] * Rotate[0][0] + m[1] * Rotate[0][1] + m[2] * Rotate[0][2] + m[3] * Rotate[0][3];
    Result[1] = m[0] * Rotate[1][0] + m[1] * Rotate[1][1] + m[2] * Rotate[1][2] + m[3] * Rotate[1][3];
    Result[2] = m[0] * Rotate[2][0] + m[1] * Rotate[2][1] + m[2] * Rotate[2][2] + m[3] * Rotate[2][3];
    Result[3] = m[0] * Rotate[3][0] + m[1] * Rotate[3][1] + m[2] * Rotate[3][2] + m[3] * Rotate[3][3];
    return Result;
}
    quat slerp(const quat& q1, const quat& q2, float t)
    {
        float dotProduct = q1.w * q2.w + q1.x * q2.x + q1.y * q2.y + q1.z * q2.z;
        
        const float threshold = 0.9995f;
        if (dotProduct > threshold) {
            // Линейная интерполяция при близких кватернионах
            quat result(
                q1.w + t * (q2.w - q1.w),
                q1.x + t * (q2.x - q1.x),
                q1.y + t * (q2.y - q1.y),
                q1.z + t * (q2.z - q1.z)
            );
            return normalize(result);
        }

        // Обеспечение кратчайшего пути
        if (dotProduct < 0.0f) {
            dotProduct = -dotProduct;
            quat q2_neg(-q2.w, -q2.x, -q2.y, -q2.z);
            return slerp(q1, q2_neg, t);
        }

        float theta_0 = acos(dotProduct);  // Угол между кватернионами
        float theta = theta_0 * t;         // Интерполированный угол

        quat q3(
            q2.w - q1.w * dotProduct,
            q2.x - q1.x * dotProduct,
            q2.y - q1.y * dotProduct,
            q2.z - q1.z * dotProduct
        );
        q3 = normalize(q3);

        return quat(
            q1.w * cos(theta) + q3.w * sin(theta),
            q1.x * cos(theta) + q3.x * sin(theta),
            q1.y * cos(theta) + q3.y * sin(theta),
            q1.z * cos(theta) + q3.z * sin(theta)
        );
    }

    vec3 mix(const vec3& a, const vec3& b, float t)
    {
        return vec3(
            a.x * (1.0f - t) + b.x * t,
            a.y * (1.0f - t) + b.y * t,
            a.z * (1.0f - t) + b.z * t
        );
    }

	quat normalize(quat q)
	{
		float magnitude = std::sqrt(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
		if (magnitude > 0.0f)
		{
			float invMag = 1.0f / magnitude;
			q.x *= invMag;
			q.y *= invMag;
			q.z *= invMag;
			q.w *= invMag;
		}
		return q;
	}
	mat4 toMat4(const quat &q)
	{
		mat4 result = mat4::identity();

		float xx = q.x * q.x;
		float yy = q.y * q.y;
		float zz = q.z * q.z;
		float xy = q.x * q.y;
		float xz = q.x * q.z;
		float yz = q.y * q.z;
		float wx = q.w * q.x;
		float wy = q.w * q.y;
		float wz = q.w * q.z;

		result[0] = vec4(1.0f - 2.0f * (yy + zz), 2.0f * (xy + wz), 2.0f * (xz - wy), 0.0f);
		result[1] = vec4(2.0f * (xy - wz), 1.0f - 2.0f * (xx + zz), 2.0f * (yz + wx), 0.0f);
		result[2] = vec4(2.0f * (xz + wy), 2.0f * (yz - wx), 1.0f - 2.0f * (xx + yy), 0.0f);
		result[3] = vec4(0.0f, 0.0f, 0.0f, 1.0f);

		return result;
	}

}