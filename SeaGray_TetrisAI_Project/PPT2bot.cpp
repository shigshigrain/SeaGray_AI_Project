﻿
#include "PPT2bot.hpp"

constexpr PPT2Sync::Command OPERATION[1][6] = {
	{PPT2Sync::Command::SoftEdge, PPT2Sync::Command::LeftEdge, PPT2Sync::Command::RightEdge, PPT2Sync::Command::LeftEdge, PPT2Sync::Command::RightEdge, PPT2Sync::Command::Hard },
};

constexpr PPT2Sync::Command skip_opr[1] = {	PPT2Sync::Command::None };

namespace shig {

	PPT2bot::PPT2bot()
	{
		threadRunning = false;
		takingOverStart = false;
		unplugAllPrev = false;
		framePrev = 0;
		playerIndex = -1;
		operationIndex = -1;
		botSpeed = 100;
		selectCharacter = 10;
		// Thinker は初期化しない
		//controller;
		GraySea = std::make_unique<shig::AiShigune>(1);
		
	}

	PPT2bot::~PPT2bot()
	{

		if (Thinker.joinable()) {
			threadRunning = false;
			Thinker.join();
		}

	}

	bool PPT2bot::SetPlayerIndex(int pi)
	{
		playerIndex = pi;
		return pi < 0;
	}

	void PPT2bot::MemorizeAddress()
	{
		if (playerIndex == -1)
		{
			// オンラインのとき
			PPT2Sync::PPT2MemoryReader::MemorizeAddress(PPT2Sync::PPT2MemoryReader::FindPlayer());
		}
		else
		{
			// オフラインで自分で決めたプレイヤーで動かしたいとき
			PPT2Sync::PPT2MemoryReader::MemorizeAddress(playerIndex);
		}
	}

	bool PPT2bot::Initialize()
	{
		if (!PPT2Sync::PPT2MemoryReader::FindProcessHandle())
		{
			return false;
		}

		controller.PlugIn();

		takingOverStart = true;
		framePrev = -1;
		playerIndex = -1;
		unplugAllPrev = false;
		operationIndex = 0;

		GraySea->loadTTRP();

		PPT2Sync::SetSpeed(botSpeed);
		PPT2Sync::SetCharacter(selectCharacter);

		return true;

	}

	bool PPT2bot::Running()
	{
		return false;
	}

	bool PPT2bot::Running(const std::atomic_bool& abort, std::atomic_int& index)
	{

		//if (!abort)return false; // trueの状態でループに入ることを想定

		//std::unique_ptr<std::ofstream> fieldDump = std::make_unique<std::ofstream>("dumpField.csv");

		while (abort)
		{
			if (GetAsyncKeyState('1' + playerIndex)) {
				if (takingOverStart) {
					takingOverStart = false;
					// dummy reading
					GetAsyncKeyState(VK_LEFT);
					GetAsyncKeyState(VK_RIGHT);
					GetAsyncKeyState(VK_UP);
					GetAsyncKeyState(VK_DOWN);
					GetAsyncKeyState(VK_RETURN);
					GetAsyncKeyState(VK_BACK);
					GetAsyncKeyState('P');
					GetAsyncKeyState('L');
					GetAsyncKeyState('U');
				}
				else {
					ScpdiCpp::X360Buttons buttons = ScpdiCpp::X360Buttons::NONE;
					if (GetAsyncKeyState(VK_LEFT)) buttons |= ScpdiCpp::X360Buttons::LEFT;
					if (GetAsyncKeyState(VK_RIGHT)) buttons |= ScpdiCpp::X360Buttons::RIGHT;
					if (GetAsyncKeyState(VK_UP)) buttons |= ScpdiCpp::X360Buttons::UP;
					if (GetAsyncKeyState(VK_DOWN)) buttons |= ScpdiCpp::X360Buttons::DOWN;
					if (GetAsyncKeyState(VK_RETURN)) buttons |= ScpdiCpp::X360Buttons::A;
					if (GetAsyncKeyState(VK_BACK)) buttons |= ScpdiCpp::X360Buttons::B;
					if (GetAsyncKeyState('P')) buttons |= ScpdiCpp::X360Buttons::START;
					if (GetAsyncKeyState('L')) buttons |= ScpdiCpp::X360Buttons::LFET_BUMPER;

					controller.PressButton(buttons);

					if (GetAsyncKeyState('U'))
					{
						if (unplugAllPrev == false)
						{
							unplugAllPrev = true;
							controller.UnplugAll();
						}
					}
					else
					{
						unplugAllPrev = false;
					}
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(30));
				continue;
			}
			else {
				takingOverStart = true;
			}

			//if (PPT2Sync::PPT2MemoryReader::IsOnline())
			//{
			//	// 配布版はオンラインでは動かさない
			//	std::this_thread::sleep_for(std::chrono::milliseconds(30));
			//	continue;
			//}

			if (PPT2Sync::PPT2MemoryReader::IsMatch())
			{
				MemorizeAddress();
				int frame = PPT2Sync::PPT2MemoryReader::GetFrameCount();
				if (frame != framePrev || PPT2Sync::PPT2MemoryReader::IsOpening())
				{
					framePrev = frame;

					PPT2Sync::Event e = PPT2Sync::Sync();
					switch (e)
					{
					case PPT2Sync::Event::StartOperation:
					{
						// PPT2側Reader;
						int field[10][40];
						PPT2Sync::PPT2MemoryReader::GetField(field);
						PPT2Sync::PPT2MemoryReader::Current current = PPT2Sync::PPT2MemoryReader::GetCurrentPiece();
						PPT2Sync::AdjustCurrent(field, current);
						PPT2Sync::PPT2MemoryReader::ComboB2B nowCB2B = PPT2Sync::PPT2MemoryReader::GetComboB2B();
						//PPT2Sync::PPT2MemoryReader::Pieces Rnext = PPT2Sync::PPT2MemoryReader::GetPieces();

						if (current.type == -1) {
							PPT2Sync::Button b = PPT2Sync::StartOperation(skip_opr, 1);

							//++operationIndex;
							index = operationIndex;
					
							controller.PressButton(b);
							break;
						}

						// Ai側Reader;
						GraySea->ReadCurrent(current.type);
						GraySea->ReadHold(PPT2Sync::PPT2MemoryReader::GetHold());
						GraySea->ReadNext(make_d_next(PPT2Sync::PPT2MemoryReader::GetPieces()));
						GraySea->ReadCB2B(nowCB2B.combo, nowCB2B.b2b);
						for (int j = 0; j < 10; j++) {
							for (int i = 0; i < 40; i++) {
								GraySea->ReadField(i, j, field[j][i]);
							}
						}

						// field Dump
						/**fieldDump << "-----< index = " << operationIndex << " >-----" << "\n";
						for (int i = 0; i < 40; i++) {
							for (int j = 0; j < 10; j++) {
								*fieldDump << field[j][i] << ",";
							}
							*fieldDump << "\n";
						}*/

						
						// Ai側Thinking;
						GraySea->thinking(); // 思考に時間が掛かる　別スレッドで動かさずに同期

						/*std::thread thk(GraySea->thinking(), this);
						thk.join()*/;

						std::unique_ptr<PPT2Sync::Command[]> nowOperate;
						int opr_size = TranscribeCommand(nowOperate, AdjustCommand(GraySea->getCmdList()));

						PPT2Sync::Button b = PPT2Sync::StartOperation(nowOperate.get(), opr_size);



						++operationIndex;
						index = operationIndex;
						/*if (operationIndex >= OPERATION_KIND)
						{
							operationIndex = 0;
						}*/

						controller.PressButton(b);
					}
					break;
					case PPT2Sync::Event::Operate:
					{
						PPT2Sync::Button b;
						if (PPT2Sync::Operate(b))
						{
							controller.PressButton(b);
						}
						else
						{
							//PPT2Sync::AdjustCurrent();
							//オペレーション生成し直し
							//b = PPT2Sync::StartOperation();
							//controller.PressButton(b);
						}
					}
					break;
					case PPT2Sync::Event::FinishOperation:
					{
						controller.PressButton(ScpdiCpp::X360Buttons::NONE);
					}
					break;
					}
				}

				// しっかり同期するためにはCPUの休憩時間は1～3msがお勧め
				Sleep(2);
			}
			else
			{
				PPT2Sync::Button b = PPT2Sync::SelectCharacter();
				controller.PressButton(b);

				// キャラクタ選択画面ではフレームカウンタが使えないので適当に１フレーム以上は経過してそうなくらい休む
				std::this_thread::sleep_for(std::chrono::milliseconds(30));
			}
		}

		return true;
	}

	bool PPT2bot::Read2Ai()
	{
		return false;
	}

	int PPT2bot::TranscribeCommand(std::unique_ptr<PPT2Sync::Command[]>& opr, const std::vector<int8_t>& cmdl)
	{
		const size_t cmd_size = cmdl.size();
		opr = make_unique<PPT2Sync::Command[]>(cmd_size);

		for (size_t i = 0; i < cmd_size; i++) {

			switch (cmdl.at(i))
			{
			case 0:
				opr[i] = PPT2Sync::Command::None;
				break;
			case 1:
				opr[i] = PPT2Sync::Command::Hold;
				break;
			case 2:
				opr[i] = PPT2Sync::Command::Soft;
				break;
			case 3:
				opr[i] = PPT2Sync::Command::Hard;
				break;
			case 4:
				opr[i] = PPT2Sync::Command::Counterclockwise;
				break;
			case 5:
				opr[i] = PPT2Sync::Command::Clockwise;
				break;
			case 6:
				opr[i] = PPT2Sync::Command::Left;
				break;
			case 7:
				opr[i] = PPT2Sync::Command::Right;
				break;
			default:
				opr[i] = PPT2Sync::Command::None;
				break;
			}
		}
		return (int)cmd_size;
	}

	std::deque<int> PPT2bot::make_d_next(const PPT2Sync::PPT2MemoryReader::Pieces& _rnext)
	{
		std::deque<int> _dnext = { _rnext.p1, _rnext.p2 , _rnext.p3 , _rnext.p4 , _rnext.p5 };
		return _dnext;
	}

	std::vector<int8_t> PPT2bot::AdjustCommand(const std::vector<int8_t>& cmd)
	{
		std::vector<int8_t> adcmd = {};
		int prev_cmd = 0;
		for (auto&& _c : cmd) {
			/*if ((_c == 6 || _c == 7) && (prev_cmd == 4 || prev_cmd == 5)) {
				adcmd.push_back(0);
			}*/
			if (prev_cmd == 6 && _c == 6) {
				adcmd.push_back(0);
				adcmd.push_back(_c);
			}
			else if (prev_cmd == 7 && _c == 7) {
				adcmd.push_back(0);
				adcmd.push_back(_c);
			}
			else if (prev_cmd == 5 && _c == 5) {
				//adcmd.push_back(0);
				adcmd.push_back(_c);
			}
			else {
				adcmd.push_back(_c);
			}

			prev_cmd = _c;

		}

		return adcmd;
	}

	bool PPT2bot::Stop()
	{
		threadRunning = false;
		return false;
	}

	bool PPT2bot::Stop(std::atomic_bool& abort)
	{
		abort = false;

		return false;
	}

	void PPT2bot::Destroy()
	{
		controller.Unplug();
		PPT2Sync::PPT2MemoryReader::Destroy();
	}


	bool ExecuteRun(shig::PPT2bot& pb, const std::atomic_bool& abort, std::atomic_int& index)
	{
		pb.Running(abort, index);

		return true;
	}

}

/*
int main(void)
{
	if (!Initialize())
	{
		std::cout << "PPT2が起動していない" << std::endl;
		return 0;
	}

	std::thread th(&Start);

	while (true)
	{
		std::string s;
		std::cout << "終了するには「z」を入力 > " << std::endl;
		std::cin >> s;
		if (s == "z")
		{
			Stop();
			th.join();
			break;
		}
	}

	Destroy();
	return 0;
}

*/
