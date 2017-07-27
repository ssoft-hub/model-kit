TEMPLATE = app
QT *= core gui widgets

INCLUDEPATH = \
    $${PWD}/include \
    $${PWD}/src \

include( $${PWD}/files.prf )
includeFiles( $${PWD}/ )

QMAKE_CXXFLAGS = -std=c++14
