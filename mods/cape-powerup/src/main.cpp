#include <BetterSMS/module.hxx>
#include "cape_data.hxx"

static CapeMod::CapeData sCapeData;

static BetterSMS::ModuleInfo sModuleInfo("CapePowerup", 0, 1, nullptr);

static void initModule() {
    sCapeData.reset();
    BetterSMS::registerModule(sModuleInfo);
}

KURIBO_MODULE_BEGIN("CapePowerup", "SMS-Decomp", "v0.1") {
    KURIBO_EXECUTE_ON_LOAD { initModule(); }
    KURIBO_EXECUTE_ON_UNLOAD { }
}
KURIBO_MODULE_END()
