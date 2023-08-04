#!/bin/sh

LIBMCFG_GZ="https://github.com/FelixEcker/mcfg/archive/refs/tags/1.0.1.tar.gz"
MB_BACKUP_BIN="https://github.com/FelixEcker/mariebuild/releases/download/0.3.1/mb"
ARCHIVE_NAME="libmcfg_src"

echo ==\> Creating lib and out dirs

mkdir lib
mkdir out

lwd="$(pwd)"

echo ==\> Building libmcfg

mkdir .depends
cd .depends

wget -O $ARCHIVE_NAME.tar.gz $LIBMCFG_GZ 
gzip -d $ARCHIVE_NAME.tar.gz
tar -xf $ARCHIVE_NAME.tar

cd $(tar --list -f $ARCHIVE_NAME.tar | head -1)

# This should be put into a setup script for libmcfg in its next release
mkdir out/butter -p
if ! command -v mb &> /dev/null
then
	echo ==\> mb not installed on the system, downloading latest binary to build
	wget $MB_BACKUP_BIN
	chmod +x ./mb
	./mb -v
else
	mb -v
fi

ranlib libmcfg.a

echo ==\> Copying libmcfg

cp libmcfg.a $lwd/lib/
cd $lwd

echo ==\> Cleaning up
rm -rf .depends

echo ==\> Setup for development!
