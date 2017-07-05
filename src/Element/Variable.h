#pragma once
#include <Element/Property.h>

/*!
 * Переменная поведенческой сущности (пространства имен метода).
 * Переопределяет тип Inplace, имеет условно-декларативный смысл.
 */
template < typename _Type >
using Variable = Property< _Type >;
