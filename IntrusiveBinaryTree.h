#pragma once

// Simple unbalanced binary tree

namespace Intrusive
{

class BinaryTreeObject
{
protected:
	BinaryTreeObject *_left;
	BinaryTreeObject *_right;
	BinaryTreeObject *_next;

	template <typename Key, typename Type, typename Less>
	friend class BinaryTree;
public:
	BinaryTreeObject() : _left(0), _right(0), _next(0) {}

	BinaryTreeObject* next();
	BinaryTreeObject* prev();
};

template <typename Key, typename Type, typename Less>
class BinaryTree
{
protected:
	BinaryTreeObject *_tree;
	Less _less;
public:
	BinaryTree(Less less = Less()) : _tree(0), _less(less) {}

	// return true if BinaryTree is empty
	bool empty() { return !_tree; }

	// find the item equal to Key or Null if not found
	Type* find(const Key &key);

	// find closest item as defined by template Min: Type* = Min(Key, Type*, Type*)
	template <typename Min>
	Type* findClosest(const Key &key, const Min &min);

	// always inserts - dupliates ok
	void insert(Type *item);

	// remove item from tree
	void remove(Type *item);

	BinaryTreeObject* begin();
	BinaryTreeObject* end() { return 0; }
};

inline
BinaryTreeObject* BinaryTreeObject::next()
{
	BinaryTreeObject *obj;
	if (_left) for (obj = _left; obj->_right; obj = obj->_right);
	else for (BinaryTreeObject *prev = this; (obj = prev->_next) && obj->_left == prev; prev = obj);
	return obj;
}

inline
BinaryTreeObject* BinaryTreeObject::prev()
{
	BinaryTreeObject *obj;
	if (_right) for (obj = _right; obj->_left; obj = obj->_left);
	else for (BinaryTreeObject *prev = this; (obj = prev->_next) && obj->_right == prev; prev = obj);
	return obj;
}

template <typename Key, typename Type, typename Less>
Type* BinaryTree<Key, Type, Less>::find(const Key &key)
{
	BinaryTreeObject *obj(_tree);
	for (; obj;)
	{
		if (_less(key, static_cast<Type*>(obj))) obj = obj->_right;
		else if (_less(static_cast<Type*>(obj), key)) obj = obj->_left;
		else break;
	}
	return static_cast<Type*>(obj);
}

template <typename Key, typename Type, typename Less>
template <typename Min>
Type* BinaryTree<Key, Type, Less>::findClosest(const Key &key, const Min &min)
{
	BinaryTreeObject *maxLess(0), *minMore(0);
	BinaryTreeObject *obj(_tree);
	if (obj) for (;;)
	{
		Type *node = static_cast<Type*>(obj);
		if (_less(key, node))
		{
			minMore = obj;
			if (!(obj = obj->_right))
			{
				obj = (maxLess = minMore->prev()) ? min(key, static_cast<Type*>(maxLess), static_cast<Type*>(minMore)) : minMore;
				break;
			}
		}
		else if (_less(node, key))
		{
			maxLess = obj;
			if (!(obj = obj->_left))
			{
				obj = (minMore = maxLess->next()) ? min(key, static_cast<Type*>(maxLess), static_cast<Type*>(minMore)) : maxLess;
				break;
			}
		}
		else break;
		return static_cast<Type*>(obj);
	}
}

template <typename Key, typename Type, typename Less>
void BinaryTree<Key, Type, Less>::insert(Type *node)
{
	BinaryTreeObject *next(0), **objPtr(&_tree);
	for (; *objPtr;)
	{
		next = *objPtr;
		if (_less(node, static_cast<Type*>(next))) objPtr = &(next->_right);
		else objPtr = &(next->_left);
	}
	*objPtr = node;
	BinaryTreeObject *obj(node);
	obj->_next = next;
	obj->_left = obj->_right = 0;
}

template <typename Key, typename Type, typename Less>
void BinaryTree<Key, Type, Less>::remove(Type *node)
{
	BinaryTreeObject *removedObj(node);
	BinaryTreeObject *obj(removedObj->_left);
	if (removedObj->_right)
	{
		obj = removedObj->_right;
		BinaryTreeObject *leftObj(obj);
		for (; leftObj->_left; leftObj = leftObj->_left);
		
		BinaryTreeObject *left(removedObj->_left);
		if ((leftObj->_left = left)) left->_next = leftObj;
	}

	BinaryTreeObject *next(removedObj->_next);
	if (obj) obj->_next = next;
	if (next)
	{
		if (next->_right == removedObj) next->_right = obj;
		else next->_left = obj;
	}
	else
	{
		_tree = obj;
	}
	removedObj->_next = removedObj->_left = removedObj->_right = 0;
}

template <typename Key, typename Type, typename Less>
BinaryTreeObject* BinaryTree<Key, Type, Less>::begin()
{
	BinaryTreeObject *node;
	for (node = _tree; node->_right; node = node->_right);
	return node;
}

} // namespace Intrusive
