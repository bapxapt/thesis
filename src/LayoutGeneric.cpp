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

#include "LayoutGeneric.h"

LayoutGeneric::LayoutGeneric(const std::string& strFileName,
							 const size_t unX, const size_t unY, 
							 const float fSourceOffsetX, 
							 const enumMaterial eMaterial, 
							 const bool bOpaque, const bool bPassable):
    FieldObjectGeneric(strFileName, unX, unY, fSourceOffsetX, eMaterial),
	m_bPassable(bPassable),
	m_bOpaque(bOpaque),
	m_bInFOV(false)
{}