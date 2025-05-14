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
config.compilers_tag = "20240706"
config.dtk_tag = "v1.3.0"
config.objdiff_tag = "v2.4.0"
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

cflags_game = [
    *cflags_base,
    "-O4,p",
    "-inline auto",
    "-fp_contract on",
    "-str reuse,readonly",
]

cflags_system = [
    *cflags_game,
    "-inline auto",
    "-opt all,nostrength",
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


config.warn_missing_config = True
config.warn_missing_source = False
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
            Object(NonMatching, "JSystem/JSupport/JSUInputStream.cpp"),
            Object(NonMatching, "JSystem/JSupport/JSUList.cpp"),
            Object(Matching, "JSystem/JSupport/JSUMemoryStream.cpp"),
            Object(Matching, "JSystem/JSupport/JSUFileStream.cpp"),

            # JGadget
            Object(NonMatching, "JSystem/JGadget/std-list.cpp"),
            Object(NonMatching, "JSystem/JGadget/std-vector.cpp"),
            Object(NonMatching, "JSystem/JGadget/linklist.cpp"),
            Object(Matching, "JSystem/JGadget/singlelinklist.cpp"),

            # JKernel
            Object(NonMatching, "JSystem/JKernel/JKRArchivePri.cpp"),
            Object(NonMatching, "JSystem/JKernel/JKRAramArchive.cpp"),
            Object(Matching, "JSystem/JKernel/JKRAramHeap.cpp"),
            Object(NonMatching, "JSystem/JKernel/JKRAram.cpp"),
            Object(NonMatching, "JSystem/JKernel/JKRArchivePub.cpp"),
            Object(NonMatching, "JSystem/JKernel/JKRCompArchive.cpp"),
            Object(Matching, "JSystem/JKernel/JKRDisposer.cpp"),
            Object(NonMatching, "JSystem/JKernel/JKRDvdArchive.cpp"),
            Object(Matching, "JSystem/JKernel/JKRDvdFile.cpp"),
            Object(NonMatching, "JSystem/JKernel/JKRExpHeap.cpp"),
            Object(Matching, "JSystem/JKernel/JKRFileCache.cpp"),
            Object(Matching, "JSystem/JKernel/JKRFileFinder.cpp"),
            Object(Matching, "JSystem/JKernel/JKRFileLoader.cpp"),
            Object(NonMatching, "JSystem/JKernel/JKRHeap.cpp"),
            Object(Matching, "JSystem/JKernel/JKRMemArchive.cpp"),
            Object(NonMatching, "JSystem/JKernel/JKRSolidHeap.cpp"),
            Object(Matching, "JSystem/JKernel/JKRStdHeap.cpp"),
            Object(Matching, "JSystem/JKernel/JKRThread.cpp"),
            Object(Matching, "JSystem/JKernel/JKRDvdRipper.cpp"),
            Object(NonMatching, "JSystem/JKernel/JKRDvdAramRipper.cpp"),
            Object(Matching, "JSystem/JKernel/JKRDecomp.cpp"),
            Object(Matching, "JSystem/JKernel/JKRAramBlock.cpp"),
            Object(Matching, "JSystem/JKernel/JKRAramPiece.cpp"),
            Object(Matching, "JSystem/JKernel/JKRAramStream.cpp"),

            # JUtility
            Object(NonMatching, "JSystem/JUtility/JUTException.cpp"),
            Object(Matching, "JSystem/JUtility/JUTDirectPrint.cpp"),
            Object(Matching, "JSystem/JUtility/JUTDbPrint.cpp"),
            Object(Matching, "JSystem/JUtility/JUTFont.cpp"),
            Object(NonMatching, "JSystem/JUtility/JUTGamePad.cpp"),
            Object(NonMatching, "JSystem/JUtility/JUTNameTab.cpp"),
            Object(Matching, "JSystem/JUtility/JUTPalette.cpp"),
            Object(Matching, "JSystem/JUtility/JUTRect.cpp"),
            Object(Matching, "JSystem/JUtility/JUTResource.cpp"),
            Object(Matching, "JSystem/JUtility/JUTTexture.cpp"),
            Object(Matching, "JSystem/JUtility/JUTAssert.cpp"),
            Object(Matching, "JSystem/JUtility/JUTVideo.cpp"),
            Object(Matching, "JSystem/JUtility/JUTResFont.cpp"),
            Object(Matching, "JSystem/JUtility/JUTRomFont.cpp"),
            Object(NonMatching, "JSystem/JUtility/JUTConsole.cpp"),
            Object(NonMatching, "JSystem/JUtility/JUTDirectFile.cpp"),

            # JDrama
            Object(NonMatching, "JSystem/JDrama/JDRActor.cpp"),
            Object(NonMatching, "JSystem/JDrama/JDRCamera.cpp"),
            Object(Matching, "JSystem/JDrama/JDRCharacter.cpp"),
            Object(NonMatching, "JSystem/JDrama/JDRDirector.cpp"),
            Object(NonMatching, "JSystem/JDrama/JDRDisplay.cpp"),
            Object(NonMatching, "JSystem/JDrama/JDRDrawBufObj.cpp"),
            Object(NonMatching, "JSystem/JDrama/JDRDStage.cpp"),
            Object(NonMatching, "JSystem/JDrama/JDRDStageGroup.cpp"),
            Object(NonMatching, "JSystem/JDrama/JDREfbCtrl.cpp"),
            Object(NonMatching, "JSystem/JDrama/JDREfbSetting.cpp"),
            Object(NonMatching, "JSystem/JDrama/JDRFrmGXSet.cpp"),
            Object(Matching, "JSystem/JDrama/JDRGraphics.cpp"),
            Object(NonMatching, "JSystem/JDrama/JDRLighting.cpp"),
            Object(Matching, "JSystem/JDrama/JDRNameRef.cpp"),
            Object(NonMatching, "JSystem/JDrama/JDRNameRefGen.cpp"),
            Object(Matching, "JSystem/JDrama/JDRPlacement.cpp"),
            Object(Matching, "JSystem/JDrama/JDRRenderMode.cpp"),
            Object(Matching, "JSystem/JDrama/JDRResolution.cpp"),
            Object(Matching, "JSystem/JDrama/JDRScreen.cpp"),
            Object(NonMatching, "JSystem/JDrama/JDRSmJ3DAct.cpp"),
            Object(NonMatching, "JSystem/JDrama/JDRSmJ3DScn.cpp"),
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
            Object(NonMatching, "JSystem/JAudio/JASystem/JASBasicWaveBank.cpp"),
            Object(NonMatching, "JSystem/JAudio/JASystem/JASBNKParser.cpp"),
            Object(Matching, "JSystem/JAudio/JASystem/JASDrumSet.cpp"),
            Object(Matching, "JSystem/JAudio/JASystem/JASInstEffect.cpp"),
            Object(NonMatching, "JSystem/JAudio/JASystem/JASInstRand.cpp"),
            Object(Matching, "JSystem/JAudio/JASystem/JASInstSense.cpp"),
            Object(Matching, "JSystem/JAudio/JASystem/JASSimpleWaveBank.cpp"),
            Object(Matching, "JSystem/JAudio/JASystem/JASWaveArcLoader.cpp"),
            Object(Matching, "JSystem/JAudio/JASystem/JASWaveBank.cpp"),
            Object(Matching, "JSystem/JAudio/JASystem/JASWaveBankMgr.cpp"),
            Object(NonMatching, "JSystem/JAudio/JASystem/JASWSParser.cpp"),
            Object(Matching, "JSystem/JAudio/JASystem/JASAudioThread.cpp"),
            Object(Matching, "JSystem/JAudio/JASystem/JASChAllocQueue.cpp"),
            Object(NonMatching, "JSystem/JAudio/JASystem/JASChannel.cpp"),
            Object(Matching, "JSystem/JAudio/JASystem/JASDriverTables.cpp"),
            Object(Matching, "JSystem/JAudio/JASystem/JASChannelMgr.cpp"),
            Object(Matching, "JSystem/JAudio/JASystem/JASChGlobal.cpp"),
            Object(Matching, "JSystem/JAudio/JASystem/JASDriverIF.cpp"),
            Object(Matching, "JSystem/JAudio/JASystem/JASDSPBuf.cpp"),
            Object(NonMatching, "JSystem/JAudio/JASystem/JASDSPChannel.cpp"),
            Object(NonMatching, "JSystem/JAudio/JASystem/JASDSPInterface.cpp"),
            Object(Matching, "JSystem/JAudio/JASystem/JASOscillator.cpp"),
            Object(Matching, "JSystem/JAudio/JASystem/JASAiCtrl.cpp"),
            Object(Matching, "JSystem/JAudio/JASystem/JASCalc.cpp"),
            Object(Matching, "JSystem/JAudio/JASystem/JASCallback.cpp"),
            Object(Matching, "JSystem/JAudio/JASystem/JASCmdStack.cpp"),
            Object(NonMatching, "JSystem/JAudio/JASystem/JASDvdThread.cpp"),
            Object(NonMatching, "JSystem/JAudio/JASystem/JASHardStream.cpp"),
            Object(Matching, "JSystem/JAudio/JASystem/JASHeapCtrl.cpp"),
            Object(Matching, "JSystem/JAudio/JASystem/JASRate.cpp"),
            Object(Matching, "JSystem/JAudio/JASystem/JASKernelDebug.cpp"),
            Object(Matching, "JSystem/JAudio/JASystem/JASProbe.cpp"),
            Object(Matching, "JSystem/JAudio/JASystem/JASSystemHeap.cpp"),
            Object(NonMatching, "JSystem/JAudio/JASystem/JASVload.cpp"),
            Object(Matching, "JSystem/JAudio/JASystem/JASNoteMgr.cpp"),
            Object(Matching, "JSystem/JAudio/JASystem/JASOuterParam.cpp"),
            Object(Matching, "JSystem/JAudio/JASystem/JASPlayer_impl.cpp"),
            Object(Matching, "JSystem/JAudio/JASystem/JASRegisterParam.cpp"),
            Object(Matching, "JSystem/JAudio/JASystem/JASSeqCtrl.cpp"),
            Object(NonMatching, "JSystem/JAudio/JASystem/JASTrack.cpp"),
            Object(Matching, "JSystem/JAudio/JASystem/JASTrackInterrupt.cpp"),
            Object(Matching, "JSystem/JAudio/JASystem/JASTrackMgr.cpp"),
            Object(Matching, "JSystem/JAudio/JASystem/JASTrackPort.cpp"),
            Object(NonMatching, "JSystem/JAudio/JASystem/JASSeqParser.cpp"),

            # J2D
            Object(Matching, "JSystem/J2D/J2DPane.cpp"),
            Object(NonMatching, "JSystem/J2D/J2DPicture.cpp"),
            Object(Matching, "JSystem/J2D/J2DPrint.cpp"),
            Object(Matching, "JSystem/J2D/J2DScreen.cpp"),
            Object(Matching, "JSystem/J2D/J2DTextBox.cpp"),
            Object(NonMatching, "JSystem/J2D/J2DWindow.cpp"),
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
            Object(NonMatching, "JSystem/J3D/J3DGraphAnimator/J3DJoint.cpp"),
            Object(NonMatching, "JSystem/J3D/J3DGraphAnimator/J3DModel.cpp"),
            Object(Matching, "JSystem/J3D/J3DGraphAnimator/J3DNode.cpp"),
            Object(Matching, "JSystem/J3D/J3DGraphAnimator/J3DAnimation.cpp"),
            Object(Matching, "JSystem/J3D/J3DGraphAnimator/J3DMaterialAnm.cpp"),
            Object(Matching, "JSystem/J3D/J3DGraphAnimator/J3DMaterialAttach.cpp"),
            # J3DGraphLoader
            Object(Matching, "JSystem/J3D/J3DGraphLoader/J3DClusterLoader.cpp"),
            Object(Matching, "JSystem/J3D/J3DGraphLoader/J3DJointFactory.cpp"),
            Object(NonMatching, "JSystem/J3D/J3DGraphLoader/J3DMaterialFactory.cpp"),
            Object(NonMatching, "JSystem/J3D/J3DGraphLoader/J3DModelLoader.cpp"),
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
            Object(NonMatching, "JSystem/JParticle/JPABaseShape.cpp"),
            Object(Matching, "JSystem/JParticle/JPAExtraShape.cpp"),
            Object(NonMatching, "JSystem/JParticle/JPAResourceManager.cpp"),
            Object(Matching, "JSystem/JParticle/JPASweepShape.cpp"),
            Object(NonMatching, "JSystem/JParticle/JPATexture.cpp"),

            # JStage
            Object(Matching, "JSystem/JStage/JSGAmbientLight.cpp"),
            Object(Matching, "JSystem/JStage/JSGCamera.cpp"),
            Object(Matching, "JSystem/JStage/JSGActor.cpp"),
            Object(Matching, "JSystem/JStage/JSGLight.cpp"),
            Object(Matching, "JSystem/JStage/JSGObject.cpp"),
            Object(Matching, "JSystem/JStage/JSGSystem.cpp"),

            Object(NonMatching, "JSystem/JMath.cpp"),
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
            Object(NonMatching, "PowerPC_EABI_Support/Msl/MSL_C/MSL_Common_Embedded/uart_consolle_io.c"),
            Object(Matching, "PowerPC_EABI_Support/Msl/MSL_C/MSL_Common/wchar_io.c"),
            Object(Matching, "PowerPC_EABI_Support/Msl/MSL_C/MSL_Common_Embedded/Math/Double_precision/e_asin.c"),
            Object(Matching, "PowerPC_EABI_Support/Msl/MSL_C/MSL_Common_Embedded/Math/Double_precision/e_atan2.c"),
            Object(Matching, "PowerPC_EABI_Support/Msl/MSL_C/MSL_Common_Embedded/Math/Double_precision/s_atan.c"),
            Object(Matching, "PowerPC_EABI_Support/Msl/MSL_C/MSL_Common_Embedded/Math/Double_precision/s_frexp.c"),
            Object(Matching, "PowerPC_EABI_Support/Msl/MSL_C/MSL_Common_Embedded/Math/Double_precision/w_atan2.c"),
            Object(Matching, "PowerPC_EABI_Support/Msl/MSL_C/MSL_Common_Embedded/Math/Single_precision/hyperbolicsf.c"),
            Object(NonMatching, "PowerPC_EABI_Support/Msl/MSL_C/MSL_Common_Embedded/Math/Single_precision/inverse_trig.c"),
            Object(NonMatching, "PowerPC_EABI_Support/Msl/MSL_C/MSL_Common_Embedded/Math/Single_precision/trigf.c"),
            Object(Matching, "PowerPC_EABI_Support/Msl/MSL_C/MSL_Common_Embedded/Math/Single_precision/common_float_tables.c"),
            Object(NonMatching, "PowerPC_EABI_Support/Msl/MSL_C/MSL_Common_Embedded/Math/Single_precision/exponentialsf.c"),
        ],
    },
    {
        "lib": "TRK_MINNOW_DOLPHIN",
        "mw_version": "GC/1.2.5",
        "cflags": [*cflags_base, "-O4,p", "-pool off", "-str readonly", "-enum min", "-sdatathreshold 0"],
        "progress_category": "sdk",
        "objects": [
            Object(NonMatching, "TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Portable/mainloop.c"),
            Object(NonMatching, "TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Portable/nubevent.c"),
            Object(NonMatching, "TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Portable/nubinit.c"),
            Object(NonMatching, "TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Portable/msg.c"),
            Object(NonMatching, "TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Portable/msgbuf.c"),
            Object(NonMatching, "TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Portable/serpoll.c"),
            Object(Matching, "TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Os/dolphin/usr_put.c"),
            Object(NonMatching, "TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Portable/dispatch.c"),
            Object(NonMatching, "TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Portable/msghndlr.c"),
            Object(NonMatching, "TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Portable/support.c"),
            Object(Matching, "TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Portable/mutex_TRK.c"),
            Object(NonMatching, "TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Portable/notify.c"),
            Object(Matching, "TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Processor/ppc/Generic/flush_cache.c"),
            Object(NonMatching, "TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Portable/mem_TRK.c"),
            Object(NonMatching, "TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Processor/ppc/Generic/targimpl.c"),
            Object(Matching, "TRK_MINNOW_DOLPHIN/__exception.s"),
            Object(NonMatching, "TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Os/dolphin/dolphin_trk.c"),
            Object(Matching, "TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Processor/ppc/Generic/mpc_7xx_603e.c"),
            Object(NonMatching, "TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Portable/main_TRK.c"),
            Object(NonMatching, "TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Os/dolphin/dolphin_trk_glue.c"),
            Object(NonMatching, "TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Os/dolphin/targcont.c"),
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
            Object(NonMatching, "dolphin/os/OSFont.c"),
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
        "cflags": [*cflags_game, "-inline deferred ", "-opt all,nostrength"],
        "progress_category": "game",
        "objects": [
            Object(Matching, "MarioUtil/DLUtil.cpp"),
            Object(NonMatching, "MarioUtil/DrawUtil.cpp"),
            Object(NonMatching, "MarioUtil/LightUtil.cpp"),
            Object(NonMatching, "MarioUtil/MathUtil.cpp"),
            Object(NonMatching, "MarioUtil/MtxUtil.cpp"),
            Object(NonMatching, "MarioUtil/ScreenUtil.cpp"),
            Object(NonMatching, "MarioUtil/ShadowUtil.cpp"),
            Object(Matching, "MarioUtil/gd-reinit-gx.cpp"),
            Object(NonMatching, "MarioUtil/EffectUtil.cpp"),
            Object(NonMatching, "MarioUtil/ModelUtil.cpp"),
            Object(NonMatching, "MarioUtil/RumbleMgr.cpp"),
            Object(NonMatching, "MarioUtil/RumbleData.cpp"),
            Object(Matching, "MarioUtil/RumbleType.cpp"),
            Object(NonMatching, "MarioUtil/PacketUtil.cpp"),
            Object(Matching, "MarioUtil/GDUtil.cpp"),
            Object(Matching, "MarioUtil/TexUtil.cpp"),
            Object(Matching, "MarioUtil/MapUtil.cpp"),
            Object(NonMatching, "MarioUtil/ToolData.cpp"),
        ],
    },
    {
        "lib": "M3DUtil",
        "mw_version": "GC/1.2.5",
        "cflags": [*cflags_system, "-inline deferred"],
        "objects": [
            Object(NonMatching, "M3DUtil/M3UJoint.cpp"),
            Object(NonMatching, "M3DUtil/M3UModel.cpp"),
            Object(NonMatching, "M3DUtil/MActor.cpp"),
            Object(NonMatching, "M3DUtil/MActorAnm.cpp"),
            Object(NonMatching, "M3DUtil/MActorData.cpp"),
            Object(NonMatching, "M3DUtil/SDLModel.cpp"),
            Object(NonMatching, "M3DUtil/MActorUtil.cpp", flags=cflags_system),
            Object(NonMatching, "M3DUtil/SampleCtrlNode.cpp"),
            Object(NonMatching, "M3DUtil/SampleCtrlModel.cpp"),
            Object(NonMatching, "M3DUtil/MotionBlendCtrl.cpp"),
            Object(NonMatching, "M3DUtil/LodAnm.cpp"),
        ],
    },
    {
        "lib": "System",
        "mw_version": "GC/1.2.5",
        "cflags": cflags_system,
        "objects": [
            Object(Matching, "System/BaseParam.cpp"),
            Object(NonMatching, "System/EmitterViewObj.cpp"),
            Object(NonMatching, "System/EventWatcher.cpp"),
            Object(
                Matching,
                "System/FlagManager.cpp",
                cflags=[*cflags_system, "-inline all,level=1,deferred"],
            ),
            Object(NonMatching, "System/GCLogoDir.cpp"),
            Object(NonMatching, "System/J3DSysFlag.cpp"),
            Object(NonMatching, "System/MarDirector.cpp", extra_cflags=["-inline deferred"]),
            Object(NonMatching, "System/MarDirectorDirect.cpp"),
            Object(NonMatching, "System/MarDirectorEvent.cpp"),
            Object(NonMatching, "System/MarDirectorInitECT.cpp", extra_cflags=["-inline deferred"]),
            Object(NonMatching, "System/MarDirectorPreEntry.cpp"),
            Object(NonMatching, "System/MarDirectorSetup2.cpp"),
            Object(Matching, "System/marerr.cpp"),
            Object(NonMatching, "System/MarNameRefGen.cpp"),
            Object(NonMatching, "System/MenuDir.cpp"),
            Object(Matching, "System/Params.cpp"),
            Object(Matching, "System/ParamInst.cpp"),
            Object(NonMatching, "System/PerformList.cpp", extra_cflags=["-inline deferred"]),
            Object(NonMatching, "System/RenderModeObj.cpp"),
            Object(NonMatching, "System/SnapTimeObj.cpp", extra_cflags=["-inline deferred"]),
            Object(NonMatching, "System/TalkCursor.cpp"),
            Object(Matching, "System/TexCache.cpp"),
            Object(Matching, "System/ZBufferCatch.cpp", extra_cflags=["-inline deferred"]),
            Object(NonMatching, "System/Application.cpp"),
            Object(NonMatching, "System/ScenarioArchiveName.cpp"),
            Object(NonMatching, "System/MarioGamePad.cpp"),
            Object(NonMatching, "System/StageEventInfo.cpp"),
            Object(Matching, "System/StageUtil.cpp"),
            Object(Matching, "System/Resolution.cpp"),
            Object(NonMatching, "System/PositionHolder.cpp"),
            Object(Matching, "System/ProcessMeter.cpp"),
            Object(NonMatching, "System/TimeRec.cpp", extra_cflags=["-inline deferred"]),
            Object(NonMatching, "System/DrawSyncManager.cpp"),
            Object(NonMatching, "System/THPRender.cpp"),
            Object(NonMatching, "System/MarNameRefGen_BossEnemy.cpp"),
            Object(NonMatching, "System/MarNameRefGen_Enemy.cpp"),
            Object(NonMatching, "System/MarNameRefGen_Map.cpp"),
            Object(NonMatching, "System/MarNameRefGen_MapObj.cpp"),
            Object(NonMatching, "System/MarNameRefGen_NPC.cpp"),
            Object(NonMatching, "System/CardManager.cpp"),
            Object(NonMatching, "System/MarDirectorLoadResource.cpp"),
            Object(NonMatching, "System/MovieDirector.cpp"),
            Object(Matching, "System/MarDirectorCreateObjects.cpp"),
            Object(NonMatching, "System/MarDirectorSetupObjects.cpp"),
            Object(NonMatching, "System/MSoundMainSide.cpp"),
            Object(Matching, "System/TargetArrow.cpp", extra_cflags=["-inline deferred"]),
        ],
    },
    {
        "lib": "Strategic",
        "mw_version": "GC/1.2.5",
        "cflags": [*cflags_game, "-inline deferred", "-opt all,nostrength"],
        "progress_category": "game",
        "objects": [
            Object(NonMatching, "Strategic/liveactor.cpp"),
            Object(NonMatching, "Strategic/liveinterp.cpp"),
            Object(NonMatching, "Strategic/livemanager.cpp"),
            Object(NonMatching, "Strategic/ObjHitCheck.cpp"),
            Object(NonMatching, "Strategic/objmanager.cpp"),
            Object(NonMatching, "Strategic/ObjModel.cpp"),
            Object(NonMatching, "Strategic/spcinterp.cpp"),
            Object(NonMatching, "Strategic/Strategy.cpp"),
            Object(NonMatching, "Strategic/question.cpp"),
            Object(Matching, "Strategic/smplcharacter.cpp"),
            Object(NonMatching, "Strategic/HitActor.cpp"),
            Object(Matching, "Strategic/binder.cpp"),
            Object(Matching, "Strategic/SharedParts.cpp"),
            Object(NonMatching, "Strategic/MirrorActor.cpp"),
        ],
    },
    {
        "lib": "Player",
        "mw_version": "GC/1.2.5",
        "cflags": cflags_game,
        "progress_category": "game",
        "objects": [
            Object(NonMatching, "Player/Atom.cpp"),
            Object(NonMatching, "Player/MarioAction.cpp"),
            Object(NonMatching, "Player/MarioAutodemo.cpp"),
            Object(NonMatching, "Player/MarioBlend.cpp"),
            Object(NonMatching, "Player/MarioCap.cpp"),
            Object(NonMatching, "Player/MarioCollision.cpp"),
            Object(NonMatching, "Player/MarioDraw.cpp"),
            Object(NonMatching, "Player/MarioJump.cpp"),
            Object(NonMatching, "Player/MarioMain.cpp"),
            Object(NonMatching, "Player/MarioMove.cpp"),
            Object(NonMatching, "Player/MarioPhysics.cpp"),
            Object(NonMatching, "Player/MarioRecord.cpp"),
            Object(NonMatching, "Player/MarioRun.cpp"),
            Object(NonMatching, "Player/MarioSpecial.cpp"),
            Object(NonMatching, "Player/MarioUpper.cpp"),
            Object(NonMatching, "Player/MarioParticle.cpp"),
            Object(NonMatching, "Player/MarioWait.cpp"),
            Object(NonMatching, "Player/SplashManager.cpp"),
            Object(NonMatching, "Player/Tongue.cpp"),
            Object(NonMatching, "Player/WaterGun.cpp"),
            Object(NonMatching, "Player/Yoshi.cpp"),
            Object(NonMatching, "Player/MarioEffect.cpp"),
            Object(NonMatching, "Player/MarioSwim.cpp"),
            Object(NonMatching, "Player/MarioAccess.cpp"),
            Object(NonMatching, "Player/MarioInit.cpp"),
            Object(NonMatching, "Player/ModelWaterManager.cpp"),
            Object(NonMatching, "Player/MarioPositionObj.cpp"),
            Object(NonMatching, "Player/MarioCheckCol.cpp"),
            Object(NonMatching, "Player/MarioReceiveMsg.cpp"),
            Object(NonMatching, "Player/MarioSound.cpp"),
        ],
    },
    {
        "lib": "NPC",
        "mw_version": "GC/1.2.5",
        "cflags": [*cflags_game, "-inline deferred"],
        "progress_category": "game",
        "objects": [
            Object(NonMatching, "NPC/NpcAnm.cpp"),
            Object(NonMatching, "NPC/NpcBase.cpp"),
            Object(NonMatching, "NPC/NpcCallback.cpp"),
            Object(NonMatching, "NPC/NpcManager.cpp"),
            Object(NonMatching, "NPC/NpcNerve.cpp"),
            Object(NonMatching, "NPC/NpcSave.cpp"),
            Object(NonMatching, "NPC/NpcEvent.cpp"),
            Object(NonMatching, "NPC/NpcInitData.cpp"),
            Object(NonMatching, "NPC/NpcInitPrg.cpp"),
            Object(NonMatching, "NPC/NpcInbetween.cpp"),
            Object(NonMatching, "NPC/NpcParts.cpp"),
            Object(NonMatching, "NPC/NpcColor.cpp"),
            Object(NonMatching, "NPC/NpcSound.cpp"),
            Object(NonMatching, "NPC/NpcChange.cpp"),
            Object(NonMatching, "NPC/NpcThrow.cpp"),
            Object(NonMatching, "NPC/NpcTrample.cpp"),
            Object(NonMatching, "NPC/NpcEffect.cpp"),
            Object(NonMatching, "NPC/NpcInitAnmData.cpp"),
            Object(Matching, "NPC/NpcInitActionData.cpp"),
            Object(NonMatching, "NPC/NpcCoin.cpp"),
            Object(Matching, "NPC/NpcBalloon.cpp"),
            Object(NonMatching, "NPC/NpcWalkTurn.cpp"),
            Object(NonMatching, "NPC/NpcCollision.cpp"),

        ],
    },
    {
        "lib": "MSound",
        "mw_version": "GC/1.2.5",
        "cflags": [*cflags_game, "-inline deferred", "-opt all,nostrength"],
        "progress_category": "game",
        "objects": [
            Object(NonMatching, "MSound/MAnmSound.cpp"),
            Object(NonMatching, "MSound/MSound.cpp"),
            Object(NonMatching, "MSound/MSoundBGM.cpp"),
            Object(NonMatching, "MSound/MSoundDebug.cpp"),
            Object(NonMatching, "MSound/MSoundScene.cpp"),
            Object(NonMatching, "MSound/MSoundSE.cpp"),
            Object(NonMatching, "MSound/MSoundStruct.cpp"),
            Object(NonMatching, "MSound/MSHandle.cpp"),
            Object(NonMatching, "MSound/MSModBgm.cpp"),
        ],
    },
    {
        "lib": "MoveBG",
        "mw_version": "GC/1.2.5",
        "cflags": [*cflags_game, "-opt all,nostrength", "-inline deferred"],
        "progress_category": "game",
        "objects": [
            Object(NonMatching, "MoveBG/WoodBarrel.cpp"),
            Object(NonMatching, "MoveBG/MapObjBase.cpp"),
            Object(NonMatching, "MoveBG/MapObjInit.cpp"),
            Object(NonMatching, "MoveBG/MapObjGeneral.cpp"),
            Object(NonMatching, "MoveBG/MapObjManager.cpp"),
            Object(NonMatching, "MoveBG/MapObjLib.cpp"),
            Object(NonMatching, "MoveBG/Item.cpp"),
            Object(NonMatching, "MoveBG/ItemManager.cpp"),
            Object(NonMatching, "MoveBG/MapObjTown.cpp"),
            Object(NonMatching, "MoveBG/MapObjBlock.cpp"),
            Object(NonMatching, "MoveBG/MapObjBianco.cpp"),
            Object(NonMatching, "MoveBG/MapObjSirena.cpp"),
            Object(NonMatching, "MoveBG/MapObjRicco.cpp"),
            Object(NonMatching, "MoveBG/MapObjMamma.cpp"),
            Object(NonMatching, "MoveBG/MapObjPinna.cpp"),
            Object(Matching, "MoveBG/MapObjSample.cpp"),
            Object(NonMatching, "MoveBG/MapObjMare.cpp"),
            Object(NonMatching, "MoveBG/MapObjFlag.cpp"),
            Object(NonMatching, "MoveBG/MapObjWave.cpp"),
            Object(NonMatching, "MoveBG/MapObjFloat.cpp"),
            Object(NonMatching, "MoveBG/MapObjPlane.cpp"),
            Object(NonMatching, "MoveBG/MapObjCloud.cpp"),
            Object(NonMatching, "MoveBG/MapObjBall.cpp"),
            Object(Matching, "MoveBG/MapObjAirport.cpp"),
            Object(NonMatching, "MoveBG/MapObjDolpic.cpp"),
            Object(NonMatching, "MoveBG/MapObjPollution.cpp"),
            Object(NonMatching, "MoveBG/MapObjGrass.cpp"),
            Object(Matching, "MoveBG/MapObjPole.cpp"),
            Object(NonMatching, "MoveBG/MapObjWater.cpp"),
            Object(NonMatching, "MoveBG/ModelGate.cpp"),
            Object(NonMatching, "MoveBG/MapObjFence.cpp"),
            Object(NonMatching, "MoveBG/MapObjOption.cpp"),
            Object(NonMatching, "MoveBG/MapObjRailBlock.cpp"),
            Object(NonMatching, "MoveBG/MapObjMonte.cpp"),
            Object(NonMatching, "MoveBG/MapObjTree.cpp"),
            Object(Matching, "MoveBG/MapObjTumblePole.cpp"),
            Object(NonMatching, "MoveBG/MapObjEx.cpp"),
            Object(Matching, "MoveBG/Pool.cpp"),
            Object(NonMatching, "MoveBG/MapObjCorona.cpp"),
            Object(NonMatching, "MoveBG/MapObjItem2.cpp"),
            Object(NonMatching, "MoveBG/MapObjHide.cpp"),
            Object(NonMatching, "MoveBG/MapObjTrap.cpp"),
        ],
    },
    {
        # NOTE: it seems that SOME files in this lib have inline deferred, but some DONT
        "lib": "Map",
        "mw_version": "GC/1.2.5",
        "cflags": [*cflags_game, "-opt all,nostrength"],
        "progress_category": "game",
        "objects": [
            Object(NonMatching, "Map/JointModel.cpp"),
            Object(NonMatching, "Map/JointModelManager.cpp"),
            Object(NonMatching, "Map/JointObj.cpp"),
            Object(NonMatching, "Map/Map.cpp"),
            Object(NonMatching, "Map/MapArea.cpp"),
            Object(NonMatching, "Map/MapCheck.cpp"),
            Object(Matching, "Map/MapCollisionData.cpp", extra_cflags=["-inline deferred"]),
            Object(NonMatching, "Map/MapCollisionEntry.cpp", extra_cflags=["-inline deferred"]),
            Object(Matching, "Map/MapCollisionManager.cpp"),
            Object(Matching, "Map/MapDraw.cpp", extra_cflags=["-inline deferred"]),
            Object(Matching, "Map/MapEvent.cpp", extra_cflags=["-inline deferred"]),
            Object(NonMatching, "Map/MapEventSink.cpp", extra_cflags=["-inline deferred"]),
            Object(NonMatching, "Map/MapMakeData.cpp", extra_cflags=["-inline deferred"]),
            Object(NonMatching, "Map/MapMakeList.cpp", extra_cflags=["-inline deferred"]),
            Object(NonMatching, "Map/MapMirror.cpp"),
            Object(NonMatching, "Map/MapModel.cpp"),
            Object(NonMatching, "Map/MapWarp.cpp"),
            Object(NonMatching, "Map/MapStaticObject.cpp", extra_cflags=["-inline deferred"]),
            Object(NonMatching, "Map/MapWire.cpp"),
            Object(NonMatching, "Map/MapWireManager.cpp"),
            Object(NonMatching, "Map/MapXlu.cpp"),
            Object(NonMatching, "Map/PollutionAction.cpp", extra_cflags=["-inline deferred"]),
            Object(NonMatching, "Map/PollutionCount.cpp", extra_cflags=["-inline deferred"]),
            Object(NonMatching, "Map/PollutionManager.cpp", extra_cflags=["-inline deferred"]),
            Object(NonMatching, "Map/PollutionObj.cpp"),
            Object(NonMatching, "Map/PollutionPos.cpp"),
            Object(NonMatching, "Map/Shimmer.cpp"),
            Object(NonMatching, "Map/Sky.cpp", extra_cflags=["-inline deferred"]),
            Object(NonMatching, "Map/MapEventSirena.cpp"),
            Object(NonMatching, "Map/PollutionLayer.cpp"),
            Object(NonMatching, "Map/PollutionEvent.cpp"),
            Object(NonMatching, "Map/MapCollisionPlane.cpp"),
            Object(NonMatching, "Map/MarineSnow.cpp", extra_cflags=["-inline deferred"]),
            Object(Matching, "Map/MapData.cpp"),
            Object(NonMatching, "Map/MapEventDolpic.cpp"),
            Object(NonMatching, "Map/MapEventMare.cpp"),
            Object(NonMatching, "Map/BathWaterManager.cpp"),
            Object(Matching, "Map/StickyStainManager.cpp", extra_cflags=["-inline deferred"]),
        ],
    },
    {
        "lib": "GC2D",
        "mw_version": "GC/1.2.5",
        "cflags": cflags_game,
        "progress_category": "game",
        "objects": [
            Object(NonMatching, "GC2D/ChangeValue.cpp"),
            Object(NonMatching, "GC2D/Coord2D.cpp"),
            Object(NonMatching, "GC2D/ExPane.cpp"),
            Object(NonMatching, "GC2D/Menu.cpp"),
            Object(NonMatching, "GC2D/ScrnFader.cpp"),
            Object(NonMatching, "GC2D/GCConsole2.cpp"),
            Object(NonMatching, "GC2D/Talk2D2.cpp"),
            Object(NonMatching, "GC2D/BoundPane.cpp"),
            Object(NonMatching, "GC2D/PauseMenu2.cpp"),
            Object(NonMatching, "GC2D/MessageLoader.cpp"),
            Object(NonMatching, "GC2D/HelpActor.cpp"),
            Object(NonMatching, "GC2D/MessageUtil.cpp"),
            Object(NonMatching, "GC2D/CardSave.cpp"),
            Object(NonMatching, "GC2D/CardLoad.cpp"),
            Object(NonMatching, "GC2D/ConsoleStr.cpp"),
            Object(NonMatching, "GC2D/SelectMenu.cpp"),
            Object(NonMatching, "GC2D/SelectDir.cpp"),
            Object(NonMatching, "GC2D/SelectShine2.cpp"),
            Object(NonMatching, "GC2D/BlendPane.cpp"),
            Object(NonMatching, "GC2D/Guide.cpp"),
            Object(NonMatching, "GC2D/SunGlass.cpp"),
            Object(NonMatching, "GC2D/ShineFader.cpp"),
            Object(NonMatching, "GC2D/ProgSelect.cpp"),
            Object(NonMatching, "GC2D/hx_wiper.c"),
            Object(NonMatching, "GC2D/MovieSubtitle.cpp"),
            Object(NonMatching, "GC2D/Option.cpp"),
            Object(NonMatching, "GC2D/MovieRumble.cpp"),
        ],
    },
    {
        "lib": "Enemy",
        "mw_version": "GC/1.2.5",
        "cflags": [*cflags_game, "-opt all,nostrength", "-inline deferred"],
        "progress_category": "game",
        "objects": [
            Object(NonMatching, "Enemy/conductor.cpp"),
            Object(NonMatching, "Enemy/effectObj.cpp"),
            Object(NonMatching, "Enemy/emario.cpp"),
            Object(NonMatching, "Enemy/enemy.cpp"),
            Object(NonMatching, "Enemy/enemyAttachment.cpp"),
            Object(NonMatching, "Enemy/enemymanager.cpp"),
            Object(NonMatching, "Enemy/enemyMario.cpp"),
            Object(NonMatching, "Enemy/feetinv.cpp"),
            Object(NonMatching, "Enemy/gesso.cpp"),
            Object(NonMatching, "Enemy/graph.cpp"),
            Object(NonMatching, "Enemy/hamukuri.cpp"),
            Object(NonMatching, "Enemy/hinokuri2.cpp"),
            Object(NonMatching, "Enemy/mameGesso.cpp"),
            Object(NonMatching, "Enemy/namekuri.cpp"),
            Object(NonMatching, "Enemy/pakkun.cpp"),
            Object(NonMatching, "Enemy/smallEnemy.cpp"),
            Object(NonMatching, "Enemy/spider.cpp"),
            Object(Matching, "Enemy/spline.cpp"),
            Object(NonMatching, "Enemy/typicalenemy.cpp"),
            Object(NonMatching, "Enemy/walker.cpp"),
            Object(NonMatching, "Enemy/walkerEnemy.cpp"),
            Object(NonMatching, "Enemy/bossgesso.cpp"),
            Object(NonMatching, "Enemy/elecNokonoko.cpp"),
            Object(NonMatching, "Enemy/telesa.cpp"),
            Object(NonMatching, "Enemy/fireWanwan.cpp"),
            Object(NonMatching, "Enemy/enemytable.cpp"),
            Object(NonMatching, "Enemy/generator.cpp"),
            Object(NonMatching, "Enemy/bosspakkun.cpp"),
            Object(NonMatching, "Enemy/tobiPuku.cpp"),
            Object(NonMatching, "Enemy/tinkoopa.cpp"),
            Object(NonMatching, "Enemy/launcher.cpp"),
            Object(NonMatching, "Enemy/bosswanwan.cpp"),
            Object(NonMatching, "Enemy/chuuhana.cpp"),
            Object(NonMatching, "Enemy/igaiga.cpp"),
            Object(NonMatching, "Enemy/poihana.cpp"),
            Object(NonMatching, "Enemy/tamaNoko.cpp"),
            Object(NonMatching, "Enemy/bosstelesa.cpp"),
            Object(NonMatching, "Enemy/riccohook.cpp"),
            Object(NonMatching, "Enemy/bombhei.cpp"),
            Object(NonMatching, "Enemy/cannon.cpp"),
            Object(NonMatching, "Enemy/bosseel.cpp"),
            Object(NonMatching, "Enemy/killer.cpp"),
            Object(NonMatching, "Enemy/beam.cpp"),
            Object(NonMatching, "Enemy/hanasambo.cpp"),
            Object(NonMatching, "Enemy/popo.cpp"),
            Object(NonMatching, "Enemy/SleepBossHanachan.cpp"),
            Object(NonMatching, "Enemy/DemoBossHanachanBase.cpp"),
            Object(NonMatching, "Enemy/fruitsboat.cpp"),
            Object(NonMatching, "Enemy/BossHanachanSub.cpp"),
            Object(NonMatching, "Enemy/BossHanachanMain.cpp"),
            Object(NonMatching, "Enemy/BossHanachanNerve.cpp"),
            Object(NonMatching, "Enemy/BossHanachanAnm.cpp"),
            Object(NonMatching, "Enemy/BossHanachanParts.cpp"),
            Object(NonMatching, "Enemy/BossHanachanSave.cpp"),
            Object(NonMatching, "Enemy/amiNoko.cpp"),
            Object(NonMatching, "Enemy/gatekeeper.cpp"),
            Object(NonMatching, "Enemy/BossHanachanEffect.cpp"),
            Object(NonMatching, "Enemy/egggen.cpp"),
            Object(NonMatching, "Enemy/seal.cpp"),
            Object(NonMatching, "Enemy/bgpoldrop.cpp"),
            Object(NonMatching, "Enemy/bgtentacle.cpp"),
            Object(NonMatching, "Enemy/effectEnemy.cpp"),
            Object(NonMatching, "Enemy/hauntLeg.cpp"),
            Object(NonMatching, "Enemy/areacylinder.cpp"),
            Object(NonMatching, "Enemy/wireTrap.cpp"),
            Object(NonMatching, "Enemy/BossHanachanSound.cpp"),
            Object(NonMatching, "Enemy/rocket.cpp"),
            Object(NonMatching, "Enemy/Kazekun.cpp"),
            Object(NonMatching, "Enemy/bossManta.cpp"),
            Object(NonMatching, "Enemy/wireBinder.cpp"),
            Object(NonMatching, "Enemy/yunbo.cpp"),
            Object(NonMatching, "Enemy/koopajr.cpp"),
            Object(NonMatching, "Enemy/Kumokun.cpp"),
            Object(NonMatching, "Enemy/Koopa.cpp"),
            Object(NonMatching, "Enemy/Kukku.cpp"),
            Object(NonMatching, "Enemy/Amenbo.cpp"),
            Object(NonMatching, "Enemy/BathtubPeach.cpp"),
            Object(NonMatching, "Enemy/BathtubKiller.cpp"),
            Object(NonMatching, "Enemy/coasterkiller.cpp"),
            Object(NonMatching, "Enemy/DebuTelesa.cpp"),
            Object(NonMatching, "Enemy/TabePuku.cpp"),
            Object(NonMatching, "Enemy/BathtubBinder.cpp"),
            Object(NonMatching, "Enemy/limitkoopa.cpp"),
            Object(NonMatching, "Enemy/limitkoopajr.cpp"),
        ],
    },
    {
        "lib": "Camera",
        "mw_version": "GC/1.2.5",
        "cflags": [*cflags_game, "-inline deferred"],
        "progress_category": "game",
        "objects": [
            Object(NonMatching, "Camera/CameraBGCheck.cpp"),
            Object(NonMatching, "Camera/CameraChange.cpp"),
            Object(NonMatching, "Camera/CameraCodeControl.cpp"),
            Object(NonMatching, "Camera/cameragc.cpp"),
            Object(NonMatching, "Camera/CameraHeightPan.cpp"),
            Object(NonMatching, "Camera/CameraInbetween.cpp"),
            Object(NonMatching, "Camera/CameraKindParam.cpp"),
            Object(NonMatching, "Camera/cameralib.cpp"),
            Object(NonMatching, "Camera/CameraMarioData.cpp"),
            Object(NonMatching, "Camera/CameraNotice.cpp"),
            Object(NonMatching, "Camera/CameraNormal.cpp"),
            Object(NonMatching, "Camera/camerasave.cpp"),
            Object(NonMatching, "Camera/camerashake.cpp"),
            Object(NonMatching, "Camera/CameraTalk.cpp"),
            Object(NonMatching, "Camera/lensflare.cpp"),
            Object(NonMatching, "Camera/lensglow.cpp"),
            Object(NonMatching, "Camera/sunmgr.cpp"),
            Object(NonMatching, "Camera/sunmodel.cpp"),
            Object(NonMatching, "Camera/CubeManagerBase.cpp"),
            Object(NonMatching, "Camera/CameraMapTool.cpp"),
            Object(NonMatching, "Camera/CubeMapTool.cpp"),
            Object(NonMatching, "Camera/CameraMultiPlayer.cpp"),
            Object(NonMatching, "Camera/CameraJetCoaster.cpp"),
            Object(NonMatching, "Camera/CameraBck.cpp"),
            Object(NonMatching, "Camera/CameraOption.cpp"),
            Object(NonMatching, "Camera/CameraDemo.cpp"),
            Object(NonMatching, "Camera/CameraWarp.cpp"),
            Object(NonMatching, "Camera/CameraMode.cpp"),
            Object(NonMatching, "Camera/CameraSecureView.cpp"),
            Object(NonMatching, "Camera/CamShakeDefine.cpp"),
        ],
    },
    {
        "lib": "Animal",
        "mw_version": "GC/1.2.5",
        "cflags": cflags_game,
        "progress_category": "game",
        "objects": [
            Object(NonMatching, "Animal/boid.cpp"),
            Object(NonMatching, "Animal/fishoid.cpp"),
            Object(NonMatching, "Animal/AnimalBase.cpp"),
            Object(NonMatching, "Animal/AnimalManager.cpp"),
            Object(NonMatching, "Animal/AnimalSave.cpp"),
            Object(NonMatching, "Animal/AnimalNerve.cpp"),
            Object(NonMatching, "Animal/Bird.cpp"),
            Object(NonMatching, "Animal/BeeHive.cpp"),
            Object(NonMatching, "Animal/Butterfly.cpp"),
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
