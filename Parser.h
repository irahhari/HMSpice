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
02-11-2022		Monika & Hari			Updated parse method

20-10-2022		Monika & Hari			Added nodes_group2

19-10-2022		Monika & Hari			Changed the vector

18-10-2022		Monika & Hari			Added print method

10-09-2022		Monika & Hari			Created the file
*/

#pragma once

#include "CircuitElement.h"

class Parser
{
public:
	vector<shared_ptr<CircuitElement>> circuitElements; // Stores the circuit elements in form of a vector
	set<string> nodes_group2;							// Stores all node names and group_2 circuit element names

	/*
	 * @brief		Parses the file (netlist) into a vector
	 *
	 * @param[ref]	file string
	 *
	 * @return		number of errors in the netlist
	 */
	int parse(const string &file);

	/*
	 * @brief		Prints the vectors which contains the circuit elements
	 *
	 * @return		void
	 */
	void print();
};
