#pragma once

#include "Object.h"
#include "Monster.h"
#include "Game_Enum.h"

#include "Stage.h"

#include <iostream>

class TestStage : public Stage{
public:
	TestStage() : Stage(0x00000000){
		
		JOINT tj = {
			JOINT_NAME::NONE,
			{0.0f, 0.0f, 0.0f}, 
			{0.0f, 0.0f, 0.0f}, 
			{1.0f, 1.0f, 1.0f},
			MATRIX4x4::Initialize()
		};
		GetSkeletal().CreateRoot(tj);
		SKEL_SEARCHER(scr);
		GetSkeletal().Create(scr, tj);	//panorama
		GetSkeletal().Create(scr, tj);	//far bg
		GetSkeletal().Create(scr, tj);	//near bg
		GetSkeletal().Create(scr, tj);	//foot hold

		MESH tm = {
			&(scr.GetChild(3)),
			new VERTEX[4], 4,
			nullptr, nullptr
		};
		//1900 600 / -640 1360
		tm.vtx[0] = {{-640.0f, 300.0f, 0.0f}, {0.5f, 0.5f, 0.5f, 1.0f}, {}, {0.0f, 0.0f}};
		tm.vtx[1] = {{1360.0f, 300.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {}, {1.0f, 0.0f}};
		tm.vtx[2] = {{-640.0f, 360.0f, 0.0f}, {0.5f, 0.5f, 0.5f, 1.0f}, {}, {0.0f, 1.0f}};
		tm.vtx[3] = {{1360.0f, 360.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {}, {1.0f, 1.0f}};
		GetMesh().CreateNode(tm);

		COL2D* tc = new BOX;
		tc->joint = &(scr.GetChild(3));
		((BOX*)tc)->_1 = {-640.0f, 300.0f, 0.0f};
		((BOX*)tc)->_2 = {1360.0f, 300.0f, 0.0f};
		((BOX*)tc)->_3 = {1360.0f, 360.0f, 0.0f};
		((BOX*)tc)->_4 = {-640.0f, 360.0f, 0.0f};
		GetCollider2D().CreateNode(tc);

		Collision::GetSingleton()->RegistTERRAList((void*)this, tc);

		m_pMonster = new Monster*[4];
		for(UINT i = 0 ; i<4 ; ++i){m_pMonster[i] = nullptr;}
	}
	void update(){
		if((GetMouse()->button[0] & 0x80) && (GetMouse()->button[0] & 0x0F)){
			for(UINT i = 0 ; i<4 ; ++i){
				if(m_pMonster[i] == nullptr){
					m_pMonster[i] = new TestMop;
					m_pMonster[i]->SetPosition({ClickOrtho().x, ClickOrtho().y, 0.0f});
					break;
				}
			}
		}
		for(UINT i = 0 ; i<4 ; ++i){
			if(m_pMonster[i] == nullptr){continue;}
			if(m_pMonster[i]->GetHP() <= 0){
				delete (TestMop*)m_pMonster[i];
				m_pMonster[i] = nullptr;
			}
		}
	}
};