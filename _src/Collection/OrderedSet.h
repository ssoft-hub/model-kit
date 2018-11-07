#pragma once

/*!
 * Упорядоченное множество уникальных элементов.
 *
 * Возможны реализации на основе ::std::set и др.
 */
template < typename _Type, typename _MultiplicityTool >
class OrderedSet;

/*!
 * Упорядоченное множество отображения уникальных ключей в неуникальные элементы.
 *
 * Возможны реализации на основе ::std::map, ::std::unordered_map и др.
 */
template < typename _Key, typename _Value, typename _MultiplicityTool >
class OrderedMap;
