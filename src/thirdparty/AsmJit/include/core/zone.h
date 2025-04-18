// This file is part of AsmJit project <https://asmjit.com>
//
// See asmjit.h or LICENSE.md for license and copyright information
// SPDX-License-Identifier: Zlib

#ifndef ASMJIT_CORE_ZONE_H_INCLUDED
#define ASMJIT_CORE_ZONE_H_INCLUDED

#include "../core/support.h"

ASMJIT_BEGIN_NAMESPACE

//! \addtogroup asmjit_zone
//! \{

//! Zone memory.
//!
//! Zone is an incremental memory allocator that allocates memory by simply incrementing a pointer. It allocates
//! blocks of memory by using C's `malloc()`, but divides these blocks into smaller segments requested by calling
//! `Zone::alloc()` and friends.
//!
//! Zone has no function to release the allocated memory. It has to be released all at once by calling `reset()`.
//! If you need a more friendly allocator that also supports `release()`, consider using `Zone` with `ZoneAllocator`.
class Zone {
public:
  ASMJIT_NONCOPYABLE(Zone)

  //! \cond INTERNAL

  //! A single block of memory managed by `Zone`.
  struct Block {
    inline uint8_t* data() const noexcept {
      return const_cast<uint8_t*>(reinterpret_cast<const uint8_t*>(this) + sizeof(*this));
    }

    //! Link to the previous block.
    Block* prev;
    //! Link to the next block.
    Block* next;
    //! Size of the block.
    size_t size;
  };

  enum Limits : size_t {
    kMinBlockSize = 256, // The number is ridiculously small, but still possible.
    kMaxBlockSize = size_t(1) << (sizeof(size_t) * 8 - 1),

    kMinAlignment = 1,
    kMaxAlignment = 64,

    kBlockSize = sizeof(Block),
    kBlockOverhead = kBlockSize + Globals::kAllocOverhead
  };

  //! Pointer in the current block.
  uint8_t* _ptr;
  //! End of the current block.
  uint8_t* _end;
  //! Current block.
  Block* _block;

  //! Block alignment shift
  uint8_t _blockAlignmentShift;
  //! Minimum log2(blockSize) to allocate.
  uint8_t _minimumBlockSizeShift;
  //! Maximum log2(blockSize) to allocate.
  uint8_t _maximumBlockSizeShift;
  //! True when the Zone has a static block (static blocks are used by ZoneTmp).
  uint8_t _hasStaticBlock;
  //! Reserved for future use, must be zero.
  uint32_t _reserved;
  //! Count of allocated blocks.
  size_t _blockCount;

  static ASMJIT_API const Block _zeroBlock;

  //! \endcond

  //! \name Construction & Destruction
  //! \{

  //! Creates a new Zone.
  //!
  //! The `blockSize` parameter describes the default size of the block. If the `size` parameter passed to `alloc()`
  //! is greater than the default size `Zone` will allocate and use a larger block, but it will not change the
  //! default `blockSize`.
  //!
  //! It's not required, but it's good practice to set `blockSize` to a reasonable value that depends on the usage
  //! of `Zone`. Greater block sizes are generally safer and perform better than unreasonably low block sizes.
  ASMJIT_INLINE_NODEBUG explicit Zone(size_t minimumBlockSize, size_t blockAlignment = 1) noexcept {
    _init(minimumBlockSize, blockAlignment, nullptr);
  }

  //! Creates a new Zone with a first block pointing to a `temporary` memory.
  ASMJIT_INLINE_NODEBUG Zone(size_t minimumBlockSize, size_t blockAlignment, const Support::Temporary& temporary) noexcept {
    _init(minimumBlockSize, blockAlignment, &temporary);
  }

  //! \overload
  ASMJIT_INLINE_NODEBUG Zone(size_t minimumBlockSize, size_t blockAlignment, const Support::Temporary* temporary) noexcept {
    _init(minimumBlockSize, blockAlignment, temporary);
  }

  //! Moves an existing `Zone`.
  //!
  //! \note You cannot move an existing `ZoneTmp` as it uses embedded storage. Attempting to move `ZoneTmp` would
  //! cause an undefined behavior (covered by assertions in debug mode).
  inline Zone(Zone&& other) noexcept
    : _ptr(other._ptr),
      _end(other._end),
      _block(other._block),
      _blockAlignmentShift(other._blockAlignmentShift),
      _minimumBlockSizeShift(other._minimumBlockSizeShift),
      _maximumBlockSizeShift(other._maximumBlockSizeShift),
      _hasStaticBlock(other._hasStaticBlock),
      _reserved(other._reserved),
      _blockCount(other._blockCount) {
    ASMJIT_ASSERT(!other.hasStaticBlock());
    other._block = const_cast<Block*>(&_zeroBlock);
    other._ptr = other._block->data();
    other._end = other._block->data();
    other._blockCount = 0u;
  }

  //! Destroys the `Zone` instance.
  //!
  //! This will destroy the `Zone` instance and release all blocks of memory allocated by it. It performs implicit
  //! `reset(ResetPolicy::kHard)`.
  ASMJIT_INLINE_NODEBUG ~Zone() noexcept { reset(ResetPolicy::kHard); }

  ASMJIT_API void _init(size_t blockSize, size_t blockAlignment, const Support::Temporary* temporary) noexcept;

  //! Resets the `Zone` invalidating all blocks allocated.
  //!
  //! See `Globals::ResetPolicy` for more details.
  ASMJIT_API void reset(ResetPolicy resetPolicy = ResetPolicy::kSoft) noexcept;

  //! \}

  //! \name Accessors
  //! \{

  //! Returns the default block alignment.
  ASMJIT_INLINE_NODEBUG size_t blockAlignment() const noexcept { return size_t(1) << _blockAlignmentShift; }
  //! Returns a minimum block size.
  ASMJIT_INLINE_NODEBUG size_t minimumBlockSize() const noexcept { return size_t(1) << _minimumBlockSizeShift; }
  //! Returns a maximum block size.
  ASMJIT_INLINE_NODEBUG size_t maximumBlockSize() const noexcept { return size_t(1) << _maximumBlockSizeShift; }
  //! Tests whether this `Zone` is actually a `ZoneTmp` that uses temporary memory.
  ASMJIT_INLINE_NODEBUG uint8_t hasStaticBlock() const noexcept { return _hasStaticBlock; }

  //! Returns remaining size of the current block.
  ASMJIT_INLINE_NODEBUG size_t remainingSize() const noexcept { return (size_t)(_end - _ptr); }

  //! Returns the current zone cursor (dangerous).
  //!
  //! This is a function that can be used to get exclusive access to the current block's memory buffer.
  template<typename T = uint8_t>
  ASMJIT_INLINE_NODEBUG T* ptr() noexcept { return reinterpret_cast<T*>(_ptr); }

  //! Returns the end of the current zone block, only useful if you use `ptr()`.
  template<typename T = uint8_t>
  ASMJIT_INLINE_NODEBUG T* end() noexcept { return reinterpret_cast<T*>(_end); }

  //! Sets the current zone pointer to `ptr` (must be within the current block).
  template<typename T>
  inline void setPtr(T* ptr) noexcept {
    uint8_t* p = reinterpret_cast<uint8_t*>(ptr);
    ASMJIT_ASSERT(p >= _ptr && p <= _end);
    _ptr = p;
  }

  //! Sets the end zone pointer to `end` (must be within the current block).
  template<typename T>
  inline void setEnd(T* end) noexcept {
    uint8_t* p = reinterpret_cast<uint8_t*>(end);
    ASMJIT_ASSERT(p >= _ptr && p <= _end);
    _end = p;
  }

  //! \}

  //! \name Utilities
  //! \{

  inline void swap(Zone& other) noexcept {
    // This could lead to a disaster.
    ASMJIT_ASSERT(!this->hasStaticBlock());
    ASMJIT_ASSERT(!other.hasStaticBlock());

    std::swap(_ptr, other._ptr);
    std::swap(_end, other._end);
    std::swap(_block, other._block);

    std::swap(_blockAlignmentShift, other._blockAlignmentShift);
    std::swap(_minimumBlockSizeShift, other._minimumBlockSizeShift);
    std::swap(_maximumBlockSizeShift, other._maximumBlockSizeShift);
    std::swap(_hasStaticBlock, other._hasStaticBlock);
    std::swap(_reserved, other._reserved);
    std::swap(_blockCount, other._blockCount);
  }

  //! Aligns the current pointer to `alignment`.
  ASMJIT_INLINE_NODEBUG void align(size_t alignment) noexcept {
    _ptr = Support::min(Support::alignUp(_ptr, alignment), _end);
  }

  //! Ensures the remaining size is at least equal or greater than `size`.
  //!
  //! \note This function doesn't respect any alignment. If you need to ensure there is enough room for an aligned
  //! allocation you need to call `align()` before calling `ensure()`.
  ASMJIT_INLINE_NODEBUG Error ensure(size_t size) noexcept {
    if (ASMJIT_LIKELY(size <= remainingSize()))
      return kErrorOk;
    else
      return _alloc(0, 1) ? kErrorOk : DebugUtils::errored(kErrorOutOfMemory);
  }

  //! \}

  //! \name Allocation
  //! \{

  //! Allocates the requested memory specified by `size`.
  //!
  //! Pointer returned is valid until the `Zone` instance is destroyed or reset by calling `reset()`. If you plan to
  //! make an instance of C++ from the given pointer use placement `new` and `delete` operators:
  //!
  //! ```
  //! using namespace asmjit;
  //!
  //! class Object { ... };
  //!
  //! // Create Zone with default block size of 65536 bytes (the maximum size per alloc() would be slightly less).
  //! Zone zone(65536);
  //!
  //! // Create your objects using zone object allocating, for example:
  //! Object* obj = static_cast<Object*>( zone.alloc(sizeof(Object)) );
  //!
  //! if (!obj) {
  //!   // Handle out of memory error.
  //! }
  //!
  //! // Placement `new` and `delete` operators can be used to instantiate it.
  //! new(obj) Object();
  //!
  //! // ... lifetime of your objects ...
  //!
  //! // To destroy the instance (if required).
  //! obj->~Object();
  //!
  //! // Reset or destroy `Zone`.
  //! zone.reset();
  //! ```
  inline void* alloc(size_t size) noexcept {
    if (ASMJIT_UNLIKELY(size > remainingSize()))
      return _alloc(size, 1);

    uint8_t* ptr = _ptr;
    _ptr += size;
    return static_cast<void*>(ptr);
  }

  //! Allocates the requested memory specified by `size` and `alignment`.
  inline void* alloc(size_t size, size_t alignment) noexcept {
    ASMJIT_ASSERT(Support::isPowerOf2(alignment));
    uint8_t* ptr = Support::alignUp(_ptr, alignment);

    if (ptr >= _end || size > (size_t)(_end - ptr))
      return _alloc(size, alignment);

    _ptr = ptr + size;
    return static_cast<void*>(ptr);
  }

  //! Allocates the requested memory specified by `size` without doing any checks.
  //!
  //! Can only be called if `remainingSize()` returns size at least equal to `size`.
  inline void* allocNoCheck(size_t size) noexcept {
    ASMJIT_ASSERT(remainingSize() >= size);

    uint8_t* ptr = _ptr;
    _ptr += size;
    return static_cast<void*>(ptr);
  }

  //! Allocates the requested memory specified by `size` and `alignment` without doing any checks.
  //!
  //! Performs the same operation as `Zone::allocNoCheck(size)` with `alignment` applied.
  inline void* allocNoCheck(size_t size, size_t alignment) noexcept {
    ASMJIT_ASSERT(Support::isPowerOf2(alignment));

    uint8_t* ptr = Support::alignUp(_ptr, alignment);
    ASMJIT_ASSERT(size <= (size_t)(_end - ptr));

    _ptr = ptr + size;
    return static_cast<void*>(ptr);
  }

  //! Allocates `size` bytes of zeroed memory. See `alloc()` for more details.
  ASMJIT_API void* allocZeroed(size_t size, size_t alignment = 1) noexcept;

  //! Like `alloc()`, but the return pointer is casted to `T*`.
  template<typename T>
  inline T* allocT(size_t size = sizeof(T), size_t alignment = alignof(T)) noexcept {
    return static_cast<T*>(alloc(size, alignment));
  }

  //! Like `allocNoCheck()`, but the return pointer is casted to `T*`.
  template<typename T>
  inline T* allocNoCheckT(size_t size = sizeof(T), size_t alignment = alignof(T)) noexcept {
    return static_cast<T*>(allocNoCheck(size, alignment));
  }

  //! Like `allocZeroed()`, but the return pointer is casted to `T*`.
  template<typename T>
  inline T* allocZeroedT(size_t size = sizeof(T), size_t alignment = alignof(T)) noexcept {
    return static_cast<T*>(allocZeroed(size, alignment));
  }

  //! Like `new(std::nothrow) T(...)`, but allocated by `Zone`.
  template<typename T>
  inline T* newT() noexcept {
    void* p = alloc(sizeof(T), alignof(T));
    if (ASMJIT_UNLIKELY(!p))
      return nullptr;
    return new(Support::PlacementNew{p}) T();
  }

  //! Like `new(std::nothrow) T(...)`, but allocated by `Zone`.
  template<typename T, typename... Args>
  inline T* newT(Args&&... args) noexcept {
    void* p = alloc(sizeof(T), alignof(T));
    if (ASMJIT_UNLIKELY(!p))
      return nullptr;
    return new(Support::PlacementNew{p}) T(std::forward<Args>(args)...);
  }

  //! \cond INTERNAL
  //!
  //! Internal alloc function used by other inlines.
  ASMJIT_API void* _alloc(size_t size, size_t alignment) noexcept;
  //! \endcond

  //! Helper to duplicate data.
  ASMJIT_API void* dup(const void* data, size_t size, bool nullTerminate = false) noexcept;

  //! Helper to duplicate data.
  inline void* dupAligned(const void* data, size_t size, size_t alignment, bool nullTerminate = false) noexcept {
    align(alignment);
    return dup(data, size, nullTerminate);
  }

  //! Helper to duplicate a formatted string, maximum size is 256 bytes.
  ASMJIT_API char* sformat(const char* str, ...) noexcept;

  //! \}

#if !defined(ASMJIT_NO_DEPRECATED)
  ASMJIT_DEPRECATED("Use Zone::minimumBlockSize() instead of Zone::blockSize()")
  ASMJIT_INLINE_NODEBUG size_t blockSize() const noexcept { return minimumBlockSize(); }

  ASMJIT_DEPRECATED("Use Zone::hasStaticBlock() instead of Zone::isTemporary()")
  ASMJIT_INLINE_NODEBUG bool isTemporary() const noexcept { return hasStaticBlock() != 0u; }
#endif
};

//! \ref Zone with `N` bytes of a static storage, used for the initial block.
//!
//! Temporary zones are used in cases where it's known that some memory will be required, but in many cases it won't
//! exceed N bytes, so the whole operation can be performed without a dynamic memory allocation.
template<size_t N>
class ZoneTmp : public Zone {
public:
  ASMJIT_NONCOPYABLE(ZoneTmp)

  //! Temporary storage, embedded after \ref Zone.
  struct Storage {
    char data[N];
  } _storage;

  //! Creates a temporary zone. Dynamic block size is specified by `blockSize`.
  inline explicit ZoneTmp(size_t blockSize, size_t blockAlignment = 1) noexcept
    : Zone(blockSize, blockAlignment, Support::Temporary(_storage.data, N)) {}
};

//! Zone-based memory allocator that uses an existing `Zone` and provides a `release()` functionality on top of it.
//! It uses `Zone` only for chunks that can be pooled, and uses libc `malloc()` for chunks that are large.
//!
//! The advantage of ZoneAllocator is that it can allocate small chunks of memory really fast, and these chunks,
//! when released, will be reused by consecutive calls to `alloc()`. Also, since ZoneAllocator uses `Zone`, you can
//! turn any `Zone` into a `ZoneAllocator`, and use it in your `Pass` when necessary.
//!
//! ZoneAllocator is used by AsmJit containers to make containers having only few elements fast (and lightweight)
//! and to allow them to grow and use dynamic blocks when require more storage.
class ZoneAllocator {
public:
  ASMJIT_NONCOPYABLE(ZoneAllocator)

  //! \cond INTERNAL

  // In short, we pool chunks of these sizes:
  //   [32, 64, 96, 128, 192, 256, 320, 384, 448, 512]

  enum : uint32_t {
    //! How many bytes per a low granularity pool (has to be at least 16).
    kLoGranularity = 32,
    //! Number of slots of a low granularity pool.
    kLoCount = 4,
    //! Maximum size of a block that can be allocated in a low granularity pool.
    kLoMaxSize = kLoGranularity * kLoCount,

    //! How many bytes per a high granularity pool.
    kHiGranularity = 64,
    //! Number of slots of a high granularity pool.
    kHiCount = 6,
    //! Maximum size of a block that can be allocated in a high granularity pool.
    kHiMaxSize = kLoMaxSize + kHiGranularity * kHiCount,

    //! Alignment of every pointer returned by `alloc()`.
    kBlockAlignment = kLoGranularity
  };

  //! Single-linked list used to store unused chunks.
  struct Slot {
    //! Link to a next slot in a single-linked list.
    Slot* next;
  };

  //! A block of memory that has been allocated dynamically and is not part of block-list used by the allocator.
  //! This is used to keep track of all these blocks so they can be freed by `reset()` if not freed explicitly.
  struct DynamicBlock {
    DynamicBlock* prev;
    DynamicBlock* next;
  };

  //! \endcond

  //! \name Members
  //! \{

  //! Zone used to allocate memory that fits into slots.
  Zone* _zone {};
  //! Indexed slots containing released memory.
  Slot* _slots[kLoCount + kHiCount] {};
  //! Dynamic blocks for larger allocations (no slots).
  DynamicBlock* _dynamicBlocks {};

  //! \}

  //! \name Construction & Destruction
  //! \{

  //! Creates a new `ZoneAllocator`.
  //!
  //! \note To use it, you must first `init()` it.
  ASMJIT_INLINE_NODEBUG ZoneAllocator() noexcept {}

  //! Creates a new `ZoneAllocator` initialized to use `zone`.
  ASMJIT_INLINE_NODEBUG explicit ZoneAllocator(Zone* zone) noexcept
    : _zone(zone) {}

  //! Destroys the `ZoneAllocator`.
  ASMJIT_INLINE_NODEBUG ~ZoneAllocator() noexcept { reset(); }

  //! Tests whether the `ZoneAllocator` is initialized (i.e. has `Zone`).
  ASMJIT_INLINE_NODEBUG bool isInitialized() const noexcept { return _zone != nullptr; }

  //! Convenience function to initialize the `ZoneAllocator` with `zone`.
  //!
  //! It's the same as calling `reset(zone)`.
  ASMJIT_INLINE_NODEBUG void init(Zone* zone) noexcept { reset(zone); }

  //! Resets this `ZoneAllocator` and also forget about the current `Zone` which is attached (if any). Reset
  //! optionally attaches a new `zone` passed, or keeps the `ZoneAllocator` in an uninitialized state, if
  //! `zone` is null.
  ASMJIT_API void reset(Zone* zone = nullptr) noexcept;

  //! \}

  //! \name Accessors
  //! \{

  //! Returns the assigned `Zone` of this allocator or null if this `ZoneAllocator` is not initialized.
  ASMJIT_INLINE_NODEBUG Zone* zone() const noexcept { return _zone; }

  //! \}

  //! \cond
  //! \name Internals
  //! \{

  //! Returns the slot index to be used for `size`. Returns `true` if a valid slot has been written to `slot` and
  //! `allocatedSize` has been filled with slot exact size (`allocatedSize` can be equal or slightly greater than
  //! `size`).
  static inline bool _getSlotIndex(size_t size, uint32_t& slot) noexcept {
    ASMJIT_ASSERT(size > 0);
    if (size > kHiMaxSize)
      return false;

    if (size <= kLoMaxSize)
      slot = uint32_t((size - 1) / kLoGranularity);
    else
      slot = uint32_t((size - kLoMaxSize - 1) / kHiGranularity) + kLoCount;

    return true;
  }

  //! \overload
  static inline bool _getSlotIndex(size_t size, uint32_t& slot, size_t& allocatedSize) noexcept {
    ASMJIT_ASSERT(size > 0);
    if (size > kHiMaxSize)
      return false;

    if (size <= kLoMaxSize) {
      slot = uint32_t((size - 1) / kLoGranularity);
      allocatedSize = Support::alignUp(size, kLoGranularity);
    }
    else {
      slot = uint32_t((size - kLoMaxSize - 1) / kHiGranularity) + kLoCount;
      allocatedSize = Support::alignUp(size, kHiGranularity);
    }

    return true;
  }

  //! \}
  //! \endcond

  //! \name Allocation
  //! \{

  //! \cond INTERNAL
  ASMJIT_API void* _alloc(size_t size, size_t& allocatedSize) noexcept;
  ASMJIT_API void* _allocZeroed(size_t size, size_t& allocatedSize) noexcept;
  ASMJIT_API void _releaseDynamic(void* p, size_t size) noexcept;
  //! \endcond

  //! Allocates `size` bytes of memory, ideally from an available pool.
  //!
  //! \note `size` can't be zero, it will assert in debug mode in such case.
  inline void* alloc(size_t size) noexcept {
    ASMJIT_ASSERT(isInitialized());
    size_t allocatedSize;
    return _alloc(size, allocatedSize);
  }

  //! Like `alloc(size)`, but provides a second argument `allocatedSize` that provides a way to know how big
  //! the block returned actually is. This is useful for containers to prevent growing too early.
  inline void* alloc(size_t size, size_t& allocatedSize) noexcept {
    ASMJIT_ASSERT(isInitialized());
    return _alloc(size, allocatedSize);
  }

  //! Like `alloc()`, but the return pointer is casted to `T*`.
  template<typename T>
  inline T* allocT(size_t size = sizeof(T)) noexcept {
    return static_cast<T*>(alloc(size));
  }

  //! Like `alloc(size)`, but returns zeroed memory.
  inline void* allocZeroed(size_t size) noexcept {
    ASMJIT_ASSERT(isInitialized());
    size_t allocatedSize;
    return _allocZeroed(size, allocatedSize);
  }

  //! Like `alloc(size, allocatedSize)`, but returns zeroed memory.
  inline void* allocZeroed(size_t size, size_t& allocatedSize) noexcept {
    ASMJIT_ASSERT(isInitialized());
    return _allocZeroed(size, allocatedSize);
  }

  //! Like `allocZeroed()`, but the return pointer is casted to `T*`.
  template<typename T>
  inline T* allocZeroedT(size_t size = sizeof(T)) noexcept {
    return static_cast<T*>(allocZeroed(size));
  }

  //! Like `new(std::nothrow) T(...)`, but allocated by `Zone`.
  template<typename T>
  inline T* newT() noexcept {
    void* p = allocT<T>();
    if (ASMJIT_UNLIKELY(!p))
      return nullptr;
    return new(Support::PlacementNew{p}) T();
  }
  //! Like `new(std::nothrow) T(...)`, but allocated by `Zone`.
  template<typename T, typename... Args>
  inline T* newT(Args&&... args) noexcept {
    void* p = allocT<T>();
    if (ASMJIT_UNLIKELY(!p))
      return nullptr;
    return new(Support::PlacementNew{p}) T(std::forward<Args>(args)...);
  }

  //! Releases the memory previously allocated by `alloc()`. The `size` argument has to be the same as used to call
  //! `alloc()` or `allocatedSize` returned  by `alloc()`.
  inline void release(void* p, size_t size) noexcept {
    ASMJIT_ASSERT(isInitialized());
    ASMJIT_ASSERT(p != nullptr);
    ASMJIT_ASSERT(size != 0);

    uint32_t slot;
    if (_getSlotIndex(size, slot)) {
      static_cast<Slot*>(p)->next = static_cast<Slot*>(_slots[slot]);
      _slots[slot] = static_cast<Slot*>(p);
    }
    else {
      _releaseDynamic(p, size);
    }
  }

  //! \}
};

//! \}

ASMJIT_END_NAMESPACE

#endif // ASMJIT_CORE_ZONE_H_INCLUDED
