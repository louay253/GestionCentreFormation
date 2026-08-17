QT += core gui sql charts printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

SOURCES += \
    connection.cpp \
    formateur.cpp \
    main.cpp \
    mainwindow.cpp \
    stagiaire.cpp \
    qrcodegen.cpp

HEADERS += \
    connection.h \
    formateur.h \
    mainwindow.h \
    stagiaire.h \
    qrcodegen.hpp

FORMS += \
    mainwindow.ui

RESOURCES += \
    resources.qrc

# Default rules for deployment
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
