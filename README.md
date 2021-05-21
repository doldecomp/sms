# Super Mario Sunshine

This repo contains a WIP decompilation of Super Mario Sunshine (JP, revision 0).

It builds the following DOL:

sms_jp_r0.dol: `sha1: 9f5a8caf56f5356aeac9d3ed28bf8de976a03625`

## Building

### Required tools

* [devkitPro](https://devkitpro.org/wiki/Getting_Started)
* gcc

### Instructions

1. Download the Gamecube SDK Dec 2001 version and make an environment variable called SDK_BASE_PATH that points to this folder such that the folder root has the dolphin folder in it.
2. Download the Codewarrior for Gamecube 1.1 and install it and make CW_BASE_PATH point to this folder where it is installed. 
3. Obtain a clean DOL of SMS JP 1.0 and place it in the base working directory and name it `baserom.dol`.
4. Download GC_WII_COMPILERS.zip from (https://cdn.discordapp.com/attachments/704241951972524063/801641758249320478/GC_WII_COMPILERS.zip) and extract it to tools/mwcc_compiler/.
5. Run the `make` command

## Contributions

Contributions and PRs are welcome.
