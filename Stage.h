#pragma once

#include "variable.h"
#include "Object.h"

class Stage : public OBJECT{
private:

protected:
	UINT m_ID;
	Monster** m_pMonster;
	UINT m_Max_Monster;

public:
	Stage(UINT i) : m_ID(i), m_pMonster(nullptr), m_Max_Monster(0)
		{RegistObject((void*)this, (UINT)eLayer::BG);}
	~Stage(){
		for(UINT i = 0 ; i<m_Max_Monster ; ++i){
			if((m_pMonster[i]) != nullptr){delete (m_pMonster[i]); (m_pMonster[i]) = nullptr;}
		}
		if(m_pMonster != nullptr){delete m_pMonster; m_pMonster = nullptr;}
		RemoveObject((void*)this, (UINT)eLayer::BG);
	}
};