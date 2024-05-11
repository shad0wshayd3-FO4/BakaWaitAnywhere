#pragma once

class Forms
{
private:
	static std::uint32_t HookInitializer_DefaultObject()
	{
		// Initializer Override
		UnarmedPlayerWeapon_DO =
			RE::DefaultObjectFormFactory::Create(
				"UnarmedPlayerWeapon_DO",
				"The unarmed weapon used to calculate the player's displayed attack stats when no weapon is equipped.",
				RE::ENUM_FORM_ID::kWEAP);

		// Add new
		BWA_bEnabled_DO =
			RE::DefaultObjectFormFactory::Create(
				"BWA_bEnabled_DO",
				RE::ENUM_FORM_ID::kGLOB);

		BWA_bOverrideInAir_DO =
			RE::DefaultObjectFormFactory::Create(
				"BWA_bOverrideInAir_DO",
				RE::ENUM_FORM_ID::kGLOB);

		BWA_bOverrideInCombat_DO =
			RE::DefaultObjectFormFactory::Create(
				"BWA_bOverrideInCombat_DO",
				RE::ENUM_FORM_ID::kGLOB);

		BWA_bOverrideTrespassing_DO =
			RE::DefaultObjectFormFactory::Create(
				"BWA_bOverrideTrespassing_DO",
				RE::ENUM_FORM_ID::kGLOB);

		BWA_bOverrideTakingHealthDamage_DO =
			RE::DefaultObjectFormFactory::Create(
				"BWA_bOverrideTakingHealthDamage_DO",
				RE::ENUM_FORM_ID::kGLOB);

		BWA_bOverrideTakingRadDamage_DO =
			RE::DefaultObjectFormFactory::Create(
				"BWA_bOverrideTakingRadDamage_DO",
				RE::ENUM_FORM_ID::kGLOB);

		logger::debug("Injected DefaultObjects"sv);
		return 1;
	}

	// members
	inline static RE::BGSDefaultObject* UnarmedPlayerWeapon_DO{ nullptr };

public:
	static void InstallHooks()
	{
		REL::Relocation<std::uintptr_t> targetDefault{ REL::ID(1158572) };
		stl::asm_replace(targetDefault.address(), 0x31, reinterpret_cast<std::uintptr_t>(HookInitializer_DefaultObject));
	}

	// members
	inline static RE::BGSDefaultObject* BWA_bEnabled_DO{ nullptr };
	inline static RE::BGSDefaultObject* BWA_bOverrideInAir_DO{ nullptr };
	inline static RE::BGSDefaultObject* BWA_bOverrideInCombat_DO{ nullptr };
	inline static RE::BGSDefaultObject* BWA_bOverrideTrespassing_DO{ nullptr };
	inline static RE::BGSDefaultObject* BWA_bOverrideTakingHealthDamage_DO{ nullptr };
	inline static RE::BGSDefaultObject* BWA_bOverrideTakingRadDamage_DO{ nullptr };
};
