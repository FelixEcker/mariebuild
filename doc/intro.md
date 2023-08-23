# Mariebuild Introduction
## Basic Syntax
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

## Scripting
Each section of type `lines` in the sector `.scripts` is a script, by default
they are executed using the bourne shell (sh). A shebang can be placed on the
first line of a script, the executable specified there is then used for
execution. It is important to note that the specified program has to be able to
take its code via stdin.

Example:
```
sector .scripts

lines prepare:
#!/usr/bin/bash
if [[ -d "./out/" ]]; then
  rm -rf out/*
else
  mkdir -p out/
fi
```
