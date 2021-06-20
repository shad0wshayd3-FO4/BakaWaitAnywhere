#include "Forms/Forms.h"
#include "Scripts/Papyrus.h"

extern "C" DLLEXPORT bool F4SEAPI F4SEPlugin_Query(const F4SE::QueryInterface* a_F4SE, F4SE::PluginInfo* a_info)
{
	// Create log
	auto logPath = logger::log_directory();
	if (!logPath)
	{
		return false;
	}

	*logPath /= fmt::format(FMT_STRING("{}.log"), Version::PROJECT);
	auto logSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logPath->string(), true);
	auto log = std::make_shared<spdlog::logger>("plugin_log"s, std::move(logSink));

	// Set default log traits
	spdlog::set_default_logger(std::move(log));
	spdlog::set_pattern("[%m/%d/%Y - %T] [%^%l%$] %v"s);
	spdlog::flush_on(spdlog::level::debug);

	// Set log level from ini
	Settings::Load();
#ifdef NDEBUG
	if (*Settings::EnableDebugLogging)
#else
	if (true)
#endif
	{
		spdlog::set_level(spdlog::level::debug);
	}
	else
	{
		spdlog::set_level(spdlog::level::info);
	}

	// Initial messages
	logger::info(FMT_STRING("{} v{} log opened."), Version::PROJECT, Version::NAME);
	logger::debug("Debug logging enabled."sv);

	// Initialize PluginInfo
	a_info->infoVersion = F4SE::PluginInfo::kVersion;
	a_info->name = Version::PROJECT.data();
	a_info->version = Version::MAJOR;

	// Check if we're being loaded in the CK.
	if (a_F4SE->IsEditor())
	{
		logger::critical("Loaded in editor, marking as incompatible."sv);
		return false;
	}

	// Check if we're being loaded by a supported version of the game.
	const auto ver = a_F4SE->RuntimeVersion();
	if (ver < F4SE::RUNTIME_1_10_163)
	{
		logger::critical(FMT_STRING("Unsupported runtime v{}, marking as incompatible."), ver.string());
		return false;
	}

	return true;
}

extern "C" DLLEXPORT bool F4SEAPI F4SEPlugin_Load(const F4SE::LoadInterface* a_F4SE)
{
	// Initialize F4SE
	F4SE::Init(a_F4SE);

	// Register Papyrus functions
	const auto papyrus = F4SE::GetPapyrusInterface();
	if (!papyrus || !papyrus->Register(Papyrus::RegisterFunctions))
	{
		logger::critical("Failed to register Papyrus functions, marking as incompatible."sv);
		return false;
	}

	// Hook Forms
	Forms::InstallHooks();

	// Finish load
	logger::info("Plugin loaded successfully."sv);
	return true;
}
