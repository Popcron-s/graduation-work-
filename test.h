#pragma once

#include "Engine_Win32.h"

class testWalker : public OBJECT{
private:

public:
	testWalker(){
		RegistObject((void*)this, 0);

		//SetPosition({1.0f, 2.0f, 3.0f});

		JOINT t_joint = {
			JOINT_NAME::NONE, 
			{200.0f, 60.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {100.0f, 100.0f, 1.0f}, 
			MATRIX4x4::Initialize()
		};

		GetSkeletal().CreateRoot(t_joint);

		Tree<JOINT>::searcher scr = GetSkeletal().Begin();

		MESH t_mesh = {&(*scr), new VERTEX[4], 4, nullptr, nullptr};
		t_mesh.vtx[0] = {{-1.0f, -1.0f, 0.0f},{1.0f, 0.0f, 0.0f, 1.0f},{},{0.0f, 0.0f}};
		t_mesh.vtx[1] = {{ 1.0f, -1.0f, 0.0f},{1.0f, 0.0f, 0.0f, 1.0f},{},{1.0f, 0.0f}};
		t_mesh.vtx[2] = {{-1.0f,  1.0f, 0.0f},{1.0f, 0.0f, 0.0f, 1.0f},{},{0.0f, 1.0f}};
		t_mesh.vtx[3] = {{ 1.0f,  1.0f, 0.0f},{1.0f, 0.0f, 0.0f, 1.0f},{},{1.0f, 1.0f}};

		IMAGE i = {};
		LoadBMP("test_24.bmp", i);

		t_mesh.material = new MATERIAL;
		*t_mesh.material = {};
		RegistTexture(i, t_mesh.material->diffuse);

		GetMesh().CreateNode(t_mesh);

		BOX* t_col = new BOX;
		t_col->joint = &(*scr);
		t_col->_1 = {-1.0f, -1.0f, 0.0f};
		t_col->_2 = { 1.0f, -1.0f, 0.0f};
		t_col->_3 = { 1.0f,  1.0f, 0.0f};
		t_col->_4 = {-1.0f,  1.0f, 0.0f};
		GetCollider2D().CreateNode(t_col);
	}
	~testWalker(){RemoveObject((void*)this, 0);}

	void update(){
		BOX target_box = {};
		target_box.joint = nullptr;
		target_box._1 = {-100.0f, -10.0f, 0.0f};
		target_box._2 = { 100.0f, -10.0f, 0.0f};
		target_box._3 = { 100.0f,  10.0f, 0.0f};
		target_box._4 = {-100.0f,  10.0f, 0.0f};

		if(CheckCollision2D(*(GetCollider2D().GetData(0)), target_box)){
			GetMesh().GetData(0).vtx[0].diffuse.x = 0.0f;
			GetMesh().GetData(0).vtx[1].diffuse.x = 0.0f;
			GetMesh().GetData(0).vtx[2].diffuse.x = 0.0f;
			GetMesh().GetData(0).vtx[3].diffuse.x = 0.0f;

			GetMesh().GetData(0).vtx[0].diffuse.y = 1.0f;
			GetMesh().GetData(0).vtx[1].diffuse.y = 1.0f;
			GetMesh().GetData(0).vtx[2].diffuse.y = 1.0f;
			GetMesh().GetData(0).vtx[3].diffuse.y = 1.0f;
		}
		else{
			GetMesh().GetData(0).vtx[0].diffuse.y = 0.0f;
			GetMesh().GetData(0).vtx[1].diffuse.y = 0.0f;
			GetMesh().GetData(0).vtx[2].diffuse.y = 0.0f;
			GetMesh().GetData(0).vtx[3].diffuse.y = 0.0f;

			GetMesh().GetData(0).vtx[0].diffuse.x = 1.0f;
			GetMesh().GetData(0).vtx[1].diffuse.x = 1.0f;
			GetMesh().GetData(0).vtx[2].diffuse.x = 1.0f;
			GetMesh().GetData(0).vtx[3].diffuse.x = 1.0f;
		}

		Tree<JOINT>::searcher scr = GetSkeletal().Begin();
		FLOAT3& pos = scr->pos;
		FLOAT3& rot = scr->rot;
		/*if(MainSystem::GetSingleton()->GetKeyboard()->KEY['W'] & 0x81){pos.y-=1.0f;}
		if(MainSystem::GetSingleton()->GetKeyboard()->KEY['S'] & 0x81){pos.y+=1.0f;}
		if(MainSystem::GetSingleton()->GetKeyboard()->KEY['A'] & 0x81){pos.x-=1.0f;}
		if(MainSystem::GetSingleton()->GetKeyboard()->KEY['D'] & 0x81){pos.x+=1.0f;}
		if(MainSystem::GetSingleton()->GetKeyboard()->KEY['Q'] & 0x81){rot.z+=1.0f;}
		if(MainSystem::GetSingleton()->GetKeyboard()->KEY['E'] & 0x81){rot.z-=1.0f;}*/
		FLOAT f = DEGREE(rot.z);
		if(f > 360.0f){f = 0.0f;}
		else{f += 1.0f;}
		rot.z = RADIAN(f);
		//SetPosition(pos);
		//SetRotation(rot);

		if(GetKeyboard()->KEY[VK_SPACE] & 0x81){
			SetPosition({});
			SetRotation({});
		}
	}
};

class Selecter : public OBJECT{
private:
	MESH* s_mesh;
public:
	Selecter(){
		RegistObject((void*)this, 0);

		SetPosition({0.0f, 0.0f, 0.0f});

		JOINT t_joint = {
			JOINT_NAME::NONE, 
			{-50.0f, 0.0f, 10.0f}, {0.0f, 0.0f, 1.0f}, {3.0f, 3.0f, 1.0f}, 
			MATRIX4x4::Initialize()
		};

		GetSkeletal().CreateRoot(t_joint);

		Tree<JOINT>::searcher scr = GetSkeletal().Begin();

		GetSkeletal().Create(scr, t_joint);

		MESH t_mesh = {&(*scr), new VERTEX[4], 4, nullptr, nullptr};
		t_mesh.vtx[0] = {{-10.0f, -10.0f, 0.0f},{0.0f, 1.0f, 0.0f, 1.0f},{},{0.0f, 0.0f}};
		t_mesh.vtx[1] = {{ 10.0f, -10.0f, 0.0f},{0.0f, 1.0f, 0.0f, 1.0f},{},{1.0f, 0.0f}};
		t_mesh.vtx[2] = {{-10.0f,  10.0f, 0.0f},{0.0f, 1.0f, 0.0f, 1.0f},{},{0.0f, 1.0f}};
		t_mesh.vtx[3] = {{ 10.0f,  10.0f, 0.0f},{0.0f, 1.0f, 0.0f, 1.0f},{},{1.0f, 1.0f}};
		GetMesh().CreateNode(t_mesh);

		BOX* t_col = new BOX;
		t_col->joint = &(*scr);
		t_col->_1 = {-10.0f, -10.0f, 0.0f};
		t_col->_2 = { 10.0f, -10.0f, 0.0f};
		t_col->_3 = { 10.0f,  10.0f, 0.0f};
		t_col->_4 = {-10.0f,  10.0f, 0.0f};
		GetCollider2D().CreateNode((BOX*)t_col);
	}
	~Selecter(){
		RemoveObject((void*)this, 0);
	}

	void update(){
		BOX& t_col = (BOX&)*(GetCollider2D().GetData(0));
		FLOAT2 v = ClickOrtho();
		//std::cout<< v.x << " / " << v.y <<std::endl;
		if(CheckCollision2D(v, (COL2D&)t_col)){
			GetMesh().GetData(0).vtx[0].diffuse.y = 1.0f;
			GetMesh().GetData(0).vtx[1].diffuse.y = 1.0f;
			GetMesh().GetData(0).vtx[2].diffuse.y = 1.0f;
			GetMesh().GetData(0).vtx[3].diffuse.y = 1.0f;

			GetMesh().GetData(0).vtx[0].diffuse.x = 0.0f;
			GetMesh().GetData(0).vtx[1].diffuse.x = 0.0f;
			GetMesh().GetData(0).vtx[2].diffuse.x = 0.0f;
			GetMesh().GetData(0).vtx[3].diffuse.x = 0.0f;
		}
		else{
			GetMesh().GetData(0).vtx[0].diffuse.x = 1.0f;
			GetMesh().GetData(0).vtx[1].diffuse.x = 1.0f;
			GetMesh().GetData(0).vtx[2].diffuse.x = 1.0f;
			GetMesh().GetData(0).vtx[3].diffuse.x = 1.0f;

			GetMesh().GetData(0).vtx[0].diffuse.y = 0.0f;
			GetMesh().GetData(0).vtx[1].diffuse.y = 0.0f;
			GetMesh().GetData(0).vtx[2].diffuse.y = 0.0f;
			GetMesh().GetData(0).vtx[3].diffuse.y = 0.0f;
		}
	}
};

class FPS : public OBJECT{
private:
	FLOAT3 c_pos;
	SOUND s1;
	testWalker* t1;
	Selecter* t2;
	static FPS* m_pSingleton;
public:
	FPS() : OBJECT(){
		RegistObject((void*)this, 0);
		c_pos = {};
		t1 = new testWalker;
		t2 = new Selecter;
		JOINT t_joint = {
			JOINT_NAME::NONE, 
			{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, 
			MATRIX4x4::Initialize()
		};

		GetSkeletal().CreateRoot(t_joint);

		SKEL_SEARCHER(scr);

		MESH t_mesh = {&(*scr), new VERTEX[4], 4, nullptr, nullptr};
		t_mesh.vtx[0] = {{-100.0f, -10.0f, 0.0f},{1.0f, 1.0f, 1.0f, 0.5f},{},{0.0f, 0.0f}};
		t_mesh.vtx[1] = {{ 100.0f, -10.0f, 0.0f},{1.0f, 1.0f, 1.0f, 0.5f},{},{1.0f, 0.0f}};
		t_mesh.vtx[2] = {{-100.0f,  10.0f, 0.0f},{1.0f, 1.0f, 1.0f, 0.5f},{},{0.0f, 1.0f}};
		t_mesh.vtx[3] = {{ 100.0f,  10.0f, 0.0f},{1.0f, 1.0f, 1.0f, 0.5f},{},{1.0f, 1.0f}};

		GetMesh().CreateNode(t_mesh);

		GetSkeletal().Create(scr, t_joint);
		scr.Child(0);
		t_mesh.joint = &(*scr);
		t_mesh.vtx = new VERTEX[4];
		t_mesh.vtx[0] = {{-10.0f, -10.0f, 0.0f},{1.0f, 1.0f, 1.0f, 0.5f},{},{0.0f, 0.0f}};
		t_mesh.vtx[1] = {{ 10.0f, -10.0f, 0.0f},{1.0f, 1.0f, 1.0f, 0.5f},{},{1.0f, 0.0f}};
		t_mesh.vtx[2] = {{-10.0f,  10.0f, 0.0f},{1.0f, 1.0f, 1.0f, 0.5f},{},{0.0f, 1.0f}};
		t_mesh.vtx[3] = {{ 10.0f,  10.0f, 0.0f},{1.0f, 1.0f, 1.0f, 0.5f},{},{1.0f, 1.0f}};

		GetMesh().CreateNode(t_mesh);

		CreateAnimation(1);
		GetAnimation()[0] = new _Graph_function<FPS>(this, 1);
		((_Graph_function<FPS>*)GetAnimation()[0])->SetNode(0, &FPS::sound, 4000);
		GetAnimation()[0]->SetAuto(true);
		GetAnimation()[0]->SetLoop(true);

		LoadWAV("Alert.wav", s1);

		BOX* t_col = new BOX;
		t_col->joint = &(*scr);
		t_col->_1 = {-10.0f, -10.0f, 0.0f};
		t_col->_2 = { 10.0f, -10.0f, 0.0f};
		t_col->_3 = {-10.0f,  10.0f, 0.0f};
		t_col->_4 = { 10.0f,  10.0f, 0.0f};
	}
	~FPS(){
		delete t1;
		delete t2;
		RemoveObject((void*)this, 0);
	}

	static FPS* GetSingleton(){
		if(m_pSingleton == nullptr){
			m_pSingleton = new FPS;
		}
		return m_pSingleton;
	}

	void sound(){
		RegistSound(s1);
	}

	void update(){
		UINT frame = 0;
		if(GetTick() != 0){
			frame = 1000/GetTick();
		}
		std::cout<< frame << "f/s" <<std::endl;
		if(GetMouse()->button[0] & 0x81){
			FLOAT2 t = ClickOrtho();
			SKEL_SEARCHER(scr);
			scr.Child(0);
			scr->pos = {t.x, t.y, 0.0f};
		}
		
		if(GetKeyboard()->KEY[VK_NUMPAD1] == 0x81){
			SOUND s = {};
			LoadWAV("Alert.wav", s);
			RegistSound(s);
		}
		if(GetKeyboard()->KEY[VK_NUMPAD2] == 0x81){
			SOUND s = {};
			LoadWAV("test_bgm2.wav", s);
			RegistSound(s);
		}

		if(GetKeyboard()->KEY['W'] & 0x81){c_pos.y -= 0.5f;}
		if(GetKeyboard()->KEY['S'] & 0x81){c_pos.y += 0.5f;}
		if(GetKeyboard()->KEY['A'] & 0x81){c_pos.x -= 0.5f;}
		if(GetKeyboard()->KEY['D'] & 0x81){c_pos.x += 0.5f;}
		Move2DCamera(c_pos);
	}
};

FPS* FPS::m_pSingleton = nullptr;
