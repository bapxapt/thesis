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

#include "FieldObjectVector2D.h"

template class FieldObjectVector2D<LayoutGeneric>;

template<class T>
FieldObjectVector2D<T>::FieldObjectVector2D(const size_t unWidth, 
											const size_t unHeight):
	m_vvpoObjects(unHeight, std::vector<std::shared_ptr<T>>(unWidth, nullptr))
{}

template<class T>
void FieldObjectVector2D<T>::Draw(const Camera2D& oCamera) const {
	for (size_t unY(oCamera.GetTopLeftExistingY()); 
		 unY <= oCamera.GetBottomRightExistingY(); ++unY)
		for (size_t unX(oCamera.GetTopLeftExistingX()); 
			 unX <= oCamera.GetBottomRightExistingX(); ++unX) {
			const std::shared_ptr<const T> poObject(Get(unX, unY));

			if (poObject->IsDiscovered())
				poObject->Draw(oCamera.GetOffsetX(), 
							   oCamera.GetOffsetY());
		}
}

template<class T>
void FieldObjectVector2D<T>::Set(const std::shared_ptr<T> poNew,
								 const bool bUseTexture) {
	ReuseOrLoadSprite(poNew, bUseTexture);
	GetAll().at(poNew->GetY()).at(poNew->GetX()) = poNew;
}

template<class T>
void FieldObjectVector2D<T>::Clear() {
	std::vector<std::vector<std::shared_ptr<T>>>& vvpoObjects(GetAll());

	std::fill(vvpoObjects.begin(), vvpoObjects.end(),
			  std::vector<std::shared_ptr<T>>(GetWidth(), nullptr));
}