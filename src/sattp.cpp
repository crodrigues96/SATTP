#include "sattp.hpp"
//----
PLUGIN_EXPORT bool PLUGIN_CALL Load(void **ppData) {
	sampgdk::Load(ppData);
	CWebserver::Load();
	//---
	sampgdk::logprintf("  |------------------------------------------|");
	sampgdk::logprintf("  |                                          |");
	sampgdk::logprintf("  |         SATTP - SA:MP WEB SERVER         |");
	sampgdk::logprintf("  |                                          |");
	sampgdk::logprintf("  |                    BY                    |");
	sampgdk::logprintf("  |                                          |");
	sampgdk::logprintf("  |         CARLOS \"GWMPT\" RODRIGUES         |");
	sampgdk::logprintf("  |                                          |");
	sampgdk::logprintf("  |      ------------------------------      |");
	sampgdk::logprintf("  |                                          |");
	sampgdk::logprintf("  |    RELEASE DATE: "__DATE__" "__TIME__"    |");
	sampgdk::logprintf("  |                                          |");
	sampgdk::logprintf("  |------------------------------------------|");
	//---
	return true;
}

PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports() {
	return sampgdk::Supports() | SUPPORTS_PROCESS_TICK;
}

PLUGIN_EXPORT void PLUGIN_CALL Unload() { // Called with exit command
	sampgdk::logprintf("Terminating Web server thread...");
	CWebserver::Unload();
	sampgdk::logprintf("Web server thread terminated.");
	sampgdk::Unload();
}

PLUGIN_EXPORT void PLUGIN_CALL ProcessTick() {
	sampgdk::ProcessTick();
}
