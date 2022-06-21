#pragma once

#include <limits>
#include <DirectXMath.h>
#include "vec3_t.h"

struct cplane_t
{
	vec3_t normal;
	float dist;
	uint8_t type; // for fast side tests
	uint8_t signbits; // signx + (signy<<1) + (signz<<1)
	uint8_t pad[2];
};

class matrix_t
{
public:
	matrix_t()
	{
	}

	matrix_t(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23)
	{
		m_mat_val[0][0] = m00;
		m_mat_val[0][1] = m01;
		m_mat_val[0][2] = m02;
		m_mat_val[0][3] = m03;
		m_mat_val[1][0] = m10;
		m_mat_val[1][1] = m11;
		m_mat_val[1][2] = m12;
		m_mat_val[1][3] = m13;
		m_mat_val[2][0] = m20;
		m_mat_val[2][1] = m21;
		m_mat_val[2][2] = m22;
		m_mat_val[2][3] = m23;
	}

	//-----------------------------------------------------------------------------
	// Creates a aMatrix where the x axis = forward
	// the y axis = left, and the z axis = up
	//-----------------------------------------------------------------------------
	void Init(const vec3_t& xAxis, const vec3_t& yAxis, const vec3_t& zAxis, const vec3_t& vecOrigin)
	{
		m_mat_val[0][0] = xAxis.x;
		m_mat_val[0][1] = yAxis.x;
		m_mat_val[0][2] = zAxis.x;
		m_mat_val[0][3] = vecOrigin.x;
		m_mat_val[1][0] = xAxis.y;
		m_mat_val[1][1] = yAxis.y;
		m_mat_val[1][2] = zAxis.y;
		m_mat_val[1][3] = vecOrigin.y;
		m_mat_val[2][0] = xAxis.z;
		m_mat_val[2][1] = yAxis.z;
		m_mat_val[2][2] = zAxis.z;
		m_mat_val[2][3] = vecOrigin.z;
	}

	//-----------------------------------------------------------------------------
	// Creates a aMatrix where the x axis = forward
	// the y axis = left, and the z axis = up
	//-----------------------------------------------------------------------------
	matrix_t(const vec3_t& xAxis, const vec3_t& yAxis, const vec3_t& zAxis, const vec3_t& vecOrigin)
	{
		Init(xAxis, yAxis, zAxis, vecOrigin);
	}

	constexpr void set_forward(const vec3_t& vecForward)
	{
		this->m_mat_val[0][0] = vecForward.x;
		this->m_mat_val[1][0] = vecForward.y;
		this->m_mat_val[2][0] = vecForward.z;
	}

	constexpr void set_left(const vec3_t& vecLeft)
	{
		this->m_mat_val[0][1] = vecLeft.x;
		this->m_mat_val[1][1] = vecLeft.y;
		this->m_mat_val[2][1] = vecLeft.z;
	}

	constexpr void set_up(const vec3_t& vecUp)
	{
		this->m_mat_val[0][2] = vecUp.x;
		this->m_mat_val[1][2] = vecUp.y;
		this->m_mat_val[2][2] = vecUp.z;
	}

	constexpr void set_origin(const vec3_t& vecOrigin)
	{
		this->m_mat_val[0][3] = vecOrigin.x;
		this->m_mat_val[1][3] = vecOrigin.y;
		this->m_mat_val[2][3] = vecOrigin.z;
	}

	void invalidate(void)
	{
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				m_mat_val[i][j] = std::numeric_limits<float>::infinity();
			}
		}
	}

	void angle_matrix(const vec3_t& angles)
	{
		float sr, sp, sy, cr, cp, cy;
		DirectX::XMScalarSinCos(&sy, &cy, DirectX::XMConvertToDegrees(angles[1]));
		DirectX::XMScalarSinCos(&sp, &cp, DirectX::XMConvertToDegrees(angles[0]));
		DirectX::XMScalarSinCos(&sr, &cr, DirectX::XMConvertToDegrees(angles[2]));

		m_mat_val[0][0] = cp * cy;
		m_mat_val[1][0] = cp * sy;
		m_mat_val[2][0] = -sp;

		float crcy = cr * cy;
		float crsy = cr * sy;
		float srcy = sr * cy;
		float srsy = sr * sy;
		m_mat_val[0][1] = sp * srcy - crsy;
		m_mat_val[1][1] = sp * srsy + crcy;
		m_mat_val[2][1] = sr * cp;

		m_mat_val[0][2] = (sp * crcy + srsy);
		m_mat_val[1][2] = (sp * crsy - srcy);
		m_mat_val[2][2] = cr * cp;

		m_mat_val[0][3] = 0.0f;
		m_mat_val[1][3] = 0.0f;
		m_mat_val[2][3] = 0.0f;
	}

	void matrix_set_column(const vec3_t& in, int column)
	{
		m_mat_val[0][column] = in.x;
		m_mat_val[1][column] = in.y;
		m_mat_val[2][column] = in.z;
	}

	void angle_matrix(const vec3_t& angles, const vec3_t& position)
	{
		angle_matrix(angles);
		matrix_set_column(position, 3);
	}

	vec3_t get_x_axis() const { return at(0); }
	vec3_t get_y_axis() const { return at(1); }
	vec3_t get_z_axis() const { return at(2); }
	vec3_t get_origin() const { return at(3); }

	vec3_t at(int i) const { return vec3_t{ m_mat_val[0][i], m_mat_val[1][i], m_mat_val[2][i] }; }

	float* operator[](int i) { return m_mat_val[i]; }
	const float* operator[](int i) const { return m_mat_val[i]; }
	float* base() { return &m_mat_val[0][0]; }
	const float* base() const { return &m_mat_val[0][0]; }

	float m_mat_val[3][4];
};

class VMatrix
{
public:
	VMatrix();
	VMatrix(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33
	);

	// Creates a aMatrix where the x axis = forward
	// the y axis = left, and the z axis = up
	VMatrix(const vec3_t& forward, const vec3_t& left, const vec3_t& up);

	// Construct from a 3x4 aMatrix
	VMatrix(const matrix_t& matrix3x4);

	// Set the values in the aMatrix.
	void Init(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33
	);


	// Initialize from a 3x4
	void Init(const matrix_t& matrix3x4);

	// array access
	float* operator[](int i)
	{
		return m[i];
	}

	const float* operator[](int i) const
	{
		return m[i];
	}

	// Get a pointer to m[ 0 ][ 0 ]
	float* Base()
	{
		return &m[0][0];
	}

	const float* Base() const
	{
		return &m[0][0];
	}

	void SetLeft(const vec3_t& vLeft);
	void SetUp(const vec3_t& vUp);
	void SetForward(const vec3_t& vForward);

	void GetBasisVectors(vec3_t& vForward, vec3_t& vLeft, vec3_t& vUp) const;
	void SetBasisVectors(const vec3_t& vForward, const vec3_t& vLeft, const vec3_t& vUp);

	// Get/Set the translation.
	vec3_t& GetTranslation(vec3_t& vTrans) const;
	void SetTranslation(const vec3_t& vTrans);

	void PreTranslate(const vec3_t& vTrans);
	void PostTranslate(const vec3_t& vTrans);

	matrix_t& As3x4();
	const matrix_t& As3x4() const;
	void CopyFrom3x4(const matrix_t& m3x4);
	void Set3x4(matrix_t& matrix3x4) const;

	bool operator==(const VMatrix& src) const;
	bool operator!=(const VMatrix& src) const { return !(*this == src); }

	// Access the basis vectors.
	vec3_t GetLeft() const;
	vec3_t GetUp() const;
	vec3_t GetForward() const;
	vec3_t GetTranslation() const;


	// aMatrix->Vector operations.
public:
	// Multiply by a 3D Vector (same as operator*).
	void V3Mul(const vec3_t& vIn, vec3_t& vOut) const;

	// Multiply by a 4D Vector.
	//void  V4Mul( const Vector4D &vIn, Vector4D &vOut ) const;

	// Applies the rotation (ignores translation in the aMatrix). (This just calls VMul3x3).
	vec3_t ApplyRotation(const vec3_t& vVec) const;

	// Multiply by a Vector (divides by w, assumes input w is 1).
	vec3_t operator*(const vec3_t& vVec) const;

	// Multiply by the upper 3x3 part of the aMatrix (ie: only apply rotation).
	vec3_t VMul3x3(const vec3_t& vVec) const;

	// Apply the inverse (transposed) rotation (only works on pure rotation aMatrix)
	vec3_t VMul3x3Transpose(const vec3_t& vVec) const;

	// Multiply by the upper 3 rows.
	vec3_t VMul4x3(const vec3_t& vVec) const;

	// Apply the inverse (transposed) transformation (only works on pure rotation/translation)
	vec3_t VMul4x3Transpose(const vec3_t& vVec) const;


	// aMatrix->plane operations.
	//public:
	// Transform the plane. The aMatrix can only contain translation and rotation.
	//void  TransformPlane( const VPlane &inPlane, VPlane &outPlane ) const;

	// Just calls TransformPlane and returns the result.
	//VPlane  operator*(const VPlane &thePlane) const;

	// aMatrix->aMatrix operations.
public:
	VMatrix& operator=(const VMatrix& mOther);

	// Add two matrices.
	const VMatrix& operator+=(const VMatrix& other);

	// Add/Subtract two matrices.
	VMatrix operator+(const VMatrix& other) const;
	VMatrix operator-(const VMatrix& other) const;

	// Negation.
	VMatrix operator-() const;

	// Return inverse aMatrix. Be careful because the results are undefined 
	// if the aMatrix doesn't have an inverse (ie: InverseGeneral returns false).
	VMatrix operator~() const;

	// aMatrix operations.
public:
	// Set to identity.
	void Identity();
	bool IsIdentity() const;
public:
	// The aMatrix.
	float m[4][4];
};

inline void MatrixGetColumn(const matrix_t& src, int nCol, vec3_t& pColumn)
{
	pColumn.x = src[0][nCol];
	pColumn.y = src[1][nCol];
	pColumn.z = src[2][nCol];
}

inline void MatrixPosition(const matrix_t& aMatrix, vec3_t& position)
{
	MatrixGetColumn(aMatrix, 3, position);
}

inline void MatrixAngles(const matrix_t& aMatrix, float* angAngles)
{
	float forward[3];
	float left[3];
	float up[3];

	//
	// Extract the basis vectors from the aMatrix. Since we only need the z
	// component of the up Vector, we don't get x and y.
	//
	forward[0] = aMatrix[0][0];
	forward[1] = aMatrix[1][0];
	forward[2] = aMatrix[2][0];
	left[0] = aMatrix[0][1];
	left[1] = aMatrix[1][1];
	left[2] = aMatrix[2][1];
	up[2] = aMatrix[2][2];

	float xyDist = sqrtf(forward[0] * forward[0] + forward[1] * forward[1]);

	// enough here to get angAngles?
	if (xyDist > 0.001f)
	{
		// (yaw)	y = ATAN( forward.y, forward.x );		-- in our space, forward is the x axis
		angAngles[1] = DirectX::XMConvertToDegrees(atan2f(forward[1], forward[0]));

		// (pitch)	x = ATAN( -forward.z, sqrt(forward.x*forward.x+forward.y*forward.y) );
		angAngles[0] = DirectX::XMConvertToDegrees(atan2f(-forward[2], xyDist));

		// (roll)	z = ATAN( left.z, up.z );
		angAngles[2] = DirectX::XMConvertToDegrees(atan2f(left[2], up[2]));
	}
	else // forward is mostly z, gimbal lock-
	{
		// (yaw)	y = ATAN( -left.x, left.y );			-- forward is mostly z, so use right for yaw
		angAngles[1] = DirectX::XMConvertToDegrees(atan2f(-left[0], left[1]));

		// (pitch)	x = ATAN( -forward.z, sqrt(forward.x*forward.x+forward.y*forward.y) );
		angAngles[0] = DirectX::XMConvertToDegrees(atan2f(-forward[2], xyDist));

		// Assume no roll in this case as one degree of freedom has been lost (i.e. yaw == roll)
		angAngles[2] = 0;
	}
}

inline void MatrixAngles(const matrix_t& aMatrix, vec3_t& angAngles)
{
	MatrixAngles(aMatrix, &angAngles.x);
}

inline void MatrixAngles(const matrix_t& aMatrix, vec3_t& angAngles, vec3_t& vecPosition)
{
	MatrixAngles(aMatrix, angAngles);
	MatrixPosition(aMatrix, vecPosition);
}
