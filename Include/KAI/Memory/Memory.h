
#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#ifndef KAI_MEMORY_MEMORY_H
#	define KAI_MEMORY_MEMORY_H

KAI_BEGIN

namespace Memory
{
	/// Memory subsystem. Supports different allocators
	template <class Allocator = StandardAllocator>
	struct System
	{
		typedef typename Allocator::VoidPtr VoidPtr;
		typedef typename Allocator::Byte Byte;
		typedef typename Allocator::BytePtr BytePtr;
		typedef typename Allocator::size_t size_t;

	private:
		Allocator allocator;

	public:
		System()
		{
		}
		
		System(Allocator alloc) : allocator(alloc)
		{
		}

		Allocator const &GetAllocator() const
		{
			return allocator;
		}

		VoidPtr AllocateBytes(size_t N)
		{
			return allocator.AllocateBytes(N);
		}

		void DeAllocateBytes(VoidPtr ptr, size_t N)
		{
			allocator.DeAllocateBytes(ptr, N);
		}

	};
}

KAI_END

#endif // KAI_MEMORY_MEMORY_H

//EOF
