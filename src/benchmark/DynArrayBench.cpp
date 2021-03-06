#include "Config.h"

#if BENCH_DYNARRAY
#include "core/Core.h"

#define BENCH_DYNARRAY_CONSTRUCTORS 1
#define BENCH_DYNARRAY_MEM_OPS 1
#define BENCH_DYNARRAY_ADD_EMPLACE_BACK 1
#define BENCH_DYNARRAY_INSERT_EMPLACE 1
#define BENCH_DYNARRAY_ERASE 1
#define BENCH_DYNARRAY_FIND 1

struct SomeData
{
	usize a = 42;
	usize b = 1337;
};

#if BENCH_DYNARRAY_CONSTRUCTORS

auto StdVectorConstruct(benchmark::State& state) -> void
{
	for (auto _ : state)
	{
		std::vector<u32> vec;
		benchmark::DoNotOptimize(vec);
	}
}
BENCHMARK(StdVectorConstruct);

auto DynArrayConstruct(benchmark::State& state) -> void
{
	Core::Alloc::Mallocator mallocator;
	for (auto _ : state)
	{
		Core::DynArray<u32> dynArr{ mallocator };
		benchmark::DoNotOptimize(dynArr);
	}
}
BENCHMARK(DynArrayConstruct);

auto StdVectorConstructReserved(benchmark::State& state) -> void
{
	for (auto _ : state)
	{
		std::vector<u32> vec;
		vec.reserve(state.range(0));
		benchmark::DoNotOptimize(vec);
	}
}
BENCHMARK(StdVectorConstructReserved)
	->DenseRange(256, 1024, 256);

auto DynArrayConstructReserved(benchmark::State& state) -> void
{
	Core::Alloc::Mallocator mallocator;
	for (auto _ : state)
	{
		Core::DynArray<u32> dynArr{ usize(state.range(0)), mallocator };
		benchmark::DoNotOptimize(dynArr);
	}
}
BENCHMARK(DynArrayConstructReserved)
	->DenseRange(256, 1024, 256);

auto StdVectorConstructFilledPrim(benchmark::State& state) -> void
{
	for (auto _ : state)
	{
		std::vector<u32> vec(state.range(0), 42);
		benchmark::DoNotOptimize(vec);
	}
}
BENCHMARK(StdVectorConstructFilledPrim)
	->DenseRange(256, 1024, 256)
	->Complexity();

auto DynArrayConstructFilledPrim(benchmark::State& state) -> void
{
	Core::Alloc::Mallocator mallocator;
	for (auto _ : state)
	{
		Core::DynArray<u32> dynArr{ usize(state.range(0)), 42, mallocator };
		benchmark::DoNotOptimize(dynArr);
	}
}
BENCHMARK(DynArrayConstructFilledPrim)
	->DenseRange(256, 1024, 256)
	->Complexity();


auto StdVectorConstructFilled(benchmark::State& state) -> void
{
	for (auto _ : state)
	{
		std::vector<SomeData> vec(state.range(0), SomeData{});
		benchmark::DoNotOptimize(vec);
	}
}
BENCHMARK(StdVectorConstructFilled)
->DenseRange(256, 1024, 256)
->Complexity();

auto DynArrayConstructFilled(benchmark::State& state) -> void
{
	Core::Alloc::Mallocator mallocator;
	for (auto _ : state)
	{
		Core::DynArray<SomeData> dynArr{ usize(state.range(0)), SomeData{}, mallocator };
		benchmark::DoNotOptimize(dynArr);
	}
}
BENCHMARK(DynArrayConstructFilled)
->DenseRange(256, 1024, 256)
->Complexity();


auto StdVectorConstructFillDefaultedPrim(benchmark::State& state) -> void
{
	for (auto _ : state)
	{
		std::vector<u32> vec(state.range(0));
		benchmark::DoNotOptimize(vec);
	}
}
BENCHMARK(StdVectorConstructFillDefaultedPrim)
	->DenseRange(256, 1024, 256)
	->Complexity();

auto DynArrayConstructFillDefaultedPrim(benchmark::State& state) -> void
{
	Core::Alloc::Mallocator mallocator;
	for (auto _ : state)
	{
		Core::DynArray<u32> dynArr{ mallocator};
		dynArr.FillDefault(state.range(0));
		benchmark::DoNotOptimize(dynArr);
	}
}
BENCHMARK(DynArrayConstructFillDefaultedPrim)
	->DenseRange(256, 1024, 256)
	->Complexity();

auto StdVectorConstructFillDefaulted(benchmark::State& state) -> void
{
	for (auto _ : state)
	{
		std::vector<SomeData> vec(state.range(0));
		benchmark::DoNotOptimize(vec);
	}
}
BENCHMARK(StdVectorConstructFillDefaulted)
->DenseRange(256, 1024, 256)
->Complexity();

auto DynArrayConstructFillDefaulted(benchmark::State& state) -> void
{
	Core::Alloc::Mallocator mallocator;
	for (auto _ : state)
	{
		Core::DynArray<SomeData> dynArr{ mallocator };
		dynArr.FillDefault(state.range(0));
		benchmark::DoNotOptimize(dynArr);
	}
}
BENCHMARK(DynArrayConstructFillDefaulted)
->DenseRange(256, 1024, 256)
->Complexity();

auto StdVectorConstructFillInitializerListPrim(benchmark::State& state) -> void
{
	for (auto _ : state)
	{
		std::vector<u32> vec({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 });
		benchmark::DoNotOptimize(vec);
	}
}
BENCHMARK(StdVectorConstructFillInitializerListPrim);

auto DynArrayConstructFillInitializerListPrim(benchmark::State& state) -> void
{
	Core::Alloc::Mallocator mallocator;
	for (auto _ : state)
	{
		Core::DynArray<u32> dynArr{ {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}, mallocator };
		benchmark::DoNotOptimize(dynArr);
	}
}
BENCHMARK(DynArrayConstructFillInitializerListPrim);

auto StdVectorConstructFillInitializerList(benchmark::State& state) -> void
{
	for (auto _ : state)
	{
		std::vector<SomeData> vec(
			{ SomeData{}, SomeData{}, SomeData{}, SomeData{},
			  SomeData{}, SomeData{}, SomeData{}, SomeData{},
			  SomeData{}, SomeData{}, SomeData{}, SomeData{} });
		benchmark::DoNotOptimize(vec);
	}
}
BENCHMARK(StdVectorConstructFillInitializerList);

auto DynArrayConstructFillInitializerList(benchmark::State& state) -> void
{
	Core::Alloc::Mallocator mallocator;
	for (auto _ : state)
	{
		Core::DynArray<SomeData> dynArr{
			{ SomeData{}, SomeData{}, SomeData{}, SomeData{},
			  SomeData{}, SomeData{}, SomeData{}, SomeData{},
			  SomeData{}, SomeData{}, SomeData{}, SomeData{}},
			mallocator };
		benchmark::DoNotOptimize(dynArr);
	}
}
BENCHMARK(DynArrayConstructFillInitializerList);

auto StdVectorConstructFillIteratorPrim(benchmark::State& state) -> void
{
	u32 data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
	for (auto _ : state)
	{
		std::vector<u32> vec(data, data + 12);
		benchmark::DoNotOptimize(vec);
	}
}
BENCHMARK(StdVectorConstructFillIteratorPrim);

auto DynArrayConstructFillIteratorPrim(benchmark::State& state) -> void
{
	Core::Alloc::Mallocator mallocator;
	u32 data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
	for (auto _ : state)
	{
		Core::DynArray<u32> dynArr{ static_cast<u32*>(data), data + 12, mallocator };
		benchmark::DoNotOptimize(dynArr);
	}
}
BENCHMARK(DynArrayConstructFillIteratorPrim);

auto StdVectorConstructFillIterator(benchmark::State& state) -> void
{
	SomeData data[] = { SomeData{}, SomeData{}, SomeData{}, SomeData{},
						SomeData{}, SomeData{}, SomeData{}, SomeData{},
						SomeData{}, SomeData{}, SomeData{}, SomeData{} };
	for (auto _ : state)
	{
		std::vector<SomeData> vec(data, data + 12);
		benchmark::DoNotOptimize(vec);
	}
}
BENCHMARK(StdVectorConstructFillIterator);

auto DynArrayConstructFillIterator(benchmark::State& state) -> void
{
	Core::Alloc::Mallocator mallocator;
	SomeData data[] = { SomeData{}, SomeData{}, SomeData{}, SomeData{},
						SomeData{}, SomeData{}, SomeData{}, SomeData{},
						SomeData{}, SomeData{}, SomeData{}, SomeData{} };
	for (auto _ : state)
	{
		Core::DynArray<SomeData> dynArr{ static_cast<SomeData*>(data), data + 12, mallocator };
		benchmark::DoNotOptimize(dynArr);
	}
}
BENCHMARK(DynArrayConstructFillIterator);

auto StdVectorCopyConstructPrim(benchmark::State& state) -> void
{
	std::vector<u32> src{ { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 } };
	for (auto _ : state)
	{
		std::vector<u32> vec(src);
		benchmark::DoNotOptimize(vec);
	}
}
BENCHMARK(StdVectorCopyConstructPrim);

auto DynArrayCopyConstructPrim(benchmark::State& state) -> void
{
	Core::Alloc::Mallocator mallocator;
	Core::DynArray<u32> src{ { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 }, mallocator };
	for (auto _ : state)
	{
		Core::DynArray<u32> dynArr{ src, mallocator };
		benchmark::DoNotOptimize(dynArr);
	}
}
BENCHMARK(DynArrayCopyConstructPrim);

auto StdVectorCopyConstruct(benchmark::State& state) -> void
{
	std::vector<SomeData> src{ { SomeData{}, SomeData{}, SomeData{}, SomeData{},
						         SomeData{}, SomeData{}, SomeData{}, SomeData{},
						         SomeData{}, SomeData{}, SomeData{}, SomeData{} } };
	for (auto _ : state)
	{
		std::vector<SomeData> vec(src);
		benchmark::DoNotOptimize(vec);
	}
}
BENCHMARK(StdVectorCopyConstruct);

auto DynArrayCopyConstruct(benchmark::State& state) -> void
{
	Core::Alloc::Mallocator mallocator;
	Core::DynArray<SomeData> src{ { SomeData{}, SomeData{}, SomeData{}, SomeData{},
								    SomeData{}, SomeData{}, SomeData{}, SomeData{},
								    SomeData{}, SomeData{}, SomeData{}, SomeData{} },
		                            mallocator};
	for (auto _ : state)
	{
		Core::DynArray<SomeData> dynArr{ src, mallocator };
		benchmark::DoNotOptimize(dynArr);
	}
}
BENCHMARK(DynArrayCopyConstruct);

auto StdVectorMove(benchmark::State& state) -> void
{
	std::vector<u32> src{ { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 } };
	for (auto _ : state)
	{
		std::vector<u32> vec(Move(src));
		benchmark::DoNotOptimize(vec);
		src = Move(vec);
	}
}
BENCHMARK(StdVectorMove);

auto DynArrayMove(benchmark::State& state) -> void
{
	Core::Alloc::Mallocator mallocator;
	Core::DynArray<u32> src{ { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 }, mallocator };
	for (auto _ : state)
	{
		Core::DynArray<u32> dynArr{ Move(src) };
		benchmark::DoNotOptimize(dynArr);
		src = Move(dynArr);
	}
}
BENCHMARK(DynArrayMove);

#endif

#if BENCH_DYNARRAY_MEM_OPS

auto StdVectorResizeLarger(benchmark::State& state) -> void
{
	for (auto _ : state)
	{
		std::vector<u32> vec;
		vec.resize(state.range(0));
		benchmark::DoNotOptimize(vec);
	}
}
BENCHMARK(StdVectorResizeLarger)
	->DenseRange(256, 1024, 256);

auto DynArrayResizeLarger(benchmark::State& state) -> void
{
	Core::Alloc::Mallocator mallocator;
	for (auto _ : state)
	{
		Core::DynArray<u32> dynArr{ mallocator };
		dynArr.Resize(state.range(0));
		benchmark::DoNotOptimize(dynArr);
	}
}
BENCHMARK(DynArrayResizeLarger)
	->DenseRange(256, 1024, 256);

auto StdVectorResizeSmaller(benchmark::State& state) -> void
{
	for (auto _ : state)
	{
		std::vector<u32> vec(usize(state.range(0)));
		vec.resize(state.range(1));
		benchmark::DoNotOptimize(vec);
	}
}
BENCHMARK(StdVectorResizeSmaller)
	->Args({ 256, 64 })
	->Args({ 512, 128 })
	->Args({ 768, 192 })
	->Args({ 1024, 256 });

auto DynArrayResizeSmaller(benchmark::State& state) -> void
{
	Core::Alloc::Mallocator mallocator;
	for (auto _ : state)
	{
		Core::DynArray<u32> dynArr{ usize(state.range(0)), mallocator };
		dynArr.Resize(state.range(1));
		benchmark::DoNotOptimize(dynArr);
	}
}
BENCHMARK(DynArrayResizeSmaller)
	->Args({ 256, 64 })
	->Args({ 512, 128 })
	->Args({ 768, 192 })
	->Args({ 1024, 256 });

auto StdVectorShrinkToFit(benchmark::State& state) -> void
{
	for (auto _ : state)
	{
		std::vector<u32> vec(usize(state.range(0)));
		vec.shrink_to_fit();
		benchmark::DoNotOptimize(vec);
	}
}
BENCHMARK(StdVectorShrinkToFit)
	->DenseRange(256, 1024, 256);

auto DynArrayShrinkToFit(benchmark::State& state) -> void
{
	Core::Alloc::Mallocator mallocator;
	for (auto _ : state)
	{
		Core::DynArray<u32> dynArr{ usize(state.range(0)), mallocator };
		dynArr.ShrinkToFit();
		benchmark::DoNotOptimize(dynArr);
	}
}
BENCHMARK(DynArrayShrinkToFit)
	->DenseRange(256, 1024, 256);

#endif

#if BENCH_DYNARRAY_ADD_EMPLACE_BACK

auto StdVectorAdd(benchmark::State& state) -> void
{
	for (auto _ : state)
	{
		std::vector<u32> vec;
		for (usize i = 0, count = state.range(0); i < count; ++i)
			vec.push_back(42);
	}
}
BENCHMARK(StdVectorAdd)
	->DenseRange(256, 1024, 256);

auto DynArrayAdd(benchmark::State& state) -> void
{
	Core::Alloc::Mallocator mallocator;
	for (auto _ : state)
	{
		Core::DynArray<u32> dynArr{ mallocator };
		for (usize i = 0, count = state.range(0); i < count; ++i)
			dynArr.Add(42);
	}
}
BENCHMARK(DynArrayAdd)
	->DenseRange(256, 1024, 256);

auto StdVectorAddReserved(benchmark::State& state) -> void
{
	for (auto _ : state)
	{
		std::vector<u32> vec;
		vec.reserve(state.range(0));
		for (usize i = 0, count = state.range(0); i < count; ++i)
			vec.push_back(42);
	}
}
BENCHMARK(StdVectorAddReserved)
->DenseRange(256, 1024, 256);

auto DynArrayAddReserved(benchmark::State& state) -> void
{
	Core::Alloc::Mallocator mallocator;
	for (auto _ : state)
	{
		Core::DynArray<u32> dynArr{ mallocator };
		dynArr.Reserve(state.range(0));
		for (usize i = 0, count = state.range(0); i < count; ++i)
			dynArr.Add(42);
	}
}
BENCHMARK(DynArrayAddReserved)
	->DenseRange(256, 1024, 256);

auto DynArrayAddDynArray(benchmark::State& state) -> void
{
	Core::Alloc::Mallocator mallocator;
	Core::DynArray<u32> src{ usize(state.range(0)), 42, mallocator };
	for (auto _ : state)
	{
		Core::DynArray<u32> dynArr{ mallocator };
		dynArr.Add(src);
	}
}
BENCHMARK(DynArrayAddDynArray)
	->DenseRange(256, 1024, 256);

auto StdVectorEmplaceBack(benchmark::State& state) -> void
{
	for (auto _ : state)
	{
		std::vector<u32> vec;
		for (usize i = 0, count = state.range(0); i < count; ++i)
			vec.push_back(42);
	}
}
BENCHMARK(StdVectorEmplaceBack)
	->DenseRange(256, 1024, 256);

auto DynArrayEmplaceBack(benchmark::State& state) -> void
{
	Core::Alloc::Mallocator mallocator;
	for (auto _ : state)
	{
		Core::DynArray<u32> dynArr{ mallocator };
		for (usize i = 0, count = state.range(0); i < count; ++i)
			dynArr.Add(42);
	}
}
BENCHMARK(DynArrayEmplaceBack)
	->DenseRange(256, 1024, 256);

#endif

#if BENCH_DYNARRAY_INSERT_EMPLACE

auto StdVectorInsertVal(benchmark::State& state) -> void
{
	for (auto _ : state)
	{
		std::vector<u32> vec{ { 0, 1, 2, 3, 4, 5, 6 } };
		for (usize i = 0, count = state.range(0); i < count; ++i)
			vec.insert(vec.begin() + 3, 42);
	}
}
BENCHMARK(StdVectorInsertVal)
	->DenseRange(256, 1024, 256);

auto DynArrayInsertVal(benchmark::State& state) -> void
{
	Core::Alloc::Mallocator mallocator;
	for (auto _ : state)
	{
		Core::DynArray<u32> dynArr{ { 0, 1, 2, 3, 4, 5, 6 }, mallocator };
		for (usize i = 0, count = state.range(0); i < count; ++i)
			dynArr.Insert(dynArr.IteratorAt(3), 42);
	}
}
BENCHMARK(DynArrayInsertVal)
	->DenseRange(256, 1024, 256);

auto StdVectorInsertValCount(benchmark::State& state) -> void
{
	for (auto _ : state)
	{
		std::vector<u32> vec{ { 0, 1, 2, 3, 4, 5, 6 } };
		for (usize i = 0, count = state.range(0); i < count; ++i)
			vec.insert(vec.begin() + 3, state.range(1), 42);
	}
}
BENCHMARK(StdVectorInsertValCount)
	->Args({ 256, 64 })
	->Args({ 512, 32 })
	->Args({ 1024, 16 });

auto DynArrayInsertValCount(benchmark::State& state) -> void
{
	Core::Alloc::Mallocator mallocator;
	for (auto _ : state)
	{
		Core::DynArray<u32> dynArr{ { 0, 1, 2, 3, 4, 5, 6 }, mallocator };
		for (usize i = 0, count = state.range(0); i < count; ++i)
			dynArr.Insert(dynArr.IteratorAt(3), state.range(1), 42);
	}
}
BENCHMARK(DynArrayInsertValCount)
	->Args({ 256, 64 })
	->Args({ 512, 32 })
	->Args({ 1024, 16 });

auto StdVectorInsertIterators(benchmark::State& state) -> void
{
	u32 data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
	for (auto _ : state)
	{
		std::vector<u32> vec{ { 0, 1, 2, 3, 4, 5, 6 } };
		for (usize i = 0, count = state.range(0); i < count; ++i)
			vec.insert(vec.begin() + 3, data, data + 12);
	}
}
BENCHMARK(StdVectorInsertIterators)
	->DenseRange(256, 1024, 256);

auto DynArrayInsertIterators(benchmark::State& state) -> void
{
	Core::Alloc::Mallocator mallocator;
	u32 data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
	for (auto _ : state)
	{
		Core::DynArray<u32> dynArr{ { 0, 1, 2, 3, 4, 5, 6 }, mallocator };
		for (usize i = 0, count = state.range(0); i < count; ++i)
			dynArr.Insert(dynArr.IteratorAt(3), reinterpret_cast<u32*>(data)
						  , data + 12);
	}
}
BENCHMARK(DynArrayInsertIterators)
	->DenseRange(256, 1024, 256);

auto StdVectorInsertInitializerList(benchmark::State& state) -> void
{
	for (auto _ : state)
	{
		std::vector<u32> vec{ { 0, 1, 2, 3, 4, 5, 6 } };
		for (usize i = 0, count = state.range(0); i < count; ++i)
			vec.insert(vec.begin() + 3, { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 });
	}
}
BENCHMARK(StdVectorInsertInitializerList)
->DenseRange(256, 1024, 256);

auto DynArrayInsertInitializerList(benchmark::State& state) -> void
{
	Core::Alloc::Mallocator mallocator;
	for (auto _ : state)
	{
		Core::DynArray<u32> dynArr{ { 0, 1, 2, 3, 4, 5, 6 }, mallocator };
		for (usize i = 0, count = state.range(0); i < count; ++i)
			dynArr.Insert(dynArr.IteratorAt(3), { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 });
	}
}
BENCHMARK(DynArrayInsertInitializerList)
->DenseRange(256, 1024, 256);

auto DynArrayInsertOther(benchmark::State& state) -> void
{
	Core::Alloc::Mallocator mallocator;
	Core::DynArray<u32> src{ { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 }, mallocator };
	for (auto _ : state)
	{
		Core::DynArray<u32> dynArr{ { 0, 1, 2, 3, 4, 5, 6 }, mallocator };
		for (usize i = 0, count = state.range(0); i < count; ++i)
			dynArr.Insert(dynArr.IteratorAt(3), src);
	}
}
BENCHMARK(DynArrayInsertOther)
->DenseRange(256, 1024, 256);

auto StdVectorEmplaceVal(benchmark::State& state) -> void
{
	for (auto _ : state)
	{
		std::vector<u32> vec{ { 0, 1, 2, 3, 4, 5, 6 } };
		for (usize i = 0, count = state.range(0); i < count; ++i)
			vec.emplace(vec.begin() + 3, 42);
	}
}
BENCHMARK(StdVectorEmplaceVal)
	->DenseRange(256, 1024, 256);

auto DynArrayEmplaceVal(benchmark::State& state) -> void
{
	Core::Alloc::Mallocator mallocator;
	for (auto _ : state)
	{
		Core::DynArray<u32> dynArr{ { 0, 1, 2, 3, 4, 5, 6 }, mallocator };
		for (usize i = 0, count = state.range(0); i < count; ++i)
			dynArr.Emplace(dynArr.IteratorAt(3), 42u);
	}
}
BENCHMARK(DynArrayEmplaceVal)
	->DenseRange(256, 1024, 256);

#endif

#if BENCH_DYNARRAY_ERASE

auto StdVectorPop(benchmark::State& state) -> void
{
	for (auto _ : state)
	{
		std::vector<u32> vec{ { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 } };
		while (!vec.empty())
			vec.pop_back();
	}
}
BENCHMARK(StdVectorPop);

auto DynArrayPop(benchmark::State& state) -> void
{
	Core::Alloc::Mallocator mallocator;
	for (auto _ : state)
	{
		Core::DynArray<u32> dynArr{ { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 }, mallocator };
		while (!dynArr.IsEmpty())
			dynArr.Pop();
	}
}
BENCHMARK(DynArrayPop);

auto StdVectorErase(benchmark::State& state) -> void
{
	for (auto _ : state)
	{
		std::vector<u32> vec{ { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 } };
		while (vec.size() > 4)
			vec.erase(vec.begin() + 3);
	}
}
BENCHMARK(StdVectorErase);

auto DynArrayErase(benchmark::State& state) -> void
{
	Core::Alloc::Mallocator mallocator;
	for (auto _ : state)
	{
		Core::DynArray<u32> dynArr{ { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 }, mallocator };
		while (dynArr.Size() > 4)
			dynArr.Erase(dynArr.IteratorAt(3));
	}
}
BENCHMARK(DynArrayErase);

auto StdVectorEraseIt(benchmark::State& state) -> void
{
	for (auto _ : state)
	{
		std::vector<u32> vec{ { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 } };
		while (vec.size() > 6)
			vec.erase(vec.begin() + 3, vec.begin() + 6);
	}
}
BENCHMARK(StdVectorEraseIt);

auto DynArrayEraseIt(benchmark::State& state) -> void
{
	Core::Alloc::Mallocator mallocator;
	for (auto _ : state)
	{
		Core::DynArray<u32> dynArr{ { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 }, mallocator };
		while (dynArr.Size() > 6)
			dynArr.Erase(dynArr.IteratorAt(3), dynArr.IteratorAt(6));
	}
}
BENCHMARK(DynArrayEraseIt);

auto DynArrayEraseCount(benchmark::State& state) -> void
{
	Core::Alloc::Mallocator mallocator;
	for (auto _ : state)
	{
		Core::DynArray<u32> dynArr{ { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 }, mallocator };
		while (dynArr.Size() > 6)
			dynArr.Erase(dynArr.IteratorAt(3), 3);
	}
}
BENCHMARK(DynArrayEraseCount);

#endif

#if BENCH_DYNARRAY_FIND

auto StdVectorFindFirst(benchmark::State& state) -> void
{
	std::vector<u32> vec{ { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 } };
	for (auto _ : state)
	{
		benchmark::DoNotOptimize(std::find(vec.begin(), vec.end(), 0));
	}
}
BENCHMARK(StdVectorFindFirst);

auto DynArrayFindFirst(benchmark::State& state) -> void
{
	Core::Alloc::Mallocator mallocator;
	Core::DynArray<u32> dynArr{ { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 }, mallocator };
	for (auto _ : state)
	{
		benchmark::DoNotOptimize(dynArr.Find(0));
	}
}
BENCHMARK(DynArrayFindFirst);

auto StdVectorFindLast(benchmark::State& state) -> void
{
	std::vector<u32> vec(usize(state.range(0)), 42);
	vec.push_back(1337);
	for (auto _ : state)
	{
		benchmark::DoNotOptimize(std::find(vec.begin(), vec.end(), 1337));
	}
}
BENCHMARK(StdVectorFindLast)
	->DenseRange(256, 1024, 256);

auto DynArrayFindLast(benchmark::State& state) -> void
{
	Core::Alloc::Mallocator mallocator;
	Core::DynArray<u32> dynArr{ usize(state.range(0)), 42, mallocator };
	dynArr.Add(1337);
	for (auto _ : state)
	{
		benchmark::DoNotOptimize(dynArr.Find(1337));
	}
}
BENCHMARK(DynArrayFindLast)
	->DenseRange(256, 1024, 256);

#endif

#endif