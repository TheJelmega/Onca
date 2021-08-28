#pragma once
#include "HashSet.h"

namespace Core
{
	template <Movable K, Hasher<K> H, Comparator<K, K> C, bool IsMultiMap>
	auto HashSet<K, H, C, IsMultiMap>::Iterator::operator++() noexcept -> Iterator
	{
		++m_it;
		return *this;
	}

	template <Movable K, Hasher<K> H, Comparator<K, K> C, bool IsMultiMap>
	auto HashSet<K, H, C, IsMultiMap>::Iterator::operator++(int) noexcept -> Iterator
	{
		Iterator it{ m_it };
		++m_it;
		return it;
	}

	template <Movable K, Hasher<K> H, Comparator<K, K> C, bool IsMultiMap>
	auto HashSet<K, H, C, IsMultiMap>::Iterator::operator->() const noexcept -> const K*
	{
		return &m_it->first;
	}

	template <Movable K, Hasher<K> H, Comparator<K, K> C, bool IsMultiMap>
	auto HashSet<K, H, C, IsMultiMap>::Iterator::operator*() const noexcept -> const K&
	{
		return m_it->first;
	}

	template <Movable K, Hasher<K> H, Comparator<K, K> C, bool IsMultiMap>
	auto HashSet<K, H, C, IsMultiMap>::Iterator::operator+(usize count) const noexcept -> Iterator
	{
		Iterator it{ m_it };
		for (usize i = 0; i < count; ++i)
			++it;
		return it;
	}

	template <Movable K, Hasher<K> H, Comparator<K, K> C, bool IsMultiMap>
	auto HashSet<K, H, C, IsMultiMap>::Iterator::operator==(const Iterator& other) const noexcept -> bool
	{
		return m_it == other.m_it;
	}

	template <Movable K, Hasher<K> H, Comparator<K, K> C, bool IsMultiMap>
	auto HashSet<K, H, C, IsMultiMap>::Iterator::operator!=(const Iterator& other) const noexcept -> bool
	{
		return m_it != other.m_it;
	}

	template <Movable K, Hasher<K> H, Comparator<K, K> C, bool IsMultiMap>
	HashSet<K, H, C, IsMultiMap>::Iterator::Iterator(const typename Map::Iterator& it) noexcept
		: m_it(it)
	{
	}

	template <Movable K, Hasher<K> H, Comparator<K, K> C, bool IsMultiMap>
	HashSet<K, H, C, IsMultiMap>::HashSet(Alloc::IAllocator& alloc) noexcept
		: m_hashMap(alloc)
	{
		ASSERT(&alloc, "No allocator supplied to a HashSet/HashMultiSet");
	}

	template <Movable K, Hasher<K> H, Comparator<K, K> C, bool IsMultiMap>
	HashSet<K, H, C, IsMultiMap>::HashSet(usize minBuckets, Alloc::IAllocator& alloc) noexcept
		: m_hashMap(minBuckets, alloc)
	{
		ASSERT(&alloc, "No allocator supplied to a HashSet/HashMultiSet");
	}

	template <Movable K, Hasher<K> H, Comparator<K, K> C, bool IsMultiMap>
	HashSet<K, H, C, IsMultiMap>::HashSet(usize minBuckets, H hasher, C comp, Alloc::IAllocator& alloc) noexcept
		: m_hashMap(minBuckets, Move(hasher), Move(comp), alloc)
	{
		ASSERT(&alloc, "No allocator supplied to a HashSet/HashMultiSet");
	}

	template <Movable K, Hasher<K> H, Comparator<K, K> C, bool IsMultiMap>
	HashSet<K, H, C, IsMultiMap>::HashSet(const InitializerList<K>& il, Alloc::IAllocator& alloc) noexcept requires CopyConstructable<K>
		: HashSet(il, 0, H{}, C{}, alloc)
	{
	}

	template <Movable K, Hasher<K> H, Comparator<K, K> C, bool IsMultiMap>
	HashSet<K, H, C, IsMultiMap>::HashSet(const InitializerList<K>& il, usize minBuckets, Alloc::IAllocator& alloc) noexcept requires CopyConstructable<K>
		: HashSet(il, minBuckets, H{}, C{}, alloc)
	{
	}

	template <Movable K, Hasher<K> H, Comparator<K, K> C, bool IsMultiMap>
	HashSet<K, H, C, IsMultiMap>::HashSet(const InitializerList<K>& il, usize minBuckets, H hasher, C comp, Alloc::IAllocator& alloc) noexcept
		requires CopyConstructable<K>
		: m_hashMap(minBuckets, Move(hasher), Move(comp), alloc)
	{
		ASSERT(&alloc, "No allocator supplied to a HashMap/HashMultiMap");
		minBuckets = Max(minBuckets, Ceil(il.size() / m_hashMap.MaxLoadFactor()));
		Rehash(minBuckets);
		for (const K& key : il)
			Insert(key);
	}

	template <Movable K, Hasher<K> H, Comparator<K, K> C, bool IsMultiMap>
	template <ForwardIterator It>
	HashSet<K, H, C, IsMultiMap>::HashSet(const It& begin, const It& end, Alloc::IAllocator& alloc) noexcept requires CopyConstructable<K>
		: HashSet(begin, end, 0, H{}, C{}, alloc)
	{
	}

	template <Movable K, Hasher<K> H, Comparator<K, K> C, bool IsMultiMap>
	template <ForwardIterator It>
	HashSet<K, H, C, IsMultiMap>::HashSet(const It& begin, const It& end, usize minBuckets, Alloc::IAllocator& alloc) noexcept requires CopyConstructable<K>
		: HashSet(begin, end, minBuckets, H{}, C{}, alloc)
	{
	}

	template <Movable K, Hasher<K> H, Comparator<K, K> C, bool IsMultiMap>
	template <ForwardIterator It>
	HashSet<K, H, C, IsMultiMap>::HashSet(const It& begin, const It& end, usize minBuckets, H hasher, C comp, Alloc::IAllocator& alloc) noexcept
		requires CopyConstructable<K>
		: m_hashMap(minBuckets, Move(hasher), Move(comp), alloc)
	{
		ASSERT(&alloc, "No allocator supplied to a HashSet/HashMultiSet");
		if constexpr (ContiguousIterator<It>)
			minBuckets = Max(minBuckets, Ceil((end - begin) / m_hashMap.MaxLoadFactor()));
		Rehash(minBuckets);
		for (It it = begin; it != end; ++it)
			Insert(*it);
	}

	template <Movable K, Hasher<K> H, Comparator<K, K> C, bool IsMultiMap>
	HashSet<K, H, C, IsMultiMap>::HashSet(const HashSet& other) noexcept requires CopyConstructable<K>
		: m_hashMap(other.m_hashMap)
	{
	}

	template <Movable K, Hasher<K> H, Comparator<K, K> C, bool IsMultiMap>
	HashSet<K, H, C, IsMultiMap>::HashSet(const HashSet& other, Alloc::IAllocator& alloc) noexcept requires CopyConstructable<K>
		: m_hashMap(other.m_hashMap, alloc)
	{
	}

	template <Movable K, Hasher<K> H, Comparator<K, K> C, bool IsMultiMap>
	HashSet<K, H, C, IsMultiMap>::HashSet(HashSet&& other) noexcept
		: m_hashMap(Move(other.m_hashMap))
	{
	}

	template <Movable K, Hasher<K> H, Comparator<K, K> C, bool IsMultiMap>
	HashSet<K, H, C, IsMultiMap>::HashSet(HashSet&& other, Alloc::IAllocator& alloc) noexcept
		: m_hashMap(Move(other.m_hashMap), alloc)
	{
	}

	template <Movable K, Hasher<K> H, Comparator<K, K> C, bool IsMultiMap>
	auto HashSet<K, H, C, IsMultiMap>::operator=(const InitializerList<K>& il) noexcept -> HashSet requires CopyConstructable<K>
	{
		m_hashMap.Clear();
		m_hashMap.Reserve(il.size());
		for (const K& key : il)
			Insert(key);
		return *this;
	}

	template <Movable K, Hasher<K> H, Comparator<K, K> C, bool IsMultiMap>
	auto HashSet<K, H, C, IsMultiMap>::operator=(const HashSet& other) noexcept -> HashSet requires CopyConstructable<K>
	{
		m_hashMap = other.m_hashMap;
		return *this;
	}

	template <Movable K, Hasher<K> H, Comparator<K, K> C, bool IsMultiMap>
	auto HashSet<K, H, C, IsMultiMap>::operator=(HashSet&& other) noexcept -> HashSet
	{
		m_hashMap = Move(other.m_hashMap);
		return *this;
	}

	template <Movable K, Hasher<K> H, Comparator<K, K> C, bool IsMultiMap>
	auto HashSet<K, H, C, IsMultiMap>::Rehash(usize count) noexcept -> void
	{
		m_hashMap.Rehash(count);
	}

	template <Movable K, Hasher<K> H, Comparator<K, K> C, bool IsMultiMap>
	auto HashSet<K, H, C, IsMultiMap>::Reserve(usize count) noexcept -> void
	{
		m_hashMap.Reserve(count);
	}

	template <Movable K, Hasher<K> H, Comparator<K, K> C, bool IsMultiMap>
	auto HashSet<K, H, C, IsMultiMap>::Insert(const K& key) noexcept -> Pair<ConstIterator, bool> requires CopyConstructable<K>
	{
		auto [it, success] = m_hashMap.TryInsert(key, Empty{});
		return Pair{ Iterator{ it }, success };
	}

	template <Movable K, Hasher<K> H, Comparator<K, K> C, bool IsMultiMap>
	auto HashSet<K, H, C, IsMultiMap>::Insert(K&& key) noexcept -> Pair<ConstIterator, bool>
	{
		auto [it, success] = m_hashMap.TryInsert(Move(key), Empty{});
		return Pair{ Iterator{ it }, success };
	}

	template <Movable K, Hasher<K> H, Comparator<K, K> C, bool IsMultiMap>
	template <typename ... Args> requires ConstructableFrom<K, Args...>
	auto HashSet<K, H, C, IsMultiMap>::Emplace(Args&&... args) noexcept -> Pair<ConstIterator, bool>
	{
		auto [it, success] = m_hashMap.TryInsert(K{ args... }, Empty{});
		return Pair{ Iterator{ it }, success };
	}

	template <Movable K, Hasher<K> H, Comparator<K, K> C, bool IsMultiMap>
	template <Hasher H2, Comparator C2>
	auto HashSet<K, H, C, IsMultiMap>::Merge(HashMap<K, Empty, H2, C2>& other) noexcept -> void
	{
		Merge(other);
	}

	template <Movable K, Hasher<K> H, Comparator<K, K> C, bool IsMultiMap>
	auto HashSet<K, H, C, IsMultiMap>::Clear(bool clearMemory) noexcept -> void
	{
		m_hashMap.Clear(clearMemory);
	}

	template <Movable K, Hasher<K> H, Comparator<K, K> C, bool IsMultiMap>
	auto HashSet<K, H, C, IsMultiMap>::Erase(ConstIterator& it) noexcept -> Iterator
	{
		typename Map::Iterator retIt = m_hashMap.Erase(it.m_it);
		return Iterator{ retIt };
	}

	template <Movable K, Hasher<K> H, Comparator<K, K> C, bool IsMultiMap>
	auto HashSet<K, H, C, IsMultiMap>::Erase(const K& key) noexcept -> usize
	{
		typename Map::Iterator retIt = m_hashMap.Erase(key);
		return Iterator{ retIt };
	}

	template <Movable K, Hasher<K> H, Comparator<K, K> C, bool IsMultiMap>
	auto HashSet<K, H, C, IsMultiMap>::Find(const K& key) const noexcept -> ConstIterator
	{
		typename Map::Iterator retIt = m_hashMap.Find(key);
		return Iterator{ retIt };
	}

	template <Movable K, Hasher<K> H, Comparator<K, K> C, bool IsMultiMap>
	template <EqualComparable<K> K2>
	auto HashSet<K, H, C, IsMultiMap>::Find(const K2& key) const noexcept -> ConstIterator
	{
		typename Map::Iterator retIt = m_hashMap.Find(key);
		return Iterator{ retIt };
	}

	template <Movable K, Hasher<K> H, Comparator<K, K> C, bool IsMultiMap>
	auto HashSet<K, H, C, IsMultiMap>::FindRange(const K& key) const noexcept -> Pair<ConstIterator, ConstIterator>
	{
		auto [begin, end] = m_hashMap.FindRange(key);
		return Pair{ Iterator{ begin }, Iterator{ end } };
	}

	template <Movable K, Hasher<K> H, Comparator<K, K> C, bool IsMultiMap>
	template <EqualComparable<K> K2>
	auto HashSet<K, H, C, IsMultiMap>::FindRange(const K2& key) const noexcept -> Pair<ConstIterator, ConstIterator>
	{
		auto [begin, end] = m_hashMap.FindRange(key);
		return Pair{ Iterator{ begin }, Iterator{ end } };
	}

	template <Movable K, Hasher<K> H, Comparator<K, K> C, bool IsMultiMap>
	auto HashSet<K, H, C, IsMultiMap>::Contains(const K& key) const noexcept -> bool
	{
		return m_hashMap.Contains(key);
	}

	template <Movable K, Hasher<K> H, Comparator<K, K> C, bool IsMultiMap>
	template <EqualComparable<K> K2>
	auto HashSet<K, H, C, IsMultiMap>::Contains(const K2& key) const noexcept -> bool
	{
		return m_hashMap.Contains(key);
	}

	template <Movable K, Hasher<K> H, Comparator<K, K> C, bool IsMultiMap>
	auto HashSet<K, H, C, IsMultiMap>::Count(const K& key) const noexcept -> usize
	{
		return m_hashMap.Count(key);
	}

	template <Movable K, Hasher<K> H, Comparator<K, K> C, bool IsMultiMap>
	template <EqualComparable<K> K2>
	auto HashSet<K, H, C, IsMultiMap>::Count(const K2& key) const noexcept -> usize
	{
		return m_hashMap.Count(key);
	}

	template <Movable K, Hasher<K> H, Comparator<K, K> C, bool IsMultiMap>
	auto HashSet<K, H, C, IsMultiMap>::Size() const noexcept -> usize
	{
		return m_hashMap.Size();
	}

	template <Movable K, Hasher<K> H, Comparator<K, K> C, bool IsMultiMap>
	auto HashSet<K, H, C, IsMultiMap>::IsEmpty() const noexcept -> bool
	{
		return m_hashMap.IsEmpty();
	}

	template <Movable K, Hasher<K> H, Comparator<K, K> C, bool IsMultiMap>
	auto HashSet<K, H, C, IsMultiMap>::BucketCount() const noexcept -> usize
	{
		return m_hashMap.BucketCount();
	}

	template <Movable K, Hasher<K> H, Comparator<K, K> C, bool IsMultiMap>
	auto HashSet<K, H, C, IsMultiMap>::BucketSize(usize idx) const noexcept -> usize
	{
		return m_hashMap.BucketSize(idx);
	}

	template <Movable K, Hasher<K> H, Comparator<K, K> C, bool IsMultiMap>
	auto HashSet<K, H, C, IsMultiMap>::BucketIdx(const K& key) const noexcept -> usize
	{
		return m_hashMap.BucketIdx(key);
	}

	template <Movable K, Hasher<K> H, Comparator<K, K> C, bool IsMultiMap>
	auto HashSet<K, H, C, IsMultiMap>::LoadFactor() const noexcept -> f32
	{
		return m_hashMap.LoadFactor();
	}

	template <Movable K, Hasher<K> H, Comparator<K, K> C, bool IsMultiMap>
	auto HashSet<K, H, C, IsMultiMap>::MaxLoadFactor() const noexcept -> usize
	{
		return m_hashMap.MaxLoadFactor();
	}

	template <Movable K, Hasher<K> H, Comparator<K, K> C, bool IsMultiMap>
	auto HashSet<K, H, C, IsMultiMap>::SetMaxLoadFactor(f32 ml) noexcept -> void
	{
		m_hashMap.SetMaxLoadFactor(ml);
	}

	template <Movable K, Hasher<K> H, Comparator<K, K> C, bool IsMultiMap>
	auto HashSet<K, H, C, IsMultiMap>::GetAllocator() const noexcept -> Alloc::IAllocator*
	{
		return m_hashMap.GetAllocator();
	}

	template <Movable K, Hasher<K> H, Comparator<K, K> C, bool IsMultiMap>
	auto HashSet<K, H, C, IsMultiMap>::Front() const noexcept -> const K&
	{
		return m_hashMap.Front().first;
	}

	template <Movable K, Hasher<K> H, Comparator<K, K> C, bool IsMultiMap>
	auto HashSet<K, H, C, IsMultiMap>::Back() const noexcept -> const K&
	{
		return m_hashMap.Back().first;
	}

	template <Movable K, Hasher<K> H, Comparator<K, K> C, bool IsMultiMap>
	auto HashSet<K, H, C, IsMultiMap>::Begin() const noexcept -> ConstIterator
	{
		return Iterator{ m_hashMap.Begin() };
	}

	template <Movable K, Hasher<K> H, Comparator<K, K> C, bool IsMultiMap>
	auto HashSet<K, H, C, IsMultiMap>::End() const noexcept -> ConstIterator
	{
		return Iterator{ m_hashMap.End() };
	}

	template <Movable K, Hasher<K> H, Comparator<K, K> C, bool IsMultiMap>
	auto HashSet<K, H, C, IsMultiMap>::begin() const noexcept -> ConstIterator
	{
		return Begin();
	}

	template <Movable K, Hasher<K> H, Comparator<K, K> C, bool IsMultiMap>
	auto HashSet<K, H, C, IsMultiMap>::cbegin() const noexcept -> ConstIterator
	{
		return Begin();
	}

	template <Movable K, Hasher<K> H, Comparator<K, K> C, bool IsMultiMap>
	auto HashSet<K, H, C, IsMultiMap>::end() const noexcept -> ConstIterator
	{
		return End();
	}

	template <Movable K, Hasher<K> H, Comparator<K, K> C, bool IsMultiMap>
	auto HashSet<K, H, C, IsMultiMap>::cend() const noexcept -> ConstIterator
	{
		return End();
	}
}
