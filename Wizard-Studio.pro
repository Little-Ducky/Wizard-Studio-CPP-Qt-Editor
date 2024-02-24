QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    codeeditor.cpp \
    dialoggetpathtofile.cpp \
    dialogsettingprogram.cpp \
    dialogunacceptable.cpp \
    formoptionbuild.cpp \
    formoptionview.cpp \
    linenumarea.cpp \
    main.cpp \
    mainwindow.cpp \
    output.cpp \
    sidebar.cpp \
    splashscreen.cpp \
    startsection.cpp \
    worksection.cpp

HEADERS += \
    codeeditor.h \
    dialoggetpathtofile.h \
    dialogsettingprogram.h \
    dialogunacceptable.h \
    formoptionbuild.h \
    formoptionview.h \
    linenumarea.h \
    mainwindow.h \
    output.h \
    sidebar.h \
    splashscreen.h \
    startsection.h \
    worksection.h

FORMS += \
    dialoggetpathtofile.ui \
    dialogsettingprogram.ui \
    dialogunacceptable.ui \
    formoptionbuild.ui \
    formoptionview.ui \
    mainwindow.ui \
    output.ui \
    splashscreen.ui \
    startsection.ui \
    worksection.ui

RESOURCES += \
    sources.qrc


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
