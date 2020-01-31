#-------------------------------------------------
#
# Project created by QtCreator 2018-07-02T16:06:02
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MotionPlanning
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    painter_widget.cpp \
    tree.cpp \
    rrt.cpp \
    common.cpp \
    node.cpp \
    rrt_star.cpp \
    rrt_connect.cpp \
    rrt_star_connect.cpp \
    rrt_star_fn.cpp \
    robot.cpp \
    point.cpp \
    rrt_dubin.cpp \
    path.cpp \
    rrt_connect_dubin.cpp \
    common_func.cpp \
    rrtm.cpp

HEADERS += \
        mainwindow.h \
    painter_widget.h \
    painter_widget.h \
    tree.h \
    common.h \
    rrt.h \
    node.h \
    rrt_star.h \
    rrt_connect.h \
    rrt_star_connect.h \
    rrt_star_fn.h \
    robot.h \
    point.h \
    rrt_dubin.h \
    path.h \
    rrt_connect_dubin.h \
    common_func.h \
    rrtm.h

FORMS += \
        mainwindow.ui
