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

#ifndef NOTAGAME_CONSUMABLE_WAND_H
#define NOTAGAME_CONSUMABLE_WAND_H

#include "HumanoidGeneric.h"
#include "LayoutMap.h"
#include "ConsumableGeneric.h"

#include <string>
#include <random> //for std::default_random_engine
#include <stdexcept>

class ConsumableWand: public ConsumableGeneric {
public:
    ConsumableWand(const std::string&, size_t, size_t, enumMaterial, 
				   const std::string&, size_t, float, LayoutMap&);
    short Effect(HumanoidGeneric&, std::default_random_engine&) override;
	std::string EffectMessage(csptr_HumanoidGeneric, short) const override;
private:
	const LayoutMap& GetLayoutMap() const;
	LayoutMap& GetLayoutMap();

    LayoutMap& m_oLayoutMap;
};

inline const LayoutMap& ConsumableWand::GetLayoutMap() const {
	return m_oLayoutMap;
}

inline LayoutMap& ConsumableWand::GetLayoutMap() {
	return m_oLayoutMap;
}

#endif