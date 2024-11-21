#include "Forms/Forms.h"
#include "Scripts/Papyrus.h"

namespace
{
	void MessageCallback(F4SE::MessagingInterface::Message* a_msg)
	{
		switch (a_msg->type)
		{
		case F4SE::MessagingInterface::kPostLoad:
		{
			Forms::Install();
			break;
		}
		default:
			break;
		}
	}
}

F4SEPluginLoad(const F4SE::LoadInterface* a_F4SE)
{
	F4SE::Init(a_F4SE);

	F4SE::GetMessagingInterface()->RegisterListener(MessageCallback);
	F4SE::GetPapyrusInterface()->Register(Papyrus::RegisterFunctions);

	return true;
}
