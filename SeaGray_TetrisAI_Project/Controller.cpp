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
		// ���ɐڑ����Ă���
		return true;
	}

	// �ڑ�����������܂Œ��킷��
	// ���s�͂������ɂ��� controllerNumber �͐ڑ�����Ă��邱�Ƃ�\��
	// ������� 10 ��܂�
	for (int i = 1; i <= 10; ++i)
	{
		if (m_scpBus->PlugIn(i))
		{
			// �ڑ��ł���
			m_controllerNumber = i;
			return true;
		}
	}

	// �ڑ��ł��Ȃ�����
	return false;
}
bool Controller::Unplug()
{
	if (m_controllerNumber == 0)
	{
		// ���ɐؒf���Ă���
		return true;
	}

	if (m_scpBus->Unplug(m_controllerNumber))
	{
		// �ؒf�ł���
		m_controllerNumber = 0;
		return true;
	}

	// �ؒf�ł��Ȃ�����
	// �R���g���[���͊��ɑ��̃v���Z�X�ɐؒf����Ă����Ɖ��肵��
	// m_controllerNumber �� 0 ��������
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
		// �O��Ɠ����{�^���������ꍇ�ω����Ȃ��̂�
		// ���ʂɏ��������Ȃ��悤�ɂ���
		return;
	}

	// �{�^���̏�Ԃ�ݒ�
	// ��NONE�͑S�{�^���𗣂����Ƃ�\��
	m_x360Controller->SetButtons(x360Buttons);
	m_scpBus->Report(m_controllerNumber, m_x360Controller);

	// ���񉟂����{�^�����o���Ă���
	m_prevX360Buttons = x360Buttons;
}
void Controller::PressButton(PPT2Sync::Button button)
{
	// Command �� X360Buttons �ɕϊ�
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
