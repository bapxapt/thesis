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

#include "NameableGeneric.h"

NameableGeneric::NameableGeneric(const std::string& strFileName,
								 const size_t unX, const size_t unY, 
								 const enumMaterial eMaterial, 
								 const std::string& strName,
								 const size_t unMaxHP, const float fWeight):
	FieldObjectGeneric(strFileName, unX, unY, 0, eMaterial),
	m_strName(strName),
	m_HP(std::make_pair(unMaxHP, unMaxHP)),
	m_PerceivedHP(std::make_pair(0, 0)),
    m_fWeight(fWeight)
{
	if (!unMaxHP)
		throw std::logic_error("NameableGeneric::NameableGeneric(): maximal "
							   "HP cannot be zero.");
}

void NameableGeneric::Update() {
	FieldObjectGeneric::Update();
	UpdatePerceivedHP();
}