#pragma once

#include "Character.h"

class Monster : public Character{
private:
	INT HP;
	INT Interval;
protected:

public:
	Monster() : Character(){RegistObject((void*)this, (UINT)eLayer::ENEMY);}
	~Monster(){
		RemoveObject((void*)this, (UINT)eLayer::ENEMY);
	}
	void BeAttacked(){
		if(IsInterval()){return;}
		--HP;
		std::cout<< "HP : " << HP <<std::endl;
		Interval = 600;
	}
	INT GetHP(){return HP;}
	void SetHP(UINT i){HP = i;}
	void AlterHP(UINT i){HP += i;}
	void DecreaseInterval(){
		Interval -= GetTick();
		if(Interval < 0){
			Interval = 0;
		}
	}
	bool IsInterval(){return (Interval != 0);}
};

class TestMop : public Monster{
private:
	ANIMATION* interval_ani;

public:
	TestMop(){
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
		mesh_iter->vtx[1] = {{ 32.0f,-128.0f, 0.0f},{1.0f, 1.0f, 1.0f, 1.0f},{},{1.000f, 0.00f}};
		mesh_iter->vtx[2] = {{-32.0f,   0.0f, 0.0f},{1.0f, 1.0f, 1.0f, 1.0f},{},{0.000f, 1.0f}};
		mesh_iter->vtx[3] = {{ 32.0f,   0.0f, 0.0f},{1.0f, 1.0f, 1.0f, 1.0f},{},{1.000f, 1.0f}};
		
		IMAGE t = {};
		LoadBMP("scarecrow.bmp", t);
		mesh_iter->material->diffuse = RegistTexture(t, mesh_iter->material->diffuse);
		delete [] t.buf;

		COL2D* t_col = new BOX;
		t_col->joint = &(*scr);
		((BOX*)t_col)->_1 = {-32.0f,-128.0f, 0.0f};
		((BOX*)t_col)->_2 = { 32.0f,-128.0f, 0.0f};
		((BOX*)t_col)->_3 = { 32.0f,   0.0f, 0.0f};
		((BOX*)t_col)->_4 = {-32.0f,   0.0f, 0.0f};
		GetCollider2D().CreateNode(t_col);

		Collision::GetSingleton()->RegistCHARAList((void*)this, t_col);
		Collision::GetSingleton()->RegistEnemyDEFList((void*)this, t_col);

		SetHP(3);

		interval_ani = nullptr;
		interval_ani = new ANIMATION((void*&)*this, 4);
		(*(interval_ani))[0] = new _Graph_template<FLOAT>(mesh_iter->vtx[0].diffuse.w, 3);
		(*(interval_ani))[1] = new _Graph_template<FLOAT>(mesh_iter->vtx[1].diffuse.w, 3);
		(*(interval_ani))[2] = new _Graph_template<FLOAT>(mesh_iter->vtx[2].diffuse.w, 3);
		(*(interval_ani))[3] = new _Graph_template<FLOAT>(mesh_iter->vtx[3].diffuse.w, 3);

		((_Graph_template<FLOAT>*)(*(interval_ani))[0])->SetNode(0, 1.0f, _GRAPH::INTERPOLATION::LINEAR, 0);
		((_Graph_template<FLOAT>*)(*(interval_ani))[1])->SetNode(0, 1.0f, _GRAPH::INTERPOLATION::LINEAR, 0);
		((_Graph_template<FLOAT>*)(*(interval_ani))[2])->SetNode(0, 1.0f, _GRAPH::INTERPOLATION::LINEAR, 0);
		((_Graph_template<FLOAT>*)(*(interval_ani))[3])->SetNode(0, 1.0f, _GRAPH::INTERPOLATION::LINEAR, 0);

		((_Graph_template<FLOAT>*)(*(interval_ani))[0])->SetNode(1, 0.4f, _GRAPH::INTERPOLATION::LINEAR, 100);
		((_Graph_template<FLOAT>*)(*(interval_ani))[1])->SetNode(1, 0.4f, _GRAPH::INTERPOLATION::LINEAR, 100);
		((_Graph_template<FLOAT>*)(*(interval_ani))[2])->SetNode(1, 0.4f, _GRAPH::INTERPOLATION::LINEAR, 100);
		((_Graph_template<FLOAT>*)(*(interval_ani))[3])->SetNode(1, 0.4f, _GRAPH::INTERPOLATION::LINEAR, 100);

		((_Graph_template<FLOAT>*)(*(interval_ani))[0])->SetNode(2, 1.0f, _GRAPH::INTERPOLATION::LINEAR, 200);
		((_Graph_template<FLOAT>*)(*(interval_ani))[1])->SetNode(2, 1.0f, _GRAPH::INTERPOLATION::LINEAR, 200);
		((_Graph_template<FLOAT>*)(*(interval_ani))[2])->SetNode(2, 1.0f, _GRAPH::INTERPOLATION::LINEAR, 200);
		((_Graph_template<FLOAT>*)(*(interval_ani))[3])->SetNode(2, 1.0f, _GRAPH::INTERPOLATION::LINEAR, 200);

		interval_ani->SetLoop(true);	interval_ani->SetAuto(true);
	}
	~TestMop(){
		SetAnimation(nullptr);
		/*if(interval_ani != nullptr){
			delete interval_ani; 
			//interval_ani = nullptr;
		}*/
		COL2D* t_col = GetCollider2D().GetData(0);
		Collision::GetSingleton()->RemoveCHARAList((void*)this, t_col);
		Collision::GetSingleton()->RemoveEnemyDEFList((void*)this, t_col);
	}
	void update(){
		SetPosition(physics.move(GetPosition()));
		if(IsInterval()){
			SetAnimation(interval_ani);
			if(IsInterval()){DecreaseInterval();}
		}
		else{
			SetAnimation(nullptr);
			List<MESH>::iterator mesh_iter = GetMesh().Begin();
			mesh_iter->vtx[0].diffuse.w = 1.0f;
			mesh_iter->vtx[1].diffuse.w = 1.0f;
			mesh_iter->vtx[2].diffuse.w = 1.0f;
			mesh_iter->vtx[3].diffuse.w = 1.0f;
		}
	}
};