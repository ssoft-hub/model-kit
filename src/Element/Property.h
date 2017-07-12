#pragma once
#include <Helper/InstanceHelper.h>
#include <Value/Instance.h>

/*!
 * Свойство классификатора объекта (структуры или класса).
 * В зависимости от типа значения и его свойств, вычисляется наиболее оптимальный
 * вариант размещения его экземпляра.
 */
template < typename _ValueType, typename _ValueTool = ::Cpp::Inplace::InplaceTool >
using Property = typename InstanceHelper< _ValueType >::type;
