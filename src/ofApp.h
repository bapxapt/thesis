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

#ifndef NOTAGAME_OFAPP_H
#define NOTAGAME_OFAPP_H

#include "MainMenuUI.h"
#include "LoadingScreenUI.h"
#include "GameFieldUI.h"
#include "InventoryUI.h"
#include "LevelMap.h"
#include "Camera2D.h"
#include "MapGenerationThread.h"
#include "define.h"

#include "ofMain.h"

#include <bitset>
#include <string>

class ofApp: public ofBaseApp {
public:
    ofApp(size_t, size_t);
private:
	enum enumGameState: unsigned char {STATE_MAIN_MENU = 0, STATE_LOADING,
									   STATE_GAME_FIELD, STATE_INVENTORY};
    void setup() override;
    void update() override;
    void draw() override;
    void keyPressed(int) override;
    void keyReleased(int) override;
    void mouseMoved(int, int) override;
    void mouseDragged(int, int, int) override;
    void mousePressed(int, int, int) override;
    void mouseReleased(int, int, int) override;
    void windowResized(int, int) override;
    void dragEvent(ofDragInfo) override;
    void gotMessage(ofMessage) override;
	void exit() override;
	bool updateLoading();
	bool updateGameField();
	bool keyPressedGameField(int);
    bool keyPressedInventory(int);
    bool keyPressedMainMenu(int);
	bool mouseMovedMainMenu(int, int);
	bool mouseMovedInventory(int, int);
	bool mouseReleasedMainMenu(int, int, int);
	bool mouseReleasedGameField(int, int, int);
    bool mouseReleasedInventory(int, int, int);
	void ChoosePlayerAction(int);
	void GoalCheck(csptr_ItemGeneric);
	void InitialiseCurrentUI();
	const LevelMap& GetLevelMap() const;
	LevelMap& GetLevelMap();
	const MainMenuUI& GetMainMenuUI() const;
	MainMenuUI& GetMainMenuUI();
	const LoadingScreenUI& GetLoadingScreenUI() const;
	LoadingScreenUI& GetLoadingScreenUI();
	const GameFieldUI& GetGameFieldUI() const;
	GameFieldUI& GetGameFieldUI();
	const InventoryUI& GetInventoryUI() const;
	InventoryUI& GetInventoryUI();
	enumGameState GetGameState() const;
	void SetGameState(enumGameState);
	size_t GetRedrawsNeeded() const;
	void SetRedrawsNeeded(size_t = 1);
	const Camera2D& GetCamera() const;
	Camera2D& GetCamera();
	MapGenerationThread& GetMapGenerationThread();
	int GetModifierKey() const;
	void SetModifierKey(int);
	bool IsGameFinished() const;
	void SetGameFinished(bool);

	LevelMap m_oLevelMap;
	enumGameState m_eGameState;
	Camera2D m_oCamera;
    MainMenuUI m_oMainMenuUI;
    LoadingScreenUI m_oLoadingScreenUI;
    GameFieldUI m_oGameFieldUI;
    InventoryUI m_oInventoryUI;
	size_t m_unRedrawsNeeded;
	MapGenerationThread m_oMapGenerationThread;
	int m_nModifierKey;
	bool m_bGameFinished; //either won or lost
};

inline void ofApp::exit() {
	GetMapGenerationThread().stopThread();
}

inline size_t ofApp::GetRedrawsNeeded() const {
	return m_unRedrawsNeeded;
}

inline void ofApp::SetRedrawsNeeded(const size_t unRedrawsNeeded) {
	m_unRedrawsNeeded = unRedrawsNeeded;
}

inline const LevelMap& ofApp::GetLevelMap() const {
	return m_oLevelMap;
}

inline LevelMap& ofApp::GetLevelMap() {
	return m_oLevelMap;
}

inline const MainMenuUI& ofApp::GetMainMenuUI() const {
	return m_oMainMenuUI;
}

inline MainMenuUI& ofApp::GetMainMenuUI() {
	return m_oMainMenuUI;
}

inline const LoadingScreenUI& ofApp::GetLoadingScreenUI() const {
	return m_oLoadingScreenUI;
}

inline LoadingScreenUI& ofApp::GetLoadingScreenUI() {
	return m_oLoadingScreenUI;
}

inline const GameFieldUI& ofApp::GetGameFieldUI() const {
	return m_oGameFieldUI;
}

inline GameFieldUI& ofApp::GetGameFieldUI() {
	return m_oGameFieldUI;
}

inline const InventoryUI& ofApp::GetInventoryUI() const {
	return m_oInventoryUI;
}

inline InventoryUI& ofApp::GetInventoryUI() {
	return m_oInventoryUI;
}

inline ofApp::enumGameState ofApp::GetGameState() const {
	return m_eGameState;
}

inline const Camera2D& ofApp::GetCamera() const {
	return m_oCamera;
}

inline Camera2D& ofApp::GetCamera() {
	return m_oCamera;
}

inline MapGenerationThread& ofApp::GetMapGenerationThread() {
	return m_oMapGenerationThread;
}

inline int ofApp::GetModifierKey() const {
	return m_nModifierKey;
}

inline void ofApp::SetModifierKey(const int nKey) {
	m_nModifierKey = nKey;
}

inline bool ofApp::IsGameFinished() const {
	return m_bGameFinished;
}

inline void ofApp::SetGameFinished(const bool bFinished) {
	m_bGameFinished = bFinished;
}

#endif