// Copyright (c) 2024 Clarisma / GeoDesk contributors
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once
#include <clarisma/data/LookupBase.h>

namespace clarisma {

/**
 * A Lookup-based template class that enables de-duplication of items
 * that are represented by a sequence of bytes (such as strings).
 * 
 * Items can be inserted using two ways:
 * 
 * - void insertUnique(T* item)
 *   Adds an item without checking whether it already exists 
 *   (Use this for items that are known to be unique)
 * 
 * - T* insert(T* item)
 *   Checks whether an item with the same content already exists;
 *   if so, returns it instead
 * 
 * Needs the following functions in the derived class:
 * 
 * - static size_t hash(T* item)
 *   Returns the hash of the given item
 * 
 * - static T** next(T* item)
 *   Returns the location of the "next item" pointer in the given item
 * 
 * Items must implement the == operator for equality check
 */

template<typename Derived, typename T>
class Deduplicator : public LookupBase<Derived, T>
{
public:
	Deduplicator() : count_(0) {}

	Deduplicator(T** table, int tableSize) :
		count_(0)
	{
		init(table, tableSize);
	}

	void insertUnique(T* item)
	{
		size_t slot = Derived::hash(item) % this->tableSize_;
		*Derived::next(item) = this->table_[slot];
		this->table_[slot] = item;
		count_++;
	}

	/**
	 * Checks if an identical item already exists in this lookup table.
	 * If so, adds to the refcount of the exiting item and returns a pointer 
	 * to it; otherwise, inserts the given item (without touching its refcount)
	 * and returns a pointer to it.
	 * 
	 * TODO: refCount
	 */
	T* insert(T* item)
	{
		size_t slot = Derived::hash(item) % this->tableSize_;
		T* existing = this->table_[slot];
		while (existing)
		{
			if (*item == *existing) return existing;
			existing = *Derived::next(existing);
		}
		*Derived::next(item) = this->table_[slot];
		this->table_[slot] = item;
		count_++;
		return item;
	}

	size_t count() const { return count_; }

	T** toArray(Arena& arena) const
	{
		return LookupBase<Derived,T>::toArray(arena, count_);
	}

private:
	size_t count_;
};

} // namespace clarisma
