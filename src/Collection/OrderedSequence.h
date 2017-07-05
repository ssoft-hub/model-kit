#pragma once

/*!
 * Упорядоченное множество неуникальных элементов с произвольным доступом
 * и включением извлечением по целочисленному индексу.
 *
 * Возможны реализации ::std::list, ::std::vector и др.
 */
template < typename _Type, typename _MultiplicityTool >
class List;

/*!
 * Упорядоченное множество неуникальных элементов.
 * Реализует FIFO порядок включения извлечения.
 */
template < typename _Type, typename _MultiplicityTool >
class Queue;

/*!
 * Упорядоченное множество неуникальных элементов.
 * Реализует LIFO порядок включения извлечения.
 */
template < typename _Type, typename _MultiplicityTool >
class Stack;
