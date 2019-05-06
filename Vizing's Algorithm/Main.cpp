#include <iostream>
#include <sstream>
#include <string>
#include "color.h"
#include "graph.h"

using namespace std;

int main()
{
	istringstream iss("4 6 1 2 1 3 1 4 2 3 2 4 3 4");
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


	ColorGraph G(v, A);
	cout << v << endl << e << endl;

	cout << G << endl;

	cout << G.json() << endl;
	system("pause");
}