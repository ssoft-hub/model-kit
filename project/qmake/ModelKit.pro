# Тип сборки
TEMPLATE = lib
CONFIG *= static

# Включение исходных файлов
!load( include_files ) : error( Can not find feature \"include_files\" )
includeFiles( $${PWD}/../../include )
includeFiles( $${PWD}/../../src )

# Включение проектных файлов
OTHER_FILES *= \
    $$files( $${PWD}/*.prf ) \
