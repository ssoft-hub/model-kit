# Тип сборки
TEMPLATE = app
TARGET = Example-ModelKit-BaseDerived

# Перечень зависимостей
DEPENDS *= \
    ModelKit \

# Поиск общей конфигурации комплекса
include( $${PWD}/config_finder.prf )

# Включение исходных файлов
include( $${PWD}/files.prf )
includeFiles( $${PWD}/../../../../example/BaseDerived )
