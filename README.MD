# Mariebuild
A build system primarly designed and intended for my use.

## Building
### Setup
To quickly setup mariebuild for development, simply run the `setup.sh` script. This will
automatically build [libmcfg](https://github.com/FelixEcker/mcfg) from the latest release.

If there is no pre-existing install of mariebuild on the system, the script will download the
latest binary and use it for building. This gets deleted after the setup script is finished.

### Compiling
If you do not have any binary of mariebuild, you can build it by running the
`build.py` python script.

If the compilation is successful a binary by the name of `mb` should be output
in the projects root directory.

## Regarding Sourcecode Quality
Since this is my first "real" project in C, the source code might be less then
good. If anyone has suggestions for improving the source please create an issue
with the details on how to improve it.

## Syntax
Mariebuild files are basically [mcfg](https://github.com/FelixEcker/mcfg) files,
with a few specifics:
* Certain fields are required:
  * .config/mariebuild/files
  * .config/mariebuild/comp_cmd
* Some fields are automatically generated:
  * .config/mariebuild/mode_flags (from <mode name>_flags)
  * .config/mariebuild/file (each element of the list .config/mariebuild/files)

Example Build-File:
```
sector .config
  fields depends:
    str includes 'include/'
    str libdir   'lib/'
    str libs     '-Llib/ -lmcfg'
    list srcs     ''
  
  fields mariebuild:
    str name          'mb'
    str compiler      'gcc'
    str src_dir       'src'
    str obj_dir       'out'
 
    str shell         'bash'

    list srcs          'mb_utils.c:mb_script.c:mb_execute.c:mb.c'

    ; A required field, holding all source files in a colon-seperated list.
    list files         '$(depends/srcs):$(srcs)'

    str std_flags     '-Wall -I$(depends/includes)'
    str debug_flags   '-ggdb'
    str release_flags '-O3'
 
    ; Another required field, insert your compilation command in here.
    ; The command within this field is run for every file within the files field.
    str comp_cmd      '$(compiler) $(mode_flags) $(std_flags) -c -o $(obj_dir)/$(file).o $(src_dir)/$(file)'

    ; An optional field, run on the finalize build stage.
    str finalize_cmd  '$(compiler) $(mode_flags) -o $(name) out/$(files).o $(depends/libs)'
```

See `doc/intro.md` and `doc/intro.md` and the files in `doc/examples/` for more details.

## mb usage
By default mb looks for a `build.mb` file which is the executed in debug mode.
```
Usage: mb [OPTION...] 
A simple build system inspired by my hate against makefiles
Author: Marie Eckert

  -c, --check                Check if a build file is valid
  -d, --disable-extensions   Disable all extensions used by the build-file
  -e, --exec=SCRIPT          Specify a specific script to be executed
  -i, --in=FILE              Specify a buildfile
  -l, --list-platforms       Get a list of platforms supported by the
                             build-file
  -m, --mode=MODE            Specify the building mode
  -p, --platform=PLATFORM    Specify the targeted platform
  -q, --quiet                Disable all output except for Important messages
  -s, --structure            Print parsed structure then exit
  -v, --verbose              Output all messages
  -?, --help                 Give this help list
      --usage                Give a short usage message
  -V, --version              Print program version

Mandatory or optional arguments to long options are also mandatory or optional
for any corresponding short options.

Report bugs to https://github.com/FelixEcker/mariebuild/issues.

```

## License
Mariebuild is licensed under the BSD 3-Clause License, see [The LICENSE file](https://github.com/FelixEcker/mariebuild/blob/master/LICENSE).
