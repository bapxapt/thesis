/*  Copyright (C) 2023  Dmitrii Chernikov

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef NOTAGAME_CALCULATOR_FOV_H
#define NOTAGAME_CALCULATOR_FOV_H

#include "LayoutMap.h"
#include "UnitGeneric.h"
#include "define.h"

#include <utility> //for std::pair
#include <unordered_set>

class CalculatorFOV {
	typedef std::pair<enumDirection, enumDirection> pair_enumDirection;
	typedef std::unordered_set<pair_long_long> uset_pair_long_long; 
public:
    CalculatorFOV(const LayoutMap&);
    void Calculate(sptr_UnitGeneric) const;
	void Calculate(sptr_UnitGeneric, enumDirection) const;
private:
	bool IsOnDiagonal(size_t, size_t, size_t, size_t) const;
	bool IsOnStraightLine(long long, long long, long long, long long) const;
	const LayoutMap& GetLayoutMap() const;
	void DiagonallyAccessibleTiles(csptr_UnitGeneric, uset_pair_size_t&, 
								   enumDirection, enumDirection) const;
	void SectorDegrees45Half(csptr_UnitGeneric, uset_pair_size_t&, 
							 const uset_pair_long_long&, enumDirection, 
							 enumDirection, size_t) const;
	void AddSectorDegrees45(sptr_UnitGeneric, enumDirection, bool) const;
	void AddLineStraight(sptr_UnitGeneric, enumDirection, bool = false) const;
	void AddLineAngled(uset_pair_size_t&, const uset_pair_long_long&, size_t, 
					   size_t, enumDirection, enumDirection, size_t) const;
	pair_enumDirection DetermineDirections(enumDirection, bool) const;
	void FindTilesOutOfSector(csptr_UnitGeneric, uset_pair_long_long&, 
							  enumDirection, enumDirection) const;
	size_t CalculateAngledDistanceFOV(size_t, size_t, size_t) const;
	size_t GetDistanceFOV(enumDirection, size_t) const;

	const LayoutMap& m_oLayoutMap;
};

inline const LayoutMap& CalculatorFOV::GetLayoutMap() const {
	return m_oLayoutMap;
}

//straight above/below or to the left/right; not relative to a direction
inline bool CalculatorFOV::IsOnStraightLine(const long long nOriginX, 
											const long long nOriginY,
											const long long nCheckableX, 
											const long long nCheckableY) const {
	return nOriginX == nCheckableX || nOriginY == nCheckableY;
}

inline size_t CalculatorFOV::GetDistanceFOV(const enumDirection eMain, 
											const size_t unStraightDistance) const {
	return IsDirectionDiagonal(eMain) ? CalculateAngledDistanceFOV(unStraightDistance, 
																   1, 1) :
		   unStraightDistance;
}

#endif