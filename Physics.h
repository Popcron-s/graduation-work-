#pragma once

#include "variable.h"

class Physics{
private:
	FLOAT3 velocity;

	void inertia(){
		//È¾¼Óµµ
		if(-0.2f < velocity.x && velocity.x < 0.2f){velocity.x = 0.0f;}
		else{velocity.x *= 0.8f;}
		//Áß·Â
		if(velocity.y < 0.0f){
			if(velocity.y > -0.2f){velocity.y = 1.0f;}
			else{velocity.y *= 0.9f;}
		}
		else if(velocity.y > 0.0f){
			if(velocity.y > 10.0f){velocity.y = 10.0f;}
			else{velocity.y /= 0.9f;}
		}
	}
public:
	void AddForce(FLOAT3 f){velocity = f;}
	FLOAT3 GetVelocity(){return velocity;}
	FLOAT3 move(FLOAT3 target){
		target += velocity;
		inertia();
		return target;
	}
};
