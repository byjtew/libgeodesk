// Copyright (c) 2024 Clarisma / GeoDesk contributors
// SPDX-License-Identifier: LGPL-3.0-only

#include "Selector.h"

namespace geodesk {

Selector::Selector(FeatureTypes types) :
	next(nullptr),
	acceptedTypes(types),
	indexBits(0),
	firstClause(nullptr),
	falseOp(Opcode::RETURN)
{
}


void Selector::addClause(TagClause* clause)
{
	TagClause** pNext = &firstClause;
	TagClause* current;
	while ((current = *pNext) != nullptr)
	{
		int comp = current->keyOp.compareTo(&clause->keyOp);
		if (comp < 0)
		{
			pNext = &current->next;
			continue;
		}
		if (comp == 0)
		{
			current->absorb(clause);
			return;
		}
		break;
	}
	if (clause->flags & TagClause::KEY_REQUIRED)
	{
		indexBits |= IndexBits::fromCategory(clause->category);
	}
	clause->next = current;
	*pNext = clause;
}


} // namespace geodesk
