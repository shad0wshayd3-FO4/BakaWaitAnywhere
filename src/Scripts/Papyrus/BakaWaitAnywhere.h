#pragma once

#include "Forms/Forms.h"

namespace WaitAnywhere
{
	namespace
	{
		inline static REL::Relocation<RE::SettingT<RE::GameSettingCollection>*> sNoWaitDefault{ REL::ID(1461952) };
		inline static REL::Relocation<RE::SettingT<RE::GameSettingCollection>*> sNoWaitTrespass{ REL::ID(537900) };
		inline static REL::Relocation<RE::SettingT<RE::GameSettingCollection>*> sNoWaitWarnToLeave{ REL::ID(1195909) };
		inline static REL::Relocation<RE::SettingT<RE::GameSettingCollection>*> sNoWaitHostileActorsNear{ REL::ID(271800) };
		inline static REL::Relocation<RE::SettingT<RE::GameSettingCollection>*> sNoWaitInAir{ REL::ID(929759) };
		inline static REL::Relocation<RE::SettingT<RE::GameSettingCollection>*> sNoWaitInCell{ REL::ID(5072) };
		inline static REL::Relocation<RE::SettingT<RE::GameSettingCollection>*> sNoWaitTakingHealthDamage{ REL::ID(664041) };
		inline static REL::Relocation<RE::SettingT<RE::GameSettingCollection>*> sNoWaitTakingRadDamage{ REL::ID(1321782) };
		inline static REL::Relocation<RE::SettingT<RE::GameSettingCollection>*> sNoWaitWhileAlarm{ REL::ID(397715) };

		bool GetGlobalValue(RE::BGSDefaultObject* a_defaultObject)
		{
			if (!a_defaultObject)
			{
				return false;
			}

			auto global = a_defaultObject->GetForm<RE::TESGlobal>();
			return global ? global->GetValue() : false;
		}

		bool GetBlockingMenuOpen(std::vector<std::string> a_menus)
		{
			if (auto UI = RE::UI::GetSingleton())
			{
				if (UI->menuMode > 0)
				{
					return true;
				}

				for (auto& menu : a_menus)
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
			WARN("WaitAnywhere::CanPassTime: Failed to GetSingleton."sv);
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
		          "PipboyMenu",
		          "PowerArmorModMenu",
		          "RobotModMenu",
		          "VATSMenu",
		          "WorkshopMenu" }))
		{
			return false;
		}

		if (PlayerCharacter->GetParentCell() &&
		    PlayerCharacter->GetParentCell()->GetCantWaitHere())
		{
			RE::SendHUDMessage::ShowHUDMessage(
				sNoWaitInCell->GetString().data(),
				"UIMenuCancel",
				true,
				true);
			return false;
		}

		if (!GetGlobalValue(Forms::BWA_bOverrideTrespassing_DO))
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

			if (PlayerCharacter->GetParentCell() &&
			    PlayerCharacter->GetParentCell()->cellFlags.any(RE::TESObjectCELL::Flag::kWarnToLeave))
			{
				RE::SendHUDMessage::ShowHUDMessage(
					sNoWaitWarnToLeave->GetString().data(),
					"UIMenuCancel",
					true,
					true);
				return false;
			}
		}

		if (!GetGlobalValue(Forms::BWA_bOverrideInAir_DO))
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

		if (!GetGlobalValue(Forms::BWA_bOverrideInCombat_DO))
		{
			if (ProcessLists->IsActorTargetingREFinPackage(
					PlayerCharacter,
					RE::PTYPE::kAlarm,
					false))
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

		if (!GetGlobalValue(Forms::BWA_bOverrideTakingHealthDamage_DO))
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

		if (!GetGlobalValue(Forms::BWA_bOverrideTakingRadDamage_DO))
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

		if (SynchedAnimManager->IsReferenceInSynchronizedScene(
				RE::PlayerCharacter::GetPlayerHandle()))
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
		if (WaitAnywhere::GetGlobalValue(Forms::BWA_bEnabled_DO) && WaitAnywhere::CanPassTime())
		{
			if (auto UIMessageQueue = RE::UIMessageQueue::GetSingleton())
			{
				UIMessageQueue->AddMessage(
					"SleepWaitMenu"sv,
					RE::UI_MESSAGE_TYPE::kShow);
			}
		}
	}
}
