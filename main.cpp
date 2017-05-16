#include <iostream>
#include <Windows.h>

#include "Game_Enum.h"
#include "Player.h"
#include "TestStage.h"
#include "Collision.h"

class Start_class : public Singleton<Start_class>{
private:
	Player* player;
	Stage* stage;

public:
	Start_class(){
		CreateLayer((UINT)eLayer::MAX_LAYER);
		player = new Player;
		stage = new TestStage;
	}
	~Start_class(){
		delete player;
		delete stage;
		DestroyLayer();
	}

	void update(){
		Collision::GetSingleton()->update();
	}
};

void start(){Start_class::GetSingleton()->update();}

void main(){
	HINSTANCE hInst=GetModuleHandle(NULL);
	int CmdShow = SW_SHOW;

	Start(hInst, L"command test", CmdShow, start, 1280, 720);
}