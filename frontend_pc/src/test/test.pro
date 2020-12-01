QT += testlib
QT += core gui
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  test_login.cpp \
            ../app/Login/loginchecker.cpp \
            ../app/commu/communhttp.cpp \
            ../app/commu/patient.cpp \
            ../app/commu/userinfo.cpp \
            ../app/commu/imageinfo.cpp \
            test_image.cpp \
            test_main.cpp \
            test_commun.cpp \
            test_patient.cpp

HEADERS += ../app/Login/loginchecker.h \
    ../app/commu/communhttp.h \
    ../app/commu/patient.h \
    ../app/commu/urlbase.h \
    ../app/commu/userinfo.h \
    ../app/commu/imageinfo.h \
    test_commun.h \
    test_image.h \
    test_login.h \
    test_patient.h

