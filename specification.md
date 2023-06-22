# MarieBuild (mb) spec
## Reserved words and characters
### Syntax Related
```
: ' ; $ ( ) ! . sector 
```

### Reserved sections, fields & scripts
```
SECTIONS

extensions
    Section containing enables/disables for installed mb extensions

mariebuild
    Required section containing general fields for compilation

platform_%s
    Platform specific fields, the %s can be replaced with any platform
    name to set fields for it. Fields which are set within a plaftorm section
    are stored in the mariebuild section

FIELDS

comp_cmd
    Required field in which the compilation command is assembled

SCRIPTS

prepare
    First script executed when a compilation is started

prepare_debug
    Script immediatly executed before compilation in debug mode is started

prepare_relase
    Script immediatly executed before compilation in release mode is started
```

## Structure
A mb build file consists of two sectors: Configuration (.config) & Script (.scripts);
these sector can be started using the `sector` keyword, example:
```
sector .config
    ; define some sections here...

sector .scripts
    ; define some scripts here...
```

Each sector is divided into sections (which are called scripts in the scripts sector).
Sections are started using their name followed by a colon, example:
```
sector .config
    mariebuild:
        ; Set some fields
```

Sections within the config sector contains fields which are set with this syntax: <br>
`<field_name> '<field_content>'` <br>
The content of a field can only be a string, the strings opening and closing delimiter
are single-quotes.

## Field accessing
To access fields inside a mb file, you use the following syntax: <br>
`$(<field name>)` <br>
To access a field from another section or are accessing them from within a script,
you need to prefix the field name with the sections name followed by a dot.

## Integrated commands
Integrated commands can be executed in scripts by prefixing their name with an
exclamation mark.

### List of Integrated Commands
```
source <script name> <file path>
    Execute the script specified in the name from the given path

stop
    Stop the current build

quit
    Quit the instance of mb in which quit was called.
```

## mb command
### Usage
```
mb <mode> <args>

Arguments:
    e exec
        Execute a specific script from the build file
    c check
        Check if the given mb file is valid
    i in
        Specify a file to be used. This is not required as mb will look for
        a mb file named "build.mb" by default.
    p platform
        Specify the targeted platforms name
    d disable-extensions
        Disable all extensions. Can not be overriden in the extensions section
    q quiet
        Disable all output from mb, only output from scripts will be allowed
    v verbose
        Enable verbose output from mb
    f force
        Force a build
    s status
        Check if the current compiled files are up to date with the source files.
```
### mb.cfg
mb.cfg is a file with default arguments for mb, it is searched for in the following order:
```
1. ./mb.cfg
2. ~/.mb.cfg
3. $XDG_CONFIG_HOME/mb/mb.cfg
4. $HOME/.config/mb/mb.cfg
5. /etc/mb.cfg
```
