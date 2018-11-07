#pragma once

/*!
 * Неупорядоченное множество уникальных элементов.
 * Реализует включение и извлечение по значению.
 *
 * Возможны реализации на основе ::std::unordered_set и др.
 */
template < typename _Type, typename _MultiplicityTool >
class UnorderedSet;


/*!
 * Упорядоченное множество отображения уникальных ключей в неуникальные элементы.
 *
 * Возможны реализации на основе ::std::unordered_map и др.
 */
template < typename _Key, typename _Value, typename _MultiplicityTool >
class UnorderedMap;
