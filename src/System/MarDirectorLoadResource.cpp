#include <System/MarDirector.hpp>
#include <JSystem/JParticle/JPAEmitterManager.hpp>
#include <JSystem/JKernel/JKRMemArchive.hpp>
#include <JSystem/JKernel/JKRDvdFile.hpp>
#include <JSystem/JKernel/JKRDvdRipper.hpp>
#include <THPPlayer/THPPlayer.h>
#include <System/Application.hpp>
#include <System/EmitterViewObj.hpp>
#include <System/Particles.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

extern void* gpSceneCmnDat;
extern int gpSceneCmnDatSize;

bool gParticleFlagLoaded[0x201];
JPAResourceManager* gpResourceManager;
JPAEmitterManager* gpEmitterManager4D2;

int TMarDirector::loadResource()
{
	TMarioParticleManager* this_00 = new TMarioParticleManager;

	gpMarioParticleManager = this_00;

	int lVar10 = 100;
	int iVar9  = 100;

	// TODO: giant switch, can't be bothered right now, sorry

	this_00->createEffectInfoAry(iVar9);
	gpResourceManager = new JPAResourceManager(0x201, 0x800, nullptr);
	// gpMarioParticleManager->unk3B8 =
	new JPAEmitterManager(gpResourceManager, lVar10, 0x100, 0x200, nullptr);
	gpEmitterManager4D2
	    = new JPAEmitterManager(nullptr, 200, 0x20, 0x40, nullptr);
	loadParticle();

	void* rawArch = SMSLoadArchive("/data/yoshi.arc", nullptr, 0, nullptr);
	JKRMemArchive* arch = new JKRMemArchive;
	if (!arch->mountFixed(rawArch, MBF_0))
		return 1;

	{
		JKRDvdFile sceneDvdFile;
		if (!sceneDvdFile.open("/data/scenecmn.bin"))
			return 1;

		gpSceneCmnDatSize = sceneDvdFile.getFileSize();
		gpSceneCmnDat     = JKRDvdRipper::loadToMainRAM(
            &sceneDvdFile, nullptr, EXPAND_SWITCH_DEFAULT, 0, nullptr,
            JKRDvdRipper::ALLOC_DIRECTION_FORWARD, 0, nullptr);
		if (gpSceneCmnDat == nullptr)
			return 1;
	}

	void* paramsBlob = new (0x20) char[0x80000];
	if (!SMSLoadArchive("/data/params.arc", paramsBlob, 0x80000, nullptr))
		return 1;

	JKRMemArchive* paramsArch = new (0x20) JKRMemArchive;
	if (!paramsArch->mountFixed(paramsBlob, MBF_0))
		return 1;

	unkB8 = gpApplication.mountStageArchive();
	if (!unkB8)
		return 1;

	if (gpApplication.mCurrArea.unk0 == 15) {
		void* optionBlob          = SMSLoadArchive("/data/option.arc", 0, 0, 0);
		JKRMemArchive* optionArch = new JKRMemArchive;
		if (!optionArch->mountFixed(optionBlob, MBF_0))
			return 1;
	}

	unkD4 = new (0x20) char[0x64000];
	unkD8 = new JKRMemArchive;
	if (mMap == 1) {
		int errc = thpInit();
		if (errc)
			return errc;
	}

	return 0;
}

void TMarDirector::initLoadParticle()
{
	for (int i = 0; i < 0x201; ++i)
		gParticleFlagLoaded[i] = 0;
}

void TMarDirector::loadParticle()
{
	void* pvVar1 = new (0x20) char[0x200000];
	SMSLoadArchive("/data/particle.arc", pvVar1, 0x200000, nullptr);
	JKRMemArchive* this_00 = new (0x20) JKRMemArchive;
	this_00->mountFixed(pvVar1, MBF_0);
	this_00->becomeCurrent("/");
	loadParticleMario();

	gpResourceManager->load("ms_shine_promi.jpa", 0x127);
	gpResourceManager->load("ms_shine_senko.jpa", 0x128);
	gpResourceManager->load("ms_shine_kira.jpa", 0x129);
	gpResourceManager->load("ms_shine_bow.jpa", 0x12a);
	gpResourceManager->load("ms_osendan.jpa", 0x52);
	gpResourceManager->load("ms_poi_sand.jpa", 0x53);
	gpResourceManager->load("ms_chu_jump.jpa", 0x54);
	gpResourceManager->load("ms_packn_ap_rock.jpa", 0x7f);
	gpResourceManager->load("ms_packn_ap_smoke.jpa", 0x80);
	gpResourceManager->load("ms_deadnamekli_m1.jpa", 0x81);
	gpResourceManager->load("ms_deadnamekli_m2.jpa", 0x82);
	gpResourceManager->load("ms_deadnamekli_n.jpa", 0x83);
	gpResourceManager->load("ms_deadnamekli_o.jpa", 0x84);
	gpResourceManager->load("ms_deaddorokli.jpa", 0x85);
	gpResourceManager->load("ms_deaddorokli_w.jpa", 0x86);
	gpResourceManager->load("ms_deadhamkli.jpa", 0x87);
	gpResourceManager->load("ms_deadhamkli_w.jpa", 0x88);
	gpResourceManager->load("ms_moe_fire_off.jpa", 0x8b);
	gpResourceManager->load("ms_popo_bomb_a.jpa", 0xa1);
	gpResourceManager->load("ms_popo_bomb_b.jpa", 0xa2);
	gpResourceManager->load("ms_deadhanekli.jpa", 0xa3);
	gpResourceManager->load("ms_deadhanekli_w.jpa", 0xa4);
	gpResourceManager->load("ms_deadmoekli.jpa", 0xa5);
	gpResourceManager->load("ms_deadmoekli_w.jpa", 0xa6);
	gpResourceManager->load("ms_smb_kafun_ow.jpa", 0xb2);
	gpResourceManager->load("ms_smb_kafun_oa.jpa", 0xb3);
	gpResourceManager->load("ms_smb_kafun_cw.jpa", 0xb4);
	gpResourceManager->load("ms_smb_kafun_ca.jpa", 0xb5);
	gpResourceManager->load("ms_smb_ap_rock.jpa", 0xb6);
	gpResourceManager->load("ms_smb_ap_smoke.jpa", 0xb7);
	gpResourceManager->load("ms_smb_hd_rock.jpa", 0xb8);
	gpResourceManager->load("ms_smb_hd_smoke.jpa", 0xb9);
	gpResourceManager->load("ms_geso_dead.jpa", 0xba);
	gpResourceManager->load("ms_geso_cannon.jpa", 0xbb);
	gpResourceManager->load("ms_geso_osenhit_a.jpa", 0xbc);
	gpResourceManager->load("ms_geso_osenhit_b.jpa", 0xbd);
	gpResourceManager->load("ms_pkh_toke.jpa", 0xbe);
	gpResourceManager->load("ms_pkh_fumi.jpa", 0xbf);
	gpResourceManager->load("ms_poi_dead.jpa", 0xc0);
	gpResourceManager->load("ms_petbtl_bomb_a.jpa", 0xc1);
	gpResourceManager->load("ms_petbtl_bomb_b.jpa", 0xc2);
	gpResourceManager->load("ms_petbtl_appear.jpa", 0xc3);
	gpResourceManager->load("ms_mnt_kira.jpa", 0x70);
	gpResourceManager->load("ms_mare_kira.jpa", 0x71);
	gpResourceManager->load("ms_mnt_yogore_b.jpa", 0x72);
	gpResourceManager->load("ms_mnt_yogore_f.jpa", 0x73);
	gpResourceManager->load("ms_mare_yogore.jpa", 0x74);
	gpResourceManager->load("ms_kino_yogore.jpa", 0x75);
	gpResourceManager->load("ms_cho_moku_a.jpa", 199);
	gpResourceManager->load("ms_cho_moku_b.jpa", 200);
	gpResourceManager->load("ms_cho_ase.jpa", 0xc9);
	gpResourceManager->load("ms_dnk_shibire_b.jpa", 0xca);
	gpResourceManager->load("ms_iga_fumi_air.jpa", 0xcb);
	gpResourceManager->load("ms_tama_flower.jpa", 0xcc);
	gpResourceManager->load("ms_tls_change.jpa", 0xcd);
	gpResourceManager->load("ms_tls_dead.jpa", 0xce);
	gpResourceManager->load("ms_teppanfire_j.jpa", 100);
	gpResourceManager->load("ms_enm_wallhit.jpa", 0xe2);
	gpResourceManager->load("ms_enm_wallhit_o.jpa", 0xe3);
	gpResourceManager->load("ms_enm_disap_a.jpa", 0xe4);
	gpResourceManager->load("ms_enm_disap_a_w.jpa", 0xe5);
	gpResourceManager->load("ms_enm_disap_b.jpa", 0xe6);
	gpResourceManager->load("ms_enm_wathit.jpa", 0xe7);
	gpResourceManager->load("ms_ex_cube_disa.jpa", 0x6d);
	gpResourceManager->load("ms_poi_kizetsu.jpa", 0x12f);
	gpResourceManager->load("ms_chu_ase.jpa", 0x130);
	gpResourceManager->load("ms_moe_fire_a.jpa", 0x135);
	gpResourceManager->load("ms_moe_fire_b.jpa", 0x136);
	gpResourceManager->load("ms_moe_fire_d.jpa", 0x137);
	gpResourceManager->load("ms_popo_shuwa_a.jpa", 0x13c);
	gpResourceManager->load("ms_popo_shuwa_b.jpa", 0x13d);
	gpResourceManager->load("ms_packn_hd_rock.jpa", 0x13e);
	gpResourceManager->load("ms_packn_hd_smoke.jpa", 0x13f);
	gpResourceManager->load("ms_geso_kiseki.jpa", 0x165);
	gpResourceManager->load("ms_npc_kokuen.jpa", 0x170);
	gpResourceManager->load("ms_npc_hamon_a.jpa", 0x171);
	gpResourceManager->load("ms_npc_hamon_b.jpa", 0x1f7);
	gpResourceManager->load("ms_mnt_wash.jpa", 0x172);
	gpResourceManager->load("ms_mare_wash.jpa", 0x173);
	gpResourceManager->load("ms_kil_smoke.jpa", 0x174);
	gpResourceManager->load("ms_pkh_doro.jpa", 0x175);
	gpResourceManager->load("ms_pkh_wash.jpa", 0x176);
	gpResourceManager->load("ms_puku_pichi.jpa", 0x177);
	gpResourceManager->load("ms_puku_awa.jpa", 0x178);
	gpResourceManager->load("ms_petbtl_smoke.jpa", 0x179);
	gpResourceManager->load("ms_dnk_biri.jpa", 0x17a);
	gpResourceManager->load("ms_dnk_spark_l.jpa", 0x17b);
	gpResourceManager->load("ms_dnk_spark_r.jpa", 0x17c);
	gpResourceManager->load("ms_dnk_shibire_a.jpa", 0x17d);
	gpResourceManager->load("ms_dnk_hibana.jpa", 0x17e);
	gpResourceManager->load("ms_bomb_limit.jpa", 0x17f);
	gpResourceManager->load("ms_amn_biri.jpa", 0x180);
	gpResourceManager->load("ms_amn_spark_l.jpa", 0x181);
	gpResourceManager->load("ms_amn_spark_r.jpa", 0x182);
	gpResourceManager->load("ms_amn_spark_m.jpa", 0x183);
	gpResourceManager->load("ms_tama_hit.jpa", 0x185);
	gpResourceManager->load("ms_tama_blur.jpa", 0x186);
	gpResourceManager->load("ms_tls_yodare_l.jpa", 0x187);
	gpResourceManager->load("ms_tls_yodare_s.jpa", 0x188);
	gpResourceManager->load("ms_ynb_onpu.jpa", 0x18b);
	gpResourceManager->load("ms_teppanfire_a.jpa", 300);
	gpResourceManager->load("ms_map_fire_a.jpa", 0x1c4);
	gpResourceManager->load("ms_map_fire_b.jpa", 0x1c5);
	gpResourceManager->load("ms_mpk_fire_a.jpa", 0x1d1);
	gpResourceManager->load("ms_mpk_fire_b.jpa", 0x1d2);
	gpResourceManager->load("ms_mari_rakuball.jpa", 0x1d8);
	gpResourceManager->load("ms_mari_rakuhamon.jpa", 0x1d9);
	gpResourceManager->load("ms_raku_indawa.jpa", 0x1da);
	gpResourceManager->load("ms_raku_kie.jpa", 0x1db);
	gpResourceManager->load("ms_teppanfire_b.jpa", 0x1f1);
	gpResourceManager->load("ms_name_indirect.jpa", 0x1ec);
	gpResourceManager->load("ms_moe_fire_c.jpa", 0x1ed);
	gpResourceManager->load("ms_map_fire_c.jpa", 0x1f5);
	SMS_LoadParticle("ms_sl_or_melt1.jpa", 0xd1);
	SMS_LoadParticle("ms_sl_or_melt2.jpa", 0xd2);
	SMS_LoadParticle("ms_sl_or_damage.jpa", 0x12e);
	SMS_LoadParticle("ms_gene_dead.jpa", 0xd0);
	SMS_LoadParticle("ms_gene_hit.jpa", 0x12d);
	gpResourceManager->load("ms_2d_pause_smo.jpa", 0x1f9);
	gpResourceManager->load("ms_2d_pause_sel.jpa", 0x1fa);
	gpResourceManager->load("ms_2d_icon_kira1.jpa", 0x1fb);
	gpResourceManager->load("ms_2d_icon_kira2.jpa", 0x1fc);
	gpResourceManager->load("ms_2d_go.jpa", 0x1fd);
	gpResourceManager->load("ms_2d_get_a.jpa", 0x1fe);
	gpResourceManager->load("ms_2d_get_b.jpa", 0x1ff);
	gpResourceManager->load("ms_2d_elecflash.jpa", 0x200);

	// gpEmitterManager4D2->unkA4 = gpResourceManager;
	this_00->unmountFixed();

	if (mMap == 4 && unk7D == 2) {
		void* hanachanJpaArch = SMSLoadArchive("/data/bosshanachanJpa.arc",
		                                       pvVar1, 0x200000, nullptr);
		this_00->mountFixed(hanachanJpaArch, MBF_0);
		this_00->becomeCurrent("/");
		// TODO:
		// TBossHanachan::staticLoadParticle();
		this_00->unmountFixed();
	}
	JKRHeap::getCurrentHeap()->freeTail();
}

void TMarDirector::loadParticleMario()
{
	SMS_LoadParticle("ms_m_rocket_a.jpa", 1);
	SMS_LoadParticle("ms_m_rocket_b.jpa", 2);
	SMS_LoadParticle("ms_m_rocket_b2.jpa", 3);
	SMS_LoadParticle("ms_m_rocket_c.jpa", 4);
	SMS_LoadParticle("ms_m_rocket_d.jpa", 5);
	SMS_LoadParticle("ms_firedmg_a.jpa", 6);
	SMS_LoadParticle("ms_fumi_a.jpa", 7);
	SMS_LoadParticle("ms_fumi_b.jpa", 8);
	SMS_LoadParticle("ms_fumi_c.jpa", 9);
	SMS_LoadParticle("ms_dmg_a.jpa", 10);
	SMS_LoadParticle("ms_dmg_b.jpa", 0xb);
	SMS_LoadParticle("ms_dmg_c.jpa", 0xc);
	SMS_LoadParticle("ms_mariwalk1_a.jpa", 0x15);
	SMS_LoadParticle("ms_mariwalk1_b.jpa", 0x16);
	SMS_LoadParticle("ms_mariwalk1_c.jpa", 0x17);
	SMS_LoadParticle("ms_ase.jpa", 0xd);
	SMS_LoadParticle("ms_itemget1_a.jpa", 0xe);
	SMS_LoadParticle("ms_itemget1_b.jpa", 0xf);
	SMS_LoadParticle("ms_itemget1_a.jpa", 0xe);
	SMS_LoadParticle("ms_itemget1_b.jpa", 0xf);
	SMS_LoadParticle("ms_jump_ed_a.jpa", 0x10);
	SMS_LoadParticle("ms_jump_ed_b.jpa", 0x11);
	SMS_LoadParticle("ms_hipdrop_a.jpa", 0x12);
	SMS_LoadParticle("ms_hipdrop_b.jpa", 0x13);
	SMS_LoadParticle("ms_hipdrop_c.jpa", 0x14);
	SMS_LoadParticle("ms_wallkick_a.jpa", 0x18);
	SMS_LoadParticle("ms_wallkick_b.jpa", 0x19);
	SMS_LoadParticle("ms_marioap_body.jpa", 0x24);
	SMS_LoadParticle("ms_marioap_head.jpa", 0x25);
	SMS_LoadParticle("ms_marioap_cap.jpa", 0x26);
	SMS_LoadParticle("ms_marioap_rhand.jpa", 0x27);
	SMS_LoadParticle("ms_marioap_lhand.jpa", 0x28);
	SMS_LoadParticle("ms_marioap_rleg.jpa", 0x29);
	SMS_LoadParticle("ms_marioap_rfoot.jpa", 0x2a);
	SMS_LoadParticle("ms_marioap_lleg.jpa", 0x2b);
	SMS_LoadParticle("ms_marioap_lfoot.jpa", 0x2c);
	SMS_LoadParticle("ms_marioap_watgun.jpa", 0x2d);
	SMS_LoadParticle("ms_watboost_on.jpa", 0x2e);
	SMS_LoadParticle("ms_m_tobikomi_a.jpa", 0x2f);
	SMS_LoadParticle("ms_m_tobikomi_b.jpa", 0x30);
	SMS_LoadParticle("ms_m_tobikomi_s_a.jpa", 0x31);
	SMS_LoadParticle("ms_m_tobikomi_s_b.jpa", 0x32);
	SMS_LoadParticle("ms_m_awahamon.jpa", 0x33);
	SMS_LoadParticle("ms_m_watrun_a.jpa", 0x34);
	SMS_LoadParticle("ms_m_watrun_b.jpa", 0x35);
	SMS_LoadParticle("ms_m_watrun_c.jpa", 0x36);
	SMS_LoadParticle("ms_coinget_a.jpa", 0x37);
	SMS_LoadParticle("ms_coinget_b.jpa", 0x38);
	SMS_LoadParticle("ms_m_amiattack.jpa", 0x39);
	SMS_LoadParticle("ms_m_walksand_a.jpa", 0x3a);
	SMS_LoadParticle("ms_m_walksand_b.jpa", 0x3b);
	SMS_LoadParticle("ms_yos_nomu_a.jpa", 0x3d);
	SMS_LoadParticle("ms_yos_nomu_b.jpa", 0x3e);
	SMS_LoadParticle("ms_yos_warp.jpa", 0x3f);
	SMS_LoadParticle("ms_m_gout_gate.jpa", 0x40);
	SMS_LoadParticle("ms_m_gout_dokan.jpa", 0x41);
	SMS_LoadParticle("ms_m_walkcloud.jpa", 0x42);
	SMS_LoadParticle("ms_m_sphipd_hit_a.jpa", 0x43);
	SMS_LoadParticle("ms_m_sphipd_hit_b.jpa", 0x44);
	SMS_LoadParticle("ms_m_sphipd_hit_c.jpa", 0x45);
	SMS_LoadParticle("ms_m_sphipd_hit_d.jpa", 0x46);
	SMS_LoadParticle("ms_redcoinget_1.jpa", 0x47);
	SMS_LoadParticle("ms_redcoinget_2.jpa", 0x48);
	SMS_LoadParticle("ms_redcoinget_3.jpa", 0x49);
	SMS_LoadParticle("ms_redcoinget_4.jpa", 0x4a);
	SMS_LoadParticle("ms_redcoinget_5.jpa", 0x4b);
	SMS_LoadParticle("ms_redcoinget_6.jpa", 0x4c);
	SMS_LoadParticle("ms_redcoinget_7.jpa", 0x4d);
	SMS_LoadParticle("ms_redcoinget_8.jpa", 0x4e);
	SMS_LoadParticle("ms_watboost_a.jpa", 0xfe);
	SMS_LoadParticle("ms_watboost_b.jpa", 0xff);
	SMS_LoadParticle("ms_hikage1_a.jpa", 0x100);
	SMS_LoadParticle("ms_hikage1_b.jpa", 0x101);
	SMS_LoadParticle("ms_mariwallsl1.jpa", 0x102);
	SMS_LoadParticle("ms_m_slipsmoke.jpa", 0x103);
	SMS_LoadParticle("ms_m_hamon_a.jpa", 0x107);
	SMS_LoadParticle("ms_m_hamon_c.jpa", 0x108);
	SMS_LoadParticle("ms_m_hamon_d.jpa", 0x109);
	SMS_LoadParticle("ms_m_blur3.jpa", 0x104);
	SMS_LoadParticle("ms_m_blur2.jpa", 0x105);
	SMS_LoadParticle("ms_m_blur2sp.jpa", 0x106);
	SMS_LoadParticle("ms_m_awa.jpa", 0x10c);
	SMS_LoadParticle("ms_nozzlesmoke.jpa", 0x10d);
	SMS_LoadParticle("ms_m_blur1.jpa", 0x10e);
	SMS_LoadParticle("ms_m_slidesand_a.jpa", 0x10f);
	SMS_LoadParticle("ms_m_slidesand_b.jpa", 0x110);
	SMS_LoadParticle("ms_m_awa_s.jpa", 0x111);
	SMS_LoadParticle("ms_m_watslide_b.jpa", 0x112);
	SMS_LoadParticle("ms_m_watslide_c.jpa", 0x113);
	SMS_LoadParticle("ms_m_spinshot_a.jpa", 0x114);
	SMS_LoadParticle("ms_m_spinshot_b.jpa", 0x115);
	SMS_LoadParticle("ms_m_biri_a.jpa", 0x116);
	SMS_LoadParticle("ms_m_biri_b.jpa", 0x117);
	SMS_LoadParticle("ms_m_biri_c.jpa", 0x118);
	SMS_LoadParticle("ms_yos_ase.jpa", 0x119);
	SMS_LoadParticle("ms_gesosurf_b.jpa", 0x121);
	SMS_LoadParticle("ms_gesosurf_c.jpa", 0x122);
	SMS_LoadParticle("ms_gesosurf_d.jpa", 0x123);
	SMS_LoadParticle("ms_poi_zzz.jpa", 0x124);
	SMS_LoadParticle("ms_m_mzbhamon_a.jpa", 0x10a);
	SMS_LoadParticle("ms_m_mzbhamon_c.jpa", 0x10b);
	SMS_LoadParticle("ms_m_sphipd_a.jpa", 0x11a);
	SMS_LoadParticle("ms_m_sphipd_b.jpa", 0x11b);
	SMS_LoadParticle("ms_m_sphipd_c.jpa", 0x11c);
	SMS_LoadParticle("ms_m_sphipd_d.jpa", 0x11d);
	SMS_LoadParticle("ms_m_seasmoke.jpa", 0x11e);
	SMS_LoadParticle("ms_toro_wind.jpa", 0x11f);
	SMS_LoadParticle("ms_toro_hibana.jpa", 0x120);
	SMS_LoadParticle("ms_m_tobikomi_c.jpa", 0x1d4);
	SMS_LoadParticle("ms_m_tobikomi_s_c.jpa", 0x1d5);
	SMS_LoadParticle("ms_gatein.jpa", 0x1d6);
	SMS_LoadParticle("ms_m_hamon_b.jpa", 0x1e8);
	SMS_LoadParticle("ms_gesosurf_a.jpa", 0x1e7);
	SMS_LoadParticle("ms_m_mzbhamon_b.jpa", 0x1e9);
	SMS_LoadParticle("ms_m_watslide_a.jpa", 0x1ea);
	SMS_LoadParticle("ms_mpk_fire_c.jpa", 0x1f8);
}

// TODO: size mismatch
int TMarDirector::thpInit()
{
	THPPlayerInit();
	if (!THPPlayerOpen("/data/ex128x144_q0.thp", FALSE))
		return 1;
	THPPlayerSetBuffer(new (0x20) u8[THPPlayerCalcNeedMemory()]);
	if (!THPPlayerPrepare(0, 1, 0))
		return 1;

	return 0;
}
