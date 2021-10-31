#pragma once
#if __RESHARPER__
#include "Pack.h"
#endif

namespace Core::Intrin
{
	template <SimdBaseType T, usize Width>
	constexpr Pack<T, Width>::Pack() noexcept
	{
		*this = Zero();
	}


	template <SimdBaseType T, usize Width>
	constexpr Pack<T, Width>::Pack(Detail::Uninit) noexcept
	{
	}

	template <SimdBaseType T, usize Width>
	constexpr Pack<T, Width>::Pack(T val) noexcept
	{
		*this = Set(val);
	}

	template <SimdBaseType T, usize Width>
	template <SameAs<T> ... Args>
	constexpr Pack<T, Width>::Pack(Args... vals) noexcept
	{
		*this = Set(vals...);
	}

	template <SimdBaseType T, usize Width>
	constexpr auto Pack<T, Width>::operator+(const Pack& other) const noexcept -> Pack
	{
		return Add(other);
	}

	template <SimdBaseType T, usize Width>
	constexpr auto Pack<T, Width>::operator-(const Pack& other) const noexcept -> Pack
	{
		return Sub(other);
	}

	template <SimdBaseType T, usize Width>
	constexpr auto Pack<T, Width>::operator*(const Pack& other) const noexcept -> Pack
	{
		return Mul(other);
	}

	template <SimdBaseType T, usize Width>
	constexpr auto Pack<T, Width>::operator/(const Pack& other) const noexcept -> Pack
	{
		return Div(other);
	}

	template <SimdBaseType T, usize Width>
	constexpr auto Pack<T, Width>::operator&(const Pack& other) const noexcept -> Pack
	{
		return And(other);
	}

	template <SimdBaseType T, usize Width>
	constexpr auto Pack<T, Width>::operator^(const Pack& other) const noexcept -> Pack
	{
		return Xor(other);
	}

	template <SimdBaseType T, usize Width>
	constexpr auto Pack<T, Width>::operator|(const Pack& other) const noexcept -> Pack
	{
		return Or(other);
	}

	template <SimdBaseType T, usize Width>
	constexpr auto Pack<T, Width>::operator+=(const Pack& other) noexcept -> Pack&
	{
		*this = *this + other;
		return *this;
	}

	template <SimdBaseType T, usize Width>
	constexpr auto Pack<T, Width>::operator-=(const Pack& other) noexcept -> Pack&
	{
		*this = *this - other;
		return *this;
	}

	template <SimdBaseType T, usize Width>
	constexpr auto Pack<T, Width>::operator*=(const Pack& other) noexcept -> Pack&
	{
		*this = *this * other;
		return *this;
	}

	template <SimdBaseType T, usize Width>
	constexpr auto Pack<T, Width>::operator/=(const Pack& other) noexcept -> Pack&
	{
		*this = *this / other;
		return *this;
	}

	template <SimdBaseType T, usize Width>
	constexpr auto Pack<T, Width>::operator&=(const Pack& other) noexcept -> Pack&
	{
		*this = *this & other;
		return *this;
	}

	template <SimdBaseType T, usize Width>
	constexpr auto Pack<T, Width>::operator^=(const Pack& other) noexcept -> Pack&
	{
		*this = *this ^ other;
		return *this;
	}

	template <SimdBaseType T, usize Width>
	constexpr auto Pack<T, Width>::operator|=(const Pack& other) noexcept -> Pack&
	{
		*this = *this | other;
		return *this;
	}


	template <SimdBaseType T, usize Width>
	template<SimdBaseType U>
	Pack<T, Width>::Pack(const Detail::PackData128<U>& data)
	{
		FREQ_ASSERT(sizeof(data) == sizeof(Detail::PackData128<U>), "Invalid size for this assignment");
		MemCpy(&this->data, &data, sizeof(data));
	}

	template <SimdBaseType T, usize Width>
	template<SimdBaseType U>
	Pack<T, Width>::Pack(const Detail::PackData256<U>& data)
	{
		FREQ_ASSERT(sizeof(data) == sizeof(Detail::PackData256<U>), "Invalid size for this assignment");
		MemCpy(&this->data, &data, sizeof(data));
	}
}