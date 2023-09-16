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

#ifndef NOTAGAME_ALIGNABLE_H
#define NOTAGAME_ALIGNABLE_H

#include "define.h"

class Alignable {
public:
	enum enumHorizontal: short {H_LEFT = 0, H_CENTER, H_RIGHT};
	enum enumVertical: short {V_TOP = 0, V_CENTER, V_BOTTOM};
    Alignable(int, int, size_t, size_t, enumHorizontal, enumVertical, int, 
			  int);
	const pair_int& GetContainerTopLeftCoordinates() const;
	void SetContainerTopLeftCoordinates(int, int);
	int GetContainerTopLeftX() const;
	int GetContainerTopLeftY() const;
	const pair_size_t& GetContainerDimensions() const;
	size_t GetContainerWidth();
	size_t GetContainerHeight();
	void SetContainerDimensions(size_t, size_t);
	const pair_int& GetShift() const;
	void SetShift(int, int);
	enumHorizontal GetAlignmentHorizontal() const;
	void SetAlignmentHorizontal(enumHorizontal);
	enumVertical GetAlignmentVertical() const;
	void SetAlignmentVertical(enumVertical);
	const pair_int& GetWindowCoordinates() const;
	const int GetWindowX() const;
	const int GetWindowY() const;
protected:
	void UpdateWindowCoordinates(size_t, size_t);
private:
	int CalculateWindowX(size_t);
	int CalculateWindowY(size_t);

	pair_int m_ContainerTopLeftXY;
	pair_size_t m_ContainerDimensions;
	pair_int m_ShiftXY; //after alignment
	enumHorizontal m_eAlignmentHorizontal;
	enumVertical m_eAlignmentVertical;
	pair_int m_WindowXY;
};

inline const std::pair<int, int>& Alignable::GetWindowCoordinates() const {
	return m_WindowXY;
}

inline const int Alignable::GetWindowX() const {
	return m_WindowXY.first;
}

inline const int Alignable::GetWindowY() const {
	return m_WindowXY.second;
}

inline void Alignable::UpdateWindowCoordinates(const size_t unElementWidth, 
											   const size_t unElementHeight) {
	m_WindowXY = std::make_pair(CalculateWindowX(unElementWidth), 
								CalculateWindowY(unElementHeight));
}

inline const pair_int& Alignable::GetContainerTopLeftCoordinates() const {
	return m_ContainerTopLeftXY;
}

inline int Alignable::GetContainerTopLeftX() const {
	return m_ContainerTopLeftXY.first;
}

inline int Alignable::GetContainerTopLeftY() const {
	return m_ContainerTopLeftXY.second;
}

inline void Alignable::SetContainerTopLeftCoordinates(const int nTopLeftX, 
													  const int nTopLeftY) {
	m_ContainerTopLeftXY = std::make_pair(nTopLeftX, nTopLeftY);
}

inline const pair_size_t& Alignable::GetContainerDimensions() const {
	return m_ContainerDimensions;
}

inline size_t Alignable::GetContainerWidth() {
	return m_ContainerDimensions.first;
}

inline size_t Alignable::GetContainerHeight() {
	return m_ContainerDimensions.second;
}

inline void Alignable::SetContainerDimensions(const size_t unWidth, 
											  const size_t unHeight) {
	m_ContainerDimensions = std::make_pair(unWidth, unHeight);
}

inline const pair_int& Alignable::GetShift() const {
	return m_ShiftXY;
}

inline void Alignable::SetShift(const int nShiftX, const int nShiftY) {
	m_ShiftXY = std::make_pair(nShiftX, nShiftY);
}

inline Alignable::enumHorizontal Alignable::GetAlignmentHorizontal() const {
	return m_eAlignmentHorizontal;
}

inline void Alignable::SetAlignmentHorizontal(const enumHorizontal eAlignment) {
	m_eAlignmentHorizontal = eAlignment;
}

inline Alignable::enumVertical Alignable::GetAlignmentVertical() const {
	return m_eAlignmentVertical;
}

inline void Alignable::SetAlignmentVertical(const enumVertical eAlignment) {
	m_eAlignmentVertical = eAlignment;
}

#endif