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

#ifndef NOTAGAME_CONSUMABLE_GENERIC_H
#define NOTAGAME_CONSUMABLE_GENERIC_H

#include "ItemGeneric.h"

#include <string>
#include <random>//for std::default_random_engine

class LevelMap; //a forward declaration

class ConsumableGeneric: public ItemGeneric {
public:
    ConsumableGeneric(const std::string&, size_t, size_t, enumMaterial, 
					  const std::string&, size_t, float, size_t = 1);
    virtual short Effect(HumanoidGeneric&, std::default_random_engine&) = 0;
	virtual std::string EffectMessage(csptr_HumanoidGeneric, short) const = 0;
private:
    //;
};

#endif