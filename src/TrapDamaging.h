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

#ifndef NOTAGAME_TRAP_DAMAGING_H
#define NOTAGAME_TRAP_DAMAGING_H

#include "UnitMap.h"
#include "TrapGeneric.h"

#include <string>
#include <random> //for std::default_random_engine

class TrapDamaging: public TrapGeneric {
public:
    TrapDamaging(const std::string&, size_t, size_t, enumMaterial, size_t, 
				 float, const UnitMap&);
    short Effect(sptr_StandingGeneric, std::default_random_engine&) override;
	std::string EffectMessage(csptr_StandingGeneric, short) const override;
private:
	const UnitMap& GetUnitMap() const;

    const UnitMap& m_oUnitMap;
};

inline const UnitMap& TrapDamaging::GetUnitMap() const {
	return m_oUnitMap;
}

#endif