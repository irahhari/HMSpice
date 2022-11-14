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
19-10-2022		Monika & Hari			Updated traverse method according to indexMap

15-09-2022		Monika & Hari			Created the file
*/

#pragma once

#include "Edge.h"

// Forward Declaration
class Edge;

class Node
{
public:
	string name;					// Name of the node
	vector<shared_ptr<Edge>> edges; // List of edges connected in the node
	bool processed;					// Flag value to know whether it is processed

	/*
	 * @brief		Traverses the map (graph of the circuit) and
	 *				populates the MNA and RHS matrices
	 *
	 * @param		indexMap map<string, int>
	 * @param		mna vector<vector<double>>
	 * @param		rhs vector<double>
	 *
	 * @return		void
	 */
	void traverse(map<string, int> &indexMap, vector<vector<double>> &mna, vector<double> &rhs);
};
