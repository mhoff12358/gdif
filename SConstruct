#!/usr/bin/env python
import os
import sys

env = SConscript("../godot-cpp/SConstruct")

# For the reference:
# - CCFLAGS are compilation flags shared between C and C++
# - CFLAGS are for C-specific compilation flags
# - CXXFLAGS are for C++-specific compilation flags
# - CPPFLAGS are for pre-processor flags
# - CPPDEFINES are for pre-processor defines
# - LINKFLAGS are for linking flags

def library_name(name):
    if env["platform"] == "macos":
        return "./bin/lib{}.{}.{}.framework/lib{}.{}.{}".format(
            name, env["platform"], env["target"], name, env["platform"], env["target"]
        )
    else:
        return "./bin/lib{}.{}.{}.{}{}".format(
            name, env["platform"], env["target"], env["arch_suffix"], env["SHLIBSUFFIX"]
        )

env.Append(CPPPATH=["src/", "../"])
sources = Glob("src/*.cpp")

library = env.SharedLibrary(
    library_name("gdif"),
    source=sources
)

Default(library)
