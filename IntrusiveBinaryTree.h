#pragma once

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

	BinaryTreeObject* next()
	{
		BinaryTreeObject *obj;
		if (_left) for (obj = _left; obj->_right; obj = obj->_right);
		else for (BinaryTreeObject *prev = this; (obj = prev->_next) && obj->_left == prev; prev = obj);
		return obj;
	}

	BinaryTreeObject* prev()
	{
		BinaryTreeObject *obj;
		if (_right) for (obj = _right; obj->_left; obj = obj->_left);
		else for (BinaryTreeObject *prev = this; (obj = prev->_next) && obj->_right == prev; prev = obj);
		return obj;
	}
};

template <typename Key, typename Type, typename Less>
class BinaryTree
{
protected:
	BinaryTreeObject *_tree;
	Less _less;
public:
	BinaryTree(Less less = Less()) : _tree(0), _less(less) {}

	void insert(Type *obj)
	{
		BinaryTreeObject *root(0), **nodePtr(&_tree);
		for (; *nodePtr;)
		{
			root = *nodePtr;
			if (_less(obj, static_cast<Type*>(root))) nodePtr = &(root->_right);
			else nodePtr = &(root->_left);
		}
		*nodePtr = obj;
		obj->_next = root;
		obj->_left = obj->_right = 0;
	}

	Type* find(const Key &key)
	{
		BinaryTreeObject *obj = _tree;
		for (; obj;)
		{
			if (_less(key, static_cast<Type*>(obj))) obj = obj->_right;
			else if (_less(static_cast<Type*>(obj), key)) obj = obj->_left;
			else break;
		}
		return static_cast<Type*>(obj);
	}

	void find(const Key &min, const Key &max)
	{
		for (node = _tree; node->_right; node = node->_right);
	}

	BinaryTreeObject* begin()
	{
		BinaryTreeObject *node;
		for (node = _tree; node->_right; node = node->_right);
		return node;
	}

	BinaryTreeObject* end()
	{
		return 0;
	}

	class iterator
	{
	protected:
		BinaryTreeObject *_node;
		iterator(BinaryTreeObject *node) : _node(node) {}
		friend class BinaryTree<Key, Type, Less>;
	public:
		iterator(const iterator &itr) : _node(itr._node) {}
		bool operator != (const iterator &itr) { return _node != itr._node; }
		Type* operator * () { return static_cast<Type*>(_node); }
		iterator& operator ++ ()
		{
			if (_node)
			{
				if (_node->_left) for (_node = _node->_left; _node->_right; _node = _node->_right);
				else for (BinaryTreeObject *prev = _node; (_node = _node->_next) && prev == _node->_left; prev = _node);
			}
			return *this;
		}
	};

	iterator beginItr()
	{
		BinaryTreeObject *node;
		for (node = _tree; node->_right; node = node->_right);
		return iterator(node);
	}

	iterator endItr()
	{
		return iterator(0);
	}
};

} // namespace Intrusive
