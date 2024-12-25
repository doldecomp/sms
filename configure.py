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
    "-I include/libc",
    f"-I build/{config.version}/include",
    f"--defsym BUILD_VERSION={version_num}",
    f"--defsym VERSION_{config.version}",
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
cflags_base = [
    "-nodefaults",
    "-proc gekko",
    "-align powerpc",
    "-enum int",
    "-fp hardware",
    "-Cpp_exceptions off",
    # "-W all",
    "-O4,p",
    "-inline auto",
    '-pragma "cats off"',
    '-pragma "warn_notinlined off"',
    "-maxerrors 1",
    "-nosyspath",
    "-RTTI off",
    "-fp_contract on",
    "-str reuse",
    "-multibyte",  # For Wii compilers, replace with `-enc SJIS`
    "-cwd source",
    "-i include",
    "-i include/libc",
    f"-i build/{config.version}/include",
    f"-DBUILD_VERSION={version_num}",
    f"-DVERSION_{config.version}",
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
    "-str reuse,pool,readonly",
    "-inline deferred,auto",
]

# REL flags
cflags_rel = [
    *cflags_base,
    "-sdata 0",
    "-sdata2 0",
]

cflags_jsystem = [
    *cflags_base,
    "-str reuse,readonly",
    "-lang=c++",
    "-fp fmadd",
    "-char signed",
    "-rostr",
    "-common on"
]

cflags_game = [
    *cflags_base,
    "-str reuse,readonly",
]

cflags_system = [
    *cflags_game,
    "-opt all,nostrength",
]

cflags_dolphin = [
    *cflags_base,
    # TODO: should these be different?
]

config.linker_version = "GC/1.2.5"


# Helper function for Dolphin libraries
def DolphinLib(lib_name: str, objects: List[Object]) -> Dict[str, Any]:
    return {
        "lib": lib_name,
        "mw_version": "GC/1.2.5n",
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
            Object(Matching, "JSystem/JASProbe.cpp"),

            # JDrama
            Object(Matching, "JSystem/JDRPlacement.cpp"),
            Object(Matching, "JSystem/JDRResolution.cpp"),

            # JUtility
            Object(Matching, "JSystem/JUTRect.cpp"),

            # J3D
            Object(Matching, "JSystem/J3DClusterLoader.cpp"),
            # Object(Matching, "JSystem/J3DModel.cpp"),
            Object(Matching, "JSystem/J3DVertex.cpp"),
            Object(Matching, "JSystem/J3DPacket.cpp"),
            # Object(Matching, "JSystem/J3DShape.cpp"),

            # JKernel
            Object(Matching, "JSystem/JKRDisposer.cpp"),
            Object(Matching, "JSystem/JKRHeap.cpp"),
            Object(Matching, "JSystem/JKRExpHeap.cpp"),
            Object(Matching, "JSystem/JKRFileLoader.cpp"),
            Object(Matching, "JSystem/JKRThread.cpp"),
            Object(Matching, "JSystem/JKRFileFinder.cpp"),
            Object(Matching, "JSystem/JKRDvdFile.cpp"),

            # JSupport
            Object(Matching, "JSystem/JSUInputStream.cpp"),
            Object(Matching, "JSystem/JSUOutputStream.cpp"),
            Object(Matching, "JSystem/JSUMemoryStream.cpp"),
            Object(Matching, "JSystem/JSUFileStream.cpp"),
            Object(Matching, "JSystem/JSUList.cpp"),
        ],
    },
    {
        "lib": "MarioUtil",
        "mw_version": "GC/1.2.5",
        "cflags": cflags_game,
        "progress_category": "game",
        "objects": [
            Object(Matching, "MarioUtil/MapUtil.cpp"),
            Object(Matching, "MarioUtil/RumbleType.cpp"),
            Object(Matching, "MarioUtil/EffectUtil.cpp"),
            Object(Matching, "MarioUtil/ModelUtil.cpp"),
            Object(Matching, "MarioUtil/TexUtil.cpp"),
            # Object(Matching, "MarioUtil/DrawUtil.cpp"),
        ],
    },
    {
        "lib": "Player",
        "mw_version": "GC/1.2.5",
        "cflags": cflags_game,
        "progress_category": "game",
        "objects": [
            Object(Matching, "Player/MarioAccess.cpp"),
        ],
    },
    {
        "lib": "NPC",
        "mw_version": "GC/1.2.5",
        "cflags": cflags_game,
        "progress_category": "game",
        "objects": [
            Object(Matching, "NPC/NpcBalloon.cpp"),
            Object(Matching, "NPC/NpcInitActionData.cpp"),
        ],
    },
    {
        "lib": "System",
        "mw_version": "GC/1.2.5",
        "cflags": cflags_system,
        "objects": [
            Object(
                Matching,
                "System/FlagManager.cpp",
                cflags=[*cflags_system, "-inline all,level=1,deferred"],
            ),
            Object(Matching, "System/ParamInst.cpp"),
            Object(Matching, "System/ProcessMeter.cpp"),
            Object(Matching, "System/Resolution.cpp"),
            Object(Matching, "System/StageUtil.cpp"),
            Object(Matching, "System/TexCache.cpp"),
        ],
    },
    {
        "lib": "Runtime.PPCEABI.H",
        "progress_category": "sdk",
        "mw_version": "GC/1.2.5",
        "cflags": cflags_runtime,
        "objects": [
            Object(Matching, "Runtime.PPCEABI.H/__mem.c"),
            Object(Matching, "Runtime.PPCEABI.H/__va_arg.c"),
            Object(Matching, "Runtime.PPCEABI.H/global_destructor_chain.c"),
            Object(Matching, "Runtime.PPCEABI.H/NMWException.cp"),
            Object(Matching, "Runtime.PPCEABI.H/ptmf.c"),
            Object(Matching, "Runtime.PPCEABI.H/ExceptionPPC.cp"),
            Object(Matching, "Runtime.PPCEABI.H/runtime.c"),
            Object(Matching, "Runtime.PPCEABI.H/__init_cpp_exceptions.cpp"),
        ],
    },
    {
        "lib": "MSL_C.PPCEABI.bare.H",
        "progress_category": "sdk",
        "mw_version": "GC/1.2.5",
        "cflags": cflags_runtime,
        "objects": [
            # Object(Matching, "MSL_C.PPCEABI.bare.H/ansi_files.c"),
            # Object(Matching, "MSL_C.PPCEABI.bare.H/abort_exit.c"),
            # Object(Matching, "MSL_C.PPCEABI.bare.H/errno.c"),
            # Object(Matching, "MSL_C.PPCEABI.bare.H/ansi_fp.c"),
            # Object(Matching, "MSL_C.PPCEABI.bare.H/buffer_io.c"),
            # Object(Matching, "MSL_C.PPCEABI.bare.H/critical_regions.ppc_eabi.c"),
            # Object(Matching, "MSL_C.PPCEABI.bare.H/ctype.c"),
            # Object(Matching, "MSL_C.PPCEABI.bare.H/direct_io.c"),
            # Object(Matching, "MSL_C.PPCEABI.bare.H/mbstring.c"),
            # Object(Matching, "MSL_C.PPCEABI.bare.H/mem.c"),
            # Object(Matching, "MSL_C.PPCEABI.bare.H/mem_funcs.c"),
            # Object(Matching, "MSL_C.PPCEABI.bare.H/misc_io.c"),
            # Object(Matching, "MSL_C.PPCEABI.bare.H/printf.c"),
            Object(Matching, "MSL_C.PPCEABI.bare.H/rand.c"),
            # Object(Matching, "MSL_C.PPCEABI.bare.H/scanf.c"),
            # Object(Matching, "MSL_C.PPCEABI.bare.H/string.c"),
            # Object(Matching, "MSL_C.PPCEABI.bare.H/strtoul.c"),
            # Object(Matching, "MSL_C.PPCEABI.bare.H/float.c"),
            # Object(Matching, "MSL_C.PPCEABI.bare.H/uart_consolle_io.c"),
            # Object(Matching, "MSL_C.PPCEABI.bare.H/wchar_io.c"),
            Object(Matching, "MSL_C.PPCEABI.bare.H/e_asin.c"),
            Object(Matching, "MSL_C.PPCEABI.bare.H/e_atan2.c"),
            Object(Matching, "MSL_C.PPCEABI.bare.H/s_atan.c"),
            Object(Matching, "MSL_C.PPCEABI.bare.H/s_frexp.c"),
            Object(Matching, "MSL_C.PPCEABI.bare.H/w_atan2.c"),
            Object(Matching, "MSL_C.PPCEABI.bare.H/hyperbolicsf.c"),
            # Object(Matching, "MSL_C.PPCEABI.bare.H/inverse_trig.c"),
            # Object(Matching, "MSL_C.PPCEABI.bare.H/trigf.c"),
            # Object(Matching, "MSL_C.PPCEABI.bare.H/common_float_tables.c"),
            # Object(Matching, "MSL_C.PPCEABI.bare.H/exponentialsf.c"),
        ],
    },
    DolphinLib("base", [
            Object(Matching, "base/PPCArch.c"),
        ]),
    DolphinLib("db", [
            Object(Matching, "db/db.c"),
        ]),
    DolphinLib("os", [
            Object(NonMatching, "os/OS.c"),
            Object(Matching, "os/OSAlarm.c"),
            Object(Matching, "os/OSAlloc.c"),
            Object(Matching, "os/OSArena.c"),
            Object(Matching, "os/OSAudioSystem.c"),
            Object(Matching, "os/OSCache.c"),
            Object(Matching, "os/OSContext.c"),
            Object(NonMatching, "os/OSError.c"),
            Object(Matching, "os/OSFont.c"),
            Object(NonMatching, "os/OSInterrupt.c"),
            Object(Matching, "os/OSLink.c"),
            Object(Matching, "os/OSMessage.c"),
            Object(NonMatching, "os/OSMemory.c"),
            Object(Matching, "os/OSMutex.c"),
            # Object(NonMatching, "os/OSReboot.c"),
            Object(NonMatching, "os/OSReset.c"),
            Object(Matching, "os/OSResetSW.c"),
            Object(NonMatching, "os/OSRtc.c"),
            Object(Matching, "os/OSStopwatch.c"),
            Object(Matching, "os/OSSync.c"),
            Object(Matching, "os/OSThread.c"),
            Object(Matching, "os/OSTime.c"),
            Object(Matching, "os/__start.c"),
            Object(Matching, "os/__ppc_eabi_init.cpp"),
        ]),
    DolphinLib("mtx", [
            Object(NonMatching, "mtx/mtx.c"),
            Object(NonMatching, "mtx/mtxvec.c"),
            Object(NonMatching, "mtx/mtx44.c"),
            Object(NonMatching, "mtx/vec.c"),
        ]),
    DolphinLib("dvd", [
            Object(NonMatching, "dvd/dvd.c"),
            Object(NonMatching, "dvd/dvdfs.c"),
            Object(NonMatching, "dvd/dvdlow.c"),
            Object(NonMatching, "dvd/dvdqueue.c"),
            Object(NonMatching, "dvd/fstload.c"),
        ]),
    DolphinLib("vi", [
            Object(NonMatching, "vi/vi.c"),
        ]),
    DolphinLib("pad", [
            Object(NonMatching, "pad/Padclamp.c"),
            Object(NonMatching, "pad/Pad.c"),
        ]),
    DolphinLib("ai", [
            Object(Matching, "ai/ai.c"),
        ]),
    DolphinLib("ar", [
            Object(Matching, "ar/ar.c"),
            Object(Matching, "ar/arq.c"),
        ]),
    DolphinLib("dsp", [
            Object(NonMatching, "dsp/dsp.c"),
            Object(NonMatching, "dsp/dsp_debug.c"),
            Object(NonMatching, "dsp/dsp_task.c"),
        ]),
    DolphinLib("card", [
            Object(NonMatching, "card/CARDBios.c"),
            Object(NonMatching, "card/CARDUnlock.c"),
            Object(NonMatching, "card/CARDRdwr.c"),
            Object(NonMatching, "card/CARDBlock.c"),
            Object(NonMatching, "card/CARDDir.c"),
            Object(NonMatching, "card/CARDCheck.c"),
            Object(NonMatching, "card/CARDMount.c"),
            Object(NonMatching, "card/CARDFormat.c"),
            Object(NonMatching, "card/CARDOpen.c"),
            Object(NonMatching, "card/CARDCreate.c"),
            Object(NonMatching, "card/CARDRead.c"),
            Object(NonMatching, "card/CARDWrite.c"),
            Object(NonMatching, "card/CARDStat.c"),
        ]),
    DolphinLib("gx", [
            Object(Matching, "gx/GXInit.c"),
            Object(Matching, "gx/GXFifo.c"),
            Object(Matching, "gx/GXAttr.c"),
            Object(Matching, "gx/GXMisc.c"),
            Object(Matching, "gx/GXGeometry.c"),
            Object(Matching, "gx/GXFrameBuf.c"),
            Object(Matching, "gx/GXLight.c"),
            Object(Matching, "gx/GXTexture.c"),
            Object(Matching, "gx/GXBump.c"),
            Object(Matching, "gx/GXTev.c"),
            Object(Matching, "gx/GXPixel.c"),
            Object(Matching, "gx/GXDraw.c"),
            Object(Matching, "gx/GXStubs.c"),
            Object(Matching, "gx/GXDisplayList.c"),
            Object(Matching, "gx/GXTransform.c"),
            Object(Matching, "gx/GXPerf.c"),
        ]),
    DolphinLib("amcstubs", [
            Object(Matching, "amcstubs/AmcExi2Stubs.c"),
        ]),
    DolphinLib("odenotstub", [
            Object(Matching, "odenotstub/odenotstub.c"),
        ]),
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
