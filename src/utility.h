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

#ifndef NOTAGAME_UTILITY_H
#define NOTAGAME_UTILITY_H

#include "define.h"

#include <cstdlib> //for abs()
#include <string>
#include <random> //for std::uniform_int_distribution
#include <regex> //for std::regex
#include <stdexcept>

short GetStepX(enumDirection);
short GetStepY(enumDirection);
bool IsTurningClockwise(enumDirection, enumDirection);
enumDirection GetDirectionFromKeyPressed(int);
short CountTurnsToFaceDirection(enumDirection, enumDirection);
bool StartsWith(const std::string&, const std::string&, bool = true);
enumDirection GetDirectionRandom(std::default_random_engine&);
enumDirection GetDirectionTurned(enumDirection, long);
long RoundDecimal(float, size_t = 0);

template<typename T, typename U>
const T& GetRandomElement(const U& setWhole,
						  std::default_random_engine& oRNG) {
	const size_t unSize(setWhole.size());

	if (!unSize)
		throw std::runtime_error("GetRandomElement(): no elements.");

	std::uniform_int_distribution<size_t> oDistribution(0, unSize - 1);

	return *std::next(setWhole.cbegin(), oDistribution(oRNG));
}

inline bool IsDirectionDiagonal(const enumDirection eDirection) {
	return eDirection == DIR_NE || eDirection == DIR_SE ||
		   eDirection == DIR_SW || eDirection == DIR_NW;
}

inline enumDirection GetDirectionOpposite(const enumDirection eDirection) {
	return GetDirectionTurned(eDirection, 4);
}

inline short GetStepFromDistance(const int nDistance) {
	return !nDistance ? 0 : (nDistance > 0 ? 1 : -1);
}

//both negative or both non-negative; 0 is non-negative
template<typename T, typename U> inline bool SameSign(T TFirst, U USecond) {
	return TFirst > 0 && USecond > 0 || TFirst < 0 && USecond < 0 ||
		   !TFirst && !USecond;
}

//removes "1.png" from "chair1.png"
/*inline std::string RemoveLastDigitsAndExtension(const std::string& strFrom) {
	return std::regex_replace(strFrom, std::regex("\\d*\\.[^\\.]*$"), "");
}*/

inline std::string GetSpriteFullPath(const std::string& strFileName) {
	return (StartsWith(strFileName, "ui_") ? "ui/" : "graphics/") + 
		   strFileName;
}

inline bool TrueWithProbability(std::default_random_engine& oRNG,
								const float fProbability) {
	return std::bernoulli_distribution(fProbability)(oRNG);
}

inline bool IsBlank(const std::string& strCheckable) {
	return strCheckable.find_first_not_of(" \t\n\v\f\r") == std::string::npos;
}

inline std::string GetIndefiniteArticle(const std::string& strCheckable) {
	return strCheckable.find_first_of("aAeEiIoO") == 0 ? "an" : "a";
}

#endif