#pragma once
#if __RESHARPER__
#include "MemUtils.h"
#endif

#include "core/Assert.h"
#include "core/math/MathUtils.h"

namespace Onca
{
	template<typename T>
	constexpr void MemCpy(T* dst, const T* src, usize numBytes) noexcept
	{
		IF_CONSTEVAL
		{
			for (usize i = 0; i < numBytes; ++i)
			{
				dst[i] = src[i];
			}
		}
		else
		{
			memcpy(dst, src, numBytes);
		}
	}

	inline void MemCpy(void* dst, const void* src, usize numBytes) noexcept
	{
		memcpy(dst, src, numBytes);
	}

	template <typename T>
	void MemCpy(T& dst, const T& src) noexcept
	{
		MemCpy(&dst, &src, sizeof(T));
	}

	template <typename T>
	void MemMove(MemRef<T>& mem, usize dst, usize src, usize numBytes) noexcept
	{
		const usize maxOffset = Math::Max(dst, src);
		const usize maxBytes = mem.Size() - maxOffset;
		numBytes = numBytes > maxBytes ? maxBytes : numBytes;
		u8* pBegin = reinterpret_cast<u8*>(mem.Ptr());
		MemMove(pBegin + dst, pBegin + src, numBytes);
	}

	inline void MemMove(void* dst, void* src, usize numBytes) noexcept
	{
		memmove(dst, src, numBytes);
	}

	inline void MemSet(void* ptr, u8 val, usize numBytes) noexcept
	{
		memset(ptr, val, numBytes);
	}

	inline void MemClear(void* ptr, usize numBytes) noexcept
	{
		MemSet(ptr, 0, numBytes);
	}

	template <typename T>
	void MemSet(MemRef<T>& mem, u8 val) noexcept
	{
		MemSet(mem.Ptr(), val, mem.Size());
	}

	template <typename T>
	void MemClear(MemRef<T>& mem) noexcept
	{
		MemClear(mem.Ptr(), mem.Size());
	}

	template<typename T>
	void MemClearData(T& val) noexcept
	{
		MemClear(&val, sizeof(T));
	}

	inline i8 MemCmp(const void* pA, const void* pB, usize numBytes) noexcept
	{
		return memcmp(pA, pB, numBytes);
	}

	template <typename T>
	i8 MemCmp(MemRef<T>& a, MemRef<T>& b) noexcept
	{
		const usize sizeA = a.Size();
		const usize sizeB = b.Size();

		i8 res = MemCmp(a.Ptr(), b.Ptr(), Math::Min(sizeA, sizeB));
		if (res != 0)
			return res;

		return a.Size() < b.Size() ? -1 : 1;
	}

}


inline namespace Literals
{
	constexpr auto operator""_KiB(u64 val) noexcept -> u64
	{
		return val * 1024;
	}

	constexpr auto operator""_MiB(u64 val) noexcept -> u64
	{
		return val * 1024 * 1024;
	}

	constexpr auto operator""_GiB(u64 val) noexcept -> u64
	{
		return val * 1024 * 1024 * 1024;
	}

	constexpr auto operator""_KB(u64 val) noexcept -> u64
	{
		return val * 1000;
	}

	constexpr auto operator""_MB(u64 val) noexcept -> u64
	{
		return val * 1000 * 1000;
	}

	constexpr auto operator""_GB(u64 val) noexcept -> u64
	{
		return val * 1000 * 1000 * 1000;
	}
}
