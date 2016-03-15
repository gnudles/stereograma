#-------------------------------------------------
#
# Project created by QtCreator 2011-07-24T20:40:23
#
#-------------------------------------------------

QT       += core gui network opengl

TARGET = stereograma
TEMPLATE = app

LIBS += -lGLU -ldl

SOURCES += main.cpp\
        mainwindow.cpp \
    imagecontainerwidget.cpp \
    imageviewer.cpp \
    model3d.cpp \
    imagefiledialog.cpp \
    stereomaker.cpp \
    glmodelview.cpp \
    presetsketch.cpp \
    preset.cpp \
    modeldepthviewer.cpp \
    basicimagewidget.cpp \
    presetedit.cpp \
    anaglyphmaker.cpp \
    stringtype.cpp \
    trirender.cpp \
    RPly/rply.c

HEADERS  += mainwindow.h \
    imagecontainerwidget.h \
    imageviewer.h \
    model3d.h \
    imagefiledialog.h \
    stereomaker.h \
    glmodelview.h \
    presetsketch.h \
    preset.h \
    modeldepthviewer.h \
    basicimagewidget.h \
    presetedit.h \
    anaglyphmaker.h \
    stringtype.h \
    trirender.h \
    RPly/rply.h \
    RPly/rplyfile.h

FORMS    += mainwindow.ui \
    imageviewer.ui \
    modeldepthviewer.ui \
    presetedit.ui \
    anaglyphmaker.ui \
    stringtype.ui

RESOURCES += \
    rsc.qrc

OTHER_FILES += \
    old_render.txt
