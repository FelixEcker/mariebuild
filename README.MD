# Mariebuild
A build system primarly designed and intended for my use.

## Building
### Setup
To quickly setup mariebuild for development, simply run the `setup.sh` script. This will
automatically build [libmcfg_2](https://github.com/FelixEcker/mcfg_2) from the latest commit on its main branch.

### Compiling
**With mariebuild (0.4.0 or higher):** `mb -t release` <br>
**Without mariebuild:** `sh build.sh`

## mb usage
By default mb looks for a `build.mb` file which is the executed in debug mode.
```
Usage: mb [OPTION...]
A simple build system inspired by my hate against makefiles
Author: Marie Eckert

  -f, --force                Force a build, regardless if target is
                             incremental
  -i, --in=FILE              Specify a buildfile
  -k, --keep-going           Ignore any failures (if possible) and keep on building
  -n, --no-splash            Disable splash screen/logo
  -t, --target=TARGET        Specify the build target
  -v, --verbosity=LEVEL      Set the verbosity level (0-3)
  -?, --help                 Give this help list
      --usage                Give a short usage message
  -V, --version              Print program version

Mandatory or optional arguments to long options are also mandatory or optional
for any corresponding short options.

Report bugs to https://github.com/FelixEcker/mariebuild/issues.
```

## TODO (for mariebuild 1.0.0)

- [X] Move to MCFG/2
- [X] Incremental and Full Builds
- [X] Unify and Singular Rules
- [X] Implementation of the "force" argument
- [X] Target-Dependant Fields
- [ ] Documentation

## License
Mariebuild is licensed under the BSD 3-Clause License, see [The LICENSE file](https://github.com/FelixEcker/mariebuild/blob/master/LICENSE).
