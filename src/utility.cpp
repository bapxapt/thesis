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

#include "utility.h"

short GetStepX(const enumDirection eDirection) {
    switch (eDirection) {
        case DIR_WEST: 
		case DIR_NW: 
		case DIR_SW: 
			return -1;
        case DIR_EAST: 
		case DIR_NE: 
		case DIR_SE: 
			return 1;
        default: 
			return 0;
    }
}

short GetStepY(const enumDirection eDirection) {
    switch (eDirection) {
        case DIR_NORTH: 
		case DIR_NW: 
		case DIR_NE: 
			return -1;
        case DIR_SOUTH: 
		case DIR_SW: 
		case DIR_SE: 
			return 1;
        default: 
			return 0;
    }
}

enumDirection GetDirectionFromKeyPressed(const int nKey) {
	switch (nKey) {
		case 56:
		case 57357:
			return DIR_NORTH;
		case 57:
			return DIR_NE;
		case 54:
		case 57358:
			return DIR_EAST;
		case 51:
			return DIR_SE;
		case 50:
		case 57359:
			return DIR_SOUTH;
		case 49:
			return DIR_SW;
		case 52:
		case 57356:
			return DIR_WEST;
		case 55:
			return DIR_NW;
		default:
			return DIR_NONE;
	}
}

//returns the number of turns to face a direction; positive is clockwise
short CountTurnsToFaceDirection(const enumDirection eCurrent, 
								const enumDirection eNew) {
	const short nDifference(eNew - eCurrent);

	if (abs(nDifference) > 4) //go in the opposite direction through 0
		return nDifference < 0 ? 8 + nDifference : nDifference - 8;
	return nDifference;
}

enumDirection GetDirectionTurned(const enumDirection eDirection, long nTurns) {
	nTurns = nTurns % 8; //8 turns is 360 degrees
	if (!nTurns)
		return eDirection;
	if (nTurns > 0) {
		const size_t unPossibleTurns(DIR_NW - eDirection);

		if (nTurns <= unPossibleTurns) //can turn right
			return static_cast<enumDirection>(eDirection + nTurns);
		return static_cast<enumDirection>(eDirection - (8 - nTurns)); //left
	}

	const size_t unPossibleTurns(eDirection - DIR_NORTH);

	if (abs(nTurns) <= unPossibleTurns) //can turn left
		return static_cast<enumDirection>(eDirection + nTurns);
	return static_cast<enumDirection>(eDirection + (8 + nTurns)); //right
}

bool IsTurningClockwise(const enumDirection eCurrent, 
						const enumDirection eNew) {
	const char nTurns(CountTurnsToFaceDirection(eCurrent, eNew));

	if (!nTurns)
		throw std::logic_error("IsTurningClockwise(): equal directions.");
	return nTurns > 0;
}

//without DIR_NONE
enumDirection GetDirectionRandom(std::default_random_engine& oRNG) { 
	std::uniform_int_distribution<unsigned short> oDistribution(0, 7);

	return static_cast<enumDirection>(oDistribution(oRNG));
}

long RoundDecimal(const float fRoundable, const size_t unDecimals) {
	const size_t unConstant(pow(10, unDecimals));

	return lroundf(fRoundable * unConstant) / unConstant;
}

bool StartsWith(const std::string& strCheckable, const std::string& strPrefix,
				const bool bCaseSensitive) {
	if (bCaseSensitive)
		return !strncmp(strCheckable.c_str(), strPrefix.c_str(), 
						strPrefix.size()); //strncmp() returns 0 if equal
	return !_strnicmp(strCheckable.c_str(), strPrefix.c_str(),
					  strPrefix.size()); //strnicmp() returns 0 if equal
}