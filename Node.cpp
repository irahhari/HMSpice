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
20-10-2022		Monika & Hari			Updated to include controlled sources

19-10-2022		Monika & Hari			Updated for the change in Edge.h

19-10-2022		Monika & Hari			Updated traverse method according to indexMap

25-09-2022		Monika & Hari			Added circuit stamp of independent voltage source

20-09-2022		Monika & Hari			Added circuit stamp of independent current source

15-09-2022		Monika & Hari			Created the file
*/

#include "Node.h"

void Node::traverse(map<string, int> &indexMap, vector<vector<double>> &mna, vector<double> &rhs)
{
	// When ground is encountered
	if (name.compare("0") == 0)
	{
		processed = true;
		return;
	}

	// Processes the node only once
	if (processed)
		return;
	processed = true;

	// Processes the all the edges connected to this node
	for (shared_ptr<Edge> edge : edges)
	{
		// Processes the edge only once
		if (edge->circuitElement->processed)
			continue;
		edge->circuitElement->processed = true;

		// Resistor
		if (edge->circuitElement->type == R)
		{
			// Group 1
			if (edge->circuitElement->group == G1)
			{
				// When source node is connected to ground
				if (edge->circuitElement->nodeA.compare("0") == 0)
				{
					int vminus = indexMap[edge->circuitElement->nodeB];

					mna[vminus][vminus] += 1.0 / edge->circuitElement->value;
				}
				// When target node is connected to ground
				else if (edge->circuitElement->nodeB.compare("0") == 0)
				{
					int vplus = indexMap[edge->circuitElement->nodeA];

					mna[vplus][vplus] += 1.0 / edge->circuitElement->value;
				}
				// When both the nodes are not connected ground
				else
				{
					int vplus = indexMap[edge->circuitElement->nodeA];
					int vminus = indexMap[edge->circuitElement->nodeB];

					mna[vplus][vplus] += 1.0 / edge->circuitElement->value;
					mna[vplus][vminus] += -1.0 / edge->circuitElement->value;
					mna[vminus][vplus] += -1.0 / edge->circuitElement->value;
					mna[vminus][vminus] += 1.0 / edge->circuitElement->value;
				}
			}
			// Group 2
			else
			{
				// When source node is connected to ground
				if (edge->circuitElement->nodeA.compare("0") == 0)
				{
					int vminus = indexMap[edge->circuitElement->nodeB];
					int i = indexMap[edge->circuitElement->name];

					mna[vminus][i] += -1.0;
					mna[i][vminus] += -1.0;
					mna[i][i] += -edge->circuitElement->value;
				}
				// When target node is connected to ground
				else if (edge->circuitElement->nodeB.compare("0") == 0)
				{
					int vplus = indexMap[edge->circuitElement->nodeA];
					int i = indexMap[edge->circuitElement->name];

					mna[vplus][i] += 1.0;
					mna[i][i] += -edge->circuitElement->value;
					mna[i][vplus] += 1.0;
				}
				// When both the nodes are not connected ground
				else
				{
					int vplus = indexMap[edge->circuitElement->nodeA];
					int vminus = indexMap[edge->circuitElement->nodeB];
					int i = indexMap[edge->circuitElement->name];

					mna[vplus][i] += 1.0;
					mna[vminus][i] += -1.0;
					mna[i][vplus] += 1.0;
					mna[i][vminus] += -1.0;
					mna[i][i] += -edge->circuitElement->value;
				}
			}
		}
		// Independent Current Source
		else if (edge->circuitElement->type == I)
		{
			// Group 1
			if (edge->circuitElement->group == G1)
			{
				// When source node is connected to ground
				if (edge->circuitElement->nodeA.compare("0") == 0)
				{
					int vminus = indexMap[edge->circuitElement->nodeB];

					rhs[vminus] += edge->circuitElement->value;
				}
				// When target node is connected to ground
				else if (edge->circuitElement->nodeB.compare("0") == 0)
				{
					int vplus = indexMap[edge->circuitElement->nodeA];

					rhs[vplus] += -edge->circuitElement->value;
				}
				// When both the nodes are not connected ground
				else
				{
					int vplus = indexMap[edge->circuitElement->nodeA];
					int vminus = indexMap[edge->circuitElement->nodeB];

					rhs[vplus] += -edge->circuitElement->value;
					rhs[vminus] += edge->circuitElement->value;
				}
			}
			// Group 2
			else
			{
				// When source node is connected to ground
				if (edge->circuitElement->nodeA.compare("0") == 0)
				{
					int vminus = indexMap[edge->circuitElement->nodeB];
					int i = indexMap[edge->circuitElement->name];

					mna[vminus][i] = -1.0;
					mna[i][i] = +1.0;
					rhs[i] = edge->circuitElement->value;
				}
				// When target node is connected to ground
				else if (edge->circuitElement->nodeB.compare("0") == 0)
				{
					int vplus = indexMap[edge->circuitElement->nodeA];
					int i = indexMap[edge->circuitElement->name];

					mna[vplus][i] = +1.0;
					mna[i][i] = +1.0;
					rhs[i] = edge->circuitElement->value;
				}
				// When both the nodes are not connected ground
				else
				{
					int vplus = indexMap[edge->circuitElement->nodeA];
					int vminus = indexMap[edge->circuitElement->nodeB];
					int i = indexMap[edge->circuitElement->name];

					mna[vplus][i] = +1.0;
					mna[vminus][i] = -1.0;
					mna[i][i] = +1.0;
					rhs[i] = edge->circuitElement->value;
				}
			}
		}
		// Independent Voltage Source (always Group 2)
		else if (edge->circuitElement->type == V)
		{
			// When source node is connected to ground
			if (edge->circuitElement->nodeA.compare("0") == 0)
			{
				int vminus = indexMap[edge->circuitElement->nodeB];
				int i = indexMap[edge->circuitElement->name];

				mna[vminus][i] += -1.0;
				mna[i][vminus] += -1.0;
				rhs[i] += edge->circuitElement->value;
			}
			// When target node is connected to ground
			else if (edge->circuitElement->nodeB.compare("0") == 0)
			{
				int vplus = indexMap[edge->circuitElement->nodeA];
				int i = indexMap[edge->circuitElement->name];

				mna[vplus][i] += 1.0;
				mna[i][vplus] += 1.0;
				rhs[i] += edge->circuitElement->value;
			}
			// When both the nodes are not connected ground
			else
			{
				int vplus = indexMap[edge->circuitElement->nodeA];
				int vminus = indexMap[edge->circuitElement->nodeB];
				int i = indexMap[edge->circuitElement->name];

				mna[vplus][i] += 1.0;
				mna[vminus][i] += -1.0;
				mna[i][vplus] += 1.0;
				mna[i][vminus] += -1.0;
				rhs[i] += edge->circuitElement->value;
			}
		}
		// Dependant Voltage Source (always Group 2)
		else if (edge->circuitElement->type == Vc)
		{
			// Current Controlled Voltage Source (CCVS)
			if (edge->circuitElement->controlling_variable == i)
			{
				// When source node is connected to ground
				if (edge->circuitElement->nodeA.compare("0") == 0)
				{
					int vminus = indexMap[edge->circuitElement->nodeB];
					int is = indexMap[edge->circuitElement->name];
					int ix = indexMap[edge->circuitElement->controlling_element->name];

					mna[vminus][is] += -1.0;
					mna[is][vminus] += -1.0;
					mna[is][ix] += -edge->circuitElement->value;
				}
				// When target node is connected to ground
				else if (edge->circuitElement->nodeB.compare("0") == 0)
				{
					int vplus = indexMap[edge->circuitElement->nodeA];
					int is = indexMap[edge->circuitElement->name];
					int ix = indexMap[edge->circuitElement->controlling_element->name];

					mna[vplus][is] += 1.0;
					mna[is][vplus] += 1.0;
					mna[is][ix] += -edge->circuitElement->value;
				}
				// When both the nodes are not connected ground
				else
				{
					int vplus = indexMap[edge->circuitElement->nodeA];
					int vminus = indexMap[edge->circuitElement->nodeB];
					int is = indexMap[edge->circuitElement->name];
					int ix = indexMap[edge->circuitElement->controlling_element->name];

					mna[vplus][is] += 1.0;
					mna[vminus][is] += -1.0;
					mna[is][vplus] += 1.0;
					mna[is][vminus] += -1.0;
					mna[is][ix] += -edge->circuitElement->value;
				}
			}
			// Voltage Controlled Voltage Source (VCVS)
			else
			{
				// When source node is connected to ground
				if (edge->circuitElement->nodeA.compare("0") == 0)
				{
					int vminus = indexMap[edge->circuitElement->nodeB];
					// When controlling_element's source node is connected to ground
					if (edge->circuitElement->controlling_element->nodeA == "0")
					{
						int vxminus = indexMap[edge->circuitElement->controlling_element->nodeB];
						int i = indexMap[edge->circuitElement->name];

						mna[vminus][i] += -1.0;
						mna[i][vminus] += -1.0;
						mna[i][vxminus] += edge->circuitElement->value;
					}
					// When controlling_element's target node is connected to ground
					else if (edge->circuitElement->controlling_element->nodeB == "0")
					{
						int vxplus = indexMap[edge->circuitElement->controlling_element->nodeA];
						int i = indexMap[edge->circuitElement->name];

						mna[vminus][i] += -1.0;
						mna[i][vminus] += -1.0;
						mna[i][vxplus] += -edge->circuitElement->value;
					}
					// When controlling_element's both the nodes are not connected ground
					else
					{
						int vxplus = indexMap[edge->circuitElement->controlling_element->nodeA];
						int vxminus = indexMap[edge->circuitElement->controlling_element->nodeB];
						int i = indexMap[edge->circuitElement->name];

						mna[vminus][i] += -1.0;
						mna[i][vminus] += -1.0;
						mna[i][vxplus] += -edge->circuitElement->value;
						mna[i][vxminus] += edge->circuitElement->value;
					}
				}
				// When target node is connected to ground
				else if (edge->circuitElement->nodeB.compare("0") == 0)
				{
					int vplus = indexMap[edge->circuitElement->nodeA];
					// When controlling_element's source node is connected to ground
					if (edge->circuitElement->controlling_element->nodeA == "0")
					{
						int vxminus = indexMap[edge->circuitElement->controlling_element->nodeB];
						int i = indexMap[edge->circuitElement->name];

						mna[vplus][i] += 1.0;
						mna[i][vplus] += 1.0;
						mna[i][vxminus] += edge->circuitElement->value;
					}
					// When controlling_element's target node is connected to ground
					else if (edge->circuitElement->controlling_element->nodeB == "0")
					{
						int vxplus = indexMap[edge->circuitElement->controlling_element->nodeA];
						int i = indexMap[edge->circuitElement->name];

						mna[vplus][i] += 1.0;
						mna[i][vplus] += 1.0;
						mna[i][vxplus] += -edge->circuitElement->value;
					}
					// When controlling_element's both the nodes are not connected ground
					else
					{
						int vxplus = indexMap[edge->circuitElement->controlling_element->nodeA];
						int vxminus = indexMap[edge->circuitElement->controlling_element->nodeB];
						int i = indexMap[edge->circuitElement->name];

						mna[vplus][i] += 1.0;
						mna[i][vplus] += 1.0;
						mna[i][vxplus] += -edge->circuitElement->value;
						mna[i][vxminus] += edge->circuitElement->value;
					}
				}
				// When both the nodes are not connected ground
				else
				{
					int vplus = indexMap[edge->circuitElement->nodeA];
					int vminus = indexMap[edge->circuitElement->nodeB];
					// When controlling_element's source node is connected to ground
					if (edge->circuitElement->controlling_element->nodeA == "0")
					{
						int vxminus = indexMap[edge->circuitElement->controlling_element->nodeB];
						int i = indexMap[edge->circuitElement->name];

						mna[vplus][i] += 1.0;
						mna[vminus][i] += -1.0;
						mna[i][vplus] += 1.0;
						mna[i][vminus] += -1.0;
						mna[i][vxminus] += edge->circuitElement->value;
					}
					// When controlling_element's target node is connected to ground
					else if (edge->circuitElement->controlling_element->nodeB == "0")
					{
						int vxplus = indexMap[edge->circuitElement->controlling_element->nodeA];
						int i = indexMap[edge->circuitElement->name];

						mna[vplus][i] += 1.0;
						mna[vminus][i] += -1.0;
						mna[i][vplus] += 1.0;
						mna[i][vminus] += -1.0;
						mna[i][vxplus] += -edge->circuitElement->value;
					}
					// When controlling_element's both the nodes are not connected ground
					else
					{
						int vxplus = indexMap[edge->circuitElement->controlling_element->nodeA];
						int vxminus = indexMap[edge->circuitElement->controlling_element->nodeB];
						int i = indexMap[edge->circuitElement->name];

						mna[vplus][i] += 1.0;
						mna[vminus][i] += -1.0;
						mna[i][vplus] += 1.0;
						mna[i][vminus] += -1.0;
						mna[i][vxplus] += -edge->circuitElement->value;
						mna[i][vxminus] += edge->circuitElement->value;
					}
				}
			}
		}
		// Dependant Current Source (always Group 1)
		else if (edge->circuitElement->type == Ic)
		{
			// Current Controlled Current Source (CCCS)
			if (edge->circuitElement->controlling_variable == i)
			{
				// When source node is connected to ground
				if (edge->circuitElement->nodeA.compare("0") == 0)
				{
					int vminus = indexMap[edge->circuitElement->nodeB];
					int i = indexMap[edge->circuitElement->controlling_element->name];

					mna[vminus][i] += -edge->circuitElement->value;
				}
				// When target node is connected to ground
				else if (edge->circuitElement->nodeB.compare("0") == 0)
				{
					int vplus = indexMap[edge->circuitElement->nodeA];
					int i = indexMap[edge->circuitElement->controlling_element->name];

					mna[vplus][i] += edge->circuitElement->value;
				}
				// When both the nodes are not connected ground
				else
				{
					int vplus = indexMap[edge->circuitElement->nodeA];
					int vminus = indexMap[edge->circuitElement->nodeB];
					int i = indexMap[edge->circuitElement->controlling_element->name];

					mna[vplus][i] += edge->circuitElement->value;
					mna[vminus][i] += -edge->circuitElement->value;
				}
			}
			// Voltage Controlled Current Source (VCCS)
			else
			{
				// When source node is connected to ground
				if (edge->circuitElement->nodeA.compare("0") == 0)
				{
					int vminus = indexMap[edge->circuitElement->nodeB];
					// When controlling_element's source node is connected to ground
					if (edge->circuitElement->controlling_element->nodeA == "0")
					{
						int vxminus = indexMap[edge->circuitElement->controlling_element->nodeB];

						mna[vminus][vxminus] += edge->circuitElement->value;
					}
					// When controlling_element's target node is connected to ground
					else if (edge->circuitElement->controlling_element->nodeB == "0")
					{
						int vxplus = indexMap[edge->circuitElement->controlling_element->nodeA];

						mna[vminus][vxplus] += -edge->circuitElement->value;
					}
					// When controlling_element's both the nodes are not connected ground
					else
					{
						int vxplus = indexMap[edge->circuitElement->controlling_element->nodeA];
						int vxminus = indexMap[edge->circuitElement->controlling_element->nodeB];

						mna[vminus][vxplus] += -edge->circuitElement->value;
						mna[vminus][vxminus] += edge->circuitElement->value;
					}
				}
				// When target node is connected to ground
				else if (edge->circuitElement->nodeB.compare("0") == 0)
				{
					int vplus = indexMap[edge->circuitElement->nodeA];
					// When controlling_element's source node is connected to ground
					if (edge->circuitElement->controlling_element->nodeA == "0")
					{
						int vxminus = indexMap[edge->circuitElement->controlling_element->nodeB];

						mna[vplus][vxminus] += -edge->circuitElement->value;
					}
					// When controlling_element's target node is connected to ground
					else if (edge->circuitElement->controlling_element->nodeB == "0")
					{
						int vxplus = indexMap[edge->circuitElement->controlling_element->nodeA];

						mna[vplus][vxplus] += edge->circuitElement->value;
					}
					// When controlling_element's both the nodes are not connected ground
					else
					{
						int vxplus = indexMap[edge->circuitElement->controlling_element->nodeA];
						int vxminus = indexMap[edge->circuitElement->controlling_element->nodeB];

						mna[vplus][vxplus] += edge->circuitElement->value;
						mna[vplus][vxminus] += -edge->circuitElement->value;
					}
				}
				// When both the nodes are not connected ground
				else
				{
					int vplus = indexMap[edge->circuitElement->nodeA];
					int vminus = indexMap[edge->circuitElement->nodeB];
					// When controlling_element's source node is connected to ground
					if (edge->circuitElement->controlling_element->nodeA == "0")
					{
						int vxminus = indexMap[edge->circuitElement->controlling_element->nodeB];

						mna[vplus][vxminus] += -edge->circuitElement->value;
						mna[vminus][vxminus] += edge->circuitElement->value;
					}
					// When controlling_element's target node is connected to ground
					else if (edge->circuitElement->controlling_element->nodeB == "0")
					{
						int vxplus = indexMap[edge->circuitElement->controlling_element->nodeA];

						mna[vplus][vxplus] += edge->circuitElement->value;
						mna[vminus][vxplus] += -edge->circuitElement->value;
					}
					// When controlling_element's both the nodes are not connected ground
					else
					{
						int vxplus = indexMap[edge->circuitElement->controlling_element->nodeA];
						int vxminus = indexMap[edge->circuitElement->controlling_element->nodeB];

						mna[vplus][vxplus] += edge->circuitElement->value;
						mna[vplus][vxminus] += -edge->circuitElement->value;
						mna[vminus][vxplus] += -edge->circuitElement->value;
						mna[vminus][vxminus] += edge->circuitElement->value;
					}
				}
			}
		}
	}

	// Traverses recursively all the nodes connected to this node
	for (shared_ptr<Edge> edge : edges)
		edge->target->traverse(indexMap, mna, rhs);
}
