#pragma once

#include "stdafx.h"

#include "Controller.h"
#include "PPT2Sync.h"
#include "shigune_AI.h"

namespace shig {

	class PPT2bot
	{
	private:
		atomic_bool threadRunning; // std::thread Thinkerの制御用;
		bool takingOverStart;
		bool unplugAllPrev;
		int framePrev;
		int playerIndex;
		int operationIndex;
		int botSpeed;
		int selectCharacter;
		std::thread Thinker;
		Controller controller;
		std::unique_ptr<shig::AiShigune> GraySea;

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
		bool Read2Ai();
		int TranscribeCommand(std::unique_ptr<PPT2Sync::Command[]>& opr, const std::vector<int8_t>& cmdl);
		std::deque<int> make_d_next(const PPT2Sync::PPT2MemoryReader::Pieces& _rnext);
		std::vector<int8_t> AdjustCommand(const std::vector<int8_t>& cmd);
		bool Stop();
		bool Stop(std::atomic_bool& abort);
		void Destroy();
	};

	bool ExecuteRun(shig::PPT2bot& pb, const std::atomic_bool& abort, std::atomic_int& index);


}



