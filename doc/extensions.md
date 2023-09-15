# Mariebuild Extensions
## Inclusion
Extensions can be included in a buildfile by adding the `extensions` lines
section to the `.config` sector. 

### Example
```
sector .config
    lines extensions
configure
dependecy_resolver
```

## Storage
Mariebuild Extensions are stored at the directory specified in the `MBEXT_HOME`
environment variable.

Each extension is stored within its own folder, whose name specifies the
extension name. Within the folder, a file with the same name as the extension
with the `mcfg` extension and format is required to exist. This file acts as the
extensions manifest, specifying its metadata and the scripts it brings along
with it.

## Manifest Format
The manifest has to have the `.manifest` sector with the sections:
* `fields description`
* `lines internal_scripts` and/or `lines external_scripts`

The section `description` holds some informative metadata about the extension.

The section `internal_scripts` holds a list of scripts defined within the
manifest file. This list has one item-per-line.

The section `external_scripts` holds a list of scripts or executables which
are seperate files to the manifest. This list has one item-per-line. NOTE: Each
file on this list has to have execute permission in order for them to work!

### Internal Scripts
As statet before, internal scripts are listed within the `internal_scripts`
section within the `.manifset` sector, and only accesible when they are.

The scripts themselves are stored within the `.scripts` sector, with each script
having a section of type `lines` with a matching name.

Each script has to start with a shebang so that Mariebuild knows with what
program to execute them. The given interpreter program has to take the script's
source code with the `-c`.
