#include "Bot.h"
#include "Controller.h"
#include "PPT2Sync.h"

#include <thread>
#include <Windows.h>

namespace
{
bool threadRunning = false;

Controller controller;

bool takingOverStart;
int framePrev;
int playerIndex;
bool unplugAllPrev;

constexpr int OPERATION_KIND = 3;
constexpr int OPERATION_LENGTH = 4;
constexpr PPT2Sync::Command OPERATION[OPERATION_KIND][OPERATION_LENGTH] =
{
	{ PPT2Sync::Command::Left, PPT2Sync::Command::Left, PPT2Sync::Command::Left, PPT2Sync::Command::Hard },
	{ PPT2Sync::Command::Right, PPT2Sync::Command::Right, PPT2Sync::Command::Right, PPT2Sync::Command::Hard },
	{ PPT2Sync::Command::Clockwise, PPT2Sync::Command::None, PPT2Sync::Command::Clockwise, PPT2Sync::Command::Hard }
};
int operationIndex;

void MemorizeAddress()
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
}

bool Initialize() {

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

	PPT2Sync::SetSpeed(100);
	PPT2Sync::SetCharacter(0);

	return true;
}

void Start() {
	threadRunning = true;
	while (threadRunning)
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
			// 配布版はオンラインでは動かさない
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
					int field[10][40];
					PPT2Sync::PPT2MemoryReader::GetField(field);
					PPT2Sync::PPT2MemoryReader::Current current = PPT2Sync::PPT2MemoryReader::GetCurrentPiece();
					PPT2Sync::AdjustCurrent(field, current);

					PPT2Sync::Button b = PPT2Sync::StartOperation(OPERATION[operationIndex], OPERATION_LENGTH);
					++operationIndex;
					if (operationIndex >= OPERATION_KIND)
					{
						operationIndex = 0;
					}

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
}
void Stop() {
	threadRunning = false;
}
void Destroy() {
	controller.Unplug();
	PPT2Sync::PPT2MemoryReader::Destroy();
}