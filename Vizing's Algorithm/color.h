#ifndef COLOR_H
#define COLOR_H
#include "graph.h"
#include <assert.h>
#include <fstream>
#include "Resources/rapidjson/document.h"     // rapidjson's DOM-style API
#include "Resources/rapidjson/prettywriter.h" // for stringify JSON

using namespace rapidjson;

typedef Color* ColorPtr;

//if color = 0, the edge is uncolored (default value);
//color starts from 1 to max.

class ColorGraph {
	friend  ostream& operator<<(ostream &output, const ColorGraph &graph);

public:
	Document doc;
	Value stepArray;

	ColorGraph(string setup);
	ColorGraph(int, Array<Edge>);
	ColorGraph(int v, int e);

	Boolean isColor(int v, Color s);
	Color missingColor(int vertex);
	Edge locateEdge(int v, Color s);
	Boolean isIn(int edVx, Array<int> endVerices, int range, int& position);
	void AddOneEdge(Array<Edge> A, int k);
	void recolor(int, Array<int>, Array<Color>, int, int);
	Array<Edge> searchEdges(Array<Edge> A, Color s, Color t);
	Graph* subGraph(Array<Edge> A, Color s, Color t);
	void switchColor(Graph *subPtr, Color s, Color t, int vertex);
	void reportChange(int v1, int v2, int oldColor, int newColor);
	string json();


private:
	Graph *G;
	//record the colors of each edge, for efficient search;
	ColorPtr *colorMatrix;

	int max; //maximum degree+1 (maximum colors allowed);
	int count; //count # of edges already colored;

};


ColorGraph::ColorGraph(string setup) {
	istringstream iss(setup);
	cin.rdbuf(iss.rdbuf());

	int v, e;
	cin >> v;
	cin >> e;
	Array<Edge> A(e);


	for (int i = 0; i < e; i++) {
		int x, y;
		cin >> x >> y;
		Edge tmp(x, y);
		A.insert(i, tmp);
	}

	doc.SetArray();
	G = new Graph(v, A);
	int size = A.getSize();
	count = 0;

	max = G->maxDegree() + 1;

	//color initialization;

	colorMatrix = new ColorPtr[v];

	for (int i = 0; i < v; i++) {

		colorMatrix[i] = new Color[v];
		for (int j = 0; j < v; j++)
			colorMatrix[i][j] = 0;
	}

	//start coloring processing
	while (count < size) {
		//Initialize the array for the current step
		Document::AllocatorType& allocator = doc.GetAllocator();
		stepArray = Value(kArrayType);
		Value edge(kArrayType);
		edge.PushBack(A[count].startVx, allocator);
		edge.PushBack(A[count].endVx, allocator);
		stepArray.PushBack(edge, allocator);
		cout << "Coloring " << A[count];
		if (count < max) {
			int start = A[count].startVx;
			int end = A[count].endVx;
			colorMatrix[start - 1][end - 1] = ++count;
			colorMatrix[end - 1][start - 1] = count;
			reportChange(start, end, 0, count);
		}
		else {
			//coloring one more edge;
			AddOneEdge(A, count++);
		}
		doc.PushBack(stepArray, allocator);
	}
}


ColorGraph::ColorGraph(int v, Array<Edge> A)
{
	doc.SetArray();
	G = new Graph(v, A);
	int size = A.getSize();
	count = 0;

	max = G->maxDegree() + 1;

	//color initialization;

	colorMatrix = new ColorPtr[v];

	for (int i = 0; i < v; i++) {

		colorMatrix[i] = new Color[v];
		for (int j = 0; j < v; j++)
			colorMatrix[i][j] = 0;
	}

	//start coloring processing
	while (count < size) {
		//Initialize the array for the current step
		Document::AllocatorType& allocator = doc.GetAllocator();
		stepArray = Value(kArrayType);
		Value edge(kArrayType);
		edge.PushBack(A[count].startVx, allocator);
		edge.PushBack(A[count].endVx, allocator);
		stepArray.PushBack(edge, allocator);
		cout << "Coloring " << A[count];
		if (count < max) {
			int start = A[count].startVx;
			int end = A[count].endVx;
			colorMatrix[start - 1][end - 1] = ++count;
			colorMatrix[end - 1][start - 1] = count;
			reportChange(start, end, 0, count);
		}
		else {
			//coloring one more edge;
			AddOneEdge(A, count++);
		}
		doc.PushBack(stepArray, allocator);
	}
}



ColorGraph::ColorGraph(int v, int e)
{
	doc.SetArray();
	G = new Graph(v);
	colorMatrix = new ColorPtr[v];

	for (int i = 0; i < v; i++) {
		colorMatrix[i] = new Color[v];
		for (int j = 0; j < v; j++)
			colorMatrix[i][j] = 0;
	}
	count = 0;
}


string
ColorGraph::json() {
	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	doc.Accept(writer);
	return buffer.GetString();
}


void
ColorGraph::reportChange(int v1, int v2, int oldColor, int newColor) {
	Document::AllocatorType& allocator = doc.GetAllocator();
	Value opArray(kArrayType);
	opArray.PushBack(v1, allocator);
	opArray.PushBack(v2, allocator);
	opArray.PushBack(oldColor, allocator);
	opArray.PushBack(newColor, allocator);
	stepArray.PushBack(opArray, allocator);
	cout << "Colorchange: " << v1 << "," << v2 << " from " << oldColor << " to " << newColor << endl;
	return;
}


Color
ColorGraph::missingColor(int vertex) {

	Color tmp;
	for (Color color = 1; color <= max; color++)
		if (!isColor(vertex, color)) {
			tmp = color;
			color = max + 1;
		}
	return tmp;
}
//Is "s" a color of some edge at v


Boolean
ColorGraph::isColor(int v, Color s) {

	int size = G->getVertices();

	for (int i = 0; i < size; i++) {
		assert(colorMatrix[v - 1][i] == colorMatrix[i][v - 1]);
		if (colorMatrix[v - 1][i] == s)
			return TRUE;
	}
	return FALSE;

}

//search for an edge with vertex v and color s
//assume that such an edge exists

Edge
ColorGraph::locateEdge(int v, Color s) {

	Edge E;
	assert(isColor(v, s));
	int vertices = G->getVertices();
	for (int i = 0; i < vertices; i++) {
		if (colorMatrix[v - 1][i] == s) {
			E.startVx = v;
			E.endVx = i + 1;
			E.color = s;
			i = vertices;
		}
	}

	return E;
}



Boolean
ColorGraph::isIn(int edVx, Array<int> endVertices, int range, int& position)
{
	for (int i = 0; i < range; i++) {
		if (edVx == endVertices[i]) {
			position = i;
			return TRUE;
		}
	}
	return FALSE;
}

void
ColorGraph::AddOneEdge(Array<Edge> A, int k) {
	Edge E = A[k];
	int x = E.startVx;
	Array<Edge> xEdges(max - 1);
	Array<int> endVertices(max - 1);

	int v = G->getVertices();
	Array<Color> missingCol(v);
	int xEdgeCount = 0;
	Color t;
	Color s = missingColor(x);
	missingCol.insert(x - 1, s);
	int tmp = E.endVx;
	Boolean flag = TRUE;
	int position;

	// constructing a fan containing vertex x
	while (!isIn(tmp, endVertices, xEdgeCount, position) && flag) {
		endVertices.insert(xEdgeCount, tmp);
		xEdges.insert(xEdgeCount, E);
		xEdgeCount++;
		t = missingColor(tmp);
		missingCol.insert(tmp - 1, t);

		if (isColor(x, t)) {
			E = locateEdge(x, t);
			tmp = E.endVx;
		}
		else {
			flag = FALSE;
		}
	}

	if (flag) {
		// entering CASE I: fan terminates at a vertex that misses a previously used color
		int y = endVertices[position];
		assert(y == tmp);
		int z = endVertices[xEdgeCount - 1];

		recolor(x, endVertices, missingCol, 0, position);

		Graph* subPtr = subGraph(A, s, t);

		if (!(subPtr->isConnected(x, y))) {
			// entering SUBCASE I: the alternating-color-path starting from vertex endVertices[position - 1] doesn't end at vertex x
			switchColor(subPtr, s, t, y);
			int oldColor = colorMatrix[x - 1][y - 1];
			colorMatrix[x - 1][y - 1] = s;
			colorMatrix[y - 1][x - 1] = s;
			reportChange(x, y, oldColor, s);
		}
		else {
			// entering SUBCASE II: the alternating-color-path starting from vertex endVertices[position - 1] ends at vertex x
			assert(!(subPtr->isConnected(x, z))); // This is guaranteed because vertex x has already used color t and still misses color s
			recolor(x, endVertices, missingCol, position, xEdgeCount - 1);
			switchColor(subPtr, s, t, z);
			int oldColor = colorMatrix[x - 1][z - 1];
			colorMatrix[x - 1][z - 1] = s;
			colorMatrix[z - 1][x - 1] = s;
			reportChange(x, z, oldColor, s);
		}
	}
	else {
		// entering CASE II: fan terminates at a vertex that misses the same color as vertex x
		for (int i = xEdgeCount - 2; i >= 0; i--) {
			int j = endVertices[i] - 1;
			int oldColor = colorMatrix[x - 1][j];
			colorMatrix[x - 1][j] = missingCol[j];
			colorMatrix[j][x - 1] = missingCol[j];
			reportChange(x, j + 1, oldColor, missingCol[j]);
		}
		int oldColor = colorMatrix[x - 1][tmp - 1];
		colorMatrix[x - 1][tmp - 1] = t;
		colorMatrix[tmp - 1][x - 1] = t;
		reportChange(x, tmp, oldColor, t);
	}
}


void
ColorGraph::recolor(int x, Array<int> endVertices, Array<Color> missingCol,
	int start, int position)
{
	int end;
	assert(start <= position);
	for (int i = start; i < position; i++) {
		end = endVertices[i];
		int oldColor = colorMatrix[x - 1][end - 1];
		colorMatrix[x - 1][end - 1] = missingCol[end - 1];
		colorMatrix[end - 1][x - 1] = missingCol[end - 1];
		reportChange(x, end, oldColor, missingCol[end - 1]);
	}
	end = endVertices[position];
	int oldColor = colorMatrix[x - 1][end - 1];
	colorMatrix[x - 1][end - 1] = 0;
	colorMatrix[end - 1][x - 1] = 0;
	reportChange(x, end, oldColor, 0);
}


Array<Edge>
ColorGraph::searchEdges(Array<Edge> A, Color s, Color t)
{
	Array<Edge> first(count);
	int tmp = 0;
	for (int i = 0; i < count; i++) {
		int start = A[i].startVx;
		int end = A[i].endVx;

		if (colorMatrix[start - 1][end - 1] == s || colorMatrix[start - 1][end - 1] == t) {
			Edge choose(start, end, colorMatrix[start - 1][end - 1]);
			first.insert(tmp++, choose);
		}
	}

	Array<Edge> final(tmp);

	for (int i = 0; i < tmp; i++) {
		final.insert(i, first[i]);
	}
		
	return final;
}


Graph*
ColorGraph::subGraph(Array<Edge> A, Color s, Color t) {
	int v = G->getVertices();
	Graph *graphPtr = new Graph(v, searchEdges(A, s, t));
	return graphPtr;
}

void
ColorGraph::switchColor(Graph *subPtr, Color s, Color t, int vertex)
{
	int v = subPtr->getVertices();

	for (int i = 0; i < v; i++) {
		List L = *((subPtr->theAdj)->adj[i]);
		ListIterator I;
		if (subPtr->isConnected(i + 1, vertex)) {
			for (I.start(L); !I.done(); I++) {

				if ((i + 1) < I()) {
					if (colorMatrix[i][I() - 1] == s) {
						colorMatrix[i][I() - 1] = t;
						colorMatrix[I() - 1][i] = t;
						reportChange(i + 1, I(), s, t);
					}
					else {
						assert(colorMatrix[i][I() - 1] == t);
						colorMatrix[i][I() - 1] = s;
						colorMatrix[I() - 1][i] = s;
						reportChange(i + 1, I(), t, s);
					}
				}
			}
		}
	}
}


ostream& operator<<(ostream &output, const ColorGraph &graph)
{
	output << "Edges  " << "  Color " << endl;

	int v = graph.G->getVertices();

	for (int i = 0; i < v; i++) {
		ListIterator I;
		List L;
		L = *(((graph.G->getAdj())->getPtrToList())[i]);
		for (I.start(L); !I.done(); I++) {
			if ((i + 1) < I())
				output << i + 1 << ' ' << I() << "       "
				<< graph.colorMatrix[i][I() - 1] << endl;
		}
	}
	return output;
}

extern "C"             //No name mangling
__declspec(dllexport)  //Tells the compiler to export the function
void                     //Function return type     
__cdecl                //Specifies calling convention, cdelc is default, 
					   //so this can be omitted 
getGraphColorJson(int* input, int size) {
	string result = "";
	int i = 0;
	while (i < size) {
		result += to_string(input[i]) + " ";
		i++;
	}
	ColorGraph cg(result);

	ofstream outputFile("edge colors.json");
	outputFile << cg.json();
}

#endif
