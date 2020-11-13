QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  \
            ../app/login/loginchecker.cpp \
            test_login.cpp \
            test_main.cpp

HEADERS += ../app/login/loginchecker.h \
    test_login.h
