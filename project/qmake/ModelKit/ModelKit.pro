# Тип сборки
TEMPLATE = lib
DEFINES *= SDK_EXPORT_MODELKIT
PRECOMPILED_HEADER = precompiled.h

# Поиск общей конфигурации комплекса
include( $${PWD}/config_finder.prf )

# Включение исходных файлов
include( $${PWD}/files.prf )
includeFiles( $${PWD}/../../../src )

# Включение проектных файлов
OTHER_FILES *= \
    $${PWD}/precompiled.h \
    $${PWD}/depends.prf \
    $${PWD}/files.prf \
