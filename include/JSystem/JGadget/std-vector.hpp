#ifndef JGADGET_VECTOR_HPP
#define JGADGET_VECTOR_HPP

#include <algorithm>
#include <memory>
#include <types.h>
#include <JSystem/JGadget/std-memory.hpp>

namespace JGadget {

template <typename T, typename Allocator = TAllocator<T> > class TVector {
	void DestroyElement_(T* pFirst, T* pLast)
	{
		// clang-format off
		JGADGET_ASSERT((pBegin_<=pFirst)&&(pFirst<pEnd_));
		JGADGET_ASSERT((pBegin_<=pLast)&&(pLast<=pEnd_));
		// clang-format on

		for (T* it = pFirst; it != pLast; ++it)
			mAllocator.destroy(it);
	}

	void DestroyElement_all_() { DestroyElement_(pBegin_, pEnd_); }

	size_t GetSize_extend_(size_t count) const
	{
		size_t sz     = size();
		size_t newSz  = sz + count;
		size_t newCap = (size_t)(sz * mGrowthMul) + mGrowthAdd;
		return newSz > newCap ? newSz : newCap;
	}

	struct TDestroyed_deallocate_ {
		TDestroyed_deallocate_(JGadget::TAllocator<T>& alloc, T* pointer)
		{
			mAllocator = &alloc;
			mPointer   = pointer;
		}

		void set(T* p) { mPointer = p; }

		~TDestroyed_deallocate_() { mAllocator->deallocate(mPointer, 0); }

		Allocator* mAllocator;
		T* mPointer;
	};

public:
	typedef T* iterator;
	typedef const T* const_iterator;

	TVector(const TAllocator<T>& alloc = TAllocator<T>())
	    : mAllocator(alloc)
	    , pBegin_(nullptr)
	    , pEnd_(pBegin_)
	    , mCapacity(0)
	    , mGrowthMul(2.0f)
	    , mGrowthAdd(0)
	{
	}
	TVector(size_t param_1, const T& param_2 = T(),
	        const TAllocator<T>& param_3 = TAllocator<T>())
	    : mAllocator(param_3)
	    , pBegin_(nullptr)
	    , pEnd_(pBegin_)
	    , mCapacity(0)
	    , mGrowthMul(2.0f)
	    , mGrowthAdd(0)
	{
		insert(end(), param_1, param_2);
	}

	TVector& operator=(const TVector& other)
	{
		if (this == &other)
			return *this;

		size_t ourSize   = size();
		size_t otherSize = other.size();
		iterator ourBeg  = begin();
		iterator ourEnd  = end();
		if (ourSize < otherSize) {
			if (mCapacity < otherSize) {
				T* newBegin = mAllocator.allocate(otherSize);

				if (!newBegin) {
					JGADGET_WARN("can't allocate memory");
					return *this;
				}

				TDestroyed_deallocate_ dealloc(mAllocator, newBegin);
				std::uninitialized_copy(pBegin_, pEnd_, newBegin);
				DestroyElement_all_();
				dealloc.set(pBegin_);
				pBegin_   = newBegin;
				pEnd_     = newBegin + otherSize;
				mCapacity = otherSize; // bug in TP
			} else {
				std::copy(ourBeg, ourBeg + ourSize, pBegin_);
				pEnd_
				    = std::uninitialized_copy(ourBeg + ourSize, ourEnd, pEnd_);
			}
		} else {
			iterator it = std::copy(ourBeg, ourEnd, pBegin_);
			DestroyElement_(it, ourEnd);
			pEnd_ = it;
		}

		return *this;
	}

	~TVector()
	{
		clear();
		// clang-format off
		JGADGET_ASSERT(size()==0);
		// clang-format on
		mAllocator.deallocate(pBegin_, mCapacity);
	}

	size_t size() const { return pBegin_ == nullptr ? 0 : pEnd_ - pBegin_; }
	size_t capacity() const { return mCapacity; }
	bool empty() const { return pBegin_ == pEnd_; }
	iterator begin() { return pBegin_; }
	const_iterator begin() const { return pBegin_; }
	iterator end() { return pEnd_; }
	const_iterator end() const { return pEnd_; }

	T& operator[](size_t u)
	{
		// clang-format off
		JGADGET_ASSERT(u<size());
		// clang-format on
		return pBegin_[u];
	}
	const T& operator[](size_t u) const
	{
		// clang-format off
		JGADGET_ASSERT(u<size());
		// clang-format on
		return pBegin_[u];
	}

	iterator insert(iterator where, const T& what)
	{
		size_t offset = where - pBegin_;
		insert(where, 1, what);
		return pBegin_ + offset;
	}

	// TODO: the only emitted instantiation of this overload,
	// TVector<TDrawSyncManager::TDrawSyncTokenRange>::insert (System.a
	// DrawSyncManager.cpp, 0x3a8), is instruction-exact with a frame of 0x120
	// against retail's 0x100 and r29/r30 swapped between `this` and
	// `how_many`. Every referenced slot sits exactly 0x20 higher than
	// retail's, so the whole excess is unreferenced low region: 32 bytes of
	// inline-expansion temporaries retail does not reserve.
	//
	// Diagnosed (header round 15) as a **depth surcharge**, not a wrong body:
	// spelling InsertRaw's body directly inside insert -- which puts the four
	// std::uninitialized_copy expansions, std::copy_backward and the two
	// DestroyElement_ expansions at depth 1 instead of depth 2 -- gives
	// retail's 0x100 exactly (8 bytes per copy/fill expansion). That is not a
	// usable fix: InsertRaw is a real function (UNUSED 0x38c here, emitted and
	// called from the four MarNameRefGen instantiations), so insert must call
	// it, and duplicating the body would drop the UNUSED symbol.
	//
	// Rejected, each measured with a full `ninja changes_all`:
	//   - dropping `O dummy = d_first;` from std::uninitialized_copy (MSL
	//     <memory>): insert 0x120 -> 0x110 with identical instructions, but it
	//     regresses five exact functions -- InsertRaw for TStageEventInfo,
	//     TScenarioArchiveName, TStagePositionInfo and void*, plus
	//     TVector_pointer_void::reserve -- and unlinks std-vector, so retail
	//     has the dummy.
	//   - flattening std::copy_backward's `__copy_backward<T>` struct level
	//     (MSL <algorithm>): insert 0x110 -> 0x108, and
	//     TVector<TCameraMapTool>::InsertRaw 60.22 -> 99.96, but
	//     TVector<TStageEventInfo>::InsertRaw 100 -> 41.97 (its
	//     __as__15TStageEventInfo stops being emitted, which is what proves
	//     __copy_backward assigns rather than copy-constructs), the other
	//     three InsertRaw drop below 100 and std-vector unlinks. Net
	//     matched_code -0.10.
	//   - `iterator it = pIt;` without the explicit conversion in InsertRaw:
	//     239 instructions instead of 234.
	//   - TDestroyed_deallocate_ with a member-initialiser list, GetSize_extend_
	//     without the `newCap` local, `DestroyElement_(pBegin_, pEnd_)` for
	//     DestroyElement_all_(), `new (d_first)` without `&*`, and
	//     TAllocator::deallocate calling ::operator delete directly (this last
	//     one moves the standalone InsertRaw 0xd8 -> 0xc0 but not insert): all
	//     +0 on insert's frame.
	//
	// Header round 26 re-measured the whole ladder and found no single 32-byte
	// object to remove: the excess is additive over the MSL helpers, and the
	// two levers that exist only reach -0x18 between them (dummy -0x10,
	// __copy_backward -0x8), both with the regressions above. `dealloc` is
	// worth 0x18 of insert's frame on its own but its destructor is four real
	// instructions, so it is retail's. Also +0 on insert (measured here):
	// InsertRaw defined out of class with `inline`, a split-declared
	// `iterator it; it = InsertRaw(...)`, dropping `I dummy` from
	// std::uninitialized_fill_n (it folds away entirely), and `if (it !=
	// pEnd_) fill; else WARN;`. Wrong direction: `if (it == end())` is +8
	// (the accessor lever works here, so the residue is a shortage, not a
	// misplacement), `std::uninitialized_fill` costs an instruction and
	// `iterator it = pIt` five. The second residue is a callee-saved swap:
	// retail ranks `this` (r30) above the `how_many` parameter (r29), we do
	// the reverse -- the known-open ranking class, and no declaration order
	// inside insert changes it.
	void insert(iterator where, size_t how_many, const T& what)
	{
		if (!how_many)
			return;

		iterator it = InsertRaw(where, how_many);

		if (it == pEnd_) {
			JGADGET_WARN("can't allocate memory");
			return;
		}

		std::uninitialized_fill_n(it, how_many, what);
		// NOTE: non-standard: forgot to return the iterator
	}

	iterator erase(iterator where)
	{
		JGADGET_ASSERT(!empty());
		return erase(where, where + 1);
	}
	iterator erase(iterator pItFirst, iterator pItLast)
	{
		// clang-format off
		JGADGET_ASSERT((pBegin_<=pItFirst)&&(pItFirst<=pEnd_));
		JGADGET_ASSERT((pBegin_<=pItLast)&&(pItLast<=pEnd_));
		JGADGET_ASSERT(pItFirst<=pItLast);
		// clang-format on

		T* newEnd2 = pItFirst;
		T* newEnd  = std::copy(pItLast, pEnd_, newEnd2);
		DestroyElement_(newEnd, pEnd_);
		pEnd_ = newEnd;
		return newEnd2;
	}

	void push_back(const T& value) { insert(end(), value); }
	void pop_back() { erase(end() - 1); }

	void clear() { erase(begin(), end()); }

	void assign(size_t, const T&) { } // not present in any other game

	void resize(size_t new_size, const T& value = T())
	{
		// NOTE: `sz` has to be const. The inliner duplicates an argument
		// expression at each use when the expression is safe to repeat, and a
		// read of a const local counts as safe. Without the const it binds
		// `new_size - sz` to a temporary instead, and the subtraction happens
		// once rather than twice.
		const size_t sz = size();
		if (new_size > sz) {
			insert(end(), new_size - sz, value);
		} else if (new_size != size()) {
			ResizeNotLarger(new_size);
		}
	}

	void reserve(size_t new_capacity)
	{
		if (new_capacity <= mCapacity)
			return;

		T* const newBegin = mAllocator.allocate(new_capacity);

		if (!newBegin) {
			JGADGET_WARN("can't allocate memory");
			return;
		}

		TDestroyed_deallocate_ dealloc(mAllocator, newBegin);

		std::uninitialized_copy(pBegin_, pEnd_, newBegin);
		DestroyElement_all_();

		dealloc.set(pBegin_);

		const size_t size = pEnd_ - pBegin_;

		pEnd_     = newBegin + size;
		pBegin_   = newBegin;
		mCapacity = new_capacity;
	}

	iterator InsertRaw(T* pIt, size_t count)
	{
		// clang-format off
		JGADGET_ASSERT((pBegin_<=pIt)&&(pIt<=pEnd_));
		// clang-format on

		// NOTE: the explicit conversion is required. It makes a second object
		// that the compiler does not fold into pIt, which is what keeps pIt
		// itself in its argument register for the returns below. A plain
		// `iterator it = pIt;` does not match.
		iterator it = iterator(pIt);

		if (count == 0)
			return pIt;

		if (count + size() <= mCapacity) {
			T* holeEnd = it + count;
			if (holeEnd < pEnd_) {
				T* split = pEnd_ - count;
				std::uninitialized_copy(split, pEnd_, pEnd_);
				std::copy_backward(it, split, pEnd_);
				DestroyElement_(it, holeEnd);
				pEnd_ += count;
				return pIt;
			} else {
				std::uninitialized_copy(it, pEnd_, holeEnd);
				DestroyElement_(it, pEnd_);
				pEnd_ += count;
				return pIt;
			}
		} else {
			size_t newCap = GetSize_extend_(count);
			T* newBegin   = mAllocator.allocate(newCap);

			if (!newBegin)
				return end();

			TDestroyed_deallocate_ dealloc(mAllocator, newBegin);

			T* holeStart = std::uninitialized_copy(pBegin_, it, newBegin);
			std::uninitialized_copy(it, pEnd_, holeStart + count);
			DestroyElement_all_();

			dealloc.set(pBegin_);

			pEnd_     = newBegin + (pEnd_ - pBegin_ + count);
			pBegin_   = newBegin;
			mCapacity = newCap;

			return holeStart;
		}
	}
	T* ResizeRaw(size_t new_size)
	{
		size_t sz = size();
		if (new_size > sz) {
			return InsertRaw(end(), new_size - sz);
		} else if (new_size != size()) {
			ResizeNotLarger(new_size);
			return end();
		}
	}
	// fabricated but present in TP as Resize_notLarger_
	void ResizeNotLarger(size_t u)
	{
		size_t uSize = size();
		// clang-format off
		JGADGET_ASSERT(u<=uSize);
		// clang-format on
		erase(pBegin_ + u, pEnd_);
	}

private:
	Allocator mAllocator;
	T* pBegin_;
	T* pEnd_;
	size_t mCapacity;
	float mGrowthMul;
	size_t mGrowthAdd;
};

class TVector_pointer_void : public TVector<void*> {
	typedef TVector<void*> Base;
	typedef void* value_type;

public:
	typedef Base::iterator iterator;

	TVector_pointer_void(
	    const TAllocator<value_type>& = TAllocator<value_type>());
	TVector_pointer_void(size_t, const value_type&,
	                     const TAllocator<value_type>&);
	TVector_pointer_void& operator=(const JGadget::TVector_pointer_void&);
	~TVector_pointer_void();

	size_t size() const { return Base::size(); }

	// TODO: the ROM `bl`s JGadget::TVector<void*>::begin() (weak 0x8, `lwz
	// r3,4(r3)`, emitted only from bosseel.cpp and MSoundMainSide.cpp) at
	// exactly two places: TBossEel::perform's inlined
	// calcAndSetCollisionCubeBite_ -- which reads element **[1]**, `lwz
	// r3,4(r3)` after the call, while every other eel site reads [0] -- and
	// MSStageCubeFade::calcParamRatioInCube, also reached one inline deep,
	// with a variable index (`slwi r0,rN,2; lwzx`). Every shallower site folds
	// the chain to a single `lwz 0x10`. So the call needs one or two more
	// inline levels than `getChildren() -> TVector_pointer<T>::begin() ->
	// TVector<void*>::begin()` gives, and adding them here is a shared-header
	// change. Measured (each line is a full `ninja changes_all`):
	//
	//   begin()/end() facades here:         MSoundMainSide gains retail's
	//     `addi 0xc` but no `bl`; TCubeManagerBase ctor -0.5. No `bl`.
	//   + a probe level inside
	//     TVector_pointer<T>::begin():      the `bl` appears -- so six levels
	//     are needed from the emitted caller, and no plausible member supplies
	//     the sixth.
	//   TVector<T>::operator[] as
	//     `begin()[u]` (+ the two deep
	//     sites respelled `getChildren()[i]`):
	//                                       no `bl`, but a +4 inline temporary
	//     everywhere operator[] is used: MarNameRefGen 48.85 -> 57.60 (four
	//     TNameRefAryT::load exact), DrawSyncManager 19.27 -> 21.77,
	//     CubeManagerBase 29.12 -> 34.04 (isInCube exact), total matched_code
	//     47.39 -> 47.43 -- against TMapObjWave::updateHeightAndAlpha 100 ->
	//     99.96 (its named locals shift +4 inside an unchanged 0x70 frame; a
	//     `*getChildren().begin()[i]` respelling there gives 0x68 instead, 8
	//     short) and TApplication::mountStageArchive 87.27 -> 87.17 (the
	//     target's `lwzx` becomes `add`+`lwz`).
	//   operator[] facade here forwarding
	//     to `Base::operator[](i)`:         isInCube exact, but the same
	//     MapObjWave and mountStageArchive drops; total matched_code -0.02.
	//   operator[] facade here forwarding
	//     to `Base::begin()[i]`:            mountStageArchive 87.27 -> 94.05,
	//     isInAreaCube 82.04 -> 82.45, but CPolarSubCamera::controlByCameraCode_
	//     99.88 -> 98.10 (the target's `add`+`lwz 0` becomes `lwzx`), and none
	//     of `&(*unk14)[i]`, `*(begin() + i)`, `begin()[i]` or
	//     `&getChildren()[i]` recovers it.
	//
	// Nothing measured produces the `bl` without a regression, so the header
	// is left alone. The `[1]` index in calcAndSetCollisionCubeBite_ is a real
	// finding and holds independently of the inlining question.

	iterator insert(iterator where, const value_type& what);
	void insert(iterator, size_t, const value_type&);

	iterator erase(iterator);
	iterator erase(iterator, iterator);

	void push_back(const value_type& value) { insert(end(), value); }

	void assign(size_t, const value_type&);
	void resize(size_t, const value_type&);
	void reserve(size_t);

	value_type* InsertRaw(value_type*, size_t);
	value_type* ResizeRaw(size_t);
};

// The template argument is the **pointer** type, not the pointee: the map
// spells the destructor `__dt__Q27JGadget35TVector_pointer<P15TStageEnemyInfo>Fv`
// (likewise `<P16TCubeGeneralInfo>`, `<P8TBaseNPC>` and the
// `<P55TNameRefAryT<...>>` of MarNameRefGen), so `value_type` is T and
// `iterator` is T*. TNameRefPtrAryT<T, U> keeps the pointee as its own
// argument (`TNameRefPtrAryT<15TStageEnemyInfo,...>`) and derives from
// `TVector_pointer<T*>`.
template <class T> class TVector_pointer : public TVector_pointer_void {
	typedef TVector_pointer_void Base;

public:
	typedef T value_type;
	typedef T* iterator;

	TVector_pointer(const JGadget::TAllocator<void*>& allocator
	                = JGadget::TAllocator<void*>())
	    : TVector_pointer_void(allocator)
	{
	}

	~TVector_pointer() { }

	iterator begin() { return iterator(Base::begin()); }
	iterator end() { return iterator(Base::end()); }
	size_t size() const { return Base::size(); }

	value_type operator[](size_t i)
	{
		return static_cast<value_type>(Base::operator[](i));
	}
	value_type operator[](size_t i) const
	{
		return static_cast<value_type>(Base::operator[](i));
	}

	void push_back(const value_type& value) { Base::push_back(value); }
};

} // namespace JGadget

#endif
