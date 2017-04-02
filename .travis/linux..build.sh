qmake stereograma.pro
make -j4
if [ "$QT5" ]; then
	QT_VER="qt5"
else
	QT_VER="qt4"
fi
export QT_VER
tar -cvzf stereograma_linux_${QT_VER}.tar.gz stereograma
