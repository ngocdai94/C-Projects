/*  checker.cpp: mutual exclusion checker

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

#include <string>
using namespace std;

#include "checker.h"

// Global Variables
static int searchCount = 0;
static int appendCount = 0;
static int modifyCount = 0;
static bool multipleSearch = false;
static bool searchAndAppend = false;

// Global Error Messages
static string twoAppendMsg =
  "VIOLATION: two append accesses at the same time";
static string twoModifyMsg =
  "VIOLATION: two modify accesses at the same time";
static string exclModifyMsg =
  "VIOLATION: modify access does not have exclusive access";
#ifdef CHECK_LIMIT
static string limitMsg =
  "VIOLATION: more than three concurrent searches";
#endif
static string multipleSearchMsg =
  "WARNING: multiple search accesses did NOT occur concurrently";
static string searchAndAppendMsg =
  "WARNING: search and append did NOT occur concurrently";

// updateAndCheck: Updates the status of the checker and checks for violations.
// Returns a pointer to an error message if a violation occurs and NULL otherwise.
//
static const char *updateAndCheck(string section, string property)
{
  if (section == "Critical") {
    if (property == "search") {
      searchCount++;
#ifdef CHECK_LIMIT
      if (searchCount > 3) return limitMsg.c_str();
#endif
    }
    else if (property == "append")
      appendCount++;
    else if (property == "modify")
      modifyCount++;

    if (appendCount > 1)
      return twoAppendMsg.c_str();
    if (modifyCount > 1)
      return twoModifyMsg.c_str();
    if ((modifyCount == 1) && ((appendCount > 0) || (searchCount > 0)))
      return exclModifyMsg.c_str();
    if (searchCount > 1)
      multipleSearch = true;
    if ((searchCount >= 1) && (appendCount == 1))
      searchAndAppend = true;
  }
  else if (section == "Exit") {
    if (property == "search")
      searchCount--;
    else if (property == "append")
      appendCount--;
    else if (property == "modify")
      modifyCount--;
  }
  else if (section == "AllFinish") {
    if (!multipleSearch)
      return multipleSearchMsg.c_str();
    if (!searchAndAppend)
      return searchAndAppendMsg.c_str();
  }

  return NULL;
}

// Callback function to register with MDAT
const char *checkerCallback(int id, const char *section, const char *property)
{
  return updateAndCheck(section, property);
}
