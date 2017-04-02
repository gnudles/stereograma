if [ "$QT5" ]; then
	PACKAGES="qtbase5-dev qttools5-dev-tools qttools5-dev"
else
	PACKAGES="libqt4-dev"
fi

sudo apt-get install -y $PACKAGES
