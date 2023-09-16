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

#ifndef NOTAGAME_FLOOR_GENERIC_H
#define NOTAGAME_FLOOR_GENERIC_H

#include "LayoutGeneric.h"
#include "define.h"
#include "utility.h"

#include "ofImage.h"

#include <memory> //for std::shared_ptr
#include <string>

class FloorGeneric: public LayoutGeneric {
public:
	FloorGeneric(size_t, size_t, float = 1, enumMaterial = MAT_STONE);
private:
	//
};

#endif