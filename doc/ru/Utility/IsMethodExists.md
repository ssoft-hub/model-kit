# Проверка существования метода на этапе компиляции

## Макроопределение MDK_IS_METHOD_EXISTS_TRAIT

Данное макроопределение позволяет свормировать структуры и переменные для возможности проверки у типа наличия метода с заданным наименованием. Имя метода передается в виде параметра.

```cpp
#define MDK_IS_METHOD_EXISTS_TRAIT( method )
```

Макрос явно определяет вспомогательные типы ```Is_ ## method ## _MethodExistsHelper```, ```Is_ ## method ## _MethodExists``` и переменную ```static constexpr bool is_ ## method ## _method_exists```, поэтому рекомендуется его использовать в отдельных пространствах имен.

Наличие метода можно проверить, как по входным параметрам, так и по полной сигнатуре (с учетом возвращаемого типа).

## Пример

```cpp
namespace Test // struct, class, etc.
{
    MDK_IS_METHOD_EXISTS_TRAIT( setValue )
    MDK_IS_METHOD_EXISTS_TRAIT( getValue )
}

class MyClass
{
    void setValue( int );
    int getValue();
};

static constexpr bool a1 = ::Test::is_setValue_method_exists< MyClass, int >; // true
static constexpr bool a2 = ::Test::is_setValue_method_exists< MyClass, double >; // false
static constexpr bool a3 = ::Test::is_setValue_method_exists< MyClass, void(int) >; // true
static constexpr bool a4 = ::Test::is_setValue_method_exists< MyClass, void(double) >; // false
static constexpr bool a5 = ::Test::is_setValue_method_exists< MyClass, int(int) >; // false

static constexpr bool b1 = ::Test::is_getValue_method_exists< MyClass, void(int) >; // false
static constexpr bool b2 = ::Test::is_getValue_method_exists< MyClass, void() >; // false
static constexpr bool b3 = ::Test::is_getValue_method_exists< MyClass >; // true
static constexpr bool b4 = ::Test::is_getValue_method_exists< MyClass, int() >; // true
static constexpr bool b5 = ::Test::is_getValue_method_exists< MyClass, int(void) >; // true
static constexpr bool b6 = ::Test::is_getValue_method_exists< MyClass, int(double) >; // false
static constexpr bool b7 = ::Test::is_getValue_method_exists< MyClass, double() >; // false
```

Применение ```::Test::is_setValue_method_exists< MyClass, int >``` эквивалентно ```::Test::Is_setValue_MethodExists< MyClass, int >::value``` и ```::Test::Is_setValue_MethodExistsHelper< MyClass, int >::Type::value```.