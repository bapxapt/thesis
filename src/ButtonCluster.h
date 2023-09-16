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

#ifndef NOTAGAME_BUTTON_CLUSTER_H
#define NOTAGAME_BUTTON_CLUSTER_H

#include "Alignable.h"
#include "DrawableGeneric.h"

#include "ofAppRunner.h" //for ofGetWindowWidth()
#include "ofImage.h"

#include <memory> //for std::shared_ptr
#include <vector>
#include <stdexcept>

class ButtonCluster: public Alignable {
public:
    ButtonCluster(int, int, const std::string&, size_t, size_t, size_t = 10);
	void Draw() const;
	const DrawableGeneric& Get(size_t) const;
	DrawableGeneric& Get(size_t);
	template<typename F> void Initialise(const F&);
	short Poll(int, int) const;
    bool Hover(int, int);
	bool IsButtonEnabled(size_t) const;
	void SetButtonEnabled(size_t, bool);
	size_t Count() const;
	std::shared_ptr<const ofImage> GetSprite() const;
	size_t GetWidth() const;
	size_t GetHeight() const;
	int GetHoveredIndex() const;
	void ResetHoveredIndex();
private:
	void SetHoveredIndex(int);
	std::shared_ptr<ofImage> GetSprite();
	const std::vector<DrawableGeneric>& GetButtons() const;
	std::vector<DrawableGeneric>& GetButtons();
	size_t CountRows() const;
	size_t CountColumns() const;
	std::shared_ptr<const ofColor> GetColourNormal() const;
	std::shared_ptr<const ofColor> GetColourDisabled() const;
	std::shared_ptr<const ofColor> GetColourHovered() const;
	size_t GetGap() const;
	void SetGap(size_t);

    const std::shared_ptr<ofImage> m_poSpriteButton;
    const std::shared_ptr<const ofColor> m_poColourNormal;
    const std::shared_ptr<const ofColor> m_poColourDisabled;
    const std::shared_ptr<const ofColor> m_poColourHovered;
    std::vector<DrawableGeneric> m_voButtons;
	int m_nHoveredIndex;
	size_t m_unGap;
	size_t m_unRows; //rows per column
};

//calculates top left window XY for a whole vector of DrawableGeneric objects
template<typename F>
void ButtonCluster::Initialise(const F& Lambda) {
	UpdateWindowCoordinates(GetWidth(), GetHeight());

	const pair_int& ClusterTopLeftXY(GetWindowCoordinates());
	const size_t unRows(CountRows());
	const size_t unColumns(CountColumns());
	const std::shared_ptr<const ofImage> poSprite(GetSprite());
	const float fSpriteWidth(poSprite->getWidth());
	const float fSpriteHeight(poSprite->getHeight());
	const size_t unGap(GetGap());

	for (size_t iii(0); iii < unColumns; ++iii) {
        for (size_t jjj(0); jjj < unRows; ++jjj) {
			const size_t unIndex(iii * unRows + jjj);
			DrawableGeneric& oSlot(Get(unIndex));
			const float fX(ClusterTopLeftXY.first + 
						   iii * (fSpriteWidth + unGap));
			const float fY(ClusterTopLeftXY.second + 
						   jjj * (fSpriteHeight + unGap));

			oSlot.SetWindowCoordinates(fX, fY);
			oSlot.SetColour(Lambda(unIndex) ? GetColourNormal() : 
							GetColourDisabled());
        }
	}
}

inline const DrawableGeneric& ButtonCluster::Get(const size_t unIndex) const {
	return GetButtons().at(unIndex);
}

inline DrawableGeneric& ButtonCluster::Get(const size_t unIndex) {
	return GetButtons().at(unIndex);
}

inline size_t ButtonCluster::Count() const {
	return GetButtons().size();
}

inline const std::vector<DrawableGeneric>& ButtonCluster::GetButtons() const {
	return m_voButtons;
}

inline std::vector<DrawableGeneric>& ButtonCluster::GetButtons() {
	return m_voButtons;
}

inline std::shared_ptr<const ofImage> ButtonCluster::GetSprite() const {
	return m_poSpriteButton;
}

inline std::shared_ptr<ofImage> ButtonCluster::GetSprite() {
	return m_poSpriteButton;
}

inline std::shared_ptr<const ofColor> ButtonCluster::GetColourNormal() const {
	return m_poColourNormal;
}

inline std::shared_ptr<const ofColor> ButtonCluster::GetColourDisabled() const {
	return m_poColourDisabled;
}

inline std::shared_ptr<const ofColor> ButtonCluster::GetColourHovered() const {
	return m_poColourHovered;
}

inline int ButtonCluster::GetHoveredIndex() const {
	return m_nHoveredIndex;
}

inline void ButtonCluster::ResetHoveredIndex() {
	m_nHoveredIndex = -1;
}

inline void ButtonCluster::SetHoveredIndex(const int nIndex) {
	m_nHoveredIndex = nIndex;
}

inline size_t ButtonCluster::GetWidth() const {
	return CountColumns() * GetSprite()->getWidth() + 
		   (CountColumns() - 1) * GetGap();
}

inline size_t ButtonCluster::GetHeight() const {
	return CountRows() * GetSprite()->getHeight() + 
		   (CountRows() - 1) * GetGap();
}

inline size_t ButtonCluster::GetGap() const {
	return m_unGap;
}

inline void ButtonCluster::SetGap(const size_t unGap) {
	m_unGap = unGap;
}

inline bool ButtonCluster::IsButtonEnabled(const size_t unIndex) const {
	return GetButtons().at(unIndex).GetColour() != GetColourDisabled();
}

inline size_t ButtonCluster::CountRows() const {
	return m_unRows;
}

inline size_t ButtonCluster::CountColumns() const {
	return std::ceil(Count() / CountRows());
}

#endif