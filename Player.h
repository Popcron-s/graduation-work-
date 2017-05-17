#pragma once

#include "Engine_Win32.h"
#include <list>
#include "Singleton.h"

#include "Character.h"
#include "command.h"
#include "Physics.h"
#include "Game_Enum.h"

#include "Collision.h"
#include "SoundLibrary.h"

class Player : public Character{
private:
	enum class ANI_ENUM : UINT{
		STAND = 0x00,
		WALK = 0x01,
		DASH = 0x02,
		SIT = 0x03,
		SIT_WALK = 0x04,
		JUMP = 0x05,
		JUMP_DASH = 0x06,
		//ATK
		STAND_A,
		STAND_B,
		STAND_C,
		STAND_D,
		SPECIAL1,
		SPECIAL2,
		SPECIAL3,
		MAX_ANIMATION
	};
	ANIMATION* ani[(UINT)ANI_ENUM::MAX_ANIMATION];
	void* texture[3];

	Command command;
	COL2D* atk_col;

	//item array equipment
	//item array inventory

public:
	Player() : Character(){
		RegistObject((void*)this, (UINT)eLayer::PLAYER);

		IMAGE t = {};
		LoadBMP("임시 이동셋.bmp", t);
		texture[0] = RegistTexture(t, texture[0]);
		delete [] t.buf;

		LoadBMP("임시 기본공격셋.bmp", t);
		texture[1] = RegistTexture(t, texture[1]);
		delete [] t.buf;

		LoadBMP("임시 특수공격셋.bmp", t);
		texture[2] = RegistTexture(t, texture[2]);
		delete [] t.buf;

		GetSkeletal().CreateRoot({
			JOINT_NAME::NONE, 
			{}, {}, {1.0f, 1.0f, 1.0f}, 
			MATRIX4x4::Initialize()
		});

		SKEL_SEARCHER(scr);

		GetMesh().CreateNode({
			&(*scr), new VERTEX[4], 4, nullptr, new MATERIAL
		});
		List<MESH>::iterator mesh_iter = GetMesh().Begin();
		mesh_iter->vtx[0] = {{-32.0f,-128.0f, 0.0f},{1.0f, 1.0f, 1.0f, 1.0f},{},{0.000f, 0.00f}};
		mesh_iter->vtx[1] = {{ 32.0f,-128.0f, 0.0f},{1.0f, 1.0f, 1.0f, 1.0f},{},{0.125f, 0.00f}};
		mesh_iter->vtx[2] = {{-32.0f,   0.0f, 0.0f},{1.0f, 1.0f, 1.0f, 1.0f},{},{0.000f, 0.25f}};
		mesh_iter->vtx[3] = {{ 32.0f,   0.0f, 0.0f},{1.0f, 1.0f, 1.0f, 1.0f},{},{0.125f, 0.25f}};
		mesh_iter->material->diffuse = texture[0];

		COL2D* t_col = new BOX;
		t_col->joint = &(*scr);
		((BOX*)t_col)->_1 = {-32.0f,-128.0f, 0.0f};
		((BOX*)t_col)->_2 = { 32.0f,-128.0f, 0.0f};
		((BOX*)t_col)->_3 = { 32.0f,   0.0f, 0.0f};
		((BOX*)t_col)->_4 = {-32.0f,   0.0f, 0.0f};
		GetCollider2D().CreateNode(t_col);

		Collision::GetSingleton()->RegistCHARAList((void*)this, t_col);

		GetSkeletal().Create(scr, {
			JOINT_NAME::NONE, 
			{0.0f, -64.0f, 0.0f}, {}, {1.0f, 1.0f, 1.0f}, 
			MATRIX4x4::Initialize()
		});
		scr.Child(0);
		GetSkeletal().Create(scr, {
			JOINT_NAME::NONE, 
			{0.0f, 0.0f, 0.0f}, {}, {1.0f, 1.0f, 1.0f}, 
			MATRIX4x4::Initialize()
		});

		t_col = new BOX; 
		t_col->joint = &(scr.GetChild(0));
		((BOX*)t_col)->_1 = {-1.0f,-1.0f, 0.0f};
		((BOX*)t_col)->_2 = { 1.0f,-1.0f, 0.0f};
		((BOX*)t_col)->_3 = { 1.0f, 1.0f, 0.0f};
		((BOX*)t_col)->_4 = {-1.0f, 1.0f, 0.0f};
		
		GetMesh().CreateNode({
			&(scr.GetChild(0)), new VERTEX[4], 4, nullptr, nullptr
		});
		++mesh_iter;
		mesh_iter->vtx[0] = {{-1.0f,-1.0f, 0.0f},{1.0f, 0.0f, 0.0f, 1.0f},{},{0.000f, 0.00f}};
		mesh_iter->vtx[1] = {{ 1.0f,-1.0f, 0.0f},{1.0f, 0.0f, 0.0f, 1.0f},{},{1.000f, 0.00f}};
		mesh_iter->vtx[2] = {{-1.0f, 1.0f, 0.0f},{1.0f, 0.0f, 0.0f, 1.0f},{},{0.000f, 1.00f}};
		mesh_iter->vtx[3] = {{ 1.0f, 1.0f, 0.0f},{1.0f, 0.0f, 0.0f, 1.0f},{},{1.000f, 1.00f}};
		atk_col = t_col;

		SettingAnimation();
		SetAnimation(ani[(UINT)ANI_ENUM::STAND]);
	}
	~Player(){
		RemoveObject((void*)this, (UINT)eLayer::PLAYER);
	}
	
	void SA_Stand(){
		List<MESH>::iterator mesh_iter = GetMesh().Begin();

		ani[(UINT)ANI_ENUM::STAND] = new ANIMATION((void*&)*this, 4);
		(*(ani[(UINT)ANI_ENUM::STAND]))[0] = new _Graph_template<FLOAT2>(mesh_iter->vtx[0].tex, 3);
		(*(ani[(UINT)ANI_ENUM::STAND]))[1] = new _Graph_template<FLOAT2>(mesh_iter->vtx[1].tex, 3);
		(*(ani[(UINT)ANI_ENUM::STAND]))[2] = new _Graph_template<FLOAT2>(mesh_iter->vtx[2].tex, 3);
		(*(ani[(UINT)ANI_ENUM::STAND]))[3] = new _Graph_template<FLOAT2>(mesh_iter->vtx[3].tex, 3);

		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::STAND]))[0])->SetNode(0, {0.000f, 0.00f}, _GRAPH::INTERPOLATION::POINT, 0);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::STAND]))[1])->SetNode(0, {0.125f, 0.00f}, _GRAPH::INTERPOLATION::POINT, 0);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::STAND]))[2])->SetNode(0, {0.000f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 0);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::STAND]))[3])->SetNode(0, {0.125f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 0);

		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::STAND]))[0])->SetNode(1, {0.125f, 0.00f}, _GRAPH::INTERPOLATION::POINT, 200);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::STAND]))[1])->SetNode(1, {0.250f, 0.00f}, _GRAPH::INTERPOLATION::POINT, 200);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::STAND]))[2])->SetNode(1, {0.125f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 200);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::STAND]))[3])->SetNode(1, {0.250f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 200);

		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::STAND]))[0])->SetNode(2, {0.000f, 0.00f}, _GRAPH::INTERPOLATION::POINT, 400);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::STAND]))[1])->SetNode(2, {0.125f, 0.00f}, _GRAPH::INTERPOLATION::POINT, 400);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::STAND]))[2])->SetNode(2, {0.000f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 400);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::STAND]))[3])->SetNode(2, {0.125f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 400);

		(ani[(UINT)ANI_ENUM::STAND])->SetLoop(true);	(ani[(UINT)ANI_ENUM::STAND])->SetAuto(true);
	}
	void SA_Walk(){
		List<MESH>::iterator mesh_iter = GetMesh().Begin();

		ani[(UINT)ANI_ENUM::WALK] = new ANIMATION((void*&)*this, 5);
		(*(ani[(UINT)ANI_ENUM::WALK]))[0] = new _Graph_template<FLOAT2>(mesh_iter->vtx[0].tex, 5);
		(*(ani[(UINT)ANI_ENUM::WALK]))[1] = new _Graph_template<FLOAT2>(mesh_iter->vtx[1].tex, 5);
		(*(ani[(UINT)ANI_ENUM::WALK]))[2] = new _Graph_template<FLOAT2>(mesh_iter->vtx[2].tex, 5);
		(*(ani[(UINT)ANI_ENUM::WALK]))[3] = new _Graph_template<FLOAT2>(mesh_iter->vtx[3].tex, 5);
		(*(ani[(UINT)ANI_ENUM::WALK]))[4] = new _Graph_function<SoundLib>(SoundLib::GetSingleton(), 2);

		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::WALK]))[0])->SetNode(0, {0.250f, 0.00f}, _GRAPH::INTERPOLATION::POINT, 0);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::WALK]))[1])->SetNode(0, {0.375f, 0.00f}, _GRAPH::INTERPOLATION::POINT, 0);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::WALK]))[2])->SetNode(0, {0.250f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 0);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::WALK]))[3])->SetNode(0, {0.375f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 0);

		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::WALK]))[0])->SetNode(1, {0.375f, 0.00f}, _GRAPH::INTERPOLATION::POINT, 200);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::WALK]))[1])->SetNode(1, {0.500f, 0.00f}, _GRAPH::INTERPOLATION::POINT, 200);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::WALK]))[2])->SetNode(1, {0.375f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 200);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::WALK]))[3])->SetNode(1, {0.500f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 200);

		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::WALK]))[0])->SetNode(2, {0.500f, 0.00f}, _GRAPH::INTERPOLATION::POINT, 400);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::WALK]))[1])->SetNode(2, {0.625f, 0.00f}, _GRAPH::INTERPOLATION::POINT, 400);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::WALK]))[2])->SetNode(2, {0.500f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 400);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::WALK]))[3])->SetNode(2, {0.625f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 400);

		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::WALK]))[0])->SetNode(3, {0.625f, 0.00f}, _GRAPH::INTERPOLATION::POINT, 600);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::WALK]))[1])->SetNode(3, {0.750f, 0.00f}, _GRAPH::INTERPOLATION::POINT, 600);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::WALK]))[2])->SetNode(3, {0.625f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 600);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::WALK]))[3])->SetNode(3, {0.750f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 600);

		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::WALK]))[0])->SetNode(4, {0.250f, 0.00f}, _GRAPH::INTERPOLATION::POINT, 800);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::WALK]))[1])->SetNode(4, {0.375f, 0.00f}, _GRAPH::INTERPOLATION::POINT, 800);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::WALK]))[2])->SetNode(4, {0.250f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 800);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::WALK]))[3])->SetNode(4, {0.375f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 800);

		((_Graph_function<SoundLib>*)(*(ani[(UINT)ANI_ENUM::WALK]))[4])->SetNode(0, &SoundLib::Walk, 400);
		((_Graph_function<SoundLib>*)(*(ani[(UINT)ANI_ENUM::WALK]))[4])->SetNode(1, &SoundLib::Walk, 800);

		(ani[(UINT)ANI_ENUM::WALK])->SetLoop(true);	(ani[(UINT)ANI_ENUM::WALK])->SetAuto(true);
	}
	void SA_Dash(){
		List<MESH>::iterator mesh_iter = GetMesh().Begin();

		ani[(UINT)ANI_ENUM::DASH] = new ANIMATION((void*&)*this, 5);
		(*(ani[(UINT)ANI_ENUM::DASH]))[0] = new _Graph_template<FLOAT2>(mesh_iter->vtx[0].tex, 5);
		(*(ani[(UINT)ANI_ENUM::DASH]))[1] = new _Graph_template<FLOAT2>(mesh_iter->vtx[1].tex, 5);
		(*(ani[(UINT)ANI_ENUM::DASH]))[2] = new _Graph_template<FLOAT2>(mesh_iter->vtx[2].tex, 5);
		(*(ani[(UINT)ANI_ENUM::DASH]))[3] = new _Graph_template<FLOAT2>(mesh_iter->vtx[3].tex, 5);
		(*(ani[(UINT)ANI_ENUM::DASH]))[4] = new _Graph_function<SoundLib>(SoundLib::GetSingleton(), 2);

		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::DASH]))[0])->SetNode(0, {0.750f, 0.00f}, _GRAPH::INTERPOLATION::POINT, 0);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::DASH]))[1])->SetNode(0, {0.875f, 0.00f}, _GRAPH::INTERPOLATION::POINT, 0);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::DASH]))[2])->SetNode(0, {0.750f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 0);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::DASH]))[3])->SetNode(0, {0.875f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 0);

		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::DASH]))[0])->SetNode(1, {0.875f, 0.00f}, _GRAPH::INTERPOLATION::POINT, 100);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::DASH]))[1])->SetNode(1, {1.000f, 0.00f}, _GRAPH::INTERPOLATION::POINT, 100);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::DASH]))[2])->SetNode(1, {0.875f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 100);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::DASH]))[3])->SetNode(1, {1.000f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 100);

		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::DASH]))[0])->SetNode(2, {0.750f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 200);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::DASH]))[1])->SetNode(2, {0.875f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 200);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::DASH]))[2])->SetNode(2, {0.750f, 0.50f}, _GRAPH::INTERPOLATION::POINT, 200);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::DASH]))[3])->SetNode(2, {0.875f, 0.50f}, _GRAPH::INTERPOLATION::POINT, 200);

		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::DASH]))[0])->SetNode(3, {0.875f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 300);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::DASH]))[1])->SetNode(3, {1.000f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 300);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::DASH]))[2])->SetNode(3, {0.875f, 0.50f}, _GRAPH::INTERPOLATION::POINT, 300);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::DASH]))[3])->SetNode(3, {1.000f, 0.50f}, _GRAPH::INTERPOLATION::POINT, 300);

		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::DASH]))[0])->SetNode(4, {0.750f, 0.00f}, _GRAPH::INTERPOLATION::POINT, 400);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::DASH]))[1])->SetNode(4, {0.875f, 0.00f}, _GRAPH::INTERPOLATION::POINT, 400);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::DASH]))[2])->SetNode(4, {0.750f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 400);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::DASH]))[3])->SetNode(4, {0.875f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 400);

		((_Graph_function<SoundLib>*)(*(ani[(UINT)ANI_ENUM::DASH]))[4])->SetNode(0, &SoundLib::Walk, 200);
		((_Graph_function<SoundLib>*)(*(ani[(UINT)ANI_ENUM::DASH]))[4])->SetNode(1, &SoundLib::Walk, 400);

		(ani[(UINT)ANI_ENUM::DASH])->SetLoop(true);	(ani[(UINT)ANI_ENUM::DASH])->SetAuto(true);
	}
	void SA_Jump(){
		List<MESH>::iterator mesh_iter = GetMesh().Begin();
	}
	void SA_SNDA(){
		List<MESH>::iterator mesh_iter = GetMesh().Begin();

		ani[(UINT)ANI_ENUM::STAND_A] = new ANIMATION((void*&)*this, 7);
		(*(ani[(UINT)ANI_ENUM::STAND_A]))[0] = new _Graph_template<FLOAT2>(mesh_iter->vtx[0].tex, 3);
		(*(ani[(UINT)ANI_ENUM::STAND_A]))[1] = new _Graph_template<FLOAT2>(mesh_iter->vtx[1].tex, 3);
		(*(ani[(UINT)ANI_ENUM::STAND_A]))[2] = new _Graph_template<FLOAT2>(mesh_iter->vtx[2].tex, 3);
		(*(ani[(UINT)ANI_ENUM::STAND_A]))[3] = new _Graph_template<FLOAT2>(mesh_iter->vtx[3].tex, 3);
		++mesh_iter;
		(*(ani[(UINT)ANI_ENUM::STAND_A]))[4] = new _Graph_template<FLOAT3>(mesh_iter->joint->pos, 2);
		(*(ani[(UINT)ANI_ENUM::STAND_A]))[5] = new _Graph_template<FLOAT3>(mesh_iter->joint->scl, 4);
		(*(ani[(UINT)ANI_ENUM::STAND_A]))[6] = new _Graph_function<Player>(this, 2);

		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::STAND_A]))[0])->SetNode(0, {0.000f, 0.00f}, _GRAPH::INTERPOLATION::POINT, 0);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::STAND_A]))[1])->SetNode(0, {0.125f, 0.00f}, _GRAPH::INTERPOLATION::POINT, 0);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::STAND_A]))[2])->SetNode(0, {0.000f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 0);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::STAND_A]))[3])->SetNode(0, {0.125f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 0);

		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::STAND_A]))[0])->SetNode(1, {0.125f, 0.00f}, _GRAPH::INTERPOLATION::POINT, 200);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::STAND_A]))[1])->SetNode(1, {0.250f, 0.00f}, _GRAPH::INTERPOLATION::POINT, 200);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::STAND_A]))[2])->SetNode(1, {0.125f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 200);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::STAND_A]))[3])->SetNode(1, {0.250f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 200);

		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::STAND_A]))[0])->SetNode(2, {0.125f, 0.00f}, _GRAPH::INTERPOLATION::POINT, 400);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::STAND_A]))[1])->SetNode(2, {0.250f, 0.00f}, _GRAPH::INTERPOLATION::POINT, 400);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::STAND_A]))[2])->SetNode(2, {0.125f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 400);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::STAND_A]))[3])->SetNode(2, {0.250f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 400);

		((_Graph_template<FLOAT3>*)(*(ani[(UINT)ANI_ENUM::STAND_A]))[4])->SetNode(0, {32.0f, -15.0f, 0.0f}, _GRAPH::INTERPOLATION::POINT,   0);
		((_Graph_template<FLOAT3>*)(*(ani[(UINT)ANI_ENUM::STAND_A]))[4])->SetNode(1, {0.0f, 0.0f, 0.0f}, _GRAPH::INTERPOLATION::POINT, 400);

		((_Graph_template<FLOAT3>*)(*(ani[(UINT)ANI_ENUM::STAND_A]))[5])->SetNode(0, {10.0f, 15.0f, 1.0f}, _GRAPH::INTERPOLATION::LINEAR,   0);
		((_Graph_template<FLOAT3>*)(*(ani[(UINT)ANI_ENUM::STAND_A]))[5])->SetNode(1, {30.0f, 15.0f, 1.0f}, _GRAPH::INTERPOLATION::LINEAR, 200);
		((_Graph_template<FLOAT3>*)(*(ani[(UINT)ANI_ENUM::STAND_A]))[5])->SetNode(2, {10.0f, 15.0f, 1.0f}, _GRAPH::INTERPOLATION::POINT, 400);
		((_Graph_template<FLOAT3>*)(*(ani[(UINT)ANI_ENUM::STAND_A]))[5])->SetNode(3, {1.0f, 1.0f, 1.0f}, _GRAPH::INTERPOLATION::POINT, 400);

		((_Graph_function<Player>*)(*(ani[(UINT)ANI_ENUM::STAND_A]))[6])->SetNode(0, &Player::RegistATKCOL, 0);
		((_Graph_function<Player>*)(*(ani[(UINT)ANI_ENUM::STAND_A]))[6])->SetNode(1, &Player::RemoveATKCOL, 400);

		(ani[(UINT)ANI_ENUM::STAND_A])->SetLoop(false);	(ani[(UINT)ANI_ENUM::STAND_A])->SetAuto(false);
	}
	void SA_SNDB(){
		List<MESH>::iterator mesh_iter = GetMesh().Begin();

		ani[(UINT)ANI_ENUM::STAND_B] = new ANIMATION((void*&)*this, 4);
		(*(ani[(UINT)ANI_ENUM::STAND_B]))[0] = new _Graph_template<FLOAT2>(mesh_iter->vtx[0].tex, 5);
		(*(ani[(UINT)ANI_ENUM::STAND_B]))[1] = new _Graph_template<FLOAT2>(mesh_iter->vtx[1].tex, 5);
		(*(ani[(UINT)ANI_ENUM::STAND_B]))[2] = new _Graph_template<FLOAT2>(mesh_iter->vtx[2].tex, 5);
		(*(ani[(UINT)ANI_ENUM::STAND_B]))[3] = new _Graph_template<FLOAT2>(mesh_iter->vtx[3].tex, 5);

		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::STAND_B]))[0])->SetNode(0, {0.250f, 0.00f}, _GRAPH::INTERPOLATION::POINT, 0);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::STAND_B]))[1])->SetNode(0, {0.375f, 0.00f}, _GRAPH::INTERPOLATION::POINT, 0);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::STAND_B]))[2])->SetNode(0, {0.250f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 0);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::STAND_B]))[3])->SetNode(0, {0.375f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 0);

		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::STAND_B]))[0])->SetNode(1, {0.375f, 0.00f}, _GRAPH::INTERPOLATION::POINT, 200);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::STAND_B]))[1])->SetNode(1, {0.500f, 0.00f}, _GRAPH::INTERPOLATION::POINT, 200);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::STAND_B]))[2])->SetNode(1, {0.375f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 200);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::STAND_B]))[3])->SetNode(1, {0.500f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 200);

		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::STAND_B]))[0])->SetNode(2, {0.500f, 0.00f}, _GRAPH::INTERPOLATION::POINT, 400);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::STAND_B]))[1])->SetNode(2, {0.625f, 0.00f}, _GRAPH::INTERPOLATION::POINT, 400);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::STAND_B]))[2])->SetNode(2, {0.500f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 400);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::STAND_B]))[3])->SetNode(2, {0.625f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 400);

		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::STAND_B]))[0])->SetNode(3, {0.625f, 0.00f}, _GRAPH::INTERPOLATION::POINT, 600);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::STAND_B]))[1])->SetNode(3, {0.750f, 0.00f}, _GRAPH::INTERPOLATION::POINT, 600);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::STAND_B]))[2])->SetNode(3, {0.625f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 600);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::STAND_B]))[3])->SetNode(3, {0.750f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 600);

		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::STAND_B]))[0])->SetNode(4, {0.625f, 0.00f}, _GRAPH::INTERPOLATION::POINT, 800);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::STAND_B]))[1])->SetNode(4, {0.750f, 0.00f}, _GRAPH::INTERPOLATION::POINT, 800);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::STAND_B]))[2])->SetNode(4, {0.625f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 800);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::STAND_B]))[3])->SetNode(4, {0.750f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 800);

		(ani[(UINT)ANI_ENUM::STAND_B])->SetLoop(false);	(ani[(UINT)ANI_ENUM::STAND_B])->SetAuto(false);
	}
	void SA_SNDC(){
		List<MESH>::iterator mesh_iter = GetMesh().Begin();

		ani[(UINT)ANI_ENUM::STAND_C] = new ANIMATION((void*&)*this, 4);
		(*(ani[(UINT)ANI_ENUM::STAND_C]))[0] = new _Graph_template<FLOAT2>(mesh_iter->vtx[0].tex, 5);
		(*(ani[(UINT)ANI_ENUM::STAND_C]))[1] = new _Graph_template<FLOAT2>(mesh_iter->vtx[1].tex, 5);
		(*(ani[(UINT)ANI_ENUM::STAND_C]))[2] = new _Graph_template<FLOAT2>(mesh_iter->vtx[2].tex, 5);
		(*(ani[(UINT)ANI_ENUM::STAND_C]))[3] = new _Graph_template<FLOAT2>(mesh_iter->vtx[3].tex, 5);

		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::STAND_C]))[0])->SetNode(0, {0.000f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 0);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::STAND_C]))[1])->SetNode(0, {0.125f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 0);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::STAND_C]))[2])->SetNode(0, {0.000f, 0.50f}, _GRAPH::INTERPOLATION::POINT, 0);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::STAND_C]))[3])->SetNode(0, {0.125f, 0.50f}, _GRAPH::INTERPOLATION::POINT, 0);

		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::STAND_C]))[0])->SetNode(1, {0.125f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 200);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::STAND_C]))[1])->SetNode(1, {0.250f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 200);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::STAND_C]))[2])->SetNode(1, {0.125f, 0.50f}, _GRAPH::INTERPOLATION::POINT, 200);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::STAND_C]))[3])->SetNode(1, {0.250f, 0.50f}, _GRAPH::INTERPOLATION::POINT, 200);

		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::STAND_C]))[0])->SetNode(2, {0.250f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 400);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::STAND_C]))[1])->SetNode(2, {0.375f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 400);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::STAND_C]))[2])->SetNode(2, {0.250f, 0.50f}, _GRAPH::INTERPOLATION::POINT, 400);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::STAND_C]))[3])->SetNode(2, {0.375f, 0.50f}, _GRAPH::INTERPOLATION::POINT, 400);

		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::STAND_C]))[0])->SetNode(3, {0.375f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 600);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::STAND_C]))[1])->SetNode(3, {0.500f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 600);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::STAND_C]))[2])->SetNode(3, {0.375f, 0.50f}, _GRAPH::INTERPOLATION::POINT, 600);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::STAND_C]))[3])->SetNode(3, {0.500f, 0.50f}, _GRAPH::INTERPOLATION::POINT, 600);

		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::STAND_C]))[0])->SetNode(4, {0.375f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 800);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::STAND_C]))[1])->SetNode(4, {0.500f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 800);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::STAND_C]))[2])->SetNode(4, {0.375f, 0.50f}, _GRAPH::INTERPOLATION::POINT, 800);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::STAND_C]))[3])->SetNode(4, {0.500f, 0.50f}, _GRAPH::INTERPOLATION::POINT, 800);

		(ani[(UINT)ANI_ENUM::STAND_C])->SetLoop(false);	(ani[(UINT)ANI_ENUM::STAND_C])->SetAuto(false);
	}
	void SA_SNDD(){
		List<MESH>::iterator mesh_iter = GetMesh().Begin();

		ani[(UINT)ANI_ENUM::STAND_D] = new ANIMATION((void*&)*this, 4);
		(*(ani[(UINT)ANI_ENUM::STAND_D]))[0] = new _Graph_template<FLOAT2>(mesh_iter->vtx[0].tex, 3);
		(*(ani[(UINT)ANI_ENUM::STAND_D]))[1] = new _Graph_template<FLOAT2>(mesh_iter->vtx[1].tex, 3);
		(*(ani[(UINT)ANI_ENUM::STAND_D]))[2] = new _Graph_template<FLOAT2>(mesh_iter->vtx[2].tex, 3);
		(*(ani[(UINT)ANI_ENUM::STAND_D]))[3] = new _Graph_template<FLOAT2>(mesh_iter->vtx[3].tex, 3);

		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::STAND_D]))[0])->SetNode(0, {0.500f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 0);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::STAND_D]))[1])->SetNode(0, {0.625f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 0);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::STAND_D]))[2])->SetNode(0, {0.500f, 0.50f}, _GRAPH::INTERPOLATION::POINT, 0);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::STAND_D]))[3])->SetNode(0, {0.625f, 0.50f}, _GRAPH::INTERPOLATION::POINT, 0);

		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::STAND_D]))[0])->SetNode(1, {0.625f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 200);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::STAND_D]))[1])->SetNode(1, {0.750f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 200);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::STAND_D]))[2])->SetNode(1, {0.625f, 0.50f}, _GRAPH::INTERPOLATION::POINT, 200);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::STAND_D]))[3])->SetNode(1, {0.750f, 0.50f}, _GRAPH::INTERPOLATION::POINT, 200);

		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::STAND_D]))[0])->SetNode(2, {0.625f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 400);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::STAND_D]))[1])->SetNode(2, {0.750f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 400);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::STAND_D]))[2])->SetNode(2, {0.625f, 0.50f}, _GRAPH::INTERPOLATION::POINT, 400);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::STAND_D]))[3])->SetNode(2, {0.750f, 0.50f}, _GRAPH::INTERPOLATION::POINT, 400);

		(ani[(UINT)ANI_ENUM::STAND_D])->SetLoop(false);	(ani[(UINT)ANI_ENUM::STAND_D])->SetAuto(false);
	}
	void SA_Special1(){
		List<MESH>::iterator mesh_iter = GetMesh().Begin();

		ani[(UINT)ANI_ENUM::SPECIAL1] = new ANIMATION((void*&)*this, 4);
		(*(ani[(UINT)ANI_ENUM::SPECIAL1]))[0] = new _Graph_template<FLOAT2>(mesh_iter->vtx[0].tex, 9);
		(*(ani[(UINT)ANI_ENUM::SPECIAL1]))[1] = new _Graph_template<FLOAT2>(mesh_iter->vtx[1].tex, 9);
		(*(ani[(UINT)ANI_ENUM::SPECIAL1]))[2] = new _Graph_template<FLOAT2>(mesh_iter->vtx[2].tex, 9);
		(*(ani[(UINT)ANI_ENUM::SPECIAL1]))[3] = new _Graph_template<FLOAT2>(mesh_iter->vtx[3].tex, 9);

		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL1]))[0])->SetNode(0, {0.000f, 0.00f}, _GRAPH::INTERPOLATION::POINT, 0);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL1]))[1])->SetNode(0, {0.125f, 0.00f}, _GRAPH::INTERPOLATION::POINT, 0);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL1]))[2])->SetNode(0, {0.000f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 0);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL1]))[3])->SetNode(0, {0.125f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 0);

		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL1]))[0])->SetNode(1, {0.125f, 0.00f}, _GRAPH::INTERPOLATION::POINT, 200);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL1]))[1])->SetNode(1, {0.250f, 0.00f}, _GRAPH::INTERPOLATION::POINT, 200);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL1]))[2])->SetNode(1, {0.125f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 200);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL1]))[3])->SetNode(1, {0.250f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 200);

		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL1]))[0])->SetNode(2, {0.250f, 0.00f}, _GRAPH::INTERPOLATION::POINT, 400);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL1]))[1])->SetNode(2, {0.375f, 0.00f}, _GRAPH::INTERPOLATION::POINT, 400);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL1]))[2])->SetNode(2, {0.250f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 400);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL1]))[3])->SetNode(2, {0.375f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 400);

		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL1]))[0])->SetNode(3, {0.375f, 0.00f}, _GRAPH::INTERPOLATION::POINT, 600);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL1]))[1])->SetNode(3, {0.500f, 0.00f}, _GRAPH::INTERPOLATION::POINT, 600);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL1]))[2])->SetNode(3, {0.375f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 600);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL1]))[3])->SetNode(3, {0.500f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 600);

		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL1]))[0])->SetNode(4, {0.500f, 0.00f}, _GRAPH::INTERPOLATION::POINT, 800);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL1]))[1])->SetNode(4, {0.625f, 0.00f}, _GRAPH::INTERPOLATION::POINT, 800);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL1]))[2])->SetNode(4, {0.500f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 800);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL1]))[3])->SetNode(4, {0.625f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 800);

		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL1]))[0])->SetNode(5, {0.625f, 0.00f}, _GRAPH::INTERPOLATION::POINT, 1000);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL1]))[1])->SetNode(5, {0.750f, 0.00f}, _GRAPH::INTERPOLATION::POINT, 1000);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL1]))[2])->SetNode(5, {0.625f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 1000);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL1]))[3])->SetNode(5, {0.750f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 1000);

		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL1]))[0])->SetNode(6, {0.750f, 0.00f}, _GRAPH::INTERPOLATION::POINT, 1200);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL1]))[1])->SetNode(6, {0.875f, 0.00f}, _GRAPH::INTERPOLATION::POINT, 1200);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL1]))[2])->SetNode(6, {0.750f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 1200);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL1]))[3])->SetNode(6, {0.875f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 1200);

		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL1]))[0])->SetNode(7, {0.875f, 0.00f}, _GRAPH::INTERPOLATION::POINT, 1400);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL1]))[1])->SetNode(7, {1.000f, 0.00f}, _GRAPH::INTERPOLATION::POINT, 1400);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL1]))[2])->SetNode(7, {0.875f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 1400);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL1]))[3])->SetNode(7, {1.000f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 1400);

		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL1]))[0])->SetNode(8, {0.875f, 0.00f}, _GRAPH::INTERPOLATION::POINT, 1600);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL1]))[1])->SetNode(8, {1.000f, 0.00f}, _GRAPH::INTERPOLATION::POINT, 1600);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL1]))[2])->SetNode(8, {0.875f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 1600);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL1]))[3])->SetNode(8, {1.000f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 1600);

		(ani[(UINT)ANI_ENUM::SPECIAL1])->SetLoop(false);	(ani[(UINT)ANI_ENUM::SPECIAL1])->SetAuto(false);
	}
	void SA_Special2(){
		List<MESH>::iterator mesh_iter = GetMesh().Begin();

		ani[(UINT)ANI_ENUM::SPECIAL2] = new ANIMATION((void*&)*this, 4);
		(*(ani[(UINT)ANI_ENUM::SPECIAL2]))[0] = new _Graph_template<FLOAT2>(mesh_iter->vtx[0].tex, 9);
		(*(ani[(UINT)ANI_ENUM::SPECIAL2]))[1] = new _Graph_template<FLOAT2>(mesh_iter->vtx[1].tex, 9);
		(*(ani[(UINT)ANI_ENUM::SPECIAL2]))[2] = new _Graph_template<FLOAT2>(mesh_iter->vtx[2].tex, 9);
		(*(ani[(UINT)ANI_ENUM::SPECIAL2]))[3] = new _Graph_template<FLOAT2>(mesh_iter->vtx[3].tex, 9);

		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL2]))[0])->SetNode(0, {0.000f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 0);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL2]))[1])->SetNode(0, {0.125f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 0);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL2]))[2])->SetNode(0, {0.000f, 0.50f}, _GRAPH::INTERPOLATION::POINT, 0);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL2]))[3])->SetNode(0, {0.125f, 0.50f}, _GRAPH::INTERPOLATION::POINT, 0);

		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL2]))[0])->SetNode(1, {0.125f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 200);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL2]))[1])->SetNode(1, {0.250f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 200);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL2]))[2])->SetNode(1, {0.125f, 0.50f}, _GRAPH::INTERPOLATION::POINT, 200);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL2]))[3])->SetNode(1, {0.250f, 0.50f}, _GRAPH::INTERPOLATION::POINT, 200);

		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL2]))[0])->SetNode(2, {0.250f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 400);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL2]))[1])->SetNode(2, {0.375f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 400);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL2]))[2])->SetNode(2, {0.250f, 0.50f}, _GRAPH::INTERPOLATION::POINT, 400);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL2]))[3])->SetNode(2, {0.375f, 0.50f}, _GRAPH::INTERPOLATION::POINT, 400);

		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL2]))[0])->SetNode(3, {0.375f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 600);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL2]))[1])->SetNode(3, {0.500f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 600);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL2]))[2])->SetNode(3, {0.375f, 0.50f}, _GRAPH::INTERPOLATION::POINT, 600);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL2]))[3])->SetNode(3, {0.500f, 0.50f}, _GRAPH::INTERPOLATION::POINT, 600);

		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL2]))[0])->SetNode(4, {0.500f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 800);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL2]))[1])->SetNode(4, {0.625f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 800);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL2]))[2])->SetNode(4, {0.500f, 0.50f}, _GRAPH::INTERPOLATION::POINT, 800);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL2]))[3])->SetNode(4, {0.625f, 0.50f}, _GRAPH::INTERPOLATION::POINT, 800);

		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL2]))[0])->SetNode(5, {0.625f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 1000);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL2]))[1])->SetNode(5, {0.750f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 1000);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL2]))[2])->SetNode(5, {0.625f, 0.50f}, _GRAPH::INTERPOLATION::POINT, 1000);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL2]))[3])->SetNode(5, {0.750f, 0.50f}, _GRAPH::INTERPOLATION::POINT, 1000);

		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL2]))[0])->SetNode(6, {0.750f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 1200);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL2]))[1])->SetNode(6, {0.875f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 1200);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL2]))[2])->SetNode(6, {0.750f, 0.50f}, _GRAPH::INTERPOLATION::POINT, 1200);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL2]))[3])->SetNode(6, {0.875f, 0.50f}, _GRAPH::INTERPOLATION::POINT, 1200);

		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL2]))[0])->SetNode(7, {0.875f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 1400);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL2]))[1])->SetNode(7, {1.000f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 1400);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL2]))[2])->SetNode(7, {0.875f, 0.50f}, _GRAPH::INTERPOLATION::POINT, 1400);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL2]))[3])->SetNode(7, {1.000f, 0.50f}, _GRAPH::INTERPOLATION::POINT, 1400);

		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL2]))[0])->SetNode(8, {0.875f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 1600);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL2]))[1])->SetNode(8, {1.000f, 0.25f}, _GRAPH::INTERPOLATION::POINT, 1600);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL2]))[2])->SetNode(8, {0.875f, 0.50f}, _GRAPH::INTERPOLATION::POINT, 1600);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL2]))[3])->SetNode(8, {1.000f, 0.50f}, _GRAPH::INTERPOLATION::POINT, 1600);

		(ani[(UINT)ANI_ENUM::SPECIAL2])->SetLoop(false);	(ani[(UINT)ANI_ENUM::SPECIAL2])->SetAuto(false);
	}
	void SA_Special3(){
		List<MESH>::iterator mesh_iter = GetMesh().Begin();

		ani[(UINT)ANI_ENUM::SPECIAL3] = new ANIMATION((void*&)*this, 4);
		(*(ani[(UINT)ANI_ENUM::SPECIAL3]))[0] = new _Graph_template<FLOAT2>(mesh_iter->vtx[0].tex, 9);
		(*(ani[(UINT)ANI_ENUM::SPECIAL3]))[1] = new _Graph_template<FLOAT2>(mesh_iter->vtx[1].tex, 9);
		(*(ani[(UINT)ANI_ENUM::SPECIAL3]))[2] = new _Graph_template<FLOAT2>(mesh_iter->vtx[2].tex, 9);
		(*(ani[(UINT)ANI_ENUM::SPECIAL3]))[3] = new _Graph_template<FLOAT2>(mesh_iter->vtx[3].tex, 9);

		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL3]))[0])->SetNode(0, {0.000f, 0.50f}, _GRAPH::INTERPOLATION::POINT, 0);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL3]))[1])->SetNode(0, {0.125f, 0.50f}, _GRAPH::INTERPOLATION::POINT, 0);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL3]))[2])->SetNode(0, {0.000f, 0.75f}, _GRAPH::INTERPOLATION::POINT, 0);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL3]))[3])->SetNode(0, {0.125f, 0.75f}, _GRAPH::INTERPOLATION::POINT, 0);

		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL3]))[0])->SetNode(1, {0.125f, 0.50f}, _GRAPH::INTERPOLATION::POINT, 200);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL3]))[1])->SetNode(1, {0.250f, 0.50f}, _GRAPH::INTERPOLATION::POINT, 200);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL3]))[2])->SetNode(1, {0.125f, 0.75f}, _GRAPH::INTERPOLATION::POINT, 200);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL3]))[3])->SetNode(1, {0.250f, 0.75f}, _GRAPH::INTERPOLATION::POINT, 200);

		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL3]))[0])->SetNode(2, {0.250f, 0.50f}, _GRAPH::INTERPOLATION::POINT, 400);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL3]))[1])->SetNode(2, {0.375f, 0.50f}, _GRAPH::INTERPOLATION::POINT, 400);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL3]))[2])->SetNode(2, {0.250f, 0.75f}, _GRAPH::INTERPOLATION::POINT, 400);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL3]))[3])->SetNode(2, {0.375f, 0.75f}, _GRAPH::INTERPOLATION::POINT, 400);

		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL3]))[0])->SetNode(3, {0.375f, 0.50f}, _GRAPH::INTERPOLATION::POINT, 600);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL3]))[1])->SetNode(3, {0.500f, 0.50f}, _GRAPH::INTERPOLATION::POINT, 600);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL3]))[2])->SetNode(3, {0.375f, 0.75f}, _GRAPH::INTERPOLATION::POINT, 600);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL3]))[3])->SetNode(3, {0.500f, 0.75f}, _GRAPH::INTERPOLATION::POINT, 600);

		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL3]))[0])->SetNode(4, {0.500f, 0.50f}, _GRAPH::INTERPOLATION::POINT, 800);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL3]))[1])->SetNode(4, {0.625f, 0.50f}, _GRAPH::INTERPOLATION::POINT, 800);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL3]))[2])->SetNode(4, {0.500f, 0.75f}, _GRAPH::INTERPOLATION::POINT, 800);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL3]))[3])->SetNode(4, {0.625f, 0.75f}, _GRAPH::INTERPOLATION::POINT, 800);

		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL3]))[0])->SetNode(5, {0.625f, 0.50f}, _GRAPH::INTERPOLATION::POINT, 1000);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL3]))[1])->SetNode(5, {0.750f, 0.50f}, _GRAPH::INTERPOLATION::POINT, 1000);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL3]))[2])->SetNode(5, {0.625f, 0.75f}, _GRAPH::INTERPOLATION::POINT, 1000);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL3]))[3])->SetNode(5, {0.750f, 0.75f}, _GRAPH::INTERPOLATION::POINT, 1000);

		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL3]))[0])->SetNode(6, {0.750f, 0.50f}, _GRAPH::INTERPOLATION::POINT, 1200);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL3]))[1])->SetNode(6, {0.875f, 0.50f}, _GRAPH::INTERPOLATION::POINT, 1200);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL3]))[2])->SetNode(6, {0.750f, 0.75f}, _GRAPH::INTERPOLATION::POINT, 1200);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL3]))[3])->SetNode(6, {0.875f, 0.75f}, _GRAPH::INTERPOLATION::POINT, 1200);

		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL3]))[0])->SetNode(7, {0.875f, 0.50f}, _GRAPH::INTERPOLATION::POINT, 1400);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL3]))[1])->SetNode(7, {1.000f, 0.50f}, _GRAPH::INTERPOLATION::POINT, 1400);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL3]))[2])->SetNode(7, {0.875f, 0.75f}, _GRAPH::INTERPOLATION::POINT, 1400);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL3]))[3])->SetNode(7, {1.000f, 0.75f}, _GRAPH::INTERPOLATION::POINT, 1400);

		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL3]))[0])->SetNode(8, {0.875f, 0.50f}, _GRAPH::INTERPOLATION::POINT, 1600);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL3]))[1])->SetNode(8, {1.000f, 0.50f}, _GRAPH::INTERPOLATION::POINT, 1600);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL3]))[2])->SetNode(8, {0.875f, 0.75f}, _GRAPH::INTERPOLATION::POINT, 1600);
		((_Graph_template<FLOAT2>*)(*(ani[(UINT)ANI_ENUM::SPECIAL3]))[3])->SetNode(8, {1.000f, 0.75f}, _GRAPH::INTERPOLATION::POINT, 1600);

		(ani[(UINT)ANI_ENUM::SPECIAL3])->SetLoop(false);	(ani[(UINT)ANI_ENUM::SPECIAL3])->SetAuto(false);
	}
	void SettingAnimation(){
		//8:4
		SA_Stand();
		SA_Walk();
		SA_Dash();
		SA_Jump();
		SA_SNDA();
		SA_SNDB();
		SA_SNDC();
		SA_SNDD();
		SA_Special1();
		SA_Special2();
		SA_Special3();
	}
	void SetSkill(){

	}
	void RegistATKCOL(){Collision::GetSingleton()->RegistPlayerATKList((void*)this, atk_col);}
	void RemoveATKCOL(){Collision::GetSingleton()->RemovePlayerATKList((void*)this, atk_col);}

	void update(){
		command.update();
		if(state != command.GetState() && !command.IsLocked()){
			state = command.GetState();
			if((UINT)(state & Cha_State::SPECIAL) != 0x00){
				switch(((UINT)(state & Cha_State::SPECIAL_TYPE))>>16){
				case 1:
					GetMesh().GetData(0).material->diffuse = texture[2];
					SetAnimation(ani[(UINT)ANI_ENUM::SPECIAL1]);
					command.Lock(0x11);
					break;
				case 2:
					GetMesh().GetData(0).material->diffuse = texture[2];
					SetAnimation(ani[(UINT)ANI_ENUM::SPECIAL2]);
					command.Lock(0x12);
					break;
				case 3:
					GetMesh().GetData(0).material->diffuse = texture[2];
					SetAnimation(ani[(UINT)ANI_ENUM::SPECIAL3]);
					command.Lock(0x13);
					break;
				}
			}
			else{
				if((UINT)(state & Cha_State::AATK) != 0x00){
					GetMesh().GetData(0).material->diffuse = texture[1];
					SetAnimation(ani[(UINT)ANI_ENUM::STAND_A]);
					command.Lock(0x01);
				}
				if((UINT)(state & Cha_State::BATK) != 0x00){
					GetMesh().GetData(0).material->diffuse = texture[1];
					SetAnimation(ani[(UINT)ANI_ENUM::STAND_B]);
					command.Lock(0x02);
				}
				if((UINT)(state & Cha_State::CATK) != 0x00){
					GetMesh().GetData(0).material->diffuse = texture[1];
					SetAnimation(ani[(UINT)ANI_ENUM::STAND_C]);
					command.Lock(0x04);
				}
				if((UINT)(state & Cha_State::DATK) != 0x00){
					GetMesh().GetData(0).material->diffuse = texture[1];
					SetAnimation(ani[(UINT)ANI_ENUM::STAND_D]);
					command.Lock(0x08);
				}
			}
			switch(state){
			case Cha_State::STAND:	SetAnimation(ani[(UINT)ANI_ENUM::STAND]);	break;
			case Cha_State::WALK:	SetAnimation(ani[(UINT)ANI_ENUM::WALK]);	break;
			case Cha_State::DASH:	SetAnimation(ani[(UINT)ANI_ENUM::DASH]);	break;
			}
			GetAnimation().Seek(0);
			GetAnimation().Play();
			cur = command.CurrentDirection();
		}
		if(command.IsLocked() && (GetAnimation().GetType() == _GRAPH::TYPE::STOP)){
			GetMesh().GetData(0).material->diffuse = texture[0];
			command.Lock(0x00);
		}
		if(!command.IsLocked()){
			FLOAT gravity = physics.GetVelocity().y;
			FLOAT vel_x = physics.GetVelocity().x;
			switch(state&Cha_State::MOVE_x){
			case Cha_State::WALK:	vel_x = 5.0f;	break;
			case Cha_State::DASH:	vel_x = 15.0f;	break;
			}
			switch(state&Cha_State::MOVE_y){
			case Cha_State::JUMP:	if(gravity == 0.0f){gravity = -15.0f;} break;
			}
			if((Direction)(cur & 0x03) == Direction::LEFT){vel_x = (vel_x > 0.0f)?-vel_x:vel_x;}
			physics.AddForce({vel_x, gravity, 0.0f});
		}
		SetPosition(physics.move(GetPosition()));
		Move2DCamera(GetPosition());
	}
};
