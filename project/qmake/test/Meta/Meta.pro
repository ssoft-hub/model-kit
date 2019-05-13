# Тип сборки
TEMPLATE = app
TARGET = Test-ModelKit-Meta

# Перечень зависимостей
DEPENDS *= \
    ModelKit \

CONFIG *= c++14

# Включение исходных файлов
!load( include_files ) : error( Can not find feature \"include_files\" )
includeFiles( $${PWD}/../../../../test/Meta )
