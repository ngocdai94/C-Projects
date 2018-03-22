/*  checker.h: mutual exclusion checker

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

#ifndef CHECKER_H
#define CHECKER_H

// Callback function to register with MDAT
extern const char *checkerCallback(int id, const char *section, const char *property);

#endif

