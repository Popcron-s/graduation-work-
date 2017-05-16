#pragma once

#define DLL_API __declspec(dllimport)
#pragma comment(lib, "Engine_Win32.lib")

#include "Enum.h"
#include "Tree.h"
#include "List.h"
#include "variable.h"

#define SKEL_SEARCHER(n) Tree<JOINT>::searcher n = GetSkeletal().Begin()

enum class JOINT_NAME : UINT{
	NONE = 0x00000000,
	SKEL = 0x01000000,
	SKEL_HEAD = 0x01000001,
	SKEL_NECK,
	SKEL_BODY1,
	SKEL_BODY2,
	SKEL_LARM1,
	SKEL_LARM2,
	SKEL_LHAND,
	SKEL_RARM1,
	SKEL_RARM2,
	SKEL_RHAND,
	SKEL_LLEG1,
	SKEL_LLEG2,
	SKEL_LFOOT,
	SKEL_RLEG1,
	SKEL_RLEG2,
	SKEL_RFOOT,
	SKEL_WEAPONE,
	SKIN,
	SKIN_HAIR,
	SKIN_FACE,
	SKIN_TWEAR,
	SKIN_BWEAR,
	SKIN_GLOVE,
	SKIN_SHOES,
	SKIN_WEAPONE
};
struct DLL_API JOINT{
	JOINT_NAME name;
	//축, 위치, 방향, 크기
	VECTOR3 pos, rot, scl;
	MATRIX4x4 world;
	//JOINT* parent;
};

struct DLL_API SCREEN{
	UINT x,y,width,height;
	MATRIX4x4 view,proj;
};
struct DLL_API VERTEX{
	FLOAT4 pos;
	FLOAT4 diffuse;
	FLOAT3 normal;
	FLOAT2 tex;
};
struct DLL_API INDEXED{
	struct _node{
		UINT _1,_2,_3;
	}*node;
	UINT m_num;
};
struct DLL_API RENDER_OBJECT{
	MATRIX4x4 world;
	VERTEX* vtx;
	UINT vtx_num;
	INDEXED* indexed;
	MATERIAL* material;
	//shader
};
class DLL_API MESH{
public:
	JOINT* joint;
	VERTEX* vtx;
	UINT vtx_num;
	INDEXED* indexed;
	MATERIAL* material;
	//shader

	RENDER_OBJECT GetGraphics();
};

enum class COLLIDER2D : UINT{
	POINT = 0x0,
	LINE = 0x1,
	BOX = 0x2,
	CIRCLE = 0x3,
	POLYGON = 0x100
};
class DLL_API COL2D{
public:
	JOINT* joint;
	virtual COLLIDER2D GetType() = 0;
};
class DLL_API BOX : public COL2D{
public:
	FLOAT3 _1, _2, _3, _4;

	COLLIDER2D GetType();
};
class DLL_API CIRCLE : public COL2D{
public:
	FLOAT3 o;
	FLOAT r;

	COLLIDER2D GetType();
};

class DLL_API _Graph{
protected:
	_GRAPH::TYPE type;
	UINT time;
	UINT cur;
	bool Loop;
	bool Auto;

public:
	_Graph();
	~_Graph();

	void SetLoop(bool b);
	bool IsLoop();

	void SetAuto(bool b);
	bool IsAuto();

	void Play();
	void Stop();
	void Pause();
	void Seek(UINT t);
	_GRAPH::TYPE GetType();

	virtual void update(UINT tick) = 0;
};
template <typename T> class _Graph_template : public _Graph{
private:
	T& target;
	struct _node{
		T value;
		_GRAPH::INTERPOLATION inter;
		UINT time;
	}*arr;
	const UINT arr_num;

public:
	_Graph_template(T& t, UINT n) : _Graph(), target(t), arr(new _node[n]), arr_num(n){}
	~_Graph_template(){delete [] arr;}

	//_node& operator [](UINT num){return arr[num];}

	void SetNode(UINT n, T v, _GRAPH::INTERPOLATION i, UINT t){
		arr[n] = {v,i,t};
	}

	void Interpolation(T prev, T next, FLOAT weight, _GRAPH::INTERPOLATION inter){
		switch(inter){
		case _GRAPH::INTERPOLATION::POINT:
			target = prev;
			return;
		case _GRAPH::INTERPOLATION::LINEAR:
			target = prev + ((next-prev)*weight);
			return;
		case _GRAPH::INTERPOLATION::SQUARED:
			target = prev + ((next-prev)*weight*weight);
			return;
		}
		return;
	}

	void update(UINT tick){
		if(type != _GRAPH::TYPE::PLAY){
			if(Auto){
				type = _GRAPH::TYPE::PLAY;
				Auto = false;
			}
			else{
				return;
			}
		}
		time += tick;
		while(true){
			if(cur == arr_num-1){
				if(Loop){
					time -= arr[cur].time;
					cur = 0;
				}
				else{
					time = 0;
					target = arr[cur].value;
					cur = 0;
					type = _GRAPH::TYPE::STOP;
					return;
				}
			}
			if(time < arr[cur+1].time){
				FLOAT w = (FLOAT)(time - arr[cur].time)/(FLOAT)(arr[cur+1].time - arr[cur].time);
				return Interpolation(arr[cur].value, arr[cur+1].value, w, arr[cur].inter);
			}
			++cur;
		}
	}
};
template <class T> class _Graph_function : public _Graph{
private:
	T* target;
	struct _node{
		void (T::*func)();
		UINT time;
	}*arr;
	const UINT arr_num;

public:
	_Graph_function(T* t, UINT n) : _Graph(), target(t), arr(new _node[n]), arr_num(n){}
	~_Graph_function(){delete [] arr;}

	//_node& operator [](UINT num){return arr[num];}

	void SetNode(UINT n, void (T::*f)(), UINT t){
		arr[n] = {f,t};
	}

	void update(UINT tick){
		if(type != _GRAPH::TYPE::PLAY){
			if(Auto){
				type = _GRAPH::TYPE::PLAY;
				Auto = false;
			}
			else{
				return;
			}
		}
		time += tick;
		if(cur == arr_num){
			if(Loop){
				time -= arr[cur-1].time;
				cur = 0;
			}
			else{
				time = 0;
				cur = 0;
				type = _GRAPH::TYPE::STOP;
				return;
			}
		}
		while(arr[cur].time < time){
			(target->*(arr[cur].func))();
			++cur;
			if(cur == arr_num){
				if(Loop){
					time -= arr[cur-1].time;
					cur = 0;
				}
				else{
					time = 0;
					cur = 0;
					type = _GRAPH::TYPE::STOP;
					return;
				}
			}
			return;
		}
	}
};
class DLL_API ANIMATION{
private:
	void*& m_Target;
	_Graph** m_Clip;
	UINT m_MaxClip;

public:
	ANIMATION(void*& target, UINT max_clip);
	~ANIMATION();

	void Play();
	void Stop();
	void Pause();
	void SetAuto(bool b);
	void SetLoop(bool b);
	void Seek(UINT time);
	_GRAPH::TYPE GetType();

	_Graph*& operator [] (UINT i);
	UINT GetNumber();
	void update(UINT tick);
};

class DLL_API OBJECT{
public:
	virtual void update() = 0;
	
private:
	FLOAT3 m_pos;
	FLOAT3 m_rot;
	FLOAT3 m_scl;
	MATRIX4x4 m_world;

	Tree<JOINT> m_skeletal;
	List<MESH> m_mesh;
	List<COL2D*> m_col2d;
	ANIMATION* m_ani;
	
	MATRIX4x4 SetWorld(VECTOR3 pos, VECTOR3 rot, VECTOR3 scl, MATRIX4x4 p_mat = MATRIX4x4::Initialize());
	void SetWorld(MATRIX4x4 p_mat = MATRIX4x4::Initialize());
	
public:
	OBJECT();
	~OBJECT();

	void WorldUpdate();
	void ActAnimation(UINT tick);

	Tree<JOINT>& GetSkeletal();
	List<MESH>& GetMesh();
	List<COL2D*>& GetCollider2D();
	void CreateAnimation(UINT num);
	void DestroyAnimation();
	ANIMATION& GetAnimation();
	void SetAnimation(ANIMATION* ani);
	MATRIX4x4 GetWorld();
	void SetPosition(FLOAT3 p);
	FLOAT3 GetPosition();
	void SetRotation(FLOAT3 r);
	FLOAT3 GetRotation();
	void SetScale(FLOAT3 s);
	FLOAT3 GetScale();
};