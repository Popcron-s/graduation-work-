#pragma once

#include "variable.h"
#include "Engine_Win32.h"
#include "Singleton.h"

#include "Character.h"

#include <list>

class Collision : public Singleton<Collision>{
private:
	struct _node{
		void* obj;
		COL2D* col;

		bool operator ==(_node n){
			if(this->obj == n.obj && this->col == n.col){return true;}
			else{return false;}
		}
	};
	std::list<_node> player_atk;
	std::list<_node> player_def;
	std::list<_node> enemy_atk;
	std::list<_node> enemy_def;
	std::list<_node> chara;
	std::list<_node> terra;

public:
	void RegistPlayerATKList(void* obj, COL2D* col){player_atk.push_front({obj, col});}
	void RegistPlayerDEFList(void* obj, COL2D* col){player_def.push_front({obj, col});}
	void RegistEnemyATKList	(void* obj, COL2D* col)	{enemy_atk.push_front({obj, col});}
	void RegistEnemyDEFList	(void* obj, COL2D* col)	{enemy_def.push_front({obj, col});}
	void RegistCHARAList	(void* obj, COL2D* col)		{chara.push_front({obj, col});}
	void RegistTERRAList	(void* obj, COL2D* col)		{terra.push_front({obj, col});}

	void RemovePlayerATKList(void* obj, COL2D* col){player_atk.remove({obj, col});}
	void RemovePlayerDEFList(void* obj, COL2D* col){player_def.remove({obj, col});}
	void RemoveEnemyATKList	(void* obj, COL2D* col)	{enemy_atk.remove({obj, col});}
	void RemoveEnemyDEFList	(void* obj, COL2D* col)	{enemy_def.remove({obj, col});}
	void RemoveCHARAList	(void* obj, COL2D* col)		{chara.remove({obj, col});}
	void RemoveTERRAList	(void* obj, COL2D* col)		{terra.remove({obj, col});}

	void update(){
		std::list<_node>::iterator iter_a;
		std::list<_node>::iterator iter_b;

		for(iter_a = player_atk.begin() ; iter_a != player_atk.end() ; ++iter_a){
			for(iter_b = enemy_def.begin() ; iter_b != enemy_def.end() ; ++iter_b){
				if(CheckCollision2D(*(iter_a->col), *(iter_b->col))){
					((Character*)iter_b->obj)->BeAttacked();
				}
			}
		}

		//iter_a = enemy_atk.begin();
		//iter_b = player_def.begin();

		for(iter_a = chara.begin() ; iter_a != chara.end() ; ++iter_a){
			bool fall = true;
			for(iter_b = terra.begin() ; iter_b != terra.end() ; ++iter_b){
				if(CheckCollision2D(*(iter_a->col), *(iter_b->col))){fall = false;}
			}
			if(fall){((Character*)(iter_a->obj))->Fall();}
			else{((Character*)(iter_a->obj))->Block();}
		}
	}
};