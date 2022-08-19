#pragma once

#include "Forms/Forms.h"

namespace WaitAnywhere
{
	namespace
	{
		inline REL::Relocation<RE::SettingT<RE::GameSettingCollection>*> sNoWaitDefault{ REL::ID(1461952) };
		inline REL::Relocation<RE::SettingT<RE::GameSettingCollection>*> sNoWaitTrespass{ REL::ID(537900) };
		inline REL::Relocation<RE::SettingT<RE::GameSettingCollection>*> sNoWaitWarnToLeave{ REL::ID(1195909) };
		inline REL::Relocation<RE::SettingT<RE::GameSettingCollection>*> sNoWaitHostileActorsNear{ REL::ID(271800) };
		inline REL::Relocation<RE::SettingT<RE::GameSettingCollection>*> sNoWaitInAir{ REL::ID(929759) };
		inline REL::Relocation<RE::SettingT<RE::GameSettingCollection>*> sNoWaitInCell{ REL::ID(5072) };
		inline REL::Relocation<RE::SettingT<RE::GameSettingCollection>*> sNoWaitTakingHealthDamage{ REL::ID(664041) };
		inline REL::Relocation<RE::SettingT<RE::GameSettingCollection>*> sNoWaitTakingRadDamage{ REL::ID(1321782) };
		inline REL::Relocation<RE::SettingT<RE::GameSettingCollection>*> sNoWaitWhileAlarm{ REL::ID(397715) };

		bool DFOBtoGLOB(RE::BGSDefaultObject* a_dfob)
		{
			if (a_dfob)
			{
				if (auto glob = a_dfob->GetForm<RE::TESGlobal>(); glob)
				{
					return glob->GetValue();
				}
			}

			return false;
		}

		bool GetBlockingMenuOpen(std::vector<std::string> a_menus)
		{
			if (auto UI = RE::UI::GetSingleton(); UI)
			{
				if (UI->menuMode > 0)
				{
					return true;
				}

				for (auto menu : a_menus)
				{
					if (UI->GetMenuOpen(menu))
					{
						return true;
					}
				}
			}

			return false;
		};
	}

	bool CanPassTime()
	{
		auto PlayerCharacter = RE::PlayerCharacter::GetSingleton();
		auto ProcessLists = RE::ProcessLists::GetSingleton();
		auto SynchedAnimManager = RE::BGSSynchronizedAnimationManager::GetSingleton();

		if (!PlayerCharacter || !ProcessLists || !SynchedAnimManager)
		{
			logger::warn("WaitAnywhere::CanPassTime: Failed to GetSingleton."sv);
			RE::SendHUDMessage::ShowHUDMessage(
				sNoWaitDefault->GetString().data(),
				"UIMenuCancel",
				true,
				true);
			return false;
		}

		if (GetBlockingMenuOpen(
				{ "CookingMenu",
				  "DialogueMenu",
				  "ExamineMenu",
				  "FavoritesMenu",
				  "LooksMenu",
				  "PowerArmorModMenu",
				  "RobotModMenu",
				  "VATSMenu",
				  "WorkshopMenu" }))
		{
			/*
			RE::SendHUDMessage::ShowHUDMessage(
				sNoWaitDefault->GetString().data(),
				"UIMenuCancel",
				true,
				true);
			*/
			return false;
		}

		if (PlayerCharacter->GetParentCell()->GetCantWaitHere())
		{
			RE::SendHUDMessage::ShowHUDMessage(
				sNoWaitInCell->GetString().data(),
				"UIMenuCancel",
				true,
				true);
			return false;
		}

		if (!DFOBtoGLOB(Forms::BWA_bOverrideTrespassing_DO))
		{
			if (PlayerCharacter->boolFlags.any(RE::Actor::BOOL_FLAGS::kIsTresspassing))
			{
				RE::SendHUDMessage::ShowHUDMessage(
					sNoWaitTrespass->GetString().data(),
					"UIMenuCancel",
					true,
					true);
				return false;
			}

			if (PlayerCharacter->GetParentCell()->cellFlags.any(RE::TESObjectCELL::Flag::kWarnToLeave))
			{
				RE::SendHUDMessage::ShowHUDMessage(
					sNoWaitWarnToLeave->GetString().data(),
					"UIMenuCancel",
					true,
					true);
				return false;
			}
		}

		if (!DFOBtoGLOB(Forms::BWA_bOverrideInAir_DO))
		{
			if (PlayerCharacter->IsJumping())
			{
				RE::SendHUDMessage::ShowHUDMessage(
					sNoWaitInAir->GetString().data(),
					"UIMenuCancel",
					true,
					true);
				return false;
			}
		}

		if (!DFOBtoGLOB(Forms::BWA_bOverrideInCombat_DO))
		{
			if (ProcessLists->IsActorTargetingREFinPackage(PlayerCharacter, RE::PTYPE::kAlarm, false))
			{
				RE::SendHUDMessage::ShowHUDMessage(
					sNoWaitWhileAlarm->GetString().data(),
					"UIMenuCancel",
					true,
					true);
				return false;
			}

			if (ProcessLists->AreHostileActorsNear(nullptr))
			{
				RE::SendHUDMessage::ShowHUDMessage(
					sNoWaitHostileActorsNear->GetString().data(),
					"UIMenuCancel",
					true,
					true);
				return false;
			}
		}

		if (!DFOBtoGLOB(Forms::BWA_bOverrideTakingHealthDamage_DO))
		{
			if (PlayerCharacter->IsTakingHealthDamageFromActiveEffect())
			{
				RE::SendHUDMessage::ShowHUDMessage(
					sNoWaitTakingHealthDamage->GetString().data(),
					"UIMenuCancel",
					true,
					true);
				return false;
			}
		}

		if (!DFOBtoGLOB(Forms::BWA_bOverrideTakingRadDamage_DO))
		{
			if (PlayerCharacter->IsTakingRadDamageFromActiveEffect())
			{
				RE::SendHUDMessage::ShowHUDMessage(
					sNoWaitTakingRadDamage->GetString().data(),
					"UIMenuCancel",
					true,
					true);
				return false;
			}
		}

		if (SynchedAnimManager->IsReferenceInSynchronizedScene(PlayerCharacter->GetHandle()))
		{
			RE::SendHUDMessage::ShowHUDMessage(
				sNoWaitDefault->GetString().data(),
				"UIMenuCancel",
				true,
				true);
			return false;
		}

		return true;
	}
}

namespace Papyrus::BakaWaitAnywhere
{
	void Wait(std::monostate)
	{
		if (WaitAnywhere::DFOBtoGLOB(Forms::BWA_bEnabled_DO) && WaitAnywhere::CanPassTime())
		{
			auto UIMessageQueue = RE::UIMessageQueue::GetSingleton();
			if (UIMessageQueue)
			{
				UIMessageQueue->AddMessage(
					"SleepWaitMenu"sv,
					RE::UI_MESSAGE_TYPE::kShow);
			}
		}
	}
}
