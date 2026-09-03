#!/usr/bin/env python3

###
# Generates build files for the project.
# This file also includes the project configuration,
# such as compiler flags and the object matching status.
#
# Usage:
#   python3 configure.py
#   ninja
#
# Append --help to see available options.
###

import argparse
import sys
from pathlib import Path
from typing import Any, Dict, List

from tools.project import (
    Object,
    ProgressCategory,
    ProjectConfig,
    calculate_progress,
    generate_build,
    is_windows,
)

# Game versions
DEFAULT_VERSION = 0
VERSIONS = [
    "GMSJ01",  # 0
    "GMSP01",  # 1
]

parser = argparse.ArgumentParser()
parser.add_argument(
    "mode",
    choices=["configure", "progress"],
    default="configure",
    help="script mode (default: configure)",
    nargs="?",
)
parser.add_argument(
    "-v",
    "--version",
    choices=VERSIONS,
    type=str.upper,
    default=VERSIONS[DEFAULT_VERSION],
    help="version to build",
)
parser.add_argument(
    "--build-dir",
    metavar="DIR",
    type=Path,
    default=Path("build"),
    help="base build directory (default: build)",
)
parser.add_argument(
    "--binutils",
    metavar="BINARY",
    type=Path,
    help="path to binutils (optional)",
)
parser.add_argument(
    "--compilers",
    metavar="DIR",
    type=Path,
    help="path to compilers (optional)",
)
parser.add_argument(
    "--map",
    action="store_true",
    help="generate map file(s)",
)
parser.add_argument(
    "--debug",
    action="store_true",
    help="build with debug info (non-matching)",
)
if not is_windows():
    parser.add_argument(
        "--wrapper",
        metavar="BINARY",
        type=Path,
        help="path to wibo or wine (optional)",
    )
parser.add_argument(
    "--dtk",
    metavar="BINARY | DIR",
    type=Path,
    help="path to decomp-toolkit binary or source (optional)",
)
parser.add_argument(
    "--objdiff",
    metavar="BINARY | DIR",
    type=Path,
    help="path to objdiff-cli binary or source (optional)",
)
parser.add_argument(
    "--sjiswrap",
    metavar="EXE",
    type=Path,
    help="path to sjiswrap.exe (optional)",
)
parser.add_argument(
    "--verbose",
    action="store_true",
    help="print verbose output",
)
parser.add_argument(
    "--non-matching",
    dest="non_matching",
    action="store_true",
    help="builds equivalent (but non-matching) or modded objects",
)
parser.add_argument(
    "--no-progress",
    dest="progress",
    action="store_false",
    help="disable progress calculation",
)
args = parser.parse_args()

config = ProjectConfig()
config.version = str(args.version)
version_num = VERSIONS.index(config.version)

# Apply arguments
config.build_dir = args.build_dir
config.dtk_path = args.dtk
config.objdiff_path = args.objdiff
config.binutils_path = args.binutils
config.compilers_path = args.compilers
config.generate_map = args.map
config.non_matching = args.non_matching
config.sjiswrap_path = args.sjiswrap
config.progress = args.progress
if not is_windows():
    config.wrapper = args.wrapper
# Don't build asm unless we're --non-matching
if not config.non_matching:
    config.asm_dir = None

# Tool versions
config.binutils_tag = "2.42-1"
config.compilers_tag = "20250520"
config.dtk_tag = "v1.3.0"
config.objdiff_tag = "v3.7.1"
config.sjiswrap_tag = "v1.2.0"
config.wibo_tag = "0.6.11"

# Project
config.config_path = Path("config") / config.version / "config.yml"
config.check_sha_path = Path("config") / config.version / "build.sha1"
config.asflags = [
    "-mgekko",
    "--strip-local-absolute",
    "-I include",
    "-I include/PowerPC_EABI_Support/Msl/MSL_C/MSL_Common",
    "-I include/PowerPC_EABI_Support/Msl/MSL_C++/MSL_Common",
    f"-I build/{config.version}/include",
    f"--defsym BUILD_VERSION={version_num}",
]
config.ldflags = [
    "-fp hardware",
    "-nodefaults",
]
if args.debug:
    config.ldflags.append("-g")  # Or -gdwarf-2 for Wii linkers
if args.map:
    config.ldflags.append("-mapunused")
    # config.ldflags.append("-listclosure") # For Wii linkers

# Use for any additional files that should cause a re-configure when modified
config.reconfig_deps = []

# Optional numeric ID for decomp.me preset
# Can be overridden in libraries or objects
config.scratch_preset_id = 61  # Super Mario Sunshine

# Base flags, common to most GC/Wii games.
# Generally leave untouched, with overrides added below.
cflags_base_base = [
    "-nodefaults",
    "-align powerpc",
    "-enum int",
    "-fp hardware",
    "-Cpp_exceptions off",
    '-pragma "cats off"',
    '-pragma "warn_notinlined off"',
    "-maxerrors 1",
    "-nosyspath",
    "-RTTI off",
    "-str reuse",
    "-multibyte",  # For Wii compilers, replace with `-enc SJIS`
    "-cwd source",
    "-i include",
    "-i include/PowerPC_EABI_Support/Msl/MSL_C/MSL_Common",
    "-i include/PowerPC_EABI_Support/Msl/MSL_C++/MSL_Common",
    f"-i build/{config.version}/include",
    f"-DBUILD_VERSION={version_num}",
    f"-DVERSION_{config.version}",
]

cflags_base = [
    *cflags_base_base,
    "-proc gekko",
    "-DGEKKO",
]

# Debug flags
if args.debug:
    # Or -sym dwarf-2 for Wii compilers
    cflags_base.extend(["-sym on", "-DDEBUG=1"])
else:
    cflags_base.append("-DNDEBUG=1")

# Metrowerks library flags
cflags_runtime = [
    *cflags_base,
    "-O4,p",
    "-inline auto",
    "-fp_contract on",
    "-str reuse,pool,readonly",
    "-inline deferred,auto",
]

cflags_jsystem = [
    *cflags_base,
    "-O4,p",
    "-opt all,nostrength",
    "-inline auto",
    "-str reuse,readonly",
    "-lang=c++",
    "-fp hard",
    "-fp_contract on",
    "-char signed",
    "-rostr",
    "-common on"
]

cflags_jsystem_dsp = [
    *cflags_base_base,
    "-lang=c++",
    "-proc 750",
    "-O4",
    "-str readonly",
    "-func_align 32",
]

cflags_game_base = [
    *cflags_base,
    "-O4,p",
    "-inline auto",
    "-fp_contract on",
    "-str reuse,readonly",
]

cflags_game = [
    *cflags_game_base,
    "-opt all,nostrength",
    "-inline deferred",
]

cflags_dolphin = [
    *cflags_base,
    "-O4,p",
    "-inline auto",
    "-fp_contract off", # NOTE: this is definitely off according to mtx.c
    # TODO: should these be different?
]

config.linker_version = "GC/1.2.5"


# Some parts of the SDK were compiled with the 1.2.5n patch, some weren't
def DolphinLib(lib_name: str, objects: List[Object]) -> Dict[str, Any]:
    return {
        "lib": lib_name,
        "mw_version": "GC/1.2.5n",
        "cflags": cflags_dolphin,
        "progress_category": "sdk",
        "objects": objects,
    }

def DolphinLibUnpatched(lib_name: str, objects: List[Object]) -> Dict[str, Any]:
    return {
        "lib": lib_name,
        "mw_version": "GC/1.2.5",
        "cflags": cflags_dolphin,
        "progress_category": "sdk",
        "objects": objects,
    }


Matching = True                   # Object matches and should be linked
NonMatching = False               # Object does not match and should not be linked
Equivalent = config.non_matching  # Object should be linked when configured with --non-matching


# Object is only matching for specific versions
def MatchingFor(*versions):
    return config.version in versions


# The retail build applied the game PCH to a specific subset of translation units.
def PCHObject(completed: bool, name: str) -> Object:
    return Object(completed, name, extra_cflags=["-prefix SMS.mch"])


config.warn_missing_config = True
config.warn_missing_source = False
config.precompiled_headers = [
    {
        "source": "SMS.pch",
        "mw_version": "GC/1.2.5",
        "cflags": ["-lang=c++", *cflags_game_base],
    },
]
config.libs = [
    {
        "lib": "main",
        "mw_version": "GC/1.2.5",
        "cflags": cflags_game,
        "progress_category": "game",
        "objects": [
            Object(Matching, "main.cpp"),
        ],
    },
    {
        "lib": "JSystem",
        "mw_version": "GC/1.2.5",
        "cflags": cflags_jsystem,
        "progress_category": "jsystem",
        "objects": [
            # JSupport
            Object(Matching, "JSystem/JSupport/JSUOutputStream.cpp"),
            Object(Matching, "JSystem/JSupport/JSUInputStream.cpp"),
            Object(Matching, "JSystem/JSupport/JSUList.cpp"),
            Object(Matching, "JSystem/JSupport/JSUMemoryStream.cpp"),
            Object(Matching, "JSystem/JSupport/JSUFileStream.cpp"),

            # JGadget
            Object(Matching, "JSystem/JGadget/std-list.cpp"),
            Object(Matching, "JSystem/JGadget/std-vector.cpp"),
            Object(Matching, "JSystem/JGadget/linklist.cpp"),
            Object(Matching, "JSystem/JGadget/singlelinklist.cpp"),

            # JKernel
            Object(Matching, "JSystem/JKernel/JKRArchivePri.cpp"),
            Object(Matching, "JSystem/JKernel/JKRAramArchive.cpp"),
            Object(Matching, "JSystem/JKernel/JKRAramHeap.cpp"),
            Object(Matching, "JSystem/JKernel/JKRAram.cpp"),
            Object(Matching, "JSystem/JKernel/JKRArchivePub.cpp"),
            Object(Matching, "JSystem/JKernel/JKRCompArchive.cpp"),
            Object(Matching, "JSystem/JKernel/JKRDisposer.cpp"),
            Object(Matching, "JSystem/JKernel/JKRDvdArchive.cpp"),
            Object(Matching, "JSystem/JKernel/JKRDvdFile.cpp"),
            Object(NonMatching, "JSystem/JKernel/JKRExpHeap.cpp"),
            Object(Matching, "JSystem/JKernel/JKRFileCache.cpp"),
            Object(Matching, "JSystem/JKernel/JKRFileFinder.cpp"),
            Object(Matching, "JSystem/JKernel/JKRFileLoader.cpp"),
            Object(Matching, "JSystem/JKernel/JKRHeap.cpp"),
            Object(Matching, "JSystem/JKernel/JKRMemArchive.cpp"),
            Object(Matching, "JSystem/JKernel/JKRSolidHeap.cpp"),
            Object(Matching, "JSystem/JKernel/JKRStdHeap.cpp"),
            Object(Matching, "JSystem/JKernel/JKRThread.cpp"),
            Object(Matching, "JSystem/JKernel/JKRDvdRipper.cpp"),
            Object(Matching, "JSystem/JKernel/JKRDvdAramRipper.cpp"),
            Object(Matching, "JSystem/JKernel/JKRDecomp.cpp"),
            Object(Matching, "JSystem/JKernel/JKRAramBlock.cpp"),
            Object(Matching, "JSystem/JKernel/JKRAramPiece.cpp"),
            Object(Matching, "JSystem/JKernel/JKRAramStream.cpp"),

            # JUtility
            Object(Matching, "JSystem/JUtility/JUTException.cpp"),
            Object(Matching, "JSystem/JUtility/JUTDirectPrint.cpp"),
            Object(Matching, "JSystem/JUtility/JUTDbPrint.cpp"),
            Object(Matching, "JSystem/JUtility/JUTFont.cpp"),
            Object(Matching, "JSystem/JUtility/JUTGamePad.cpp"),
            Object(Matching, "JSystem/JUtility/JUTNameTab.cpp"),
            Object(Matching, "JSystem/JUtility/JUTPalette.cpp"),
            Object(Matching, "JSystem/JUtility/JUTRect.cpp"),
            Object(Matching, "JSystem/JUtility/JUTResource.cpp"),
            Object(Matching, "JSystem/JUtility/JUTTexture.cpp"),
            Object(Matching, "JSystem/JUtility/JUTAssert.cpp"),
            Object(Matching, "JSystem/JUtility/JUTVideo.cpp"),
            Object(Matching, "JSystem/JUtility/JUTResFont.cpp"),
            Object(Matching, "JSystem/JUtility/JUTRomFont.cpp"),
            Object(Matching, "JSystem/JUtility/JUTConsole.cpp"),
            Object(Matching, "JSystem/JUtility/JUTDirectFile.cpp"),

            # JDrama
            Object(NonMatching, "JSystem/JDrama/JDRActor.cpp"),
            Object(NonMatching, "JSystem/JDrama/JDRCamera.cpp"),
            Object(Matching, "JSystem/JDrama/JDRCharacter.cpp"),
            Object(Matching, "JSystem/JDrama/JDRDirector.cpp"),
            Object(NonMatching, "JSystem/JDrama/JDRDisplay.cpp"),
            Object(Matching, "JSystem/JDrama/JDRDrawBufObj.cpp"),
            Object(Matching, "JSystem/JDrama/JDRDStage.cpp"),
            Object(Matching, "JSystem/JDrama/JDRDStageGroup.cpp"),
            Object(Matching, "JSystem/JDrama/JDREfbCtrl.cpp"),
            Object(NonMatching, "JSystem/JDrama/JDREfbSetting.cpp"),
            Object(NonMatching, "JSystem/JDrama/JDRFrmGXSet.cpp"),
            Object(Matching, "JSystem/JDrama/JDRGraphics.cpp"),
            Object(Matching, "JSystem/JDrama/JDRLighting.cpp"),
            Object(Matching, "JSystem/JDrama/JDRNameRef.cpp"),
            Object(Matching, "JSystem/JDrama/JDRNameRefGen.cpp"),
            Object(Matching, "JSystem/JDrama/JDRPlacement.cpp"),
            Object(Matching, "JSystem/JDrama/JDRRenderMode.cpp"),
            Object(Matching, "JSystem/JDrama/JDRResolution.cpp"),
            Object(Matching, "JSystem/JDrama/JDRScreen.cpp"),
            Object(NonMatching, "JSystem/JDrama/JDRSmJ3DAct.cpp"),
            Object(Matching, "JSystem/JDrama/JDRSmJ3DScn.cpp"),
            Object(Matching, "JSystem/JDrama/JDRVideo.cpp"),
            Object(Matching, "JSystem/JDrama/JDRViewConnecter.cpp"),
            Object(Matching, "JSystem/JDrama/JDRViewObj.cpp"),
            Object(Matching, "JSystem/JDrama/JDRViewport.cpp"),

            # JAudio
            # JADebug
            Object(Matching, "JSystem/JAudio/JADebug/JADHioNode.cpp"),
            # JALibrary
            Object(NonMatching, "JSystem/JAudio/JALibrary/JALCalc.cpp"),
            Object(NonMatching, "JSystem/JAudio/JALibrary/JALModSe.cpp"),
            # JAInterface
            Object(Matching, "JSystem/JAudio/JAInterface/JAIAsnData.cpp"),
            Object(NonMatching, "JSystem/JAudio/JAInterface/JAIAnimation.cpp"),
            Object(NonMatching, "JSystem/JAudio/JAInterface/JAIBasic.cpp"),
            Object(Matching, "JSystem/JAudio/JAInterface/JAIConst.cpp"),
            Object(Matching, "JSystem/JAudio/JAInterface/JAIDebug.cpp"),
            Object(NonMatching, "JSystem/JAudio/JAInterface/JAIData.cpp"),
            Object(NonMatching, "JSystem/JAudio/JAInterface/JAIEntry.cpp"),
            Object(Matching, "JSystem/JAudio/JAInterface/JAIEntrySe.cpp"),
            Object(Matching, "JSystem/JAudio/JAInterface/JAIEntrySequence.cpp"),
            Object(Matching, "JSystem/JAudio/JAInterface/JAIEntryStream.cpp"),
            Object(NonMatching, "JSystem/JAudio/JAInterface/JAIGFrameSe.cpp"),
            Object(NonMatching, "JSystem/JAudio/JAInterface/JAIGFrameSequence.cpp"),
            Object(NonMatching, "JSystem/JAudio/JAInterface/JAIGFrameStream.cpp"),
            Object(NonMatching, "JSystem/JAudio/JAInterface/JAIGlobalParameter.cpp"),
            Object(NonMatching, "JSystem/JAudio/JAInterface/JAISound.cpp"),
            Object(NonMatching, "JSystem/JAudio/JAInterface/JAISystemInterface.cpp"),
            # JASystem
            Object(Matching, "JSystem/JAudio/JASystem/JASBank.cpp"),
            Object(NonMatching, "JSystem/JAudio/JASystem/JASBankMgr.cpp"),
            Object(Matching, "JSystem/JAudio/JASystem/JASBasicBank.cpp"),
            Object(Matching, "JSystem/JAudio/JASystem/JASBasicInst.cpp"),
            Object(Matching, "JSystem/JAudio/JASystem/JASBasicWaveBank.cpp"),
            Object(Matching, "JSystem/JAudio/JASystem/JASBNKParser.cpp"),
            Object(Matching, "JSystem/JAudio/JASystem/JASDrumSet.cpp"),
            Object(Matching, "JSystem/JAudio/JASystem/JASInstEffect.cpp"),
            Object(Matching, "JSystem/JAudio/JASystem/JASInstRand.cpp"),
            Object(Matching, "JSystem/JAudio/JASystem/JASInstSense.cpp"),
            Object(Matching, "JSystem/JAudio/JASystem/JASSimpleWaveBank.cpp"),
            Object(Matching, "JSystem/JAudio/JASystem/JASWaveArcLoader.cpp"),
            Object(Matching, "JSystem/JAudio/JASystem/JASWaveBank.cpp"),
            Object(Matching, "JSystem/JAudio/JASystem/JASWaveBankMgr.cpp"),
            Object(Matching, "JSystem/JAudio/JASystem/JASWSParser.cpp"),
            Object(Matching, "JSystem/JAudio/JASystem/JASAudioThread.cpp"),
            Object(Matching, "JSystem/JAudio/JASystem/JASChAllocQueue.cpp"),
            Object(NonMatching, "JSystem/JAudio/JASystem/JASChannel.cpp"),
            Object(Matching, "JSystem/JAudio/JASystem/JASDriverTables.cpp"),
            Object(Matching, "JSystem/JAudio/JASystem/JASChannelMgr.cpp"),
            Object(Matching, "JSystem/JAudio/JASystem/JASChGlobal.cpp"),
            Object(Matching, "JSystem/JAudio/JASystem/JASDriverIF.cpp"),
            Object(Matching, "JSystem/JAudio/JASystem/JASDSPBuf.cpp"),
            Object(NonMatching, "JSystem/JAudio/JASystem/JASDSPChannel.cpp"),
            Object(Matching, "JSystem/JAudio/JASystem/JASDSPInterface.cpp"),
            Object(Matching, "JSystem/JAudio/JASystem/JASOscillator.cpp"),
            Object(Matching, "JSystem/JAudio/JASystem/JASAiCtrl.cpp"),
            Object(Matching, "JSystem/JAudio/JASystem/JASCalc.cpp"),
            Object(Matching, "JSystem/JAudio/JASystem/JASCallback.cpp"),
            Object(Matching, "JSystem/JAudio/JASystem/JASCmdStack.cpp"),
            Object(Matching, "JSystem/JAudio/JASystem/JASDvdThread.cpp"),
            Object(NonMatching, "JSystem/JAudio/JASystem/JASHardStream.cpp"),
            Object(Matching, "JSystem/JAudio/JASystem/JASHeapCtrl.cpp"),
            Object(Matching, "JSystem/JAudio/JASystem/JASRate.cpp"),
            Object(Matching, "JSystem/JAudio/JASystem/JASKernelDebug.cpp"),
            Object(Matching, "JSystem/JAudio/JASystem/JASProbe.cpp"),
            Object(Matching, "JSystem/JAudio/JASystem/JASSystemHeap.cpp"),
            Object(Matching, "JSystem/JAudio/JASystem/JASVload.cpp"),
            Object(Matching, "JSystem/JAudio/JASystem/JASNoteMgr.cpp"),
            Object(Matching, "JSystem/JAudio/JASystem/JASOuterParam.cpp"),
            Object(Matching, "JSystem/JAudio/JASystem/JASPlayer_impl.cpp"),
            Object(Matching, "JSystem/JAudio/JASystem/JASRegisterParam.cpp"),
            Object(Matching, "JSystem/JAudio/JASystem/JASSeqCtrl.cpp"),
            Object(NonMatching, "JSystem/JAudio/JASystem/JASTrack.cpp"),
            Object(Matching, "JSystem/JAudio/JASystem/JASTrackInterrupt.cpp"),
            Object(Matching, "JSystem/JAudio/JASystem/JASTrackMgr.cpp"),
            Object(Matching, "JSystem/JAudio/JASystem/JASTrackPort.cpp"),
            Object(Matching, "JSystem/JAudio/JASystem/JASSeqParser.cpp"),

            # J2D
            Object(Matching, "JSystem/J2D/J2DPane.cpp"),
            Object(Matching, "JSystem/J2D/J2DPicture.cpp"),
            Object(Matching, "JSystem/J2D/J2DPrint.cpp"),
            Object(Matching, "JSystem/J2D/J2DScreen.cpp"),
            Object(Matching, "JSystem/J2D/J2DTextBox.cpp"),
            Object(Matching, "JSystem/J2D/J2DWindow.cpp"),
            Object(Matching, "JSystem/J2D/J2DGrafContext.cpp"),
            Object(Matching, "JSystem/J2D/J2DOrthoGraph.cpp"),

            # J3D
            # J3DGraphBase
            Object(Matching, "JSystem/J3D/J3DGraphBase/J3DTransform.cpp"),
            Object(Matching, "JSystem/J3D/J3DGraphBase/J3DMaterial.cpp"),
            Object(Matching, "JSystem/J3D/J3DGraphBase/J3DShape.cpp"),
            Object(Matching, "JSystem/J3D/J3DGraphBase/J3DSys.cpp"),
            Object(Matching, "JSystem/J3D/J3DGraphBase/J3DVertex.cpp"),
            Object(Matching, "JSystem/J3D/J3DGraphBase/J3DPacket.cpp"),
            Object(Matching, "JSystem/J3D/J3DGraphBase/J3DTevs.cpp"),
            Object(Matching, "JSystem/J3D/J3DGraphBase/J3DDrawBuffer.cpp"),
            # J3DGraphAnimator
            Object(NonMatching, "JSystem/J3D/J3DGraphAnimator/J3DCluster.cpp"),
            Object(Matching, "JSystem/J3D/J3DGraphAnimator/J3DJoint.cpp"),
            Object(NonMatching, "JSystem/J3D/J3DGraphAnimator/J3DModel.cpp"),
            Object(Matching, "JSystem/J3D/J3DGraphAnimator/J3DNode.cpp"),
            Object(Matching, "JSystem/J3D/J3DGraphAnimator/J3DAnimation.cpp"),
            Object(Matching, "JSystem/J3D/J3DGraphAnimator/J3DMaterialAnm.cpp"),
            Object(Matching, "JSystem/J3D/J3DGraphAnimator/J3DMaterialAttach.cpp"),
            # J3DGraphLoader
            Object(Matching, "JSystem/J3D/J3DGraphLoader/J3DClusterLoader.cpp"),
            Object(Matching, "JSystem/J3D/J3DGraphLoader/J3DJointFactory.cpp"),
            Object(NonMatching, "JSystem/J3D/J3DGraphLoader/J3DMaterialFactory.cpp"),
            Object(Matching, "JSystem/J3D/J3DGraphLoader/J3DModelLoader.cpp"),
            Object(Matching, "JSystem/J3D/J3DGraphLoader/J3DShapeFactory.cpp"),
            Object(Matching, "JSystem/J3D/J3DGraphLoader/J3DAnmLoader.cpp"),
            Object(NonMatching, "JSystem/J3D/J3DGraphLoader/J3DMaterialFactory_v21.cpp"),

            # JParticle
            Object(NonMatching, "JSystem/JParticle/JPAEmitter.cpp"),
            Object(NonMatching, "JSystem/JParticle/JPAEmitterManager.cpp"),
            Object(NonMatching, "JSystem/JParticle/JPAField.cpp"),
            Object(NonMatching, "JSystem/JParticle/JPAMath.cpp"),
            Object(NonMatching, "JSystem/JParticle/JPAParticle.cpp"),
            Object(Matching, "JSystem/JParticle/JPADrawSetupTev.cpp"),
            Object(NonMatching, "JSystem/JParticle/JPADraw.cpp"),
            Object(NonMatching, "JSystem/JParticle/JPADrawVisitor.cpp"),
            Object(Matching, "JSystem/JParticle/JPAExTexShape.cpp"),
            Object(Matching, "JSystem/JParticle/JPADataBlock.cpp"),
            Object(Matching, "JSystem/JParticle/JPAEmitterLoader.cpp"),
            Object(Matching, "JSystem/JParticle/JPABaseShape.cpp"),
            Object(Matching, "JSystem/JParticle/JPAExtraShape.cpp"),
            Object(Matching, "JSystem/JParticle/JPAResourceManager.cpp"),
            Object(Matching, "JSystem/JParticle/JPASweepShape.cpp"),
            Object(Matching, "JSystem/JParticle/JPATexture.cpp"),

            # JStage
            Object(Matching, "JSystem/JStage/JSGAmbientLight.cpp"),
            Object(Matching, "JSystem/JStage/JSGCamera.cpp"),
            Object(Matching, "JSystem/JStage/JSGActor.cpp"),
            Object(Matching, "JSystem/JStage/JSGLight.cpp"),
            Object(Matching, "JSystem/JStage/JSGObject.cpp"),
            Object(Matching, "JSystem/JStage/JSGSystem.cpp"),

            Object(Matching, "JSystem/JMath.cpp"),
            Object(Matching, "JSystem/JRenderer.cpp"),
            Object(Matching, "JSystem/random.cpp"),
            Object(Matching, "JSystem/dspproc.c", cflags=cflags_jsystem_dsp, mw_version="GC/1.2.5n"),
            Object(Matching, "JSystem/dsptask.c", cflags=cflags_jsystem_dsp, mw_version="GC/1.2.5n"),
            Object(Matching, "JSystem/osdsp.c", cflags=cflags_jsystem_dsp, mw_version="GC/1.2.5n"),
            Object(Matching, "JSystem/osdsp_task.c", cflags=cflags_jsystem_dsp, mw_version="GC/1.2.5n"),
        ],
    },
    {
        "lib": "Runtime.PPCEABI.H",
        "progress_category": "sdk",
        "mw_version": "GC/1.2.5",
        "cflags": cflags_runtime,
        "objects": [
            Object(Matching, "PowerPC_EABI_Support/Runtime/__mem.c"),
            Object(Matching, "PowerPC_EABI_Support/Runtime/__va_arg.c"),
            Object(Matching, "PowerPC_EABI_Support/Runtime/global_destructor_chain.c"),
            Object(Matching, "PowerPC_EABI_Support/Runtime/NMWException.cp"),
            Object(Matching, "PowerPC_EABI_Support/Runtime/ptmf.c"),
            Object(Matching, "PowerPC_EABI_Support/Runtime/ExceptionPPC.cp"),
            Object(Matching, "PowerPC_EABI_Support/Runtime/runtime.c"),
            Object(Matching, "PowerPC_EABI_Support/Runtime/__init_cpp_exceptions.cpp"),
        ],
    },
    {
        "lib": "MSL_C.PPCEABI.bare.H",
        "progress_category": "sdk",
        "mw_version": "GC/1.2.5",
        "cflags": cflags_runtime,
        "objects": [
            Object(Matching, "PowerPC_EABI_Support/Msl/MSL_C/MSL_Common/ansi_files.c"),
            Object(Matching, "PowerPC_EABI_Support/Msl/MSL_C/MSL_Common/abort_exit.c"),
            Object(Matching, "PowerPC_EABI_Support/Msl/MSL_C/MSL_Common/errno.c"),
            Object(NonMatching, "PowerPC_EABI_Support/Msl/MSL_C/MSL_Common_Embedded/ansi_fp.c"),
            Object(Matching, "PowerPC_EABI_Support/Msl/MSL_C/MSL_Common_Embedded/uart_console_io.c"),
            Object(Matching, "PowerPC_EABI_Support/Msl/MSL_C/MSL_Common/buffer_io.c"),
            Object(Matching, "PowerPC_EABI_Support/Msl/MSL_C/PPC_EABI/critical_regions.ppc_eabi.c"),
            Object(Matching, "PowerPC_EABI_Support/Msl/MSL_C/MSL_Common/ctype.c"),
            Object(Matching, "PowerPC_EABI_Support/Msl/MSL_C/MSL_Common/direct_io.c"),
            Object(Matching, "PowerPC_EABI_Support/Msl/MSL_C/MSL_Common/mbstring.c"),
            Object(Matching, "PowerPC_EABI_Support/Msl/MSL_C/MSL_Common/mem.c"),
            Object(Matching, "PowerPC_EABI_Support/Msl/MSL_C/MSL_Common/mem_funcs.c"),
            Object(Matching, "PowerPC_EABI_Support/Msl/MSL_C/MSL_Common/misc_io.c"),
            Object(Matching, "PowerPC_EABI_Support/Msl/MSL_C/MSL_Common/printf.c"),
            Object(Matching, "PowerPC_EABI_Support/Msl/MSL_C/MSL_Common/rand.c"),
            Object(Matching, "PowerPC_EABI_Support/Msl/MSL_C/MSL_Common/scanf.c"),
            Object(Matching, "PowerPC_EABI_Support/Msl/MSL_C/MSL_Common/string.c"),
            Object(Matching, "PowerPC_EABI_Support/Msl/MSL_C/MSL_Common/strtoul.c"),
            Object(Matching, "PowerPC_EABI_Support/Msl/MSL_C/MSL_Common/float.c"),
            Object(Matching, "PowerPC_EABI_Support/Msl/MSL_C/MSL_Common_Embedded/uart_consolle_io.c"),
            Object(Matching, "PowerPC_EABI_Support/Msl/MSL_C/MSL_Common/wchar_io.c"),
            Object(Matching, "PowerPC_EABI_Support/Msl/MSL_C/MSL_Common_Embedded/Math/Double_precision/e_asin.c"),
            Object(Matching, "PowerPC_EABI_Support/Msl/MSL_C/MSL_Common_Embedded/Math/Double_precision/e_atan2.c"),
            Object(Matching, "PowerPC_EABI_Support/Msl/MSL_C/MSL_Common_Embedded/Math/Double_precision/s_atan.c"),
            Object(Matching, "PowerPC_EABI_Support/Msl/MSL_C/MSL_Common_Embedded/Math/Double_precision/s_frexp.c"),
            Object(Matching, "PowerPC_EABI_Support/Msl/MSL_C/MSL_Common_Embedded/Math/Double_precision/w_atan2.c"),
            Object(Matching, "PowerPC_EABI_Support/Msl/MSL_C/MSL_Common_Embedded/Math/Single_precision/hyperbolicsf.c"),
            Object(Matching, "PowerPC_EABI_Support/Msl/MSL_C/MSL_Common_Embedded/Math/Single_precision/inverse_trig.c"),
            Object(Matching, "PowerPC_EABI_Support/Msl/MSL_C/MSL_Common_Embedded/Math/Single_precision/trigf.c"),
            Object(Matching, "PowerPC_EABI_Support/Msl/MSL_C/MSL_Common_Embedded/Math/Single_precision/common_float_tables.c"),
            Object(NonMatching, "PowerPC_EABI_Support/Msl/MSL_C/MSL_Common_Embedded/Math/Single_precision/exponentialsf.c"),
        ],
    },
    {
        "lib": "TRK_MINNOW_DOLPHIN",
        "mw_version": "GC/1.1p1",
        "cflags": [*cflags_base, "-O4,p", "-pool off", "-str readonly", "-enum min", "-sdatathreshold 0"],
        "progress_category": "sdk",
        "objects": [
            Object(Matching, "TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Portable/mainloop.c"),
            Object(Matching, "TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Portable/nubevent.c"),
            Object(Matching, "TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Portable/nubinit.c"),
            Object(Matching, "TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Portable/msg.c"),
            Object(Matching, "TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Portable/msgbuf.c"),
            Object(Matching, "TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Portable/serpoll.c"),
            Object(Matching, "TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Os/dolphin/usr_put.c"),
            Object(Matching, "TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Portable/dispatch.c"),
            Object(Matching, "TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Portable/msghndlr.c"),
            Object(NonMatching, "TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Portable/support.c"),
            Object(Matching, "TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Portable/mutex_TRK.c"),
            Object(Matching, "TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Portable/notify.c"),
            Object(Matching, "TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Processor/ppc/Generic/flush_cache.c"),
            Object(Matching, "TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Portable/mem_TRK.c"),
            Object(Matching, "TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Processor/ppc/Generic/targimpl.c"),
            Object(Matching, "TRK_MINNOW_DOLPHIN/__exception.s"),
            Object(Matching, "TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Os/dolphin/dolphin_trk.c"),
            Object(Matching, "TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Processor/ppc/Generic/mpc_7xx_603e.c"),
            Object(Matching, "TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Portable/main_TRK.c"),
            Object(Matching, "TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Os/dolphin/dolphin_trk_glue.c"),
            Object(Matching, "TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Os/dolphin/targcont.c"),
        ],
    },
    DolphinLib("base", [
            Object(Matching, "dolphin/base/PPCArch.c"),
        ]),
    DolphinLib("db", [
            Object(Matching, "dolphin/db/db.c"),
        ]),
    DolphinLib("os", [
            Object(Matching, "dolphin/os/OS.c"),
            Object(Matching, "dolphin/os/OSAlarm.c"),
            Object(Matching, "dolphin/os/OSAlloc.c"),
            Object(Matching, "dolphin/os/OSArena.c"),
            Object(Matching, "dolphin/os/OSAudioSystem.c"),
            Object(Matching, "dolphin/os/OSCache.c"),
            Object(Matching, "dolphin/os/OSContext.c"),
            Object(Matching, "dolphin/os/OSError.c"),
            Object(Matching, "dolphin/os/OSFont.c"),
            Object(Matching, "dolphin/os/OSInterrupt.c"),
            Object(Matching, "dolphin/os/OSLink.c"),
            Object(Matching, "dolphin/os/OSMessage.c"),
            Object(Matching, "dolphin/os/OSMemory.c"),
            Object(Matching, "dolphin/os/OSMutex.c"),
            Object(Matching, "dolphin/os/OSReboot.c"),
            Object(Matching, "dolphin/os/OSReset.c"),
            Object(Matching, "dolphin/os/OSResetSW.c"),
            Object(Matching, "dolphin/os/OSRtc.c"),
            Object(Matching, "dolphin/os/OSStopwatch.c"),
            Object(Matching, "dolphin/os/OSSync.c"),
            Object(Matching, "dolphin/os/OSThread.c"),
            Object(Matching, "dolphin/os/OSTime.c"),
            Object(Matching, "dolphin/os/__start.c"),
            Object(Matching, "dolphin/os/__ppc_eabi_init.cpp"),
        ]),
    DolphinLibUnpatched("mtx", [
            Object(Matching, "dolphin/mtx/mtx.c"),
            Object(Matching, "dolphin/mtx/mtxvec.c"),
            Object(Matching, "dolphin/mtx/mtx44.c"),
            Object(Matching, "dolphin/mtx/vec.c"),
        ]),
    DolphinLib("dvd", [
            Object(Matching, "dolphin/dvd/dvdlow.c"),
            Object(Matching, "dolphin/dvd/dvdfs.c"),
            Object(Matching, "dolphin/dvd/dvd.c"),
            Object(Matching, "dolphin/dvd/dvdqueue.c"),
            Object(Matching, "dolphin/dvd/dvderror.c"),
            Object(Matching, "dolphin/dvd/fstload.c"),
        ]),
    DolphinLib("vi", [
            Object(Matching, "dolphin/vi/vi.c"),
        ]),
    DolphinLib("pad", [
            Object(Matching, "dolphin/pad/Padclamp.c"),
            Object(Matching, "dolphin/pad/Pad.c"),
        ]),
    DolphinLib("ai", [
            Object(Matching, "dolphin/ai/ai.c"),
        ]),
    DolphinLib("ar", [
            Object(Matching, "dolphin/ar/ar.c"),
            Object(Matching, "dolphin/ar/arq.c"),
        ]),
    DolphinLib("dsp", [
            Object(Matching, "dolphin/dsp/dsp.c"),
            Object(Matching, "dolphin/dsp/dsp_debug.c"),
            Object(Matching, "dolphin/dsp/dsp_task.c"),
        ]),
    DolphinLib("card", [
            Object(Matching, "dolphin/card/CARDBios.c"),
            Object(Matching, "dolphin/card/CARDUnlock.c"),
            Object(Matching, "dolphin/card/CARDRdwr.c"),
            Object(Matching, "dolphin/card/CARDBlock.c"),
            Object(Matching, "dolphin/card/CARDDir.c"),
            Object(Matching, "dolphin/card/CARDCheck.c"),
            Object(Matching, "dolphin/card/CARDMount.c"),
            Object(Matching, "dolphin/card/CARDFormat.c"),
            Object(Matching, "dolphin/card/CARDOpen.c"),
            Object(Matching, "dolphin/card/CARDCreate.c"),
            Object(Matching, "dolphin/card/CARDRead.c"),
            Object(Matching, "dolphin/card/CARDWrite.c"),
            Object(Matching, "dolphin/card/CARDStat.c"),
            Object(Matching, "dolphin/card/CARDNet.c"),
        ]),
    DolphinLib("gx", [
            Object(Matching, "dolphin/gx/GXInit.c"),
            Object(Matching, "dolphin/gx/GXFifo.c"),
            Object(Matching, "dolphin/gx/GXAttr.c"),
            Object(Matching, "dolphin/gx/GXMisc.c"),
            Object(Matching, "dolphin/gx/GXGeometry.c"),
            Object(Matching, "dolphin/gx/GXFrameBuf.c"),
            Object(Matching, "dolphin/gx/GXLight.c"),
            Object(Matching, "dolphin/gx/GXTexture.c"),
            Object(Matching, "dolphin/gx/GXBump.c"),
            Object(Matching, "dolphin/gx/GXTev.c"),
            Object(Matching, "dolphin/gx/GXPixel.c"),
            Object(Matching, "dolphin/gx/GXDraw.c"),
            Object(Matching, "dolphin/gx/GXStubs.c"),
            Object(Matching, "dolphin/gx/GXDisplayList.c"),
            Object(Matching, "dolphin/gx/GXTransform.c"),
            Object(Matching, "dolphin/gx/GXPerf.c"),
        ]),
    DolphinLib("OdemuExi2", [
            Object(Matching, "OdemuExi2/DebuggerDriver.c", cflags=[*cflags_dolphin, "-inline auto,deferred"]),
        ]),
    DolphinLib("amcstubs", [
            Object(Matching, "dolphin/amcstubs/AmcExi2Stubs.c"),
        ]),
    DolphinLib("odenotstub", [
            Object(Matching, "dolphin/odenotstub/odenotstub.c"),
        ]),
    DolphinLib("gd", [
            Object(Matching, "dolphin/gd/GDBase.c"),
            Object(Matching, "dolphin/gd/GDGeometry.c"),
            Object(Matching, "dolphin/gd/GDLight.c"),
            Object(Matching, "dolphin/gd/GDPixel.c"),
            Object(Matching, "dolphin/gd/GDTev.c"),
            Object(Matching, "dolphin/gd/GDTransform.c"),
        ]),
    DolphinLib("si", [
            Object(Matching, "dolphin/si/SIBios.c"),
            Object(Matching, "dolphin/si/SISamplingRate.c"),
        ]),
    DolphinLib("exi", [
            Object(Matching, "dolphin/exi/EXIBios.c"),
            Object(Matching, "dolphin/exi/EXIUart.c"),
        ]),
    DolphinLibUnpatched("thp", [
            Object(Matching, "dolphin/thp/THPDec.c"),
            Object(Matching, "dolphin/thp/THPAudio.c"),
        ]),
    # Note that this is NOT in fact part of the SDK, as it integrates
    # with jsystem and game code
    {
        "lib": "THPPlayer",
        "mw_version": "GC/1.2.5",
        "cflags": [*cflags_base, "-O4,p", "-inline auto", "-fp_contract on", "-str reuse,readonly", "-lang=c++", "-inline deferred"],
        "progress_category": "game",
        "objects": [
            Object(NonMatching, "THPPlayer/THPAudioDecode.c"),
            Object(Matching, "THPPlayer/THPDraw.c"),
            Object(NonMatching, "THPPlayer/THPPlayer.c"),
            Object(Matching, "THPPlayer/THPRead.c"),
            Object(Matching, "THPPlayer/THPVideoDecode.c"),
        ],
    },
    {
        "lib": "MarioUtil",
        "mw_version": "GC/1.2.5",
        "cflags": cflags_game,
        "progress_category": "game",
        "objects": [
            PCHObject(Matching, "MarioUtil/DLUtil.cpp"),
            PCHObject(NonMatching, "MarioUtil/DrawUtil.cpp"),
            PCHObject(NonMatching, "MarioUtil/LightUtil.cpp"),
            PCHObject(NonMatching, "MarioUtil/MathUtil.cpp"),
            PCHObject(NonMatching, "MarioUtil/MtxUtil.cpp"),
            PCHObject(NonMatching, "MarioUtil/ScreenUtil.cpp"),
            PCHObject(NonMatching, "MarioUtil/ShadowUtil.cpp"),
            Object(Matching, "MarioUtil/gd-reinit-gx.cpp"),
            Object(NonMatching, "MarioUtil/EffectUtil.cpp"),
            Object(NonMatching, "MarioUtil/ModelUtil.cpp"),
            Object(Matching, "MarioUtil/RumbleMgr.cpp"),
            Object(Matching, "MarioUtil/RumbleData.cpp"),
            Object(Matching, "MarioUtil/RumbleType.cpp"),
            PCHObject(NonMatching, "MarioUtil/PacketUtil.cpp"),
            PCHObject(Matching, "MarioUtil/GDUtil.cpp"),
            Object(Matching, "MarioUtil/TexUtil.cpp"),
            Object(Matching, "MarioUtil/MapUtil.cpp"),
            PCHObject(Matching, "MarioUtil/ToolData.cpp"),
        ],
    },
    {
        "lib": "M3DUtil",
        "mw_version": "GC/1.2.5",
        "cflags": cflags_game,
        "progress_category": "game",
        "objects": [
            PCHObject(NonMatching, "M3DUtil/M3UJoint.cpp"),
            PCHObject(NonMatching, "M3DUtil/M3UModel.cpp"),
            PCHObject(NonMatching, "M3DUtil/MActor.cpp"),
            PCHObject(Matching, "M3DUtil/MActorAnm.cpp"),
            Object(NonMatching, "M3DUtil/MActorData.cpp"),
            PCHObject(NonMatching, "M3DUtil/SDLModel.cpp"),
            Object(Matching, "M3DUtil/MActorUtil.cpp"),
            PCHObject(NonMatching, "M3DUtil/SampleCtrlNode.cpp"),
            PCHObject(Matching, "M3DUtil/SampleCtrlModel.cpp"),
            Object(Matching, "M3DUtil/MotionBlendCtrl.cpp"),
            Object(Matching, "M3DUtil/LodAnm.cpp"),
        ],
    },
    {
        "lib": "System",
        "mw_version": "GC/1.2.5",
        "cflags": cflags_game,
        "progress_category": "game",
        "objects": [
            PCHObject(Matching, "System/BaseParam.cpp"),
            PCHObject(NonMatching, "System/EmitterViewObj.cpp"),
            PCHObject(NonMatching, "System/EventWatcher.cpp"),
            PCHObject(NonMatching, "System/FlagManager.cpp"),
            PCHObject(NonMatching, "System/GCLogoDir.cpp"),
            PCHObject(Matching, "System/J3DSysFlag.cpp"),
            Object(NonMatching, "System/MarDirector.cpp"),
            PCHObject(NonMatching, "System/MarDirectorDirect.cpp"),
            PCHObject(NonMatching, "System/MarDirectorEvent.cpp"),
            PCHObject(NonMatching, "System/MarDirectorInitECT.cpp"),
            PCHObject(NonMatching, "System/MarDirectorPreEntry.cpp"),
            PCHObject(NonMatching, "System/MarDirectorSetup2.cpp"),
            Object(Matching, "System/marerr.cpp"),
            PCHObject(NonMatching, "System/MarNameRefGen.cpp"),
            PCHObject(NonMatching, "System/MenuDir.cpp"),
            Object(Matching, "System/Params.cpp"),
            Object(Matching, "System/ParamInst.cpp"),
            PCHObject(NonMatching, "System/PerformList.cpp"),
            PCHObject(NonMatching, "System/RenderModeObj.cpp"),
            PCHObject(NonMatching, "System/SnapTimeObj.cpp"),
            PCHObject(NonMatching, "System/TalkCursor.cpp"),
            Object(Matching, "System/TexCache.cpp"),
            PCHObject(Matching, "System/ZBufferCatch.cpp"),
            PCHObject(NonMatching, "System/Application.cpp"),
            PCHObject(Matching, "System/ScenarioArchiveName.cpp"),
            Object(NonMatching, "System/MarioGamePad.cpp"),
            PCHObject(Matching, "System/StageEventInfo.cpp"),
            PCHObject(Matching, "System/StageUtil.cpp"),
            Object(Matching, "System/Resolution.cpp"),
            PCHObject(Matching, "System/PositionHolder.cpp"),
            Object(Matching, "System/ProcessMeter.cpp"),
            PCHObject(NonMatching, "System/TimeRec.cpp"),
            Object(NonMatching, "System/DrawSyncManager.cpp"),
            PCHObject(Matching, "System/THPRender.cpp"),
            PCHObject(NonMatching, "System/MarNameRefGen_BossEnemy.cpp"),
            PCHObject(NonMatching, "System/MarNameRefGen_Enemy.cpp"),
            PCHObject(NonMatching, "System/MarNameRefGen_Map.cpp"),
            PCHObject(NonMatching, "System/MarNameRefGen_MapObj.cpp"),
            PCHObject(NonMatching, "System/MarNameRefGen_NPC.cpp"),
            PCHObject(NonMatching, "System/CardManager.cpp"),
            PCHObject(NonMatching, "System/MarDirectorLoadResource.cpp"),
            PCHObject(NonMatching, "System/MovieDirector.cpp"),
            Object(Matching, "System/MarDirectorCreateObjects.cpp"),
            PCHObject(NonMatching, "System/MarDirectorSetupObjects.cpp"),
            PCHObject(NonMatching, "System/MSoundMainSide.cpp"),
            PCHObject(Matching, "System/TargetArrow.cpp"),
        ],
    },
    {
        "lib": "Strategic",
        "mw_version": "GC/1.2.5",
        "cflags": cflags_game,
        "progress_category": "game",
        "objects": [
            PCHObject(NonMatching, "Strategic/liveactor.cpp"),
            PCHObject(NonMatching, "Strategic/liveinterp.cpp"),
            PCHObject(NonMatching, "Strategic/livemanager.cpp"),
            Object(NonMatching, "Strategic/ObjHitCheck.cpp"),
            PCHObject(NonMatching, "Strategic/objmanager.cpp"),
            PCHObject(NonMatching, "Strategic/ObjModel.cpp"),
            Object(NonMatching, "Strategic/spcinterp.cpp"),
            PCHObject(NonMatching, "Strategic/Strategy.cpp"),
            PCHObject(NonMatching, "Strategic/question.cpp"),
            PCHObject(Matching, "Strategic/smplcharacter.cpp"),
            PCHObject(NonMatching, "Strategic/HitActor.cpp"),
            PCHObject(Matching, "Strategic/binder.cpp"),
            PCHObject(Matching, "Strategic/SharedParts.cpp"),
            PCHObject(NonMatching, "Strategic/MirrorActor.cpp"),
        ],
    },
    {
        "lib": "Player",
        "mw_version": "GC/1.2.5",
        "cflags": cflags_game,
        "progress_category": "game",
        "objects": [
            Object(Matching, "Player/Atom.cpp"),
            Object(Matching, "Player/MarioAction.cpp"),
            Object(NonMatching, "Player/MarioAutodemo.cpp"),
            PCHObject(NonMatching, "Player/MarioBlend.cpp"),
            PCHObject(NonMatching, "Player/MarioCap.cpp"),
            PCHObject(NonMatching, "Player/MarioCollision.cpp"),
            PCHObject(NonMatching, "Player/MarioDraw.cpp"),
            Object(NonMatching, "Player/MarioJump.cpp"),
            Object(NonMatching, "Player/MarioMain.cpp"),
            PCHObject(NonMatching, "Player/MarioMove.cpp"),
            Object(NonMatching, "Player/MarioPhysics.cpp"),
            Object(Matching, "Player/MarioRecord.cpp"),
            PCHObject(NonMatching, "Player/MarioRun.cpp"),
            Object(NonMatching, "Player/MarioSpecial.cpp"),
            Object(NonMatching, "Player/MarioUpper.cpp"),
            PCHObject(NonMatching, "Player/MarioParticle.cpp"),
            Object(NonMatching, "Player/MarioWait.cpp"),
            PCHObject(NonMatching, "Player/SplashManager.cpp"),
            PCHObject(NonMatching, "Player/Tongue.cpp"),
            PCHObject(NonMatching, "Player/WaterGun.cpp"),
            PCHObject(NonMatching, "Player/Yoshi.cpp"),
            PCHObject(NonMatching, "Player/MarioEffect.cpp"),
            Object(NonMatching, "Player/MarioSwim.cpp"),
            Object(NonMatching, "Player/MarioAccess.cpp"),
            PCHObject(NonMatching, "Player/MarioInit.cpp"),
            PCHObject(NonMatching, "Player/ModelWaterManager.cpp"),
            PCHObject(Matching, "Player/MarioPositionObj.cpp"),
            Object(NonMatching, "Player/MarioCheckCol.cpp"),
            PCHObject(NonMatching, "Player/MarioReceiveMsg.cpp"),
            Object(NonMatching, "Player/MarioSound.cpp"),
        ],
    },
    {
        "lib": "NPC",
        "mw_version": "GC/1.2.5",
        "cflags": cflags_game,
        "progress_category": "game",
        "objects": [
            PCHObject(NonMatching, "NPC/NpcAnm.cpp"),
            PCHObject(NonMatching, "NPC/NpcBase.cpp"),
            Object(NonMatching, "NPC/NpcCallback.cpp"),
            PCHObject(NonMatching, "NPC/NpcManager.cpp"),
            PCHObject(NonMatching, "NPC/NpcNerve.cpp"),
            PCHObject(Matching, "NPC/NpcSave.cpp"),
            PCHObject(NonMatching, "NPC/NpcEvent.cpp"),
            PCHObject(Matching, "NPC/NpcInitData.cpp"),
            PCHObject(NonMatching, "NPC/NpcInitPrg.cpp"),
            Object(NonMatching, "NPC/NpcInbetween.cpp"),
            PCHObject(NonMatching, "NPC/NpcParts.cpp"),
            Object(NonMatching, "NPC/NpcColor.cpp"),
            PCHObject(Matching, "NPC/NpcSound.cpp"),
            Object(NonMatching, "NPC/NpcChange.cpp"),
            Object(NonMatching, "NPC/NpcThrow.cpp"),
            Object(Matching, "NPC/NpcTrample.cpp"),
            Object(NonMatching, "NPC/NpcEffect.cpp"),
            PCHObject(Matching, "NPC/NpcInitAnmData.cpp"),
            PCHObject(Matching, "NPC/NpcInitActionData.cpp"),
            Object(NonMatching, "NPC/NpcCoin.cpp"),
            Object(NonMatching, "NPC/NpcBalloon.cpp"),
            Object(NonMatching, "NPC/NpcWalkTurn.cpp"),
            Object(NonMatching, "NPC/NpcCollision.cpp"),

        ],
    },
    {
        "lib": "MSound",
        "mw_version": "GC/1.2.5",
        "cflags": cflags_game,
        "progress_category": "game",
        "objects": [
            PCHObject(NonMatching, "MSound/MAnmSound.cpp"),
            PCHObject(NonMatching, "MSound/MSound.cpp"),
            PCHObject(NonMatching, "MSound/MSoundBGM.cpp"),
            Object(Matching, "MSound/MSoundDebug.cpp"),
            Object(NonMatching, "MSound/MSoundScene.cpp"),
            PCHObject(NonMatching, "MSound/MSoundSE.cpp"),
            PCHObject(NonMatching, "MSound/MSoundStruct.cpp"),
            PCHObject(NonMatching, "MSound/MSHandle.cpp"),
            PCHObject(NonMatching, "MSound/MSModBgm.cpp"),
        ],
    },
    {
        "lib": "MoveBG",
        "mw_version": "GC/1.2.5",
        "cflags": cflags_game,
        "progress_category": "game",
        "objects": [
            PCHObject(NonMatching, "MoveBG/WoodBarrel.cpp"),
            PCHObject(NonMatching, "MoveBG/MapObjBase.cpp"),
            PCHObject(NonMatching, "MoveBG/MapObjInit.cpp"),
            PCHObject(NonMatching, "MoveBG/MapObjGeneral.cpp"),
            PCHObject(NonMatching, "MoveBG/MapObjManager.cpp"),
            PCHObject(NonMatching, "MoveBG/MapObjLib.cpp"),
            PCHObject(NonMatching, "MoveBG/Item.cpp"),
            PCHObject(NonMatching, "MoveBG/ItemManager.cpp"),
            PCHObject(NonMatching, "MoveBG/MapObjTown.cpp"),
            PCHObject(NonMatching, "MoveBG/MapObjBlock.cpp"),
            PCHObject(NonMatching, "MoveBG/MapObjBianco.cpp"),
            PCHObject(NonMatching, "MoveBG/MapObjSirena.cpp"),
            PCHObject(NonMatching, "MoveBG/MapObjRicco.cpp"),
            PCHObject(NonMatching, "MoveBG/MapObjMamma.cpp"),
            PCHObject(NonMatching, "MoveBG/MapObjPinna.cpp"),
            PCHObject(Matching, "MoveBG/MapObjSample.cpp"),
            PCHObject(NonMatching, "MoveBG/MapObjMare.cpp"),
            PCHObject(NonMatching, "MoveBG/MapObjFlag.cpp"),
            PCHObject(NonMatching, "MoveBG/MapObjWave.cpp"),
            PCHObject(NonMatching, "MoveBG/MapObjFloat.cpp"),
            PCHObject(NonMatching, "MoveBG/MapObjPlane.cpp"),
            PCHObject(NonMatching, "MoveBG/MapObjCloud.cpp"),
            PCHObject(NonMatching, "MoveBG/MapObjBall.cpp"),
            PCHObject(Matching, "MoveBG/MapObjAirport.cpp"),
            PCHObject(NonMatching, "MoveBG/MapObjDolpic.cpp"),
            PCHObject(NonMatching, "MoveBG/MapObjPollution.cpp"),
            PCHObject(NonMatching, "MoveBG/MapObjGrass.cpp"),
            PCHObject(Matching, "MoveBG/MapObjPole.cpp"),
            PCHObject(NonMatching, "MoveBG/MapObjWater.cpp"),
            PCHObject(NonMatching, "MoveBG/ModelGate.cpp"),
            PCHObject(NonMatching, "MoveBG/MapObjFence.cpp"),
            PCHObject(NonMatching, "MoveBG/MapObjOption.cpp"),
            PCHObject(NonMatching, "MoveBG/MapObjRailBlock.cpp"),
            PCHObject(NonMatching, "MoveBG/MapObjMonte.cpp"),
            PCHObject(NonMatching, "MoveBG/MapObjTree.cpp"),
            Object(Matching, "MoveBG/MapObjTumblePole.cpp"),
            PCHObject(NonMatching, "MoveBG/MapObjEx.cpp"),
            PCHObject(Matching, "MoveBG/Pool.cpp"),
            PCHObject(NonMatching, "MoveBG/MapObjCorona.cpp"),
            PCHObject(NonMatching, "MoveBG/MapObjItem2.cpp"),
            PCHObject(NonMatching, "MoveBG/MapObjHide.cpp"),
            PCHObject(NonMatching, "MoveBG/MapObjTrap.cpp"),
        ],
    },
    {
        "lib": "Map",
        "mw_version": "GC/1.2.5",
        "cflags": cflags_game,
        "progress_category": "game",
        "objects": [
            PCHObject(Matching, "Map/JointModel.cpp"),
            PCHObject(Matching, "Map/JointModelManager.cpp"),
            PCHObject(Matching, "Map/JointObj.cpp"),
            PCHObject(NonMatching, "Map/Map.cpp"),
            Object(NonMatching, "Map/MapArea.cpp"),
            Object(NonMatching, "Map/MapCheck.cpp"),
            PCHObject(Matching, "Map/MapCollisionData.cpp"),
            PCHObject(NonMatching, "Map/MapCollisionEntry.cpp"),
            Object(Matching, "Map/MapCollisionManager.cpp"),
            PCHObject(Matching, "Map/MapDraw.cpp"),
            PCHObject(Matching, "Map/MapEvent.cpp"),
            PCHObject(NonMatching, "Map/MapEventSink.cpp"),
            Object(NonMatching, "Map/MapMakeData.cpp"),
            Object(NonMatching, "Map/MapMakeList.cpp"),
            PCHObject(NonMatching, "Map/MapMirror.cpp"),
            PCHObject(NonMatching, "Map/MapModel.cpp"),
            PCHObject(NonMatching, "Map/MapWarp.cpp"),
            PCHObject(NonMatching, "Map/MapStaticObject.cpp"),
            PCHObject(NonMatching, "Map/MapWire.cpp"),
            PCHObject(NonMatching, "Map/MapWireManager.cpp"),
            Object(NonMatching, "Map/MapXlu.cpp"),
            Object(NonMatching, "Map/PollutionAction.cpp"),
            PCHObject(NonMatching, "Map/PollutionCount.cpp"),
            PCHObject(NonMatching, "Map/PollutionManager.cpp"),
            PCHObject(NonMatching, "Map/PollutionObj.cpp"),
            Object(NonMatching, "Map/PollutionPos.cpp"),
            PCHObject(NonMatching, "Map/Shimmer.cpp"),
            PCHObject(NonMatching, "Map/Sky.cpp"),
            PCHObject(NonMatching, "Map/MapEventSirena.cpp"),
            PCHObject(NonMatching, "Map/PollutionLayer.cpp"),
            PCHObject(NonMatching, "Map/PollutionEvent.cpp"),
            Object(NonMatching, "Map/MapCollisionPlane.cpp"),
            PCHObject(Matching, "Map/MarineSnow.cpp"),
            Object(Matching, "Map/MapData.cpp"),
            PCHObject(NonMatching, "Map/MapEventDolpic.cpp"),
            PCHObject(NonMatching, "Map/MapEventMare.cpp"),
            PCHObject(NonMatching, "Map/BathWaterManager.cpp"),
            PCHObject(Matching, "Map/StickyStainManager.cpp"),
        ],
    },
    {
        "lib": "GC2D",
        "mw_version": "GC/1.2.5",
        "cflags": cflags_game,
        "progress_category": "game",
        "objects": [
            Object(Matching, "GC2D/ChangeValue.cpp"),
            Object(Matching, "GC2D/Coord2D.cpp"),
            Object(Matching, "GC2D/ExPane.cpp"),
            PCHObject(NonMatching, "GC2D/Menu.cpp"),
            PCHObject(NonMatching, "GC2D/ScrnFader.cpp"),
            PCHObject(NonMatching, "GC2D/GCConsole2.cpp"),
            PCHObject(NonMatching, "GC2D/Talk2D2.cpp"),
            Object(NonMatching, "GC2D/BoundPane.cpp"),
            PCHObject(NonMatching, "GC2D/PauseMenu2.cpp"),
            Object(NonMatching, "GC2D/MessageLoader.cpp"),
            PCHObject(NonMatching, "GC2D/HelpActor.cpp"),
            Object(Matching, "GC2D/MessageUtil.cpp"),
            PCHObject(NonMatching, "GC2D/CardSave.cpp"),
            PCHObject(NonMatching, "GC2D/CardLoad.cpp"),
            PCHObject(NonMatching, "GC2D/ConsoleStr.cpp"),
            PCHObject(NonMatching, "GC2D/SelectMenu.cpp"),
            PCHObject(NonMatching, "GC2D/SelectDir.cpp"),
            PCHObject(NonMatching, "GC2D/SelectShine2.cpp"),
            PCHObject(Matching, "GC2D/BlendPane.cpp"),
            PCHObject(NonMatching, "GC2D/Guide.cpp"),
            PCHObject(NonMatching, "GC2D/SunGlass.cpp"),
            PCHObject(Matching, "GC2D/ShineFader.cpp"),
            PCHObject(NonMatching, "GC2D/ProgSelect.cpp"),
            Object(NonMatching, "GC2D/hx_wiper.c"),
            PCHObject(NonMatching, "GC2D/MovieSubtitle.cpp"),
            PCHObject(NonMatching, "GC2D/Option.cpp"),
            PCHObject(NonMatching, "GC2D/MovieRumble.cpp"),
        ],
    },
    {
        "lib": "Enemy",
        "mw_version": "GC/1.2.5",
        "cflags": cflags_game,
        "progress_category": "game",
        "objects": [
            PCHObject(NonMatching, "Enemy/conductor.cpp"),
            PCHObject(NonMatching, "Enemy/effectObj.cpp"),
            PCHObject(NonMatching, "Enemy/emario.cpp"),
            PCHObject(NonMatching, "Enemy/enemy.cpp"),
            PCHObject(NonMatching, "Enemy/enemyAttachment.cpp"),
            PCHObject(NonMatching, "Enemy/enemymanager.cpp"),
            PCHObject(NonMatching, "Enemy/enemyMario.cpp"),
            PCHObject(NonMatching, "Enemy/feetinv.cpp"),
            PCHObject(NonMatching, "Enemy/gesso.cpp"),
            PCHObject(NonMatching, "Enemy/graph.cpp"),
            PCHObject(NonMatching, "Enemy/hamukuri.cpp"),
            PCHObject(NonMatching, "Enemy/hinokuri2.cpp"),
            PCHObject(NonMatching, "Enemy/mameGesso.cpp"),
            PCHObject(NonMatching, "Enemy/namekuri.cpp"),
            PCHObject(NonMatching, "Enemy/pakkun.cpp"),
            PCHObject(NonMatching, "Enemy/smallEnemy.cpp"),
            PCHObject(NonMatching, "Enemy/spider.cpp"),
            Object(Matching, "Enemy/spline.cpp"),
            PCHObject(Matching, "Enemy/typicalenemy.cpp"),
            PCHObject(NonMatching, "Enemy/walker.cpp"),
            PCHObject(NonMatching, "Enemy/walkerEnemy.cpp"),
            PCHObject(NonMatching, "Enemy/bossgesso.cpp"),
            PCHObject(NonMatching, "Enemy/elecNokonoko.cpp"),
            PCHObject(NonMatching, "Enemy/telesa.cpp"),
            PCHObject(NonMatching, "Enemy/fireWanwan.cpp"),
            PCHObject(NonMatching, "Enemy/enemytable.cpp"),
            PCHObject(NonMatching, "Enemy/generator.cpp"),
            PCHObject(NonMatching, "Enemy/bosspakkun.cpp"),
            PCHObject(NonMatching, "Enemy/tobiPuku.cpp"),
            PCHObject(NonMatching, "Enemy/tinkoopa.cpp"),
            PCHObject(NonMatching, "Enemy/launcher.cpp"),
            PCHObject(NonMatching, "Enemy/bosswanwan.cpp"),
            PCHObject(NonMatching, "Enemy/chuuhana.cpp"),
            PCHObject(NonMatching, "Enemy/igaiga.cpp"),
            PCHObject(NonMatching, "Enemy/poihana.cpp"),
            PCHObject(NonMatching, "Enemy/tamaNoko.cpp"),
            PCHObject(NonMatching, "Enemy/bosstelesa.cpp"),
            PCHObject(NonMatching, "Enemy/riccohook.cpp"),
            PCHObject(NonMatching, "Enemy/bombhei.cpp"),
            PCHObject(NonMatching, "Enemy/cannon.cpp"),
            PCHObject(NonMatching, "Enemy/bosseel.cpp"),
            PCHObject(NonMatching, "Enemy/killer.cpp"),
            PCHObject(NonMatching, "Enemy/beam.cpp"),
            PCHObject(NonMatching, "Enemy/hanasambo.cpp"),
            PCHObject(NonMatching, "Enemy/popo.cpp"),
            PCHObject(NonMatching, "Enemy/SleepBossHanachan.cpp"),
            PCHObject(NonMatching, "Enemy/DemoBossHanachanBase.cpp"),
            PCHObject(NonMatching, "Enemy/fruitsboat.cpp"),
            PCHObject(NonMatching, "Enemy/BossHanachanSub.cpp"),
            PCHObject(NonMatching, "Enemy/BossHanachanMain.cpp"),
            PCHObject(NonMatching, "Enemy/BossHanachanNerve.cpp"),
            Object(NonMatching, "Enemy/BossHanachanAnm.cpp"),
            PCHObject(NonMatching, "Enemy/BossHanachanParts.cpp"),
            PCHObject(NonMatching, "Enemy/BossHanachanSave.cpp"),
            PCHObject(NonMatching, "Enemy/amiNoko.cpp"),
            PCHObject(NonMatching, "Enemy/gatekeeper.cpp"),
            PCHObject(NonMatching, "Enemy/BossHanachanEffect.cpp"),
            PCHObject(NonMatching, "Enemy/egggen.cpp"),
            PCHObject(NonMatching, "Enemy/seal.cpp"),
            PCHObject(NonMatching, "Enemy/bgpoldrop.cpp"),
            PCHObject(NonMatching, "Enemy/bgtentacle.cpp"),
            PCHObject(NonMatching, "Enemy/effectEnemy.cpp"),
            PCHObject(NonMatching, "Enemy/hauntLeg.cpp"),
            PCHObject(NonMatching, "Enemy/areacylinder.cpp"),
            PCHObject(NonMatching, "Enemy/wireTrap.cpp"),
            PCHObject(NonMatching, "Enemy/BossHanachanSound.cpp"),
            PCHObject(NonMatching, "Enemy/rocket.cpp"),
            PCHObject(NonMatching, "Enemy/Kazekun.cpp"),
            PCHObject(NonMatching, "Enemy/bossManta.cpp"),
            PCHObject(NonMatching, "Enemy/wireBinder.cpp"),
            PCHObject(NonMatching, "Enemy/yunbo.cpp"),
            PCHObject(NonMatching, "Enemy/koopajr.cpp"),
            PCHObject(NonMatching, "Enemy/Kumokun.cpp"),
            PCHObject(NonMatching, "Enemy/Koopa.cpp"),
            PCHObject(NonMatching, "Enemy/Kukku.cpp"),
            PCHObject(NonMatching, "Enemy/Amenbo.cpp"),
            PCHObject(NonMatching, "Enemy/BathtubPeach.cpp"),
            PCHObject(NonMatching, "Enemy/BathtubKiller.cpp"),
            PCHObject(NonMatching, "Enemy/coasterkiller.cpp"),
            PCHObject(NonMatching, "Enemy/DebuTelesa.cpp"),
            PCHObject(NonMatching, "Enemy/TabePuku.cpp"),
            PCHObject(NonMatching, "Enemy/BathtubBinder.cpp"),
            PCHObject(NonMatching, "Enemy/limitkoopa.cpp"),
            PCHObject(NonMatching, "Enemy/limitkoopajr.cpp"),
        ],
    },
    {
        "lib": "Camera",
        "mw_version": "GC/1.2.5",
        "cflags": cflags_game,
        "progress_category": "game",
        "objects": [
            Object(NonMatching, "Camera/CameraBGCheck.cpp"),
            PCHObject(NonMatching, "Camera/CameraChange.cpp"),
            Object(NonMatching, "Camera/CameraCodeControl.cpp"),
            PCHObject(NonMatching, "Camera/cameragc.cpp"),
            PCHObject(Matching, "Camera/CameraHeightPan.cpp"),
            Object(NonMatching, "Camera/CameraInbetween.cpp"),
            Object(Matching, "Camera/CameraKindParam.cpp"),
            Object(NonMatching, "Camera/cameralib.cpp"),
            Object(NonMatching, "Camera/CameraMarioData.cpp"),
            PCHObject(NonMatching, "Camera/CameraNotice.cpp"),
            PCHObject(NonMatching, "Camera/CameraNormal.cpp"),
            PCHObject(Matching, "Camera/camerasave.cpp"),
            Object(NonMatching, "Camera/camerashake.cpp"),
            Object(NonMatching, "Camera/CameraTalk.cpp"),
            PCHObject(NonMatching, "Camera/lensflare.cpp"),
            PCHObject(NonMatching, "Camera/lensglow.cpp"),
            PCHObject(NonMatching, "Camera/sunmgr.cpp"),
            PCHObject(NonMatching, "Camera/sunmodel.cpp"),
            PCHObject(NonMatching, "Camera/CubeManagerBase.cpp"),
            PCHObject(Matching, "Camera/CameraMapTool.cpp"),
            PCHObject(Matching, "Camera/CubeMapTool.cpp"),
            Object(NonMatching, "Camera/CameraMultiPlayer.cpp"),
            PCHObject(NonMatching, "Camera/CameraJetCoaster.cpp"),
            PCHObject(NonMatching, "Camera/CameraBck.cpp"),
            PCHObject(NonMatching, "Camera/CameraOption.cpp"),
            PCHObject(NonMatching, "Camera/CameraDemo.cpp"),
            Object(NonMatching, "Camera/CameraWarp.cpp"),
            PCHObject(NonMatching, "Camera/CameraMode.cpp"),
            Object(NonMatching, "Camera/CameraSecureView.cpp"),
            PCHObject(Matching, "Camera/CamShakeDefine.cpp"),
        ],
    },
    {
        "lib": "Animal",
        "mw_version": "GC/1.2.5",
        "cflags": cflags_game,
        "progress_category": "game",
        "objects": [
            PCHObject(NonMatching, "Animal/boid.cpp"),
            PCHObject(NonMatching, "Animal/fishoid.cpp"),
            PCHObject(NonMatching, "Animal/AnimalBase.cpp"),
            PCHObject(NonMatching, "Animal/AnimalManager.cpp"),
            PCHObject(Matching, "Animal/AnimalSave.cpp"),
            PCHObject(NonMatching, "Animal/AnimalNerve.cpp"),
            PCHObject(NonMatching, "Animal/Bird.cpp"),
            PCHObject(NonMatching, "Animal/BeeHive.cpp"),
            PCHObject(NonMatching, "Animal/Butterfly.cpp"),
        ],
    },
]

# Optional extra categories for progress tracking
# Adjust as desired for your project
config.progress_categories = [
    ProgressCategory("game", "Game Code"),
    ProgressCategory("jsystem", "JSystem Middleware"),
    ProgressCategory("sdk", "SDK Code"),
]
config.progress_each_module = args.verbose

if args.mode == "configure":
    # Write build.ninja and objdiff.json
    generate_build(config)
elif args.mode == "progress":
    # Print progress and write progress.json
    calculate_progress(config)
else:
    sys.exit("Unknown mode: " + args.mode)
