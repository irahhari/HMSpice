/*
 * Copyright (c) 2022, Shiv Nadar University, Delhi NCR, India. All Rights
 * Reserved. Permission to use, copy, modify and distribute this software for
 * educational, research, and not-for-profit purposes, without fee and without a
 * signed license agreement, is hereby granted, provided that this paragraph and
 * the following two paragraphs appear in all copies, modifications, and
 * distributions.
 *
 * IN NO EVENT SHALL SHIV NADAR UNIVERSITY BE LIABLE TO ANY PARTY FOR DIRECT,
 * INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST
 * PROFITS, ARISING OUT OF THE USE OF THIS SOFTWARE.
 *
 * SHIV NADAR UNIVERSITY SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT
 * NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE. THE SOFTWARE PROVIDED HEREUNDER IS PROVIDED "AS IS". SHIV
 * NADAR UNIVERSITY HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
 * ENHANCEMENTS, OR MODIFICATIONS.
 */

/*
Revision History:
Date			By						Change Notes
06-11-2022		Monika & Hari			Added extra header files as required by data validation

20-10-2022		Monika & Hari			Updated to include controlled sources

10-09-2022		Monika & Hari			Created the file
*/

#pragma once

// All necessary header files
#include <vector>
#include <set>
#include <map>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <algorithm>
#include <string>
#include <memory>
#include <sstream>
#include <cmath>

using namespace std;

enum Component {V, I, R, Ic, Vc};						// Identifies type of element
enum Group {G1, G2};									// Groups element based on output required
enum Variable {none, v, i};								// Controlling variable

struct CircuitElement
{
	string name;										// Name of the element
	Component type;										// Type of the element
	string nodeA;										// Starting node
	string nodeB;										// Ending node
	Group group;										// Group of the element
	double value;										// Value of the element (or scale factor if it is controlled source)
	Variable controlling_variable;						// Only for controlled sources, controlling variable
	shared_ptr<CircuitElement> controlling_element;		// Only for controlled sources, controlling element
	bool processed;										// Flag value to know whether it is processed
};
