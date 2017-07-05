#pragma once
#include <Helper/InstanceHelper.h>
#include <Value/Instance.h>
#include <Value/Tool/Cpp/Inplace/InplaceTool.h>

/*!
 * Свойство классификатора объекта (структуры или класса).
 * В зависимости от типа значения и его свойств, вычисляется наиболее оптимальный
 * вариант размещения его экземпляра.
 */
template < typename _Type >
using Property = typename InstanceHelper< _Type >::InstanceType;
