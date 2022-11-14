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
04-11-2022		Monika & Hari			Added Solver

02-11-2022		Monika & Hari			Updated as per change in parse method

19-10-2022		Monika & Hari			Updated makeIndexMap

17-10-2022		Monika & Hari			Changed the way node is picked for traversal

30-09-2022		Monika & Hari			Created printMNAandRHS

25-09-2022		Monika & Hari			Created makeGraph

22-09-2022		Monika & Hari			Created makeIndexMap

19-09-2022		Monika & Hari			Tested parse method

12-09-2022		Monika & Hari			Created the file
*/

#include "main.h"

void makeIndexMap(map<string, int> &indexMap, Parser &parser)
{
	int i = 0;

	for (string str : parser.nodes_group2)
		if (str.compare("0") != 0)
			indexMap[str] = i++;
}

void printMNAandRHS(vector<vector<double>> &mna, map<string, int> &indexMap, vector<double> &rhs)
{
	cout << fixed;
	cout << setprecision(5);

	map<string, int>::iterator k = indexMap.begin();

	for (size_t i = 0; i < mna.size(); i++, k++)
	{
		for (size_t j = 0; j < mna.size(); j++)
			cout << mna[i][j] << "\t\t";
		cout << "\t\t" << k->first << "\t\t" << rhs[i] << endl;
	}
}

void makeGraph(map<string, shared_ptr<Node>> &nodeMap, Parser &parser)
{
	for (shared_ptr<CircuitElement> circuitElement : parser.circuitElements)
	{
		// Creates/retrieves start node
		shared_ptr<Node> nodeStart;
		map<string, shared_ptr<Node>>::iterator startNodeIter = nodeMap.find(circuitElement->nodeA);
		if (startNodeIter != nodeMap.end())
			nodeStart = startNodeIter->second;
		else
		{
			nodeStart = make_shared<Node>();
			nodeStart->name = circuitElement->nodeA;
			nodeMap[circuitElement->nodeA] = nodeStart;
		}

		// Creates/retrieves end node
		shared_ptr<Node> nodeEnd;
		map<string, shared_ptr<Node>>::iterator endNodeIter = nodeMap.find(circuitElement->nodeB);
		if (endNodeIter != nodeMap.end())
			nodeEnd = endNodeIter->second;
		else
		{
			nodeEnd = make_shared<Node>();
			nodeEnd->name = circuitElement->nodeB;
			nodeMap[circuitElement->nodeB] = nodeEnd;
		}

		// Edge from start node to end node for nodeStart
		shared_ptr<Edge> edgeStartEnd = make_shared<Edge>();
		edgeStartEnd->source = nodeStart;
		edgeStartEnd->target = nodeEnd;
		edgeStartEnd->circuitElement = circuitElement;
		nodeStart->edges.push_back(edgeStartEnd);

		// Edge from end node to start node for nodeEnd
		shared_ptr<Edge> edgeEndStart = make_shared<Edge>();
		edgeEndStart->source = nodeEnd;
		edgeEndStart->target = nodeStart;
		edgeEndStart->circuitElement = circuitElement;
		nodeEnd->edges.push_back(edgeEndStart);
	}
}

void printxX(map<string, int> &indexMap, Eigen::MatrixXd &X)
{
	cout << fixed;
	cout << setprecision(5);

	map<string, int>::iterator k = indexMap.begin();

	cout << "\n";
	for (size_t i = 0; i < indexMap.size(); i++, k++)
		cout << k->first << "\t\t" << X(i) << endl;
}

int main()
{
	// Creates a parser to store the circuit in form of vector
	Parser parser;
	if (parser.parse("circuit.hms") != 0)
		return 1;

	// Map to store all nodes' and group_2 elements' index position in MNA and RHS matrix
	map<string, int> indexMap;
	makeIndexMap(indexMap, parser);

	// Creates MNA and RHS matrix and initializes to 0.0
	int m = int(indexMap.size());
	vector<vector<double>> mna(m, vector<double>(m, 0.0));
	vector<double> rhs(m, 0.0);

	// Map to store the graph of the circuit
	map<string, shared_ptr<Node>> nodeMap;
	makeGraph(nodeMap, parser);

	// Picks the first node from nodeMap
	map<string, shared_ptr<Node>>::iterator startNodeIter = nodeMap.begin();
	advance(startNodeIter, 1);

	// Traverses the whole graph and populates the MNA and RHS matrices
	if (startNodeIter != nodeMap.end())
		startNodeIter->second->traverse(indexMap, mna, rhs);

	// Code to solve using Eigen Library
	vector<double> v;
	for (int i = 0; i < m; i++)
		for (int j = 0; j < m; j++)
			v.push_back(mna[i][j]);

	Eigen::MatrixXd MNA;
	MNA.resize(m, m);
	MNA = Eigen::MatrixXd::Map(&v[0], m, m).transpose();

	Eigen::MatrixXd RHS;
	RHS.resize(m, 1);
	RHS = Eigen::MatrixXd::Map(&rhs[0], m, 1);

	// De-allocating previously allocated
	// memory for solve method to use
	parser.circuitElements.clear();
	for (map<string, shared_ptr<Node>>::iterator i = nodeMap.begin(); i != nodeMap.end(); i++)
		i->second->edges.clear();
	nodeMap.clear();
	mna.clear();
	rhs.clear();
	v.clear();

	Eigen::MatrixXd X = MNA.lu().solve(RHS);

	printxX(indexMap, X);

	return 0;
}
