#pragma once
#include <type_traits>
#include "Config.h"

namespace Onca
{
	namespace Detail
	{
		template<usize Size>
		struct UnsignedOfSize              { using Type = void; };
		template<>
		struct UnsignedOfSize<sizeof(u64)> { using Type = u64;  };
		template<>
		struct UnsignedOfSize<sizeof(u32)> { using Type = u32;  };
		template<>
		struct UnsignedOfSize<sizeof(u16)> { using Type = u16;  };
		template<>
		struct UnsignedOfSize<sizeof(u8)>  { using Type = u8;   };

		template<usize Size>
		struct SignedOfSize              { using Type = void; };
		template<>
		struct SignedOfSize<sizeof(i64)> { using Type = i64;  };
		template<>
		struct SignedOfSize<sizeof(i32)> { using Type = i32;  };
		template<>
		struct SignedOfSize<sizeof(i16)> { using Type = i16;  };
		template<>
		struct SignedOfSize<sizeof(i8)>  { using Type = i8;   };

		template<usize Size>
		struct FloatOfSize              { using Type = void; };
		template<>
		struct FloatOfSize<sizeof(i64)> { using Type = f64; };
		template<>
		struct FloatOfSize<sizeof(i32)> { using Type = f32; };
	}

	/**
	 * Is a type nothrow default constructable
	 * \tparam T Type to check
	 */
	template<typename T>
	constexpr bool IsNothrowDefaultConstructable = std::is_nothrow_default_constructible_v<T>;

	/**
	 * Is a type default constructable
	 * \tparam T Type to check
	 */
	template<typename T>
	constexpr bool IsCopyConstructable = std::is_copy_constructible_v<T>;
	/**
	 * Is a type default Movable
	 * \tparam T Type to check
	 */
	template<typename T>
	constexpr bool IsMoveConstructable = std::is_move_constructible_v<T>;

	/**
	 * Check if 2 types are the same
	 * \tparam A First type
	 * \tparam B Second type
	 */
	template<typename A, typename B>
	constexpr bool IsSame = std::is_same_v<A, B>;

	/**
	 * Is a type an integral
	 * \tparam T Type to check
	 */
	template<typename T>
	constexpr  bool IsIntegral = std::is_integral_v<T>;
	/**
	 * Is a type an floating point
	 * \tparam T Type to check
	 */
	template<typename T>
	constexpr  bool IsFloatingPoint = std::is_floating_point_v<T>;
	/**
	 * Is a type a boolean
	 * \tparam T Type to check
	 */
	template<typename T>
	constexpr bool IsBoolean = IsSame<T, bool>;
	/**
	 * Is a type an pointer
	 * \tparam T Type to check
	 */
	template<typename T>
	constexpr bool IsPointer = std::is_pointer_v<T>;
	/**
	 * Is a type void
	 * \tparam T Type to check
	 */
	template<typename T>
	constexpr bool IsVoid = std::is_void_v<T>;
	/**
	 * Is a type an nullptr_t
	 * \tparam T Type to check
	 */
	template<typename T>
	constexpr bool IsNullptrT = std::is_null_pointer_v<T>;
	/**
	 * Is a type a primitive type
	 * \tparam T Type to check
	 */
	template<typename T>
	constexpr bool IsPrimitive = std::is_fundamental_v<T>;
	/**
	 * Is a type an lvalue reference
	 * \tparam T Type to check
	 */
	template<typename T>
	constexpr bool IsLValueReference = std::is_lvalue_reference_v<T>;

	/**
	 * Does an iterator have contiguous data
	 * \tparam T Type to check
	 * \note This should be overloaded to allow for specific optimizations
	 */
	template<typename T>
	constexpr bool IteratorHasContiguousData = IsPointer<T>;

	/**
	 * Removes the reference from a type
	 * \tparam T Type to remove reference from
	 */
	template<typename T>
	using RemoveReference = std::remove_reference_t<T>;

	/**
	 * Removes 'const' from a type
	 * \tparam T Type to remove 'const' from
	 */
	template<typename T>
	using RemoveConst = std::remove_const_t<T>;

	/**
	 * \brief Decay a type into its base type
	 * \tparam T Type to decay
	 */
	template<typename T>
	using Decay = std::decay_t<T>;


	template<bool C, typename T, typename U>
	using Conditional = std::conditional_t<C, T, U>;

	template<typename T>
	using UnderlyingType = std::underlying_type_t<T>;


	template<typename T>
	using UnsignedOfSameSize = typename Detail::UnsignedOfSize<sizeof(T)>::Type;
	template<typename T>
	using SignedOfSameSize = typename Detail::SignedOfSize<sizeof(T)>::Type;
	template<typename T>
	using FloatOfSameSize = typename Detail::FloatOfSize<sizeof(T)>::Type;

}
