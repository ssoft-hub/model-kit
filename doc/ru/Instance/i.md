**Instance** - это то, что похоже на само значение, но с дополнительными особенностями.

**ValueType** - тип, на который имитирует **Instance**.

**ValueTool** - инструмент, используемый **Instance** для наделения **ValueType** дополнительными особенностями.

**HolderType** - тип, агрегирующий экземпляр **ValueType** и имеющий необходимые свойства для управления дополнительными особенностями.

**Instance** агрегирует только экземпляр **HolderType**.

**InstanceResolver** - вычисляет необходимый способ предоставления доступа к значению Instance при конструировании или операции присвоения.

Если типы Instance совместимы или другой совместим с какой-либо внутренней частью этого, то операция конструирования или присвоения выполняется между внутренними представлениями Instance. Если данный тип совместим с внутренней частью другого, то другой "разрешается" до типа совместимого с этим. Во всех других случаях другой "разрешается" до базового внутреннего значения.

**InstanceCompatibleResolver**

**InstanceFeatureResolver**

**InstanceValueResolver**