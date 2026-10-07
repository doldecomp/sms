#include "NPC/NpcBase.hpp"
#include "NPC/NpcManager.hpp"
#include "Strategic/LiveManager.hpp"
#include <System/MarNameRefGen.hpp>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>

JDrama::TNameRef* TMarNameRefGen::getNameRef_NPC(const char* name) const
{
	if (strcmp(name, "NPCMonteM") == 0)
		return new TBaseNPC(ACTOR_TYPE_NPC_MONTE_M);

	if (strcmp(name, "NPCMonteMA") == 0)
		return new TBaseNPC(ACTOR_TYPE_NPC_MONTE_MA);

	if (strcmp(name, "NPCMonteMB") == 0)
		return new TBaseNPC(ACTOR_TYPE_NPC_MONTE_MB);

	if (strcmp(name, "NPCMonteMC") == 0)
		return new TBaseNPC(ACTOR_TYPE_NPC_MONTE_MC);

	if (strcmp(name, "NPCMonteMD") == 0)
		return new TBaseNPC(ACTOR_TYPE_NPC_MONTE_MD);

	if (strcmp(name, "NPCMonteME") == 0)
		return new TBaseNPC(ACTOR_TYPE_NPC_MONTE_ME);

	if (strcmp(name, "NPCMonteMF") == 0)
		return new TBaseNPC(ACTOR_TYPE_NPC_MONTE_MF);

	if (strcmp(name, "NPCMonteMG") == 0)
		return new TBaseNPC(ACTOR_TYPE_NPC_MONTE_MG);

	if (strcmp(name, "NPCMonteMH") == 0)
		return new TBaseNPC(ACTOR_TYPE_NPC_MONTE_MH);

	if (strcmp(name, "NPCMonteW") == 0)
		return new TBaseNPC(ACTOR_TYPE_NPC_MONTE_W);

	if (strcmp(name, "NPCMonteWA") == 0)
		return new TBaseNPC(ACTOR_TYPE_NPC_MONTE_WA);

	if (strcmp(name, "NPCMonteWB") == 0)
		return new TBaseNPC(ACTOR_TYPE_NPC_MONTE_WB);

	if (strcmp(name, "NPCMonteWC") == 0)
		return new TBaseNPC(ACTOR_TYPE_NPC_MONTE_WC);

	if (strcmp(name, "NPCMareM") == 0)
		return new TBaseNPC(ACTOR_TYPE_NPC_MARE_M);

	if (strcmp(name, "NPCMareMA") == 0)
		return new TBaseNPC(ACTOR_TYPE_NPC_MARE_MA);

	if (strcmp(name, "NPCMareMB") == 0)
		return new TBaseNPC(ACTOR_TYPE_NPC_MARE_MB);

	if (strcmp(name, "NPCMareMC") == 0)
		return new TBaseNPC(ACTOR_TYPE_NPC_MARE_MC);

	if (strcmp(name, "NPCMareMD") == 0)
		return new TBaseNPC(ACTOR_TYPE_NPC_MARE_MD);

	if (strcmp(name, "NPCMareW") == 0)
		return new TBaseNPC(ACTOR_TYPE_NPC_MARE_W);

	if (strcmp(name, "NPCMareWA") == 0)
		return new TBaseNPC(ACTOR_TYPE_NPC_MARE_WA);

	if (strcmp(name, "NPCMareWB") == 0)
		return new TBaseNPC(ACTOR_TYPE_NPC_MARE_WB);

	if (strcmp(name, "NPCKinopio") == 0)
		return new TBaseNPC(ACTOR_TYPE_NPC_KINOPIO);

	if (strcmp(name, "NPCKinojii") == 0)
		return new TBaseNPC(ACTOR_TYPE_NPC_KINOJII);

	if (strcmp(name, "NPCPeach") == 0)
		return new TBaseNPC(ACTOR_TYPE_NPC_PEACH);

	if (strcmp(name, "NPCRaccoonDog") == 0)
		return new TBaseNPC(ACTOR_TYPE_NPC_RACCOON_DOG);

	if (strcmp(name, "NPCSunflowerL") == 0)
		return new TBaseNPC(ACTOR_TYPE_NPC_SUNFLOWER_L);

	if (strcmp(name, "NPCSunflowerS") == 0)
		return new TBaseNPC(ACTOR_TYPE_NPC_SUNFLOWER_S);

	if (strcmp(name, "NPCDummy") == 0)
		return new TBaseNPC(ACTOR_TYPE_NPC_DUMMY);

	if (strcmp(name, "NPCBoard") == 0)
		return new TBaseNPC(ACTOR_TYPE_NPC_BOARD);

	if (strcmp(name, "MonteMManager") == 0)
		return new TMonteMManager;

	if (strcmp(name, "MonteMAManager") == 0)
		return new TMonteMAManager;

	if (strcmp(name, "MonteMBManager") == 0)
		return new TMonteMBManager;

	if (strcmp(name, "MonteMCManager") == 0)
		return new TMonteMCManager;

	if (strcmp(name, "MonteMDManager") == 0)
		return new TMonteMDManager;

	if (strcmp(name, "MonteMEManager") == 0)
		return new TMonteMEManager;

	if (strcmp(name, "MonteMFManager") == 0)
		return new TMonteMFManager;

	if (strcmp(name, "MonteMGManager") == 0)
		return new TMonteMGManager;

	if (strcmp(name, "MonteMHManager") == 0)
		return new TMonteMHManager;

	if (strcmp(name, "MonteWManager") == 0)
		return new TMonteWManager;

	if (strcmp(name, "MonteWAManager") == 0)
		return new TMonteWAManager;

	if (strcmp(name, "MonteWBManager") == 0)
		return new TMonteWBManager;

	if (strcmp(name, "MonteWCManager") == 0)
		return new TMonteWCManager;

	if (strcmp(name, "MareMManager") == 0)
		return new TMareMManager;

	if (strcmp(name, "MareMAManager") == 0)
		return new TMareMAManager;

	if (strcmp(name, "MareMBManager") == 0)
		return new TMareMBManager;

	if (strcmp(name, "MareMCManager") == 0)
		return new TMareMCManager;

	if (strcmp(name, "MareMDManager") == 0)
		return new TMareMDManager;

	if (strcmp(name, "MareWManager") == 0)
		return new TMareWManager;

	if (strcmp(name, "MareWAManager") == 0)
		return new TMareWAManager;

	if (strcmp(name, "MareWBManager") == 0)
		return new TMareWBManager;

	if (strcmp(name, "KinopioManager") == 0)
		return new TKinopioManager;

	if (strcmp(name, "KinojiiManager") == 0)
		return new TKinojiiManager;

	if (strcmp(name, "PeachManager") == 0)
		return new TPeachManager;

	if (strcmp(name, "RaccoonDogManager") == 0)
		return new TRaccoonDogManager;

	if (strcmp(name, "SunflowerLManager") == 0)
		return new TSunflowerLManager;

	if (strcmp(name, "SunflowerSManager") == 0)
		return new TSunflowerSManager;

	if (strcmp(name, "MareJellyFish") == 0)
		return new TMareJellyFishManager("?");

	if (strcmp(name, "BoardNpcManager") == 0)
		return new TBoardNpcManager;

	return nullptr;
}
