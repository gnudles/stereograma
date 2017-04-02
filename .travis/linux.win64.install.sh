#!/usr/bin/env bash
set -e

if [ "$QT5" ]; then
	MINGW_PACKAGES="mingw64-x-qt5base"
else
	MINGW_PACKAGES="mingw64-x-qt"
fi

MINGW_PACKAGES="mingw64-x-glib2	mingw64-x-pkgconfig mingw64-x-binutils mingw64-x-gcc mingw64-x-runtime $MINGW_PACKAGES"

export MINGW_PACKAGES

PACKAGES="nsis cloog-isl libmpc3 qt4-linguist-tools mingw64 $MINGW_PACKAGES"

# shellcheck disable=SC2086
sudo apt-get install -y $PACKAGES
