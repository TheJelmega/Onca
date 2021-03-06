#pragma once
#if __RESHARPER__
#include "Pack.h"
#endif

namespace Onca::Intrin
{
	template <SimdBaseType T, usize Width>
	constexpr Pack<T, Width>::Pack() noexcept
	{
		*this = Zero();
	}


	template <SimdBaseType T, usize Width>
	constexpr Pack<T, Width>::Pack(UnInitTag) noexcept
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
	constexpr auto Pack<T, Width>::operator++() noexcept -> Pack&
	{
		*this += Set(T(1));
		return *this;
	}

	template <SimdBaseType T, usize Width>
	constexpr auto Pack<T, Width>::operator++(int) noexcept -> Pack
	{
		Pack tmp = *this;
		operator++();
		return tmp;
	}

	template <SimdBaseType T, usize Width>
	constexpr auto Pack<T, Width>::operator--() noexcept -> Pack&
	{
		*this -= Set(T(1));
		return *this;
	}

	template <SimdBaseType T, usize Width>
	constexpr auto Pack<T, Width>::operator--(int) noexcept -> Pack
	{
		Pack tmp = *this;
		operator--();
		return tmp;
	}

	template <SimdBaseType T, usize Width>
	constexpr auto Pack<T, Width>::operator+() const noexcept -> Pack
	{
		return Pack(data);
	}

	template <SimdBaseType T, usize Width>
	constexpr auto Pack<T, Width>::operator-() const noexcept -> Pack
	{
		return Neg();
	}

	template <SimdBaseType T, usize Width>
	constexpr auto Pack<T, Width>::operator~() const noexcept -> Pack
	{
		return Not();
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
	constexpr auto Pack<T, Width>::operator%(const Pack& other) const noexcept -> Pack
	{
		return Mod(other);
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
	constexpr auto Pack<T, Width>::operator>>(const Pack& count) const noexcept -> Pack
	{
		if constexpr (UnsignedIntegral<T>)
			return ShiftRL(count);
		else
			return ShiftRA(count);
	}

	template <SimdBaseType T, usize Width>
	template <UnsignedIntegral U>
	constexpr auto Pack<T, Width>::operator>>(U count) const noexcept -> Pack
	{
		if constexpr (UnsignedIntegral<T>)
			return ShiftRL(count);
		else
			return ShiftRA(count);
	}

	template <SimdBaseType T, usize Width>
	constexpr auto Pack<T, Width>::operator<<(const Pack& count) const noexcept -> Pack
	{
		return ShiftL(count);
	}

	template <SimdBaseType T, usize Width>
	template <UnsignedIntegral U>
	constexpr auto Pack<T, Width>::operator<<(U count) const noexcept -> Pack
	{
		return ShiftL(count);
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
	constexpr auto Pack<T, Width>::operator%=(const Pack& other) noexcept -> Pack&
	{
		*this = *this % other;
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
	constexpr auto Pack<T, Width>::operator>>=(const Pack& count) noexcept -> Pack&
	{
		*this = *this >> count;
		return *this;
	}

	template <SimdBaseType T, usize Width>
	template <UnsignedIntegral U>
	constexpr auto Pack<T, Width>::operator>>=(U count) noexcept -> Pack&
	{
		*this = *this >> count;
		return *this;
	}

	template <SimdBaseType T, usize Width>
	constexpr auto Pack<T, Width>::operator<<=(const Pack& count) noexcept -> Pack&
	{
		*this = *this << count;
		return *this;
	}

	template <SimdBaseType T, usize Width>
	template <UnsignedIntegral U>
	constexpr auto Pack<T, Width>::operator<<=(U count) noexcept -> Pack&
	{
		*this = *this << count;
		return *this;
	}

	template <SimdBaseType T, usize Width>
	constexpr auto Pack<T, Width>::operator==(const Pack& other) const noexcept -> Pack
	{
		return Compare<ComparisonOp::Eq>(other);
	}

	template <SimdBaseType T, usize Width>
	constexpr auto Pack<T, Width>::operator!=(const Pack& other) const noexcept -> Pack
	{
		return Compare<ComparisonOp::NEq>(other);
	}

	template <SimdBaseType T, usize Width>
	constexpr auto Pack<T, Width>::operator<(const Pack& other) const noexcept -> Pack
	{
		return Compare<ComparisonOp::Lt>(other);
	}

	template <SimdBaseType T, usize Width>
	constexpr auto Pack<T, Width>::operator<=(const Pack& other) const noexcept -> Pack
	{
		return Compare<ComparisonOp::Le>(other);
	}

	template <SimdBaseType T, usize Width>
	constexpr auto Pack<T, Width>::operator>(const Pack& other) const noexcept -> Pack
	{
		return Compare<ComparisonOp::Gt>(other);
	}

	template <SimdBaseType T, usize Width>
	constexpr auto Pack<T, Width>::operator>=(const Pack& other) const noexcept -> Pack
	{
		return Compare<ComparisonOp::Ge>(other);
	}
}
