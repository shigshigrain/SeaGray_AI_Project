#pragma once

#ifdef SCPDICPP_EXPORTS
#define SCPDICPP_API __declspec(dllexport)
#else
#define SCPDICPP_API __declspec(dllimport)
#endif

#include "X360Buttons.h"

namespace ScpdiCpp
{
class SCPDICPP_API X360Controller
{
public:
	virtual X360Buttons GetButtons() const = 0;
	virtual void SetButtons(X360Buttons buttons) = 0;

	virtual unsigned char GetLeftTrigger() const = 0;
	virtual void SetLeftTrigger(unsigned char value) = 0;

	virtual unsigned char GetRightTrigger() const = 0;
	virtual void SetRightTrigger(unsigned char value) = 0;

	virtual short GetLeftStickX() const = 0;
	virtual void SetLeftStickX(short value) = 0;

	virtual short GetLeftStickY() const = 0;
	virtual void SetLeftStickY(short value) = 0;

	virtual short GetRightStickX() const = 0;
	virtual void SetRightStickX(short value) = 0;

	virtual short GetRightStickY() const = 0;
	virtual void SetRightStickY(short value) = 0;

public:
	virtual ~X360Controller() {}
};
}