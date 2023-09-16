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

#include "LyingGeneric.h"

LyingGeneric::LyingGeneric(const std::string& strFileName, const size_t unX, 
						   const size_t unY, const enumMaterial eMaterial, 
						   const std::string& strName, const size_t unMaxHP, 
						   const float fWeight, const bool bPickable):
    NameableGeneric(strFileName, unX, unY, eMaterial, strName, unMaxHP, 
					fWeight),
	m_bPickable(bPickable)
{}