// Copyright (c) 2024 Clarisma / GeoDesk contributors
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <clarisma/text/Format.h>
#include <geodesk/feature/Tex.h>
#include <geodesk/feature/Tip.h>

namespace geodesk {

/// \cond lowlevel
///
struct ForeignFeatureRef
{
	ForeignFeatureRef(Tip tip_, Tex tex_) : tip(tip_), tex(tex_) {}
	ForeignFeatureRef() : tip(0), tex(0) {}

	bool isNull() const { return tip.isNull(); }

	char* format(char* buf) const
	{
		tip.format(buf);
		buf[6] = '#';
		return clarisma::Format::integer(&buf[7], tex);
	}

	std::string toString() const
	{
		char buf[32];
		format(buf);
		return std::string(buf);
	}

	bool operator==(const ForeignFeatureRef& other) const
	{
		return tip == other.tip && tex == other.tex;
	}

	Tip tip;
	Tex tex;
};

template<typename Stream>
Stream& operator<<(Stream& out, const ForeignFeatureRef& ref)
{
	char buf[32];
	char* p = ref.format(buf);
	assert(p - buf < sizeof(buf));
	out.write(buf, p-buf);
	return out;
}

// \endcond


} // namespace geodesk
