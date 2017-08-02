Здесь располагаются проектные файлы программного модуля. Эти проектные файлы не могут быть использованы отдельно без проектных файлов программного комплекса.
Под программным комплексом подразумевается любой набор взаимосвязанных программных модулей.

Проектные файлы программного комплекса должны располагаться выше модуля (обычно в корневой папке программного комплекса) в директории project/qmake

* Конфигурационный файл комплекса project/qmake/config.prf
  В этом файле определяется: куда и как собирать программные модули.
  ТИПОВОЕ СОДЕРЖИМОЕ:

____________________________________________________________________________________________________
    # Общая конфигурация комплекса
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

    # Пути сборки комплекса
    ################################################################################
    CONFIG( debug, debug|release ) {
        DESTDIR = $${PWD}/../../bin/debug
        GENERATED_FILES_DIR=$${PWD}/../../build/debug/$${TARGET}
    } else {
        DESTDIR = $${PWD}/../../bin/release
        GENERATED_FILES_DIR=$${PWD}/../../build/release/$${TARGET}
    }

    LIBS *= -L$${DESTDIR}
    
    # Расположение генерируемых файлов
    ################################################################################
    OBJECTS_DIR = $${GENERATED_FILES_DIR}/obj
    MOC_DIR = $${GENERATED_FILES_DIR}/moc
    RCC_DIR = $${GENERATED_FILES_DIR}/rcc
    UI_DIR = $${GENERATED_FILES_DIR}/ui

    # Автоматическое разрешение зависимостей между компонентами комплекса
    ################################################################################
    include( $${PWD}/depends_resolver.prf )
====================================================================================================

* Файл project/qmake/depends_resolver.prf
  Содержит процедуру поиска программных модулей и разрешения зависимостей между компонентами комплекса
  СТАНДАРТНОЕ СОДЕРЖИМОЕ:

____________________________________________________________________________________________________
    # Разрешение зависимостей между компонентами комплекса
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

* Файл project/qmake/depends_path.prf
  Содержит перечень директорий, в которых располагаются модули.
  ФОРМИРУЕТСЯ ДЛЯ ПРОГРАММНОГО КОМПЛЕКСА ВРУЧНУЮ.
  Переменная DEPEND_PATH должна содержать перечень путей каждому программному модулю
  (месту, где располагается project/qmake/<Module>/<Module>.pro), либо сразу к группе программных
  модулей, если их пути можно привести к виду <Module>/project/qmake/<Module>/<Module>.pro.
  ТИПОВОЕ СОДЕРЖИМОЕ:
____________________________________________________________________________________________________

# Задание путей, в которых осуществляется поиск проектных файлов компонентов
################################################################################

DEPEND_PATH *= \
    $${PWD}/ ... \
    ... \

====================================================================================================
