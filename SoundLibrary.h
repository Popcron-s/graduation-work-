#pragma once

#include "Singleton.h"
#include "variable.h"

#include "Engine_Win32.h"
#include<iostream>
enum class eSound : UINT{
	WALK = 0,
	DAMAGED,
	MAX
};

class SoundLib : public Singleton<SoundLib>{
private:
	SOUND arr[(UINT)eSound::MAX];

public:
	SoundLib(){
		LoadWAV("°È±â.wav", arr[(UINT)eSound::WALK]);
		LoadWAV("º£±â1.wav", arr[(UINT)eSound::DAMAGED]);
	}
	~SoundLib(){
		for(UINT i = 0 ; i<(UINT)eSound::MAX ; ++i){
			delete arr[i].buf;
		}
	}

	void Walk(){RegistSound(arr[(UINT)eSound::WALK]);}
	void Damaged(){RegistSound(arr[(UINT)eSound::DAMAGED]);}
};