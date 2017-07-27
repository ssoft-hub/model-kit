# Тип сборки
TEMPLATE = app
TARGET = Test-ModelKit-Develop

# Перечень зависимостей
DEPENDS *= \
    ModelKit \

# Поиск общей конфигурации комплекса
include( $${PWD}/config_finder.prf )

# Включение исходных файлов
include( $${PWD}/files.prf )
includeFiles( $${PWD}/../../../../test/Develop )
