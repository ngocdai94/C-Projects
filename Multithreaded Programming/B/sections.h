/*  sections.h: mutual exclusion sections

    Copyright (C) 2013  Eric Larson and Rochelle Palting
    elarson@seattleu.edu

    This file is part of the MDAT infrastructure.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef SECTIONS_H
#define SECTIONS_H

#ifdef __cplusplus
extern "C" {
#endif

enum AccessType { SEARCH, APPEND, MODIFY };

void sectionInitGlobals();
void sectionEntrySection(enum AccessType accessType);
void sectionExitSection(enum AccessType accessType);
void sectionCriticalSection(enum AccessType accessType);
void sectionRemainderSection(enum AccessType accessType);

#ifdef __cplusplus
}
#endif

#endif
