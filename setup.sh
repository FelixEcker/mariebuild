#!/bin/sh

LIBMCFG_GZ="https://github.com/FelixEcker/mcfg/archive/refs/tags/1.0.2.tar.gz"
ARCHIVE_NAME="libmcfg_src"

mode="debug"
if [[ $1 ]]; then
  mode=$1
fi

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
  bash build.bash
else
  mb -v -m $mode
fi

echo ==\> Copying libmcfg

cp libmcfg.a $lwd/lib/
cd $lwd

echo ==\> Cleaning up
rm -rf .depends

echo ==\> Setup for development!
