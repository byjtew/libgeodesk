// Copyright (c) 2024 Clarisma / GeoDesk contributors
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <limits>
#include <clarisma/math/Math.h>
#include <clarisma/text/Format.h>

namespace clarisma {

// TODO: Fix strict parsing!

///
/// A string containing a Decimal in canonical form:
/// - does not start with superfluous zeroes
/// - does not end with a dot
/// - does not end with non-numeric characters
/// - does not represent negative zero
class Decimal
{
public:
	Decimal(std::string_view s, bool strict = false) :
		value_(parse(s, strict))
	{
	}

	Decimal(int64_t mantissa, int scale) :
		value_((mantissa << 4) | scale)
	{
		assert(scale >= 0 && scale <= 15);
	}

	bool isValid() const noexcept { return value_ != INVALID;  }

	constexpr int64_t mantissa() const noexcept
	{
		return value_ >> 4;
	}

	constexpr int scale() const noexcept
	{
		return static_cast<int>(value_ & 15);
	}

	operator int64_t() const noexcept
	{
		if (value_ == INVALID) return value_;
		if (scale() == 0) return mantissa();
		return mantissa() / static_cast<int64_t>(Math::POWERS_OF_10[scale()]);
	}

	operator double() const noexcept 
	{
		if (value_ == INVALID) return std::numeric_limits<double>::quiet_NaN();
		double m = static_cast<double>(mantissa());
		if (scale() == 0) return m;
		return m / Math::POWERS_OF_10[scale()];
	}

	operator std::string() const
	{
		char buf[32];
		format(buf);
		return std::string(buf);
	}

	bool operator==(double val) const noexcept
	{
		return static_cast<double>(*this) == val;
	}

	bool operator!=(double val) const noexcept
	{
		return static_cast<double>(*this) != val;
	}


	char* format(char* buf) const noexcept;

	bool operator==(int val) const noexcept
	{
		return static_cast<int64_t>(*this) == val;
	}

	bool operator!=(int val) const noexcept
	{
		return !(*this==val);
	}

private:
	static int64_t parse(std::string_view s, bool strict);

	static constexpr int64_t INVALID = INT64_MIN;

	int64_t value_;
};


template<typename Stream>
Stream& operator<<(Stream& out, const Decimal& d)
{
	char buf[32];
	char* p = d.format(buf);
	out.write(buf, p - buf);
	return out;
}

} // namespace clarisma
