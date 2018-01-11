#pragma once

#include <functional>

namespace Intrusive
{

class LinkedListObject
{
protected:
	LinkedListObject *_prev;
	LinkedListObject *_next;
	friend class LinkedList;
	template <typename T>
	friend class LinkedObjectPool;
public:
	LinkedListObject() : _prev(this), _next(this) {}
	LinkedListObject *next() { return _next; }
	LinkedListObject *prev() { return _prev; }
	void unlink() { _prev->_next = _next; _next->_prev = _prev; _prev = _next = this; }
	void linkAfter(LinkedListObject *o) { o->_prev = this; o->_next = _next; _next->_prev = o; _next = o; }
	void linkBefore(LinkedListObject *o) { o->_prev = _prev; o->_next = this; _prev->_next = o; _prev = o; }
	void reset() { _prev = _next = this; }
};

class LinkedList: public LinkedListObject
{
protected:
	LinkedListObject _list;
public:
	LinkedList() {}
	LinkedList(const LinkedList &list) {}
	bool empty() { return _list._prev == &_list; }
	void clear() { _list.unlink(); }
	void push_back(LinkedListObject *o) { o->_prev = _list._prev; o->_next = &_list; _list._prev->_next = o; _list._prev = o; }
	void push_front(LinkedListObject *o) { o->_prev = &_list; o->_next = _list._next; _list._next->_prev = o; _list._next = o; }
	LinkedListObject *begin() { return _list._next; }
	LinkedListObject *rbegin() { return _list._prev; }
	LinkedListObject *end() { return &_list; }
	LinkedListObject *pop_front() { LinkedListObject *n(0); if (_list._next != &_list) { n = _list._next; n->unlink(); } return n; }
	LinkedListObject *pop_back() { LinkedListObject *p(0); if (_list._prev != &_list) { p = _list._prev; p->unlink(); } return p; }
	size_t size() { size_t s = 0; for (LinkedListObject *o = _list._next; o != &_list; o = o->_next) ++s; return s; }
private:
	LinkedList& operator = (const LinkedList &) = delete;
};

} // namespace Intrusive
