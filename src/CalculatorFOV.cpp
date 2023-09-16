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

#include "CalculatorFOV.h"

CalculatorFOV::CalculatorFOV(const LayoutMap& oLayoutMap):
    m_oLayoutMap(oLayoutMap)
{}

//on any 45-degree diagonal; not relative;
bool CalculatorFOV::IsOnDiagonal(const size_t unOriginX, 
								 const size_t unOriginY,
								 const size_t unCheckableX, 
								 const size_t unCheckableY) const {
	const size_t unDeltaX(unCheckableX > unOriginX ? unCheckableX - unOriginX : 
						  unOriginX - unCheckableX); //absolute
	const size_t unDeltaY(unCheckableY > unOriginY ? unCheckableY - unOriginY : 
						  unOriginY - unCheckableY); //absolute

	return unDeltaX == unDeltaY;
}

//tiles in the inner part of a sector "diagonally accessible" from a direction line 
void CalculatorFOV::DiagonallyAccessibleTiles(const csptr_UnitGeneric poUnit,
											  uset_pair_size_t& setFOVXY,
											  const enumDirection eMain,
											  const enumDirection eAdjacent) const {
	const size_t unUnitX(poUnit->GetX());
	const size_t unUnitY(poUnit->GetY());
	const size_t unDistanceFOV(GetDistanceFOV(eMain, 
											  poUnit->GetBaseDistanceFOV()));
	const LayoutMap& oLayoutMap(GetLayoutMap());
	const short nStepMainX(GetStepX(eMain));
	const short nStepMainY(GetStepY(eMain));
	const short nStepAdjacentX(GetStepX(eAdjacent));
	const short nStepAdjacentY(GetStepY(eAdjacent));
	const size_t unLinePartLengthMax((unDistanceFOV + 1) / 2);

	for (size_t iii(1); iii != unDistanceFOV; ++iii) { //iii(1): the unit is "at" iii=0
		const size_t unX(unUnitX + iii * nStepMainX); //move along the main direction
		const size_t unY(unUnitY + iii * nStepMainY);

		if (oLayoutMap.Get(unX, unY)->IsOpaque())
			break; //met an opaque tile, no need to check further
		if (iii >= unLinePartLengthMax) //not on the main direction line
			setFOVXY.emplace(std::make_pair(unX + nStepAdjacentX, 
											unY + nStepAdjacentY)); //"look" into inner part
	}
}

//does not clear any parts of the FOV
void CalculatorFOV::AddLineAngled(uset_pair_size_t& setFOVXY,
								  const uset_pair_long_long& setLimitXY,
								  const size_t unUnitX, const size_t unUnitY,
								  const enumDirection eMain,
								  const enumDirection eAdjacent,
								  const size_t unPartLength) const {
	const LayoutMap& oLayoutMap(GetLayoutMap());
	const short nStepStraightX(GetStepX(eMain));
	const short nStepStraightY(GetStepY(eMain));
	const short nStepDiagonalX(GetStepX(eAdjacent));
	const short nStepDiagonalY(GetStepY(eAdjacent));

	for (size_t unFirstPartLength(1); unFirstPartLength <= unPartLength; 
		 ++unFirstPartLength) { //length of the first part varies from 1 to unPartLength
		size_t unPartX(unUnitX); //start from the unit's coordinates
		size_t unPartY(unUnitY);
		bool bOpaqueInFirstPart(false);

		if (unFirstPartLength > 1) //if ==1, we're at the unit's coordinates
			for (size_t iii(0); iii < unFirstPartLength - 1; ++iii) { //the 1st tile (unit) is already checked, so -1
				unPartX += nStepStraightX;
				unPartY += nStepStraightY;
				if (oLayoutMap.Get(unPartX, unPartY)->IsOpaque()) {
					bOpaqueInFirstPart = true;
					break;
				}
			}
		if (bOpaqueInFirstPart) //no way to step away from the straight line
			continue;

		bool bDone(false);

		unPartX += nStepDiagonalX; //move diagonally to the beginning of a next part
		unPartY += nStepDiagonalY;
		while (!bDone) { //begin making full line parts
			for (size_t iii(0); iii < unPartLength; ++iii) {
				if (!IsOnDiagonal(unUnitX, unUnitY, unPartX, unPartY) &&
					!IsOnStraightLine(unUnitX, unUnitY, unPartX, unPartY))
					setFOVXY.emplace(std::make_pair(unPartX, unPartY)); //add tile
				if (oLayoutMap.Get(unPartX, unPartY)->IsOpaque()) {
					bDone = true;
					break;
				}
				if (iii == unPartLength - 1) { //last tile of the current part
					unPartX += nStepDiagonalX; //move diagonally to the beginning of a next part
					unPartY += nStepDiagonalY;
				}
				else { //keep moving to the end of the current part
					unPartX += nStepStraightX;
					unPartY += nStepStraightY;
				}
				if (setLimitXY.count(std::make_pair(unPartX, unPartY))) {
					bDone = true;
					break;
				}
			}
		}
	}
}

//maximal length of a non-straight (angled) FOV line, including a diagonal one
size_t CalculatorFOV::CalculateAngledDistanceFOV(const size_t unDistanceStraightFOV,
												 const size_t unAngleSlices,
												 const size_t unAngleSlicesTotal) const {
	if (!unAngleSlices || !unAngleSlicesTotal || 
		unAngleSlices > unAngleSlicesTotal)
		throw std::logic_error("CalculatorFOV::GetAngledDistanceFOV(): invalid "
							   "number of angle parts.");

	const float fAngle(PI / 4); //45.0 degrees * PI / 180.0
	const float fDistanceDiagonalFOV(unDistanceStraightFOV * cos(fAngle));
	const size_t unDistanceDiagonalFOV(RoundDecimal(fDistanceDiagonalFOV));

	if (unAngleSlices == unAngleSlicesTotal)
		return unDistanceDiagonalFOV;

	const float fAngleSlice(fAngle / unDistanceDiagonalFOV);
	const float fAngleCurrent(fAngleSlice * unAngleSlices);
	
	return RoundDecimal(unDistanceStraightFOV * cos(fAngleCurrent));
}

void CalculatorFOV::FindTilesOutOfSector(const csptr_UnitGeneric poUnit,
										 uset_pair_long_long& setXY, 
										 const enumDirection eOrthogonal, 
										 const enumDirection eDiagonal) const {
	const long long nOriginX(poUnit->GetX());
	const long long nOriginY(poUnit->GetY());
	const short nStepOrthogonalX(GetStepX(eOrthogonal)); //X or Y is 0
	const short nStepOrthogonalY(GetStepY(eOrthogonal));
	const short nTurns(CountTurnsToFaceDirection(eOrthogonal, eDiagonal));
	enumDirection ePerpendicular(GetDirectionTurned(eDiagonal, nTurns));
	const short nStepPerpendicularX(GetStepX(ePerpendicular)); //X or Y is 0
	const short nStepPerpendicularY(GetStepY(ePerpendicular));
	const size_t unDistanceOrthogonalFOV(poUnit->GetBaseDistanceFOV());
	const size_t unDistanceDiagonalFOV(CalculateAngledDistanceFOV(unDistanceOrthogonalFOV,
																  1, 1));
	long long nDeltaOrthogonalPrevious(0);

	for (size_t unDeltaDiagonal(1); unDeltaDiagonal <= unDistanceDiagonalFOV; 
		 ++unDeltaDiagonal) { //from 1: at 0 DeltaStraight==FOV_DISTANCE_STRAIGHT
		const long long nDeltaOrthogonal(CalculateAngledDistanceFOV(unDistanceOrthogonalFOV,
																	unDeltaDiagonal,
																	unDistanceDiagonalFOV));
		const long long nOriginShiftedX(nOriginX + 
										unDeltaDiagonal * nStepPerpendicularX);
		const long long nOriginShiftedY(nOriginY + 
										unDeltaDiagonal * nStepPerpendicularY);
		const long long nDeltaX((nDeltaOrthogonal + 1) * nStepOrthogonalX);
		const long long nDeltaY((nDeltaOrthogonal + 1) * nStepOrthogonalY);
		const long long nBeyondLastOrthogonalX(nOriginShiftedX + nDeltaX);
		const long long nBeyondLastOrthogonalY(nOriginShiftedY + nDeltaY);

		setXY.emplace(std::make_pair(nBeyondLastOrthogonalX, 
									 nBeyondLastOrthogonalY));
		if (nDeltaOrthogonalPrevious && 
			nDeltaOrthogonal != nDeltaOrthogonalPrevious) {
			const long long nExtraTileX(nBeyondLastOrthogonalX + 
										nStepOrthogonalX);
			const long long nExtraTileY(nBeyondLastOrthogonalY + 
										nStepOrthogonalY);

			setXY.emplace(std::make_pair(nExtraTileX, nExtraTileY));
		}
		nDeltaOrthogonalPrevious = nDeltaOrthogonal;
	}
}

//to the right of a diagonal dir. == to the left of the right adjacent straight
std::pair<enumDirection, enumDirection> CalculatorFOV::DetermineDirections(const enumDirection eMain,
																		   const bool bToRight) const {
	const enumDirection eAdjacent(GetDirectionTurned(eMain, 
													 bToRight ? 1 : -1));

	return IsDirectionDiagonal(eMain) ? std::make_pair(eAdjacent, eMain) :
		   std::make_pair(eMain, eAdjacent); //straight, diagonal
}

void CalculatorFOV::SectorDegrees45Half(const csptr_UnitGeneric poUnit,
										uset_pair_size_t& setFOVXY,
										const uset_pair_long_long& setOutOfFOVXY,
										const enumDirection eMain,
										const enumDirection eAdjacent,
										const size_t unPartLengthInitial) const {
	const size_t unUnitX(poUnit->GetX()); 
	const size_t unUnitY(poUnit->GetY());
	const size_t unDistanceFOV(GetDistanceFOV(eMain, 
											  poUnit->GetBaseDistanceFOV()));
	const size_t unLinePartLengthMax(RoundDecimal((unDistanceFOV + 1.0) / 
												  2));

	for (size_t unPartLength(unPartLengthInitial);
		 unPartLength <= unLinePartLengthMax; ++unPartLength)
		AddLineAngled(setFOVXY, setOutOfFOVXY, unUnitX, unUnitY, eMain, 
					  eAdjacent, unPartLength);
}

//adds a 45-degree sector between a diagonal dir. and a straight dir.
void CalculatorFOV::AddSectorDegrees45(const sptr_UnitGeneric poUnit, 
									   const enumDirection eMain,
									   const bool bToRight) const { 
	const pair_enumDirection Directions(DetermineDirections(eMain, bToRight));
	const enumDirection eOrthogonal(Directions.first);
	const enumDirection eDiagonal(Directions.second);
	const short nTurns(CountTurnsToFaceDirection(poUnit->GetDirection(),
												 eMain));
	const short nSubSetIndexFOV(bToRight ? nTurns * 2 + 1 : nTurns * 2 - 1);
	uset_pair_size_t setFOVXY;
	uset_pair_long_long setOutOfFOVXY;

	FindTilesOutOfSector(poUnit, setOutOfFOVXY, eOrthogonal, eDiagonal);
	DiagonallyAccessibleTiles(poUnit, setFOVXY, eOrthogonal, eDiagonal);
	DiagonallyAccessibleTiles(poUnit, setFOVXY, eDiagonal, eOrthogonal);
	SectorDegrees45Half(poUnit, setFOVXY, setOutOfFOVXY, eDiagonal, eOrthogonal, 
						2);
	SectorDegrees45Half(poUnit, setFOVXY, setOutOfFOVXY, eOrthogonal, eDiagonal, 
						3);
	poUnit->ClearPartFOV(nSubSetIndexFOV);
	for (const pair_size_t& InFOVXY : setFOVXY)
		poUnit->AddToFOV(nSubSetIndexFOV, InFOVXY);
}

//also clears the
void CalculatorFOV::AddLineStraight(const sptr_UnitGeneric poUnit, 
									const enumDirection eLineDirection,
									const bool bIncludeUnit) const {
	const LayoutMap& oLayoutMap(GetLayoutMap());
	const size_t unUnitX(poUnit->GetX());
	const size_t unUnitY(poUnit->GetY());
	const short nStepLineX(GetStepX(eLineDirection));
	const short nStepLineY(GetStepY(eLineDirection));
	const short nTurns(CountTurnsToFaceDirection(poUnit->GetDirection(), 
												 eLineDirection));
	const short nIndex(nTurns * 2); //lines are at -2, 0, 2, sectors are at -1, -1
	const size_t unBaseDistanceFOV(poUnit->GetBaseDistanceFOV());
	size_t unDistanceFOV(GetDistanceFOV(eLineDirection, unBaseDistanceFOV)); 

	poUnit->ClearPartFOV(nIndex);
	for (size_t iii(bIncludeUnit ? 0 : 1); iii <= unDistanceFOV; ++iii) {
		const size_t unX(unUnitX + iii * nStepLineX);
		const size_t unY(unUnitY + iii * nStepLineY);

		poUnit->AddToFOV(nIndex, unX, unY);
		if (oLayoutMap.Get(unX, unY)->IsOpaque())
			return;
	}
}

//calculates a new FOV from scratch
void CalculatorFOV::Calculate(const sptr_UnitGeneric poUnit) const {
	const LayoutMap& oLayoutMap(GetLayoutMap());
	const size_t unUnitX(poUnit->GetX());
	const size_t unUnitY(poUnit->GetY());

	if (oLayoutMap.Get(unUnitX, unUnitY)->IsOpaque()) //unit is in an opaque tile
		return;

	const enumDirection eMain(poUnit->GetDirection());

	AddLineStraight(poUnit, GetDirectionTurned(eMain, -1));
	AddSectorDegrees45(poUnit, eMain, true);
	AddLineStraight(poUnit, eMain, true); //in the unit's direction
	AddSectorDegrees45(poUnit, eMain, false);
	AddLineStraight(poUnit, GetDirectionTurned(eMain, 1));
}

//calculates FOV on unit turn
void CalculatorFOV::Calculate(const sptr_UnitGeneric poUnit,
							  const enumDirection eOld) const {
	const enumDirection eCurrent(poUnit->GetDirection());
	const long long nTurns(CountTurnsToFaceDirection(eOld, eCurrent));
	const size_t unTurnsAbsolute(abs(nTurns));

	if (eCurrent == eOld)
		throw std::logic_error("CalculatorFOV::Calculate(): "
							   "a same direction.");
	if (unTurnsAbsolute > 2) { 
		Calculate(poUnit); //recalculate a new FOV from scratch
		return;
	}

	std::unordered_map<short, uset_pair_size_t>& mFOV(poUnit->GetFOV());
	const enumDirection eMain(poUnit->GetDirection());
	const bool bRight(nTurns > 0);

	if (unTurnsAbsolute == 2) { //new leftmost = old rightmost if turning ->
		mFOV.at(-nTurns) = mFOV.at(nTurns); 
		AddSectorDegrees45(poUnit, eMain, false);
		AddLineStraight(poUnit, eMain, true); //in the unit's direction
		AddSectorDegrees45(poUnit, eMain, true);
		AddLineStraight(poUnit, GetDirectionTurned(eMain, bRight ? 1 : -1));
		return;
	} //implies unTurnsAbsolute==1; will leave more than half of FOV untouched

	const pair_size_t& UnitXY(poUnit->GetCoordinates());
	uset_pair_size_t& setPreviousEdgeLine(mFOV.at(nTurns * 2));
	uset_pair_size_t& setPreviousMiddleLine(mFOV.at(0));

	setPreviousMiddleLine.erase(UnitXY); //will no longer be the middle line
	setPreviousEdgeLine.insert(UnitXY); //will be the new middle line
	mFOV.at(-nTurns) = mFOV.at(nTurns);
	mFOV.at(-nTurns * 2) = setPreviousMiddleLine;
	setPreviousMiddleLine = setPreviousEdgeLine;
	AddSectorDegrees45(poUnit, eMain, bRight); //a new sector
	AddLineStraight(poUnit, GetDirectionTurned(eMain, nTurns));
}