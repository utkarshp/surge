#include <iostream>
#include "resources.h"

using namespace std;


double difftime(mytm end, mytm begin)
{
	double difference=0;
	difference = (end.getday() - begin.getday()) * 24;
	difference += end.gethour() - begin.gethour();
	difference += (end.getmin() - begin.getmin())/60.0;

	return difference;
}

main()
{
	mytm end, begin;
	int day,hour,min;
	cout << "Enter end (day hour:min) ";
	cin >> day >> hour >>  min;
	end.update(hour,min,day);
	
	cout << "Enter beginning (day hour:min) ";
	cin >> day >> hour >>  min;
	begin.update(hour,min,day);

	cout << "Difference = end - begin = ";
	end.print();
	cout << "\tminus";
	begin.print();
	cout << difftime(end,begin) << " Hours\n";
}
