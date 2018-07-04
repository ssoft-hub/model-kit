# Тип сборки
TEMPLATE = app
TARGET = Test-ModelKit-Develop

# Перечень зависимостей
DEPENDS *= \
    ModelKit \

# Включение исходных файлов
!load( include_files ) : error( Can not find feature \"include_files\" )
includeFiles( $${PWD}/../../../../test/Develop )
