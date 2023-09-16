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

#ifndef NOTAGAME_GENERIC_UI_H
#define NOTAGAME_GENERIC_UI_H

class GenericUI {
public:
    GenericUI();
	bool IsInitialisationNeeded() const;
	void SetInitialisationNeeded(bool);
private:
	bool m_bInitialisationNeeded;
};

inline bool GenericUI::IsInitialisationNeeded() const {
	return m_bInitialisationNeeded;
}

inline void GenericUI::SetInitialisationNeeded(const bool bNeeded) {
	m_bInitialisationNeeded = bNeeded;
}

#endif