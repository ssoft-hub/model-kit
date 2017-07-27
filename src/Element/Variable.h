#pragma once
#include <ModelKit/Element/Property.h>

/*!
 * Переменная поведенческой сущности (пространства имен метода).
 */
template < typename _ValueType, typename _ValueTool = ::Cpp::Inplace::DefaultTool >
using Variable = Property< _ValueType, _ValueTool >;
