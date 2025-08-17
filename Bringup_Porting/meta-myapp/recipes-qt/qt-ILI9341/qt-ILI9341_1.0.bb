SUMMARY = "Qt6 ILI9341 doreamon application"
DESCRIPTION = "GUI app built with Qt6 using ILI9341 display"
LICENSE = "CLOSED"

SRC_URI = " \
    file://qt_ILI9341.pro \
    file://main.cpp \
    file://mainwindow.cpp \
    file://mainwindow.h \
    file://mainwindow.ui \
    file://resource.qrc \
    file://images/btn_cake.png \
    file://images/btn_mourse.png \
    file://images/btn_nobita.jpg \
    file://images/doraemon_eating.png \
    file://images/doraemon_nobita.png \
    file://images/doraemon.png \
    file://images/doraemon_scared.jpg \
"

S = "${WORKDIR}"

DEPENDS += "qtbase qtdeclarative qttools"

inherit qt6-qmake

do_install() {
    install -d ${D}${bindir}
    install -m 0755 qt_ILI9341 ${D}${bindir}/
}
