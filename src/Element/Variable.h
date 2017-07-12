#pragma once
#include <Element/Property.h>

/*!
 * Переменная поведенческой сущности (пространства имен метода).
 * Переопределяет тип Inplace, имеет условно-декларативный смысл.
 */
template < typename _ValueType, typename _ValueTool = ::Cpp::Inplace::InplaceTool >
using Variable = Property< _ValueType, _ValueTool >;
