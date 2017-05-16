#pragma once

#include "Engine_Win32.h"
#include <list>
#include "Singleton.h"
#include <iostream>

#define KEY_SETTING(var, init)	\
private:\
	unsigned char var = init;	\
public:	\
	unsigned char Get##var##(){return var;}\
	void Set##var##(unsigned char key){var = key;}

class KEY_SETTER{
	KEY_SETTING(A, 'A')
	KEY_SETTING(B, 'S')
	KEY_SETTING(C, 'D')
	KEY_SETTING(D, 'F')
	KEY_SETTING(LEFT,	VK_LEFT)
	KEY_SETTING(RIGHT,	VK_RIGHT)
	KEY_SETTING(UP,		VK_UP)
	KEY_SETTING(DOWN,	VK_DOWN)

private:
	static KEY_SETTER* m_pSingleton;

public:
	static KEY_SETTER* GetSingleton(){
		if(m_pSingleton == nullptr){
			m_pSingleton = new KEY_SETTER;
		}
		return m_pSingleton;
	}
};

KEY_SETTER* KEY_SETTER::m_pSingleton = nullptr;
#undef KEY_SETTING
#define KEY(k) KEY_SETTER::GetSingleton()->Get##k##()
enum class GAME_KEY : unsigned int{
	A = 0x01,
	B = 0x02,
	C = 0x04,
	D = 0x08,
	//L1 = 0x10,
	//L2 = 0x20,
	//R1 = 0x40,
	//R2 = 0x80,
	DL = 0x100,
	DR = 0x200,
	DU = 0x400,
	DD = 0x800
	//DLU = 0x500,
	//DLD = 0x900,
	//DRU = 0x600,
	//DRD = 0xA00
};

constexpr GAME_KEY operator | (GAME_KEY a, GAME_KEY b){return (GAME_KEY)((UINT)a | (UINT)b);}
constexpr GAME_KEY operator & (GAME_KEY a, GAME_KEY b){return (GAME_KEY)((UINT)a & (UINT)b);}
constexpr GAME_KEY operator | (GAME_KEY a, UINT b){return (GAME_KEY)((UINT)a | b);}
constexpr GAME_KEY operator & (GAME_KEY a, UINT b){return (GAME_KEY)((UINT)a & b);}

enum class Direction : unsigned char{
	CENTER = 0x00,
	LEFT = 0x01,
	RIGHT = 0x02,
	UP = 0x04,
	DOWN = 0x08
};

constexpr Direction operator | (Direction a, Direction b){return (Direction)((UINT)a | (UINT)b);}
constexpr Direction operator & (Direction a, Direction b){return (Direction)((UINT)a & (UINT)b);}
constexpr Direction operator | (Direction a, UINT b){return (Direction)((UINT)a | b);}
constexpr Direction operator & (Direction a, UINT b){return (Direction)((UINT)a & b);}

enum class Cha_State : UINT{
	STAND = 0x00,
	WALK = 0x01,
	DASH = 0x02,
	SIT = 0x10,
	JUMP = 0x20,
	DJUMP = 0x40,
	MOVE_x = 0x0F,
	MOVE_y = 0xF0,
	MOVE = 0xFF,
	AATK = 0x100,
	BATK = 0x200,
	CATK = 0x400,
	DATK = 0x800,
	ATK = 0xF00,
	SPECIAL = 0x1000,
	SPECIAL_TYPE = 0xFF0000
};
//2bit move_x	0x0F
//2bit move_y	0xF0
//4bit act attack	0xF00
//4bit special	0xF000
//8bit special enum 0xFF0000

constexpr Cha_State operator | (Cha_State a, Cha_State b){return (Cha_State)((UINT)a | (UINT)b);}
constexpr Cha_State operator & (Cha_State a, Cha_State b){return (Cha_State)((UINT)a & (UINT)b);}
constexpr Cha_State operator | (Cha_State a, UINT b){return (Cha_State)((UINT)a | b);}
constexpr Cha_State operator & (Cha_State a, UINT b){return (Cha_State)((UINT)a & b);}

//통상
//앞
//뒤(보류)
//앉아
//앉아 앞
//점프(아래 점프->대점프)
//점프 앞
//점프 아래
//점프 위
//캔슬 A-> B/C/D
//캔슬 B-> C/D
//캔슬 C-> 특수기
//캔슬 D-> A
//캔슬 특수기->불가(스킬 행동 종료후 패턴중엔 사용 가능

//걷기
//뛰기
//앉기
//앉아 걷기
//앉아 대쉬 //존재하지 않음 앉기중 대쉬불가 vs 대쉬중 앉기 불가 둘다 인걸로
//점프 //앉은상태에서 점프시 대점프
//점프 걷기 //공격외 벡터만
//점프 대쉬 //일시적으로 큰폭의 속도 상승 //공격외 조작 불가
//공격 //캔슬표에 맞춰서

class Command{
private:
	struct _node{
		GAME_KEY key;
		UINT time;
	};
	std::list<_node> command_queue;
	Direction cur;
	Cha_State state;
	GAME_KEY WaitAction;
	UINT lock;
	FLOAT3 move_vector;

public:
	void update(){
		WaitAction = (GAME_KEY)0x00;
			 if(GetKeyboard()->KEY[KEY(A)] == 0x81){command_queue.push_front({GAME_KEY::A, 0}); WaitAction = GAME_KEY::A;}
		else if(GetKeyboard()->KEY[KEY(B)] == 0x81){command_queue.push_front({GAME_KEY::B, 0}); WaitAction = GAME_KEY::B;}
		else if(GetKeyboard()->KEY[KEY(C)] == 0x81){command_queue.push_front({GAME_KEY::C, 0}); WaitAction = GAME_KEY::C;}
		else if(GetKeyboard()->KEY[KEY(D)] == 0x81){command_queue.push_front({GAME_KEY::D, 0}); WaitAction = GAME_KEY::D;}

		UINT dir = 0x00;
		if(GetKeyboard()->KEY[KEY(DOWN)] & 0x81)	{dir |= 0x1; dir <<= 1;}	else{dir <<= 1;}
		if(GetKeyboard()->KEY[KEY(UP)] & 0x81)		{dir |= 0x1; dir <<= 1;}	else{dir <<= 1;}
		if(GetKeyboard()->KEY[KEY(RIGHT)] & 0x81)	{dir |= 0x1; dir <<= 1;}	else{dir <<= 1;}		
		if(GetKeyboard()->KEY[KEY(LEFT)] & 0x81)	{dir |= 0x1;}
		if((dir & 0x03) == 0x03){dir &= 0x0C;}
		if((dir & 0x0C) == 0x0C){dir &= 0x03;}
		if(dir != 0x00){
			if(cur != (Direction)dir){
				cur = (Direction)dir;
				command_queue.push_front({(GAME_KEY)(dir<<8), 0});
			}
		}
		else{cur = Direction::CENTER;}
		while(command_queue.size() > 100){command_queue.pop_back();}
		action();
		std::list<_node>::iterator iter = command_queue.begin();
		while(iter != command_queue.end()){
			(*iter).time += GetTick();
			++iter;
		}
	}

	void action(){
		Cha_State t_state = Cha_State::STAND;
		if((cur & 0x03) != Direction::CENTER){
			std::list<_node>::iterator iter = command_queue.begin();
			++iter;
			if(((UINT)((*iter).key & 0x0F00)>>8) == (UINT)(cur & 0x03)){
				if(((*iter).time < 200) || ((state & Cha_State::MOVE_x) == Cha_State::DASH)){
					t_state = t_state|Cha_State::DASH;
				}
				else{
					t_state = t_state|Cha_State::WALK;
				}
			}
			else{
				t_state = t_state|Cha_State::WALK;
			}
		}
		if((cur & 0x0C) != Direction::CENTER){
			if((cur & 0x0C) == Direction::UP){t_state = t_state|Cha_State::JUMP;}
			if((cur & 0x0C) == Direction::DOWN){t_state = t_state|Cha_State::SIT;}
		}
		t_state = t_state|AttackAction(WaitAction);
		/*switch(WaitAction){
		case GAME_KEY::A:	t_state = t_state|Cha_State::AATK;	break;
		case GAME_KEY::B:	t_state = t_state|Cha_State::BATK;	break;
		case GAME_KEY::C:	t_state = t_state|Cha_State::CATK;	break;
		case GAME_KEY::D:	t_state = t_state|Cha_State::DATK;	break;
		}*/
		state = t_state;
	}

	Cha_State AttackAction(GAME_KEY type){
		Cha_State Attack = Cha_State::STAND;
		std::list<_node>::iterator iter = command_queue.begin(); ++iter;
		switch(WaitAction){
		case GAME_KEY::A:{
			if(iter->key == GAME_KEY::DL){
				++iter;
				if(iter->key == (GAME_KEY::DL|GAME_KEY::DD)){
					UINT tick = iter->time;
					++iter;
					if(iter->key == GAME_KEY::DD && tick<200){
						Attack = Cha_State::SPECIAL | 0x010000;
					}
				}
			}
			else if(iter->key == GAME_KEY::DR){
				++iter;
				if(iter->key == (GAME_KEY::DR|GAME_KEY::DD)){
					UINT tick = iter->time;
					++iter;
					if(iter->key == GAME_KEY::DD && tick<200){
						Attack = Cha_State::SPECIAL | 0x010000;
					}
				}
			}
			Attack = Attack|Cha_State::AATK; 
		}
			break;
		case GAME_KEY::B:	Attack = Attack|Cha_State::BATK; break;
		case GAME_KEY::C:	Attack = Attack|Cha_State::CATK; break;
		case GAME_KEY::D:	Attack = Attack|Cha_State::DATK; break;
		}
		return Attack;
	}

	void Lock(UINT type){lock = type;}
	bool IsLocked(){return (lock != 0x00);}

	Cha_State GetState(){return state;}
	Direction CurrentDirection(){return cur;}

	void run(){
		system("CLS");
		std::cout<< "State : ";
		if(state == Cha_State::STAND){std::cout<<"STAND";}
		else{
			switch(state&0x03){
			case Cha_State::WALK:	std::cout<<"WALK";	break;
			case Cha_State::DASH:	std::cout<<"DASH";	break;
			}
			switch(state&0x0C){
			case Cha_State::JUMP:	std::cout<<"JUMP";	break;
			case Cha_State::DJUMP:	std::cout<<"DJUMP";	break;
			case Cha_State::SIT:	std::cout<<"SIT";	break;
			}
		}

		switch(state&0xF0){
		case Cha_State::AATK:	std::cout<<"AATK";	break;
		case Cha_State::BATK:	std::cout<<"BATK";	break;
		case Cha_State::CATK:	std::cout<<"CATK";	break;
		case Cha_State::DATK:	std::cout<<"DATK";	break;
		}
		std::cout<<std::endl;

		std::cout<< "Direction : ";
		switch(cur){
		case Direction::CENTER:	std::cout<<"□"<<std::endl;	break;
		case Direction::LEFT:	std::cout<<"←"<<std::endl;	break;
		case Direction::RIGHT:	std::cout<<"→"<<std::endl;	break;
		case Direction::UP:		std::cout<<"↑"<<std::endl;	break;
		case Direction::DOWN:	std::cout<<"↓"<<std::endl;	break;
		case (Direction::LEFT | Direction::UP):	std::cout<<"↖"<<std::endl;	break;
		case (Direction::LEFT | Direction::DOWN):	std::cout<<"↙"<<std::endl;	break;
		case (Direction::RIGHT | Direction::UP):	std::cout<<"↗"<<std::endl;	break;
		case (Direction::RIGHT | Direction::DOWN):	std::cout<<"↘"<<std::endl;	break;
		default:	std::cout<<std::endl;	break;
		}

		std::list<_node>::iterator iter = command_queue.begin();
		while(iter != command_queue.end()){
			switch((*iter).key){
			case GAME_KEY::A:	std::cout<<"A";	break;
			case GAME_KEY::B:	std::cout<<"B";	break;
			case GAME_KEY::C:	std::cout<<"C";	break;
			case GAME_KEY::D:	std::cout<<"D";	break;
			case GAME_KEY::DL:	std::cout<<"←";	break;
			case GAME_KEY::DR:	std::cout<<"→";	break;
			case GAME_KEY::DU:	std::cout<<"↑";	break;
			case GAME_KEY::DD:	std::cout<<"↓";	break;
				//case GAME_KEY::DLU:	std::cout<<"↖";	break;
				//case GAME_KEY::DLD:	std::cout<<"↙";	break;
				//case GAME_KEY::DRU:	std::cout<<"↗";	break;
				//case GAME_KEY::DRD:	std::cout<<"↘";	break;
			default:
			{
				if((GAME_KEY::DL | GAME_KEY::DU) == (*iter).key){std::cout<<"↖";}
				if((GAME_KEY::DL | GAME_KEY::DD) == (*iter).key){std::cout<<"↙";}
				if((GAME_KEY::DR | GAME_KEY::DU) == (*iter).key){std::cout<<"↗";}
				if((GAME_KEY::DR | GAME_KEY::DD) == (*iter).key){std::cout<<"↘";}
			}
			break;
			}
		}
	}
};
