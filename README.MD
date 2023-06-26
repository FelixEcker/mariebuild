# Mariebuild
A build system primarly designed and intended for my use.

## Building
If you do not have any binary of mariebuild, you can build it by running the
`build.py` python script.

If the compilation is successful a binary by the name of `mb` should be output
in the projects root directory.

## Syntax
Example Build-File:
```
sector .config
  depends:
    includes 'include/'
    libs     ''

  ; This section is required for building, it holds the main information
  ; for the build file
  mariebuild:
    name          'mariebuild'
    compiler      'gcc'
    src_dir       'src'
    obj_dir       'out'
 
    ; A required field. Place all source files in here in a colon seperated list
    files         'mb_utils.c:mb_parse.c:mb_execute.c:mb.c'

    std_flags     '-Wall -I$(depends/includes)'
    debug_flags   '-ggdb'
    release_flags ''
 
    ; Another required field, insert your compilation command in here.
    ; The command within this field is run for every file within the files field.
    comp_cmd      '$(compiler) $(mode_flags) $(std_flags) -c -o $(file) $(obj_dir)/$(file).o'

    ; An optional field, run on the finalize build stage.
    finalize_cmd  '$(compiler) $(mode_flags) -o $(name) $(files).o'
```

See `docs/specification.mb` and the files in `examples/` for more details.

## mb usage
By default mb looks for a `build.mb` file which is the executed in debug mode.

### Parameters
**-i ; --in** <br>
Specify a build file with this parameter.

**-e ; --exec** <br>
Execute a specific script from the selected build file.

**-c ; --check** <br>
Check if the selected build file is a valid build file.

**-p ; --platform** <br>
Specify a target platform.

**--platforms** <br>
List the build file's supported target platforms.

**--disable-extensions** <br>
Don't allow any extensions to be loaded.

**-q ; --quiet** <br>
Only allow important build messages to be shown.

**-v ; --verbose** <br>
Show all build messages.

## License
Mariebuild is licensed under the (CHOOSE LICENSE) License, see
[https://github.com/FelixEcker/mariebuild/blob/master/LICENSE](The LICENSE file).
