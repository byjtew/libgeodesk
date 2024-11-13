// Copyright (c) 2024 Clarisma / GeoDesk contributors
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <cmath>
#include <cstdint>
#include <functional>	// for std::hash
#include <geodesk/geom/Mercator.h>

namespace geodesk {

/// @brief A pair of Cartesian coordinate values.
///
///
class Coordinate
{
public:
	constexpr Coordinate(int32_t x_, int32_t y_) : x(x_), y(y_) {}
	Coordinate(double x_, double y_) :
		x(static_cast<int32_t>(std::round(x_))),
		y(static_cast<int32_t>(std::round(y_))) {}
	constexpr Coordinate() : x(0), y(0) {}

	/// @brief Creates a Coordinate with the given longitude and latitude.
	///
	static Coordinate ofLonLat(double lon, double lat)
	{
		return {Mercator::xFromLon(lon), Mercator::yFromLat(lat)};
	}

	/// @brief Creates a Coordinate with the given latitude and longitude.
	///
	static Coordinate ofLatLon(double lat, double lon)
	{
		return {Mercator::xFromLon(lon), Mercator::yFromLat(lat)};
	}

	double lon() const noexcept { return Mercator::lonFromX(x); }
	double lat() const noexcept { return Mercator::latFromY(y); }

	bool isNull() const noexcept { return (x | y) == 0; };

	bool operator== (const Coordinate& other) const noexcept
	{
		return x==other.x && y==other.y;			// less efficient
	}

	explicit operator int64_t() const noexcept
	{
		return (static_cast<int64_t>(y) << 32) | static_cast<uint32_t>(x);
	}

	int32_t	x;
	int32_t	y;
};

static_assert(sizeof(Coordinate) == 8, "Compiler is padding Coordinate structure");
} // namespace geodesk


// Make Coordinate hashable

template<>
struct std::hash<geodesk::Coordinate>
{
	size_t operator()(const geodesk::Coordinate& c) const noexcept
	{
		return hash<int>()(c.x) ^ (hash<int>()(c.y) << 1);
		// Shift y's hash to ensure better distribution
	}
};

