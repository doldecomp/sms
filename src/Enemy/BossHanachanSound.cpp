#include <Enemy/BossHanachan.hpp>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>

static const char* bosshanachan_bastable[] = {
	"/scene/bosshanachan/bas/hanabody_damage.bas",
	nullptr,
	nullptr,
	"/scene/bosshanachan/bas/hanabody_getup_l2.bas",
	"/scene/bosshanachan/bas/hanabody_getup_l3.bas",
	nullptr,
	"/scene/bosshanachan/bas/hanabody_getup_r2.bas",
	"/scene/bosshanachan/bas/hanabody_getup_r3.bas",
	"/scene/bosshanachan/bas/hanabody_hipdrop_long.bas",
	nullptr,
	nullptr,
	"/scene/bosshanachan/bas/hanabody_jitabata2.bas",
	"/scene/bosshanachan/bas/hanabody_jump_reaction.bas",
	nullptr,
	"/scene/bosshanachan/bas/hanabody_pikupiku2.bas",
	"/scene/bosshanachan/bas/hanabody_run.bas",
	"/scene/bosshanachan/bas/hanabody_start.bas",
	nullptr,
	nullptr,
	"/scene/bosshanachan/bas/hanabody_walk.bas",
	nullptr,
	"/scene/bosshanachan/bas/hanahead_end.bas",
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	"/scene/bosshanachan/bas/hanahead_hipdrop_reaction.bas",
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	"/scene/bosshanachan/bas/hanahead_start.bas",
	"/scene/bosshanachan/bas/hanahead_tumble_L.bas",
	"/scene/bosshanachan/bas/hanahead_tumble_R.bas",
	nullptr,
	nullptr,
};

const char** TBossHanachanPartsBase::getBasNameTable() const
{
	return bosshanachan_bastable;
}
