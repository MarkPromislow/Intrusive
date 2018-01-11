#pragma once

#include <new>

namespace Intrusive
{

template <typename BaseType, typename Type>
class ObjectPool
{
protected:
	size_t _blockSize;
	BaseType *_next;

	struct Block
	{
		size_t _size;
		Block *_next;
		Type *_objects;

		void initialize(size_t size)
		{
			_size = size;
			_objects = new (this + 1) Type[_size];
			Type *itr = _objects;
			for (Type *end = _objects + _size - 1; itr < end; ++itr)
				itr->_next = itr + 1;
			itr->_next = 0;
		}

		~Block()
		{
			for (Type *itr = _objects, *end = _objects + _size; itr < end; ++itr)
				itr->~Type();
		}
	};
	Block *_blocks;
public:
	ObjectPool(size_t blockSize = 256) : _blockSize(blockSize), _next(0), _blocks(0) {}

	Type *allocate();
	void free(Type *object);

	~ObjectPool();
};

template <typename BaseType, typename Type>
Type* ObjectPool<BaseType, Type>::allocate()
{
	BaseType *object = _next;
	if (! object)
	{
		Block *block = static_cast<Block*>(operator new (sizeof(Block) + sizeof(size_t) + _blockSize * sizeof(Type)));
		block->_next = _blocks;
		_blocks = block;

		block->initialize(_blockSize);
		object = block->_objects;
	}
	_next = object->_next;
	return static_cast<Type*>(object);
}

template <typename BaseType, typename Type>
void ObjectPool<BaseType, Type>::free(Type *object)
{
	static_cast<BaseType*>(object)->_next = _next;
	_next = object;
}

template <typename BaseType, typename Type>
ObjectPool<BaseType, Type>::~ObjectPool()
{
	for (Block *block = _blocks; block; block = _blocks)
	{
		_blocks = block->_next;
		delete block;
	}
}

} // namespace Intrusive
