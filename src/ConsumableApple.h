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

#ifndef NOTAGAME_CONSUMABLE_APPLE_H
#define NOTAGAME_CONSUMABLE_APPLE_H

#include "ConsumableGeneric.h"
#include "HumanoidGeneric.h"

#include <string>
#include <utility> //for std::pair
#include <random> //for std::default_random_engine
#include <stdexcept>

class ConsumableApple: public ConsumableGeneric {
public:
    ConsumableApple(size_t, size_t);
    short Effect(HumanoidGeneric&, std::default_random_engine&) override;
	std::string EffectMessage(csptr_HumanoidGeneric, short) const override;
private:
    //;
};

#endif