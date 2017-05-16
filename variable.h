#pragma once

#ifdef _M_X64
typedef __int32				INT16;
typedef unsigned __int32	UINT16;
typedef __int64				INT;
typedef unsigned __int64	UINT;
typedef double				FLOAT;
#else
typedef short				INT16;
typedef unsigned short		UINT16;
typedef int					INT;
typedef unsigned int		UINT;
typedef float				FLOAT;
#endif

typedef unsigned char BYTE;

#define DEGREE(r) r*(180.0f/3.141592f)
#define RADIAN(d) d*(3.141592f/180.0f)

//default
typedef class FLOAT2{
public:
	FLOAT x;
	FLOAT y;

	FLOAT2  operator + (FLOAT2& v){return {this->x+v.x, this->y+v.y};}
	FLOAT2  operator - (FLOAT2& v){return {this->x-v.x, this->y-v.y};}
	FLOAT2  operator * (FLOAT& f) {return {this->x*f, this->y*f};}
	FLOAT2  operator / (FLOAT& f) {return {this->x/f, this->y/f};}
	FLOAT2& operator +=(FLOAT2& v){this->x += v.x; this->y += v.y; return *this;}
	FLOAT2& operator -=(FLOAT2& v){this->x -= v.x; this->y -= v.y; return *this;}
	FLOAT2& operator *=(FLOAT& f) {this->x *= f; this->y *= f; return *this;}
	FLOAT2& operator /=(FLOAT& f) {this->x /= f; this->y /= f; return *this;}
}VECTOR2;

typedef class FLOAT3{
public:
	FLOAT x;
	FLOAT y;
	FLOAT z;

	FLOAT3  operator + (FLOAT3& v){return {this->x+v.x, this->y+v.y, this->z+v.z};}
	FLOAT3  operator - (FLOAT3& v){return {this->x-v.x, this->y-v.y, this->z-v.z};}
	FLOAT3  operator * (FLOAT& f) {return {this->x*f, this->y*f, this->z*f};}
	FLOAT3  operator / (FLOAT& f) {return {this->x/f, this->y/f, this->z/f};}
	FLOAT3& operator +=(FLOAT3& v){this->x += v.x; this->y += v.y; this->z += v.z; return *this;}
	FLOAT3& operator -=(FLOAT3& v){this->x -= v.x; this->y -= v.y; this->z -= v.z; return *this;}
	FLOAT3& operator *=(FLOAT& f) {this->x *= f; this->y *= f; this->z *= f; return *this;}
	FLOAT3& operator /=(FLOAT& f) {this->x /= f; this->y /= f; this->z /= f; return *this;}
}VECTOR3;

typedef class FLOAT4{
public:
	FLOAT x;
	FLOAT y;
	FLOAT z;
	FLOAT w;

	FLOAT4  operator + (FLOAT4& v){return {this->x+v.x, this->y+v.y, this->z+v.z, this->w+v.w};}
	FLOAT4  operator - (FLOAT4& v){return {this->x-v.x, this->y-v.y, this->z-v.z, this->w-v.w};}
	FLOAT4  operator * (FLOAT& f) {return {this->x*f, this->y*f, this->z*f, this->w*f};}
	FLOAT4  operator / (FLOAT& f) {return {this->x/f, this->y/f, this->z/f, this->w/f};}
	FLOAT4& operator +=(FLOAT4& v){this->x += v.x; this->y += v.y; this->z += v.z; this->w += v.w; return *this;}
	FLOAT4& operator -=(FLOAT4& v){this->x -= v.x; this->y -= v.y; this->z -= v.z; this->w -= v.w; return *this;}
	FLOAT4& operator *=(FLOAT& f) {this->x *= f; this->y *= f; this->z *= f; this->w *= f; return *this;}
	FLOAT4& operator /=(FLOAT& f) {this->x /= f; this->y /= f; this->z /= f; this->w /= f; return *this;}
}VECTOR4;

typedef class FLOAT2x2{
public:
	FLOAT _11, _12;
	FLOAT _21, _22;

	static FLOAT2x2 Initialize(){
		return {1,0,
				0,1};
	};
}MATRIX2x2;

typedef class FLOAT3x3{
public:
	FLOAT _11, _12, _13;
	FLOAT _21, _22, _23;
	FLOAT _31, _32, _33;

	static FLOAT3x3 Initialize(){
		return {1,0,0,
				0,1,0,
				0,0,1};
	};
}MATRIX3x3;

typedef class FLOAT4x4{
public:
	FLOAT _11, _12, _13, _14;
	FLOAT _21, _22, _23, _24;
	FLOAT _31, _32, _33, _34;
	FLOAT _41, _42, _43, _44;

	static FLOAT4x4 Initialize(){
		return {1,0,0,0,
				0,1,0,0,
				0,0,1,0,
				0,0,0,1};
	};
}MATRIX4x4;

//Interface
struct MOUSE{
	INT x, y, z;
	BYTE button[8];
};

struct KEYBOARD{
	BYTE KEY[256];
};

//resource
struct IMAGE{
	UINT width;
	UINT height;
	UINT bit_depth;
	bool alpha_able;
	BYTE* buf;
};

struct SOUND{
	UINT16 wFormatTag;         /* format type */
	UINT16 nChannels;          /* number of channels (i.e. mono, stereo...) */
	UINT nSamplesPerSec;     /* sample rate */
	UINT nAvgBytesPerSec;    /* for buffer estimation */
	UINT16 nBlockAlign;        /* block size of data */
	UINT16 wBitsPerSample;     /* number of bits per sample of mono data */
	UINT16 cbSize;             /* the count in bytes of the size of */
	UINT16 samplerate;
	UINT size;
	BYTE* buf;
};

struct MATERIAL{
	void* diffuse;
	void* height;
	void* normal;
	void* specular;
};