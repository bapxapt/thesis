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

#include "Alignable.h"

Alignable::Alignable(const int nTopLeftX, const int nTopLeftY, 
					 const size_t unWidth, const size_t unHeight, 
					 const enumHorizontal eHorizontal,
					 const enumVertical eVertical, const int nShiftX, 
					 const int nShiftY):
    m_ContainerTopLeftXY(std::make_pair(nTopLeftX, nTopLeftY)),
	m_ContainerDimensions(std::make_pair(unWidth, unHeight)),
	m_ShiftXY(std::make_pair(nShiftX, nShiftY)),
	m_eAlignmentHorizontal(eHorizontal),
	m_eAlignmentVertical(eVertical),
	m_WindowXY(std::make_pair(0, 0))
{}

int Alignable::CalculateWindowX(const size_t unElementWidth) {
	const pair_int& TopLeftXY(GetContainerTopLeftCoordinates());
	const pair_int& DimensionsWH(GetContainerDimensions());
	const pair_int& ShiftXY(GetShift());

	switch (GetAlignmentHorizontal()) {
		case H_LEFT:
			return TopLeftXY.first + ShiftXY.first;
		case H_CENTER:
			return TopLeftXY.first + DimensionsWH.first * 0.5 - 
				   unElementWidth * 0.5 + ShiftXY.first;
		case H_RIGHT:
			return TopLeftXY.first + DimensionsWH.first - 
				   unElementWidth + ShiftXY.first;
	}
	throw std::runtime_error("Alignable::CalculateWindowX(): "
							 "unexpected alignment.");
}

int Alignable::CalculateWindowY(const size_t unElementHeight) {
	const pair_int& TopLeftXY(GetContainerTopLeftCoordinates());
	const pair_int& DimensionsWH(GetContainerDimensions());
	const pair_int& ShiftXY(GetShift());
	const size_t unLineHeight(11);

	switch (GetAlignmentVertical()) {
		case V_TOP:
			return TopLeftXY.second + ShiftXY.second + unLineHeight;
		case V_CENTER:
			return TopLeftXY.second + DimensionsWH.second * 0.5 - 
				   unElementHeight * 0.5 + ShiftXY.second + 
				   unLineHeight;
		case V_BOTTOM:
			return TopLeftXY.second + DimensionsWH.second - 
				   unElementHeight + ShiftXY.second + unLineHeight;
	}
	throw std::runtime_error("Alignable::CalculateWindowY(): "
							 "unexpected alignment.");
}