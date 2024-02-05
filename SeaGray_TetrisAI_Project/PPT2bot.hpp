#pragma once

#include "stdafx.h"

#include "Controller.h"
#include "PPT2Sync.h"


namespace shig {

	class PPT2bot
	{
	private:
		bool threadRunning; // = false;
		bool takingOverStart;
		bool unplugAllPrev;
		int framePrev;
		int playerIndex;
		int operationIndex;
		int botSpeed;
		int selectCharacter;
		Controller controller;


	public:
		PPT2bot();
		~PPT2bot();

	private:
		PPT2bot(const PPT2bot&) = delete;
		PPT2bot& operator=(const PPT2bot&) = delete;
		PPT2bot(PPT2bot&&) = delete;
		PPT2bot& operator=(PPT2bot&&) = delete;

	public:
		bool SetPlayerIndex(int pi);
		void MemorizeAddress();
		bool Initialize();
		bool Running();
		bool Running(const std::atomic_bool& abort, std::atomic_int& index);
		bool Stop();
		bool Stop(std::atomic_bool& abort);
		void Destroy();



	};


}



