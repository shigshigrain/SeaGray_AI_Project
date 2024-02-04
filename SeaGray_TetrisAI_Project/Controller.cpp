#include "Controller.h"

Controller::Controller()
	: m_scpBus(ScpdiCpp::CreateScpBus())
	, m_x360Controller(ScpdiCpp::CreateX360Controller())
	, m_controllerNumber()
	, m_prevX360Buttons(ScpdiCpp::X360Buttons::NONE)
{
}
Controller::~Controller()
{
	ScpdiCpp::DeleteX360Controller(m_x360Controller);
	ScpdiCpp::DeleteScpBus(m_scpBus);
}
bool Controller::PlugIn()
{
	if (m_controllerNumber != 0)
	{
		// 既に接続している
		return true;
	}

	// 接続が成功するまで挑戦する
	// 失敗はもう既にその controllerNumber は接続されていることを表す
	// ※挑戦回数 10 回まで
	for (int i = 1; i <= 10; ++i)
	{
		if (m_scpBus->PlugIn(i))
		{
			// 接続できた
			m_controllerNumber = i;
			return true;
		}
	}

	// 接続できなかった
	return false;
}
bool Controller::Unplug()
{
	if (m_controllerNumber == 0)
	{
		// 既に切断している
		return true;
	}

	if (m_scpBus->Unplug(m_controllerNumber))
	{
		// 切断できた
		m_controllerNumber = 0;
		return true;
	}

	// 切断できなかった
	// コントローラは既に他のプロセスに切断されていたと仮定して
	// m_controllerNumber に 0 を代入する
	m_controllerNumber = 0;
	return false;
}
void Controller::UnplugAll()
{
	m_scpBus->UnplugAll();
	m_controllerNumber = 0;
}
void Controller::PressButton(ScpdiCpp::X360Buttons x360Buttons)
{
	if (x360Buttons == m_prevX360Buttons)
	{
		// 前回と同じボタンだった場合変化がないので
		// 無駄に処理をしないようにする
		return;
	}

	// ボタンの状態を設定
	// ※NONEは全ボタンを離すことを表す
	m_x360Controller->SetButtons(x360Buttons);
	m_scpBus->Report(m_controllerNumber, m_x360Controller);

	// 今回押したボタンを覚えておく
	m_prevX360Buttons = x360Buttons;
}
void Controller::PressButton(PPT2Sync::Button button)
{
	// Command を X360Buttons に変換
	ScpdiCpp::X360Buttons x360Buttons = ScpdiCpp::X360Buttons::NONE;
	switch (button)
	{
	case PPT2Sync::Button::LeftBumper:
		x360Buttons = ScpdiCpp::X360Buttons::LFET_BUMPER;
		break;
	case PPT2Sync::Button::Left:
		x360Buttons = ScpdiCpp::X360Buttons::LEFT;
		break;
	case PPT2Sync::Button::Right:
		x360Buttons = ScpdiCpp::X360Buttons::RIGHT;
		break;
	case PPT2Sync::Button::B:
		x360Buttons = ScpdiCpp::X360Buttons::B;
		break;
	case PPT2Sync::Button::A:
		x360Buttons = ScpdiCpp::X360Buttons::A;
		break;
	case PPT2Sync::Button::Down:
		x360Buttons = ScpdiCpp::X360Buttons::DOWN;
		break;
	case PPT2Sync::Button::Up:
		x360Buttons = ScpdiCpp::X360Buttons::UP;
		break;
	}

	PressButton(x360Buttons);
}
