# Тип Holder

## Введение

Каждый инструментарий содержит тип **Holder**, который имеет стандартизованный интерфейс для управления временем жизни, наделением дополнительными свойствами и предоставлением доступа к экземпляру агрегированного значения.

Внутренняя реализация **Holder** должна обеспечивать наличие валидного экземпляра агрегированного значения в момент предоставления доступа, однако может не гарантировать, что последующий доступ будет предоставлен к тому же самому экземпляру агрегированного значения.

В случае выполнения операции переноса (`std::move(..)`) тип **Holder** может находиться в некорректном состоянии.

## Интерфейс

Реализация **Holder** имеет стандартизованный интерфейс следующего вида

```cpp
template < typename _Value >
struct Holder
{
    using ThisType = Holder< _Value >;
    using Value = _Value;

    template < typename ... _Arguments >
    Holder ( _Arguments && ... arguments );

    Holder ( ThisType && other );
    Holder ( const ThisType && other );
    Holder ( volatile ThisType && other );
    Holder ( const volatile ThisType && other );
    Holder ( ThisType & other );
    Holder ( const ThisType & other );
    Holder ( volatile ThisType & other );
    Holder ( const volatile ThisType & other );

    template < typename _OtherValue >
    Holder ( Holder< _OtherValue > && other );
    template < typename _OtherValue >
    Holder ( const Holder< _OtherValue > && other );
    template < typename _OtherValue >
    Holder ( volatile Holder< _OtherValue > && other );
    template < typename _OtherValue >
    Holder ( const volatile Holder< _OtherValue > && other );
    template < typename _OtherValue >
    Holder ( Holder< _OtherValue > & other );
    template < typename _OtherValue >
    Holder ( const Holder< _OtherValue > & other );
    template < typename _OtherValue >
    Holder ( volatile Holder< _OtherValue > & other );
    template < typename _OtherValue >
    Holder ( const volatile Holder< _OtherValue > & other );

    template < typename _Argument >
    void operator = ( _Argument && argument );

    void operator = ( ThisType && other );
    void operator = ( const ThisType && other );
    void operator = ( volatile ThisType && other );
    void operator = ( const volatile ThisType && other );
    void operator = ( ThisType & other );
    void operator = ( const ThisType & other );
    void operator = ( volatile ThisType & other );
    void operator = ( const volatile ThisType & other );

    template < typename _OtherValue >
    void operator = ( Holder< _OtherValue > && other );
    template < typename _OtherValue >
    void operator = ( const Holder< _OtherValue > && other );
    template < typename _OtherValue >
    void operator = ( volatile Holder< _OtherValue > && other );
    template < typename _OtherValue >
    void operator = ( const volatile Holder< _OtherValue > && other );
    template < typename _OtherValue >
    void operator = ( Holder< _OtherValue > & other );
    template < typename _OtherValue >
    void operator = ( const Holder< _OtherValue > & other );
    template < typename _OtherValue >
    void operator = ( volatile Holder< _OtherValue > & other );
    template < typename _OtherValue >
    void operator = ( const volatile Holder< _OtherValue > & other );

    static void guard ( ThisType && );
    static void guard ( const ThisType && );
    static void guard ( volatile ThisType && );
    static void guard ( const volatile ThisType && );
    static void guard ( ThisType & );
    static void guard ( const ThisType & );
    static void guard ( volatile ThisType & );
    static void guard ( const volatile ThisType & );

    static void unguard ( ThisType && );
    static void unguard ( const ThisType && );
    static void unguard ( volatile ThisType && );
    static void unguard ( const volatile ThisType && );
    static void unguard ( ThisType & );
    static void unguard ( const ThisType & );
    static void unguard ( volatile ThisType & );
    static void unguard ( const volatile ThisType & );

    static Value && value ( ThisType && holder );
    static const Value && value ( const ThisType && holder );
    static Value && value ( volatile ThisType && holder );
    static const Value && value ( const volatile ThisType && holder );
    static Value & value ( ThisType & holder );
    static const Value & value ( const ThisType & holder );
    static Value & value ( volatile ThisType & holder );
    static const Value & value ( const volatile ThisType & holder );
};
```

***Важно!*** *Необходимо явно реализовать все указанные методы. В противном случае компилятор может выбрать неверную интерпретацию методов, что может привести к ошибке компиляции.*

### Методы конструирования и операторы присвоения

#### Конструктор инициализации

Конструктор инициализации агрегированного значения по заданным параметрам используется, если количество параметров не равно 1 или тип заданного параметра не является этим же или подобным **Holder**.

```cpp
template < typename ... _Arguments >
Holder ( _Arguments && ... arguments );
```

Если для типа агрегированного значения не предусмотрен подходящий конструктор, то это приведет к ошибке компиляции.

#### Оператор присвоения заданному параметру

Оператор присвоения агрегированного значения заданному параметру используется, если тип заданного параметра не является этим же или совместимым **Holder**.

```cpp
template < typename _Argument >
void operator = ( _Argument && argument );
```
Если для типа агрегированного значения не предусмотрен подходящий оператор присвоения, то это приведет к ошибке компиляции.

#### Конструкторы копирования и перемещения для этого же типа Holder

Конструкторы копирования и перемещения для этого же типа **Holder** используются при полном соответствии типа параметра данному типу **Holder**.

```cpp
Holder ( ThisType && other );
Holder ( const ThisType && other );
Holder ( volatile ThisType && other );
Holder ( const volatile ThisType && other );
Holder ( ThisType & other );
Holder ( const ThisType & other );
Holder ( volatile ThisType & other );
Holder ( const volatile ThisType & other );
```

Для пользовательских типов **Holder** должна быть обязательно обеспечена реализация всех этих конструкторов, иначе вместо них на этапе компиляции будет использован конструктор инициализации агрегированного значения по заданным параметрам, что может привести к ошибке компиляции.

#### Операторы присвоения для этого же типа Holder

Операторы присвоения для этого же типа **Holder** используются при полном соответствии типа параметра данному типу **Holder**.

```cpp
void operator = ( ThisType && other );
void operator = ( const ThisType && other );
void operator = ( volatile ThisType && other );
void operator = ( const volatile ThisType && other );
void operator = ( ThisType & other );
void operator = ( const ThisType & other );
void operator = ( volatile ThisType & other );
void operator = ( const volatile ThisType & other );
```

Для пользовательских типов **Holder** должна быть обязательно обеспечена реализация всех этих операторов, иначе вместо них на этапе компиляции будет использован оператор присвоения заданному параметру, что может привести к ошибке компиляции.

#### Конструкторы копирования и перемещения для совместимого типа Holder< _OtherValue >

Конструкторы копирования и перемещения для совместимого типа **Holder< _OtherValue >** используются при совместимости типа параметра данному типу **Holder**. Типы **Holder** являются совместимыми, если тип агрегированного экземпляра значения параметра **Holder< _OtherValue >** является тем же самым или наследником от типа экземпляра агрегированного значения этого **Holder**, с учетом cv модификаторов (const/volatile).

```cpp
template < typename _OtherValue >
Holder ( Holder< _OtherValue > && other );
template < typename _OtherValue >
Holder ( const Holder< _OtherValue > && other );
template < typename _OtherValue >
Holder ( volatile Holder< _OtherValue > && other );
template < typename _OtherValue >
Holder ( const volatile Holder< _OtherValue > && other );
template < typename _OtherValue >
Holder ( Holder< _OtherValue > & other );
template < typename _OtherValue >
Holder ( const Holder< _OtherValue > & other );
template < typename _OtherValue >
Holder ( volatile Holder< _OtherValue > & other );
template < typename _OtherValue >
Holder ( const volatile Holder< _OtherValue > & other );
```

Для пользовательских типов **Holder** должна быть обязательно обеспечена реализация всех этих конструкторов, иначе вместо них на этапе компиляции будет использован конструктор инициализации агрегированного значения по заданным параметрам, что может привести к ошибке компиляции.

#### Операторы присвоения для совместимого типа Holder< _OtherValue >

Операторы присвоения для совместимого типа **Holder< _OtherValue >** используются при совместимости типа параметра данному типу **Holder**. Типы **Holder** являются совместимыми, если тип агрегированного экземпляра значения параметра **Holder< _OtherValue >** является тем же самым или наследником от типа экземпляра агрегированного значения этого **Holder**, с учетом cv модификаторов (const/volatile).

```cpp
template < typename _OtherValue >
void operator = ( Holder< _OtherValue > && other );
template < typename _OtherValue >
void operator = ( const Holder< _OtherValue > && other );
template < typename _OtherValue >
void operator = ( volatile Holder< _OtherValue > && other );
template < typename _OtherValue >
void operator = ( const volatile Holder< _OtherValue > && other );
template < typename _OtherValue >
void operator = ( Holder< _OtherValue > & other );
template < typename _OtherValue >
void operator = ( const Holder< _OtherValue > & other );
template < typename _OtherValue >
void operator = ( volatile Holder< _OtherValue > & other );
template < typename _OtherValue >
void operator = ( const volatile Holder< _OtherValue > & other );
```

Для пользовательских типов **Holder** должна быть обязательно обеспечена реализация всех этих операторов, иначе вместо них на этапе компиляции будет использован оператор присвоения заданному параметру, что может привести к ошибке компиляции.

### Методы применения особенностей

Данный набор методов предусматривает включение **guard()** и отключение **unguard()** дополнительных особенностей к внутреннему агрегированному экземпляру значения. В зависимости от типа ссылки на значение **Holder** (rvalue/lvalue) данные методы позволяют реализовать разные способы применения свойств.

```cpp
static void guard ( ThisType && );
static void guard ( const ThisType && );
static void guard ( volatile ThisType && );
static void guard ( const volatile ThisType && );
static void guard ( ThisType & );
static void guard ( const ThisType & );
static void guard ( volatile ThisType & );
static void guard ( const volatile ThisType & );

static void unguard ( ThisType && );
static void unguard ( const ThisType && );
static void unguard ( volatile ThisType && );
static void unguard ( const volatile ThisType && );
static void unguard ( ThisType & );
static void unguard ( const ThisType & );
static void unguard ( volatile ThisType & );
static void unguard ( const volatile ThisType & );
```

Обычно (но не обязательно) эти методы вызываются в совокупности с методом **value()**, в следующем порядке - **guard()**, **value()**, **unguard()**.
Для пользовательских типов **Holder** должна быть обязательно обеспечена реализация всех этих методов, отсутвие хотя бы одного из них может привести к ошибке компиляции.

### Доступ к экземпляру агрегированного значения

Доступ к экземпляру агрегированного значения осуществляются с помощью методов **value()**.

```cpp
static Value && value ( ThisType && holder );
static const Value && value ( const ThisType && holder );
static Value && value ( volatile ThisType && holder );
static const Value && value ( const volatile ThisType && holder );
static Value & value ( ThisType & holder );
static const Value & value ( const ThisType & holder );
static Value & value ( volatile ThisType & holder );
static const Value & value ( const volatile ThisType & holder );
```

Для пользовательских типов **Holder** должна быть обязательно обеспечена реализация всех этих методов, отсутвие хотя бы одного из них может привести к ошибке компиляции.
