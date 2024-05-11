#pragma once

#include "Papyrus/BakaWaitAnywhere.h"

namespace Papyrus
{
	bool RegisterFunctions(RE::BSScript::IVirtualMachine* a_VM)
	{
		a_VM->BindNativeMethod("BakaWaitAnywhere", "Wait", BakaWaitAnywhere::Wait, true);
		return true;
	}
}
