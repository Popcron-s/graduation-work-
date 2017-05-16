#pragma once

#include "Object.h"
#include "variable.h"
#include "Physics.h"
#include "command.h"

class Character : public OBJECT{
protected:
	Physics physics;
	Direction cur;
	Cha_State state;

public:
	Character() : OBJECT(){}
	~Character(){}

	virtual void BeAttacked(){

	}
	virtual void Fall(){
		FLOAT gravity = physics.GetVelocity().y;
		FLOAT vel_x = physics.GetVelocity().x;
		if(gravity == 0.0f){gravity = 1.0f;}
		physics.AddForce({vel_x, gravity, 0.0f});
	}
	virtual void Block(){
		FLOAT3 t = GetPosition();
		t.y = 300.0f;
		SetPosition(t);
		FLOAT vel_x = physics.GetVelocity().x;
		physics.AddForce({vel_x, 0.0f, 0.0f});
	}
};