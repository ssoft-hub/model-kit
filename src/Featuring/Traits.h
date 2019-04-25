#pragma once
#ifndef SCL_FEATURED_TRAITS_H
#define SCL_FEATURED_TRAITS_H

#include <type_traits>

namespace Inplace { struct DefaultTool; }
namespace SclPrivate { template < typename _Value, typename _Tool > class Instance; }
namespace SclPrivate { template < typename, typename > class Instance; }

namespace SclPrivate
{
    template < typename _Type > struct InstanceSimplifyHelper { using Type = _Type; };
    template < typename _Value, typename _Tool >
    struct InstanceSimplifyHelper< ::SclPrivate::Instance< ::SclPrivate::Instance< _Value, _Tool >, _Tool > >
    { using Type = ::SclPrivate::Instance< _Value, _Tool >; };
}

namespace Scl
{
    template < typename _Value, typename _Tool = ::Inplace::DefaultTool >
    using Instance = typename ::SclPrivate::InstanceSimplifyHelper< ::SclPrivate::Instance< _Value, _Tool > >::Type;
        //!< This is definition of Instance type. No duplicates guarantee.
}

namespace Scl
{
    template < typename _Test > struct IsInstance;
    template < typename _Test > static constexpr bool is_instance = IsInstance< _Test >::value;
        //!< Признак того, что экземпляр данного типа является Instance.

    template < typename _Test, typename _Other > struct IsCompatible;
    template < typename _Test, typename _Other > static constexpr bool is_compatible = IsCompatible< _Test, _Other >::value;
        //!< Признак совместимости типов.

    template < typename _Test, typename _Other > struct IsThisPartOfOther;
    template < typename _Test, typename _Other > static constexpr bool is_this_part_of_other = IsThisPartOfOther< _Test, _Other >::value;
        //!< Признак вложенности одного типа в другой.

    template < typename _Type, typename _Other > struct IsSimilar;
    template < typename _Type, typename _Other > static constexpr bool is_similar = IsSimilar< _Type, _Other >::value;
        //!< Признак подобия типов. Типы считаются подобными, если у них одинаковые
        /// признаки const/volatile и rvalue/lvalue.
}

namespace SclPrivate
{
    template < typename _Type, typename _Refer >
    struct SimilarReferHelper
    {
        //static_assert( !::std::is_reference< _Type >::value, "The template parameter _Type must to be a not reference." );
        static_assert( ::std::is_reference< _Refer >::value, "The template parameter _Refer must to be a reference type." );

        using ValueFromRefer = ::std::remove_reference_t< _Refer >;
        static constexpr bool is_lvalue = ::std::is_lvalue_reference< _Refer >::value;
        static constexpr bool is_rvalue = ::std::is_rvalue_reference< _Refer >::value;
        static constexpr bool is_const = ::std::is_const< ValueFromRefer >::value;
        static constexpr bool is_volatile = ::std::is_volatile< ValueFromRefer >::value;

        using VCheckedType = ::std::conditional_t< is_volatile, ::std::add_volatile_t< _Type >, _Type >;
        using CVCheckedType =::std::conditional_t< is_const, ::std::add_const_t< VCheckedType >, VCheckedType >;
        using Type = ::std::conditional_t< is_rvalue, ::std::add_rvalue_reference_t< CVCheckedType >, ::std::add_lvalue_reference_t< CVCheckedType > >;

        //static_assert( ::Scl::is_similar< Type, _Refer >, "Result Type must to be similar _Refer" );
    };
}

namespace Scl
{
    template < typename _Type, typename _Refer >
    using SimilarRefer = typename ::SclPrivate::SimilarReferHelper< _Type, _Refer >::Type;
        //!< Наделение типа свойствами ссылки.
}

namespace Scl
{
    template < typename > struct IsInstance : public ::std::false_type {};
        //!< По умолчанию, типы не являются Instance.

    template < typename _Test, typename _Tool >
    struct IsInstance< ::SclPrivate::Instance< _Test, _Tool > > : public ::std::true_type {};
}

namespace Scl
{
    //! Типы являются совместимыми, если они одинаковые тип _Test является
    /// производным от другого _Other.
    template < typename _Test, typename _Other >
    struct IsCompatible
        : public ::std::integral_constant< bool, ::Scl::is_similar< _Test, _Other >
            && ( ::std::is_same< ::std::decay_t< _Test >, ::std::decay_t< _Other > >::value
               || ::std::is_base_of< ::std::decay_t< _Test >, ::std::decay_t< _Other > >::value ) >
    {};

    //! Типы Instance являются совместимыми, если в них используется идентичный
    /// инструмент _Tool, и вложенные типы также являются совместимыми.
    template < typename _Test, typename _Other, typename _Tool >
    struct IsCompatible< ::SclPrivate::Instance< _Test, _Tool >, ::SclPrivate::Instance< _Other, _Tool > >
       : public ::std::integral_constant< bool, is_compatible< _Test, _Other > >
    {};
}

namespace Scl
{
    //! Типы, не являющиеся Instance, не могут быть вложенными.
    template < typename _Test, typename _Other >
    struct IsThisPartOfOther : public ::std::false_type {};

    //!< Один тип Instance является частью другого, если он совместим с любой
    /// вложенной частью другого.
    template < typename _Test, typename _TestTool, typename _Other, typename _OtherTool >
    struct IsThisPartOfOther< ::SclPrivate::Instance< _Test, _TestTool >, ::SclPrivate::Instance< _Other, _OtherTool > >
        : public ::std::integral_constant< bool,
               is_compatible< ::SclPrivate::Instance< _Test, _TestTool >, _Other >
            || is_this_part_of_other< ::SclPrivate::Instance< _Test, _TestTool >, _Other > >
    {};
}

namespace Scl
{
    template < typename _Type, typename _Other >
    struct IsSimilar
        : public ::std::integral_constant< bool,
               ::std::is_rvalue_reference< _Type >::value == ::std::is_rvalue_reference< _Other >::value
            && ::std::is_lvalue_reference< _Type >::value == ::std::is_lvalue_reference< _Other >::value
            && ( ::std::is_const< ::std::remove_reference_t< _Type > >::value || !::std::is_const< ::std::remove_reference_t< _Other > >::value )
            && ( ::std::is_volatile< ::std::remove_reference_t< _Type > >::value || !::std::is_volatile< ::std::remove_reference_t< _Other > >::value ) >
    {
    };
}

#endif
