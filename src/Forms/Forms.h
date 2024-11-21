#pragma once

class Forms
{
public:
	static void Install()
	{
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

		DEBUG("Injected DefaultObjects"sv);
	}

	// members
	inline static RE::BGSDefaultObject* BWA_bEnabled_DO{ nullptr };
	inline static RE::BGSDefaultObject* BWA_bOverrideInAir_DO{ nullptr };
	inline static RE::BGSDefaultObject* BWA_bOverrideInCombat_DO{ nullptr };
	inline static RE::BGSDefaultObject* BWA_bOverrideTrespassing_DO{ nullptr };
	inline static RE::BGSDefaultObject* BWA_bOverrideTakingHealthDamage_DO{ nullptr };
	inline static RE::BGSDefaultObject* BWA_bOverrideTakingRadDamage_DO{ nullptr };
};
