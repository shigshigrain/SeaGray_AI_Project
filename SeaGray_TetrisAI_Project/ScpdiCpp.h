#pragma once

#ifdef SCPDICPP_EXPORTS
#define SCPDICPP_API __declspec(dllexport)
#else
#define SCPDICPP_API __declspec(dllimport)
#endif

#include "ScpBus.h"

namespace ScpdiCpp
{
extern "C" SCPDICPP_API ScpBus* CreateScpBus();
extern "C" SCPDICPP_API void DeleteScpBus(ScpBus* scpBus);

extern "C" SCPDICPP_API X360Controller* CreateX360Controller();
extern "C" SCPDICPP_API void DeleteX360Controller(X360Controller* controller);
}

