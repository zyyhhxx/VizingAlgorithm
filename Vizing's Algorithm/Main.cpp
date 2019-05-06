#include <iostream>
#include <sstream>
#include <string>
#include "color.h"
#include "graph.h"

using namespace std;

int main()
{
	ColorGraph G("4 6 1 2 1 3 1 4 2 3 2 4 3 4");

	cout << G << endl;

	cout << G.json() << endl;
	system("pause");
}