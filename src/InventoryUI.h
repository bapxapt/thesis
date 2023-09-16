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

#ifndef NOTAGAME_INVENTORY_UI_H
#define NOTAGAME_INVENTORY_UI_H

#include "GenericUI.h"
#include "TextLabel.h"
#include "ButtonCluster.h"
#include "HumanoidGeneric.h"
#include "define.h"

#include "ofAppRunner.h" //for ofGetWindowWidth()
#include "ofImage.h"
#include "ofUtils.h" //for ofToString()

#include <vector>
#include <stdexcept>

class InventoryUI: public GenericUI {
	typedef std::pair<TextLabel, TextLabel> pair_TextLabel;
public:
    InventoryUI();
	void Draw() const;
    void DrawItems() const;
	void Initialise();
	void CalculateCoordinatesBackpack(size_t = 0);
	void CalculateCoordinatesEquipment(size_t);
	short PollSlotsEquipment(int, int) const;
    short PollSlotsBackpack(int, int) const;
    bool HoverSlotBackpack(int, int);
    bool HoverSlotEquipment(int, int);
	void EnableSlotBackpack(size_t, bool);
	void EnableSlotEquipment(size_t, bool);
	void SetPlayer(sptr_HumanoidGeneric);
	void UpdateLabelTextItemName(size_t);
	void UpdateLabelTextItemQuantity(size_t);
	bool IsSlotBackpackEnabled(size_t) const;
private:
	csptr_HumanoidGeneric GetPlayer() const;
	sptr_HumanoidGeneric GetPlayer();
	const ButtonCluster& GetButtonClusterBackpack() const;
	ButtonCluster& GetButtonClusterBackpack();
	const ButtonCluster& GetButtonClusterEquipment() const;
	ButtonCluster& GetButtonClusterEquipment();
	const ofColor& GetColourBackground() const;
	const std::vector<pair_TextLabel>& GetButtonTextLabels() const;
	std::vector<pair_TextLabel>& GetButtonTextLabels();

	sptr_HumanoidGeneric m_poPlayer;
    const ofColor m_oColourBackground;
	ButtonCluster m_oButtonClusterBackpack;
	ButtonCluster m_oButtonClusterEquipment;
	std::vector<pair_TextLabel> m_voBackpackButtonLabels;
};

inline csptr_HumanoidGeneric InventoryUI::GetPlayer() const {
	return m_poPlayer;
}

inline sptr_HumanoidGeneric InventoryUI::GetPlayer() {
	return m_poPlayer;
}

inline void InventoryUI::SetPlayer(const sptr_HumanoidGeneric poPlayer) {
	m_poPlayer = poPlayer;
}

inline bool InventoryUI::IsSlotBackpackEnabled(const size_t unIndex) const {
	return GetButtonClusterBackpack().IsButtonEnabled(unIndex);
}

inline const ButtonCluster& InventoryUI::GetButtonClusterBackpack() const {
	return m_oButtonClusterBackpack;
}

inline ButtonCluster& InventoryUI::GetButtonClusterBackpack() {
	return m_oButtonClusterBackpack;
}

inline const ButtonCluster& InventoryUI::GetButtonClusterEquipment() const {
	return m_oButtonClusterEquipment;
}

inline ButtonCluster& InventoryUI::GetButtonClusterEquipment() {
	return m_oButtonClusterEquipment;
}

inline const ofColor& InventoryUI::GetColourBackground() const {
	return m_oColourBackground;
}

inline short InventoryUI::PollSlotsEquipment(const int nX,
											 const int nY) const {
	return GetButtonClusterEquipment().Poll(nX, nY);
}

inline short InventoryUI::PollSlotsBackpack(const int nX, const int nY) const {
	return GetButtonClusterBackpack().Poll(nX, nY);
}

inline bool InventoryUI::HoverSlotBackpack(const int nX, const int nY) {
	return GetButtonClusterBackpack().Hover(nX, nY);
}

inline bool InventoryUI::HoverSlotEquipment(const int nX, const int nY) {
	return GetButtonClusterEquipment().Hover(nX, nY);
}

inline void InventoryUI::EnableSlotBackpack(const size_t unIndex, 
											const bool bEnable) {
	GetButtonClusterBackpack().SetButtonEnabled(unIndex, bEnable);
}

inline void InventoryUI::EnableSlotEquipment(const size_t unIndex, 
											 const bool bEnable) {
	GetButtonClusterEquipment().SetButtonEnabled(unIndex, bEnable);
}

inline const std::vector<std::pair<TextLabel, TextLabel>>& InventoryUI::GetButtonTextLabels() const {
	return m_voBackpackButtonLabels;
}

inline std::vector<std::pair<TextLabel, TextLabel>>& InventoryUI::GetButtonTextLabels() {
	return m_voBackpackButtonLabels;
}

#endif