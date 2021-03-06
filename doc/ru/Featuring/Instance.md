# Тип Instance

Тип **Instance** является классом-оберткой вокруг любого пользовательского типа.

```cpp
template < typename _Value, typename _Tool >
class Instance;
```

Экземпляр типа **Instance** полностью эмулирует сам экземпляр типа **_Value** с добавлением любых особенностей, реализуемых посредством инструментария **_Tool**.

К таким особенностям относятся, например,
- выбор размещения экземпляра **_Value**: по месту владельца/в куче.
- ленивые вычисления: copy-on-write и т.п.
- потокобезопасность
- и др.

Собственно, экземпляр типа **Instance** агрегирует (любым способом) экземпляр типа **_Value**, владеет им, управляет временем его жизни, обеспечивает применение дополнительных свойств и предоставляет доступ к экземпляру типа **_Value**.



# Суперпозиция Instance

Возможна суперпозиция при применении обертки **Instance**. Порядок декларации оберток в суперпозиции определяет порядок применения свойств к внутреннему экземпляру значения **_Value**.

Например, в записи

```cpp
Instance< Instance< Value, Implicit::SharedTool >, ThreadSafe::RecursiveMutexTool > first;
Instance< Instance< Value, ThreadSafe::RecursiveMutexTool >, Implicit::SharedTool > second;
```

экземпляр значения ```first``` является потокобезопасным неявно распределнным значением типа **Value**, а экземпляр значения ```second``` - наявно распределенным потокобезопасным значением типа **Value**. И это не одно и тоже! По своей сути сам экземпляр значения ```first``` не является неявно распределенным, а ```second``` - потокобезопасным. Этими особенностями обладают вложенные в них значения.

# Совместимость типов и вложенность Instance

Для проверки, является ли тип разновидностью **Instance**, определено значение

```cpp
template < typename _Test >
static constexpr bool is_instance;
```
которое равно ```true```, если тип **_Test** является **Instance**, и ```false``` в остальных случаях.

Между конкретными типами определено отношение совместимости, для проверки условия, когда первый тип **_Test** может быть представлен, как другой **_Other**.

```cpp
template < _Test, _Other >
static constexpr bool is_compatible;
```

Для произвольных типов значение этого отношение равно ```true```, если тип **_Other** является базовым или идентичным типу **_Test** с учетом const/volatile модификаторов. Для типов **Instance** значение отношения совместимости равно ```true```, если для них задан одинаковый инструментарий **_Tool** и типы внутренних значений **_Value** являются совместимыми. В остальных случаях значение отношения совместимости равно ```false```.

Для конкретных типов **Instance** также определено отношение вложенности, для проверки условия, когда внутренний тип **_Other** (любой степени вложенности) совместим с типом **_Test**.

```cpp
template < _Test, _Other >
static constexpr bool is_this_part_of_other;
```

Для произвольных типов значение этого отношения равно ```false```. Для типов **Instance** значение отношения равно ```true```, если тип **_Test** совместим с любой вложенной частью типа **_Other**.

Совместимость и вложенность типов **Instance** является важным аспектом в реализации их взаимодействия в бинарных операторах. Если типы **Instance** совместимы, то для них может быть реализована специализация бинарных операторов. Если типы **Instance** имеют отношение вложености, то они "раскрываются" до совместимых типов, между которыми реализуется бинарный оператор.

# Доступ к значениям

Перед тем, как для **Instance** предоставить доступ к внутреннему экземпляру значения **_Value**, к последнему необходимо применить особенности, реализуемые инструментарием **_Tool**. Применение этих особенностей обеспечивают типы из раздела **Access** - **InstanceGuard**, **ValueGuard** и **ValuePointer**.

Если у вложенного типа **_Value** не переопределен унарный оператор получения адреса '&', то результатом действия этого оператора будет специальный указатель **ValuePointer**, который на всем протяжении своего времени жизни обеспечивает применение всех особенностей для экземпляра значения **_Value**, на который он указывает.
