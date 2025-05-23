// Copyright (c) 2024 Clarisma / GeoDesk contributors
// SPDX-License-Identifier: LGPL-3.0-only

#include <geodesk/geom/LengthUnit.h>
#include <cstring>	// required for LengthUnit_attr

#include "LengthUnit_attr.cxx"

namespace geodesk {

const char* LengthUnit::VALID_UNITS = "meters (m), kilometers (km), feet (ft), yards (yd) or miles (mi)";

int LengthUnit::unitFromString(std::string_view unit)
{
	Unit* attr = LengthUnit_AttrHash::lookup(unit.data(), unit.length());
	if (attr) return attr->index;
	return -1;
}

const double LengthUnit::METERS_TO_UNIT[]
{
	1.0,		// meters
	0.001,      // km 
	3.28084,    // feet
	1.093613,   // yards
	0.0006213711922373339,	// miles
};

const double LengthUnit::UNITS_TO_METERS[] =
{
	1.0,			// meters
	1.0 / 0.001,    // km
	1.0 / 3.28084,  // feet
	1.0 / 1.093613, // yards
	1.0 / 0.0006213711922373339,   // miles
};

} // namespace geodesk
