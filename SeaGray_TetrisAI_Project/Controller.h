// １インスタンスにつき仮想コントローラ１つ分を表すクラス
//
#pragma once

#include "PPT2Sync.h"
#include "ScpdiCpp.h"

class Controller
{
public:
	Controller();
	~Controller();

private:
	Controller(const Controller&) = delete;
	Controller& operator=(const Controller&) = delete;
	Controller(Controller&&) = delete;
	Controller& operator=(Controller&&) = delete;

public:
	bool PlugIn();
	bool Unplug();
	void UnplugAll();
	void PressButton(ScpdiCpp::X360Buttons x360Buttons);
	void PressButton(PPT2Sync::Button button);

	bool IsPressing() const { return m_prevX360Buttons != ScpdiCpp::X360Buttons::NONE; }

private:
	ScpdiCpp::ScpBus* m_scpBus;
	ScpdiCpp::X360Controller* m_x360Controller;
	int m_controllerNumber;
	ScpdiCpp::X360Buttons m_prevX360Buttons;
};
