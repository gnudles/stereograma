#-------------------------------------------------
#
# Project created by QtCreator 2011-07-24T20:40:23
#
#-------------------------------------------------

QT       += core gui network opengl widgets

TARGET = stereograma
TEMPLATE = app

linux:LIBS += -lGLU
win32:LIBS += -lGLU32 -lOpengl32
win32:RC_FILE = icon_res.rc
win32:QMAKE_CXXFLAGS += -Wa,-mbig-obj
DEFINES+= USE_FLOAT

SOURCES += main.cpp\
    ClickableLabel.cpp \
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
    RPly/rply.c \
    parse.cpp \
    FormulaGen.cpp

HEADERS  += mainwindow.h \
    ClickableLabel.h \
    exprtk/exprtk.hpp \
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
    RPly/rplyfile.h \
    parse.h \
    FormulaGen.h

FORMS    += mainwindow.ui \
    imageviewer.ui \
    modeldepthviewer.ui \
    presetedit.ui \
    anaglyphmaker.ui \
    stringtype.ui \
    FormulaGen.ui

RESOURCES += \
    rsc.qrc

OTHER_FILES += \
    old_render.txt
