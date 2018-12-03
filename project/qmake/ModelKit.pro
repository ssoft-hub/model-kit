# Тип сборки
TEMPLATE = lib
CONFIG *= static

# Включение исходных файлов
!load( include_files ) : error( Can not find feature \"include_files\" )
includeFiles( $${PWD}/../../include )
includeFiles( $${PWD}/../../src )
includeFiles( $${PWD}/../../doc )

# Включение проектных файлов
OTHER_FILES *= \
    $$files( $${PWD}/*.prf ) \
