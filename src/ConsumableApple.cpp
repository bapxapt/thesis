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

#include "ConsumableApple.h"

ConsumableApple::ConsumableApple(const size_t unX, const size_t unY):
	ConsumableGeneric("apple.png", unX, unY, MAT_FLESH, "Apple", 1, 0.5)
{}

short ConsumableApple::Effect(HumanoidGeneric& oUser,
							  std::default_random_engine& oRNG) {
	const pair_size_t& UnitHP(oUser.GetHP());
	const size_t unNewCurrentHP(UnitHP.first + 3);

	if (UnitHP.first == UnitHP.second) //full HP
		return -1;
	oUser.SetHPCurrent(unNewCurrentHP > UnitHP.second ? UnitHP.second :
					   unNewCurrentHP);
	return 0;
}

std::string ConsumableApple::EffectMessage(const csptr_HumanoidGeneric poUser,
										   const short nResult) const {
	switch (nResult) {
		case -1:
			return "You are already at full health.";
		case 0:
			return "You restored some health.";
		default:
			throw std::runtime_error("ConsumableApple::EffectMessage(): "
									 "an unexpected value.");
	}
}