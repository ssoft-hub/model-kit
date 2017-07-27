#pragma once
#include <ModelKit/Helper/DefaultHelper.h>
#include <ModelKit/Value/Instance.h>

/*!
 * Свойство классификатора объекта (структуры или класса).
 */
template < typename _ValueType, typename _ValueTool = ::Cpp::Inplace::DefaultTool >
using Property = typename DefaultHelper< _ValueType >::type;
