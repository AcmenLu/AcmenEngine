#pragma once

#include "Interface.h"
#include "IRuntimeModule.h"

namespace Acmen
{
	Interface IApplication : implements IRuntimeModule
	{
	public:
		virtual int Initialize() = 0;
		virtual void Finalize() = 0;
		virtual void Tick() = 0;
		virtual bool IsQuit() = 0;
	};
}
