����� ������������� ��������� ����� ������������ ������. ��� ��������� ����� �� ����� ���� ������������ �������� ��� ��������� ������ ������������ ���������.
��� ����������� ���������� ��������������� ����� ����� ��������������� ����������� �������.

��������� ����� ������������ ��������� ������ ������������� ���� ������ (������ � �������� ����� ������������ ���������) � ���������� project/qmake

* ���������������� ���� ��������� project/qmake/config.prf
  � ���� ����� ������������: ���� � ��� �������� ����������� ������.
  ������� ����������:

____________________________________________________________________________________________________
    # ����� ������������ ���������
    ################################################################################
    CONFIG *= \
        embed_manifest_exe \
        exceptions \
        largefile \
        object_with_source \
        precompile_header \
        rtti \
        silent \
        stl \
        thread \
        warn_on \

    !win32-msvc* {
        CONFIG -= \
            debug_and_release \
            debug_and_release_target \
    }

    # ���� ������ ���������
    ################################################################################
    CONFIG( debug, debug|release ) {
        DESTDIR = $${PWD}/../../bin/debug
        GENERATED_FILES_DIR=$${PWD}/../../build/debug/$${TARGET}
    } else {
        DESTDIR = $${PWD}/../../bin/release
        GENERATED_FILES_DIR=$${PWD}/../../build/release/$${TARGET}
    }

    LIBS *= -L$${DESTDIR}
    
    # ������������ ������������ ������
    ################################################################################
    OBJECTS_DIR = $${GENERATED_FILES_DIR}/obj
    MOC_DIR = $${GENERATED_FILES_DIR}/moc
    RCC_DIR = $${GENERATED_FILES_DIR}/rcc
    UI_DIR = $${GENERATED_FILES_DIR}/ui

    # �������������� ���������� ������������ ����� ������������ ���������
    ################################################################################
    include( $${PWD}/depends_resolver.prf )
====================================================================================================

* ���� project/qmake/depends_resolver.prf
  �������� ��������� ������ ����������� ������� � ���������� ������������ ����� ������������ ���������
  ����������� ����������:

____________________________________________________________________________________________________
    # ���������� ������������ ����� ������������ ���������
    ################################################################################

    # private
    defineTest( loadDependency ) {
        for( module, ARGS ) {

            DEPENDS -= $${module}
            LOADED_PRF *= $${module}

            for(qq, DEPEND_PATH) {
                q = $${qq}/project/qmake/$${module}/depends.prf
                exists($${q}): break()
                q = $${qq}/$${module}/project/qmake/$${module}/depends.prf
                exists($${q}): break()
            }
            !include( $${q} ) {
                error([$$basename(_PRO_FILE_)] Dependency resolver cannot find the module: $${module})
            }
        }

        export( QMAKE_CXXFLAGS )
        export( DEPENDS )
        export( LOADED_PRF )
        export( INCLUDEPATH )
        export( DEPENDPATH )
        export( LIBS )
        export( QT )
        export( DEFINES )
        export( HEADERS )
        export( SOURCES )
        return( true )
    }

    # private
    defineTest( recursiveLoadAllDependicies ) {
        for( tmp, 1..100 ) {
            !isEmpty( DEPENDS ) {
                DEPENDS = $$unique( DEPENDS )
                loadDependency( $${DEPENDS} )
            } else {
                break()
            }
        }
        # more than
        !isEmpty( DEPENDS ) {
            error(insufficient depth)
        }

        export (QMAKE_CXXFLAGS)
        export (DEPENDS)
        export (LOADED_PRF)
        export (INCLUDEPATH)
        export (DEPENDPATH)
        export (LIBS)
        export (QT)
        export (DEFINES)
        export (HEADERS)
        export (SOURCES)
        return(true)
    }

    # public
    defineTest(loadDependencies) {

        exists( $$dirname(_PRO_FILE_)/depends.prf ) {
            include( $$dirname(_PRO_FILE_)/depends.prf )
            $${TARGET}.depends = $${DEPENDS}
            loadDependency( $${DEPENDS} )
            LIBS -= -l$${TARGET}
        }

        recursiveLoadAllDependicies()

        QMAKE_CXXFLAGS = $$unique(QMAKE_CXXFLAGS)
        DEPENDS = $$unique(DEPENDS)
        LOADED_PRF = $$unique(LOADED_PRF)
        INCLUDEPATH = $$unique(INCLUDEPATH)
        DEPENDPATH = $$unique(DEPENDPATH)
        LIBS = $$unique(LIBS)
        QT = $$unique(QT)
        DEFINES = $$unique(DEFINES)
        HEADERS = $$unique(HEADERS)
        SOURCES = $$unique(SOURCES)

        export (QMAKE_CXXFLAGS)
        export (DEPENDS)
        export (LOADED_PRF)
        export (INCLUDEPATH)
        export (DEPENDPATH)
        export (LIBS)
        export (QT)
        export (DEFINES)
        export (HEADERS)
        export (SOURCES)
        return(true)
    }

    # global
    include( $${PWD}/depends_path.prf )
    loadDependencies()
====================================================================================================

* ���� project/qmake/depends_path.prf
  �������� �������� ����������, � ������� ������������� ������.
  ����������� ��� ������������ ��������� �������.
  ���������� DEPEND_PATH ������ ��������� �������� ����� ������� ������������ ������
  (�����, ��� ������������� project/qmake/<Module>/<Module>.pro), ���� ����� � ������ �����������
  �������, ���� �� ���� ����� �������� � ���� <Module>/project/qmake/<Module>/<Module>.pro.
  ������� ����������:
____________________________________________________________________________________________________

# ������� �����, � ������� �������������� ����� ��������� ������ �����������
################################################################################

DEPEND_PATH *= \
    $${PWD}/ ... \
    ... \

====================================================================================================
