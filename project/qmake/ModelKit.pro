# Тип сборки
TEMPLATE = lib

# Включение исходных файлов
!load( include_files ) : error( Can not find feature \"include_files\" )
includeFiles( $${PWD}/../../src )

# Включение проектных файлов
OTHER_FILES *= \
    $$files( $${PWD}/*.prf ) \
