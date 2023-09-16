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

#include "InventoryUI.h"

InventoryUI::InventoryUI():
	m_poPlayer(nullptr),
    m_oColourBackground(240, 220, 160),
	m_oButtonClusterBackpack(-200, 0, "ui_slot_backpack.png", SLOT_N_BACKPACK, 
							 10),
    m_oButtonClusterEquipment(250, 0, "ui_slot_equipment.png", 
							  SLOT_N_EQUIPMENT, 4),
	m_voBackpackButtonLabels(SLOT_N_BACKPACK,
							 std::make_pair(TextLabel(Alignable::H_LEFT, 
													  Alignable::V_CENTER, 
													  SPRITE_W, 0, "?", 
													  ofColor(0, 0, 0)),
											TextLabel(Alignable::H_RIGHT, 
													  Alignable::V_CENTER, 
													  -10, 0, "?", 
													  ofColor(0, 0, 0))))
{}

void InventoryUI::Initialise() {
	if (!IsInitialisationNeeded())
		return;

	const csptr_HumanoidGeneric poPlayer(GetPlayer());
	const size_t unBackpackItemCount(poPlayer->CountBackpackItems());
	const auto LambdaBackpack([unBackpackItemCount] (const size_t unIndex) -> bool {
								  return unIndex < unBackpackItemCount;
							  });
	const auto LambdaEquipment([poPlayer] (const size_t unIndex) -> bool {
								   return !!poPlayer->GetEquipped(unIndex); 
							   });
	ButtonCluster& oClusterBackpack(GetButtonClusterBackpack());
	ButtonCluster& oClusterEquipment(GetButtonClusterEquipment());
	const size_t unBackpackSlots(oClusterBackpack.Count());
	const size_t unEquipmentSlots(oClusterEquipment.Count());
	const pair_size_t& PlayerHP(poPlayer->GetHP());
	const pair_float& PlayerEncumbrance(poPlayer->GetEncumbrance());
	std::vector<pair_TextLabel>& voLabels(GetButtonTextLabels());
	const int nWindowWidth(ofGetWindowWidth());
	const int nWindowHeight(ofGetWindowHeight());

	oClusterBackpack.SetContainerDimensions(nWindowWidth, nWindowHeight);
	oClusterBackpack.Initialise(LambdaBackpack);
	oClusterEquipment.SetContainerDimensions(nWindowWidth, nWindowHeight);
	oClusterEquipment.Initialise(LambdaEquipment);
	for (size_t iii(0); iii < unEquipmentSlots; ++iii)
		CalculateCoordinatesEquipment(iii);
	for (size_t iii(0); iii < unBackpackSlots; ++iii) {
		const DrawableGeneric& oSlot(oClusterBackpack.Get(iii));
		const int nSlotWindowX(oSlot.GetWindowX());
		const int nSlotWindowY(oSlot.GetWindowY());
		const std::shared_ptr<const ofImage> poSprite(oSlot.GetSprite());
		const size_t unWidth(poSprite->getWidth());
		const size_t unHeight(poSprite->getHeight());
		pair_TextLabel& Labels(voLabels.at(iii));
		TextLabel& oLabelName(Labels.first);
		TextLabel& oLabelQuantity(Labels.second);

		oLabelName.SetContainerTopLeftCoordinates(nSlotWindowX, nSlotWindowY);
		oLabelQuantity.SetContainerTopLeftCoordinates(nSlotWindowX, 
													  nSlotWindowY);
		oLabelName.SetContainerDimensions(unWidth, unHeight);
		oLabelQuantity.SetContainerDimensions(unWidth, unHeight);
		oLabelName.Initialise();
		oLabelQuantity.Initialise();
	}
	CalculateCoordinatesBackpack(); //for all backpack items
	SetInitialisationNeeded(false);
}

void InventoryUI::Draw() const {
	ofPushStyle();
	ofBackground(GetColourBackground());
	ofPopStyle();
	GetButtonClusterBackpack().Draw();
	GetButtonClusterEquipment().Draw();
	DrawItems();
}

void InventoryUI::DrawItems() const {
	const csptr_HumanoidGeneric poPlayer(GetPlayer());
	const ButtonCluster& oClusterBackpack(GetButtonClusterBackpack());
	const ButtonCluster& oClusterEquipment(GetButtonClusterEquipment());
	const size_t unSlotBackpackWidth(oClusterBackpack.GetSprite()->getWidth());
	const size_t unBackpackItems(poPlayer->CountBackpackItems());
	const std::vector<pair_TextLabel>& voButtonLabels(GetButtonTextLabels());

    for (size_t iii(0); iii < unBackpackItems; ++iii) {
		const csptr_ItemGeneric poItem(poPlayer->GetBackpackItem(iii));
		const pair_TextLabel& Labels(voButtonLabels.at(iii));

        poItem->Draw();
		Labels.first.Draw();
		Labels.second.Draw();
    }

	const size_t unEquipmentSlots(oClusterEquipment.Count());

    for (size_t iii(0); iii < unEquipmentSlots; ++iii) {
		const csptr_ItemGeneric poGear(poPlayer->GetEquipped(iii));

        if (poGear)
            poGear->Draw();
	}
}

void InventoryUI::CalculateCoordinatesEquipment(const size_t unIndex) {
	const sptr_ItemGeneric poItem(GetPlayer()->GetEquipped(unIndex));

	if (!poItem)
		return;

	const DrawableGeneric& oSlot(GetButtonClusterEquipment().Get(unIndex));
	
	poItem->SetWindowCoordinates(oSlot.GetWindowX() + 8, 
								 oSlot.GetWindowY() - 5);
	poItem->UpdatePerceivedWindowCoordinates();
}

//if unIndex is the last one, calculates only for the last item
void InventoryUI::CalculateCoordinatesBackpack(const size_t unIndex) {
	const size_t unBackpackItems(GetPlayer()->CountBackpackItems());

	for (size_t iii(unIndex); iii < unBackpackItems; ++iii) {
		const sptr_ItemGeneric poItem(GetPlayer()->GetBackpackItem(iii));
		const DrawableGeneric& oSlot(GetButtonClusterBackpack().Get(iii));

		poItem->SetWindowCoordinates(oSlot.GetWindowX(), 
									 oSlot.GetWindowY() - 14);
		poItem->UpdatePerceivedWindowCoordinates();
		UpdateLabelTextItemName(iii);
		UpdateLabelTextItemQuantity(iii);
	}
}

void InventoryUI::UpdateLabelTextItemName(const size_t unIndex) {
	const csptr_ItemGeneric poItem(GetPlayer()->GetBackpackItem(unIndex));
	
	GetButtonTextLabels().at(unIndex).first.SetText(poItem->GetName());
}

void InventoryUI::UpdateLabelTextItemQuantity(const size_t unIndex) {
	const csptr_ItemGeneric poItem(GetPlayer()->GetBackpackItem(unIndex));
	const std::string& strQuantity(ofToString(poItem->GetQuantity()));
	TextLabel& oLabelQuantity(GetButtonTextLabels().at(unIndex).second);
	const size_t unLengthCurrent(oLabelQuantity.GetText().size());
	const bool bLengthChanged(strQuantity.size() != unLengthCurrent);

	oLabelQuantity.SetText(strQuantity);
	if (bLengthChanged)
		oLabelQuantity.Initialise();
}