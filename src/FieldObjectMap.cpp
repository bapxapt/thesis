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

#include "FieldObjectMap.h"

template class FieldObjectMap<StandingGeneric>;
template class FieldObjectMap<TrapGeneric>;

template<class T>
FieldObjectMap<T>::FieldObjectMap():
	m_mpoObjects()
{}

template<class T>
void FieldObjectMap<T>::Draw(const Camera2D& oCamera) const {
	const std::unordered_map<pair_size_t, std::shared_ptr<T>>& mpoObjects(GetAll());
	const size_t unVisibleTiles(oCamera.CountVisibleExistingTiles());

	if (unVisibleTiles < mpoObjects.size()) { //iterate over coordinates
		for (size_t unY(oCamera.GetTopLeftExistingY()); 
			 unY <= oCamera.GetBottomRightExistingY(); ++unY)
			for (size_t unX(oCamera.GetTopLeftExistingX()); 
				 unX <= oCamera.GetBottomRightExistingX(); ++unX) {
					const std::shared_ptr<const T> poObject(Get(unX, unY));

					if (poObject && poObject->IsDiscovered())
						poObject->Draw(oCamera.GetOffsetX(), 
									   oCamera.GetOffsetY());
			}
		return;
	} //faster to iterate over all objects
	for (const std::pair<pair_size_t, std::shared_ptr<T>>& KeyValue : mpoObjects) {
		const std::shared_ptr<const T> poObject(KeyValue.second);

		if (poObject->IsDiscovered() && 
			oCamera.IsInView(poObject->GetCoordinates()))
			poObject->Draw(oCamera.GetOffsetX(), oCamera.GetOffsetY());
	}
}

template<class T>
void FieldObjectMap<T>::Add(const std::shared_ptr<T> poNew, 
							const bool bUseTexture) {
	if (!GetAll().emplace(poNew->GetCoordinates(), poNew).second)
		throw std::runtime_error("FieldObjectMap<T>::Add(): coordinate key "
								 "already exists");
	ReuseOrLoadSprite(poNew, bUseTexture);
}

template<class T>
void FieldObjectMap<T>::UpdateCoordinateKey(const pair_size_t& OldXY) {
	const std::shared_ptr<T> poObject(Get(OldXY)); //has new XY in it

	GetAll().erase(OldXY); //erasing by the old XY, not by the new
	Add(poObject, true); //adding by the new XY
}