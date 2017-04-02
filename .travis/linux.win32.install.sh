#!/usr/bin/env bash
set -e

if [ "$QT5" ]; then
	MINGW_PACKAGES="mingw32-x-qt5base"
else
	MINGW_PACKAGES="mingw32-x-qt"
fi

MINGW_PACKAGES="mingw32-x-glib2	mingw32-x-pkgconfig mingw32-x-binutils mingw32-x-gcc mingw32-x-runtime $MINGW_PACKAGES"

export MINGW_PACKAGES

PACKAGES="nsis cloog-isl libmpc3 qt4-linguist-tools mingw32 $MINGW_PACKAGES"

# shellcheck disable=SC2086
sudo apt-get install -y $PACKAGES
