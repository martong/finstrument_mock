#!/usr/bin/env python
import shutil
import os

compiler_setups = [
    " -O0",
    " -O0 -fsanitize=mock",
    " -O0 -fsanitize=mock -DSUB",
    " -O0 -fsanitize=mock -fno-inline-functions",
    " -O0 -fsanitize=mock -fno-inline-functions -DSUB",

    " -O2",
    " -O2 -fsanitize=mock",
    " -O2 -fsanitize=mock -DSUB",
    " -O2 -fsanitize=mock -fno-inline-functions",
    " -O2 -fsanitize=mock -fno-inline-functions -DSUB",

    " -O0 -finstrument-functions",
    " -O0 -finstrument-functions -fno-inline-functions",
    " -O2 -finstrument-functions",
    " -O2 -finstrument-functions -fno-inline-functions",
]

for setup in compiler_setups:
    dir = setup.replace(" -", "__").replace("=mock", "_mock")
    print dir
    try:
        shutil.rmtree(dir)
    except OSError:
        """ nop """
    shutil.copytree("Benchmark", dir)
    os.system("CPPFLAGS='%s' make -C %s report" % (setup, dir))
