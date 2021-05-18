def apply(config, args):
    config['mapfile'] = 'build/sms_jp_r0/sms_jp_r0.map'
    config['myimg'] = 'build/sms_jp_r0/main.dol'
    config['baseimg'] = 'baserom.dol'
    config['makeflags'] = []
    config['source_directories'] = ['src', 'include', 'spec']
    config["arch"] = "ppc"
    config["map_format"] = "mw" # gnu or mw
    config["mw_build_dir"] = "build/" # only needed for mw map format
    config["makeflags"] = []
    config["objdump_executable"] = "powerpc-linux-gnu-objdump"