#pragma once

#ifdef SCPDICPP_EXPORTS
#define SCPDICPP_API __declspec(dllexport)
#else
#define SCPDICPP_API __declspec(dllimport)
#endif

#include "X360Controller.h"

namespace ScpdiCpp
{
class SCPDICPP_API ScpBus
{
public:
	virtual bool PlugIn(int controllerNumber) = 0;
	virtual bool Unplug(int controllerNumber) = 0;
	virtual bool UnplugAll() = 0;
	virtual bool Report(int controllerNumber, X360Controller* controller) = 0;

public:
	virtual ~ScpBus() {}
};
}

