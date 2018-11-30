# Тип Tool

## Введение

Тип **Tool** вспомогательным типом, введенным для удобства работы с **Holder** в составе **Featured**.

## Применение

Введением вспомогательного типа **Tool** позволяет упростить определение различных типо **Featured** и использовать запись вида

```cpp
Featured< Type, Tool > value;
```

вместо

```cpp
Featured< Type, Holder< Type > > value;
```

В последней записи требуется корректно указывать параметры шаблона для **Holder**, что является потенциальным местом для ошибок. К тому же, ещё одной функцией **Tool** является возможность дополнительной шаблонной параметризации **Holder**, например, так

```cpp
template < typename _Locker >
struct MyLockTool
{
    template < typename _Value >
    struct Holder
    {
        _Locker m_locker;
        // ...
    };
};

```

и использования в дальнейшем записей в виде

```cpp
Featured< Type, MyLockTool< ::std::mutex > > first;
Featured< Type, MyLockTool< ::std::recursive_mutex > > second;
Featured< Type, MyLockTool< ::std::recursive_timed_mutex > > third;
```
