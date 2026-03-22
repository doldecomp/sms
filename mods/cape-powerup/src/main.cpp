#include <BetterSMS/module.hxx>

static BetterSMS::ModuleInfo sModuleInfo("Cape Powerup", 1, 0, nullptr);

static void initModule() {
    BetterSMS::registerModule(sModuleInfo);
}

KURIBO_MODULE_BEGIN("Cape Powerup", "SMS Decomp", "1.0") {
    KURIBO_EXECUTE_ON_LOAD { initModule(); }
    KURIBO_EXECUTE_ON_UNLOAD { }
}
KURIBO_MODULE_END()
