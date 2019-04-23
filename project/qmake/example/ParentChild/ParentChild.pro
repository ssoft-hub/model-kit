# Тип сборки
TEMPLATE = app
TARGET = Example-ModelKit-ParentChild
CONFIG *= console

# Перечень зависимостей
DEPENDS *= \
    ModelKit \

# Включение исходных файлов
!load( include_files ) : error( Can not find feature \"include_files\" )
includeFiles( $${PWD}/../../../../example/ParentChild )
