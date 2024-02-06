
#include "PPT2bot.hpp"

constexpr int OPERATION_KIND = 3;
constexpr int OPERATION_LENGTH = 4;
constexpr PPT2Sync::Command OPERATION[OPERATION_KIND][OPERATION_LENGTH] =
{
	{ PPT2Sync::Command::Left, PPT2Sync::Command::Left, PPT2Sync::Command::Left, PPT2Sync::Command::Hard },
	{ PPT2Sync::Command::Right, PPT2Sync::Command::Right, PPT2Sync::Command::Right, PPT2Sync::Command::Hard },
	{ PPT2Sync::Command::Clockwise, PPT2Sync::Command::None, PPT2Sync::Command::Clockwise, PPT2Sync::Command::Hard }
};

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
		selectCharacter = 0;
		//controller;
		GraySea = std::make_unique<shig::AiShigune>(1);
		GraySea->load_ttrp();
	}

	PPT2bot::~PPT2bot()
	{
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
		playerIndex = 0;
		unplugAllPrev = false;
		operationIndex = 0;

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

			if (PPT2Sync::PPT2MemoryReader::IsOnline())
			{
				// 配布版はオンラインでは動かさない<<厳守>>
				std::this_thread::sleep_for(std::chrono::milliseconds(30));
				continue;
			}

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
						
						// Ai側Thinking;

						GraySea->thinking(); // 思考に時間が掛かる　別スレッドで動かさずに同期
						std::unique_ptr<PPT2Sync::Command[]> nowOperate;
						int opr_size = TranscribeCommand(nowOperate, GraySea->getCmdList());

						PPT2Sync::Button b = PPT2Sync::StartOperation(nowOperate.get(), opr_size);
						++operationIndex;
						
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
				Sleep(3);
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

	int PPT2bot::TranscribeCommand(std::unique_ptr<PPT2Sync::Command[]>& opr, const std::vector<int>& cmdl)
	{
		const size_t cmd_size = cmdl.size();
		opr = make_unique<PPT2Sync::Command[]>(cmd_size);

		for (size_t i = 0; i < cmd_size; i++) {

			switch (cmdl.at(i))
			{
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
