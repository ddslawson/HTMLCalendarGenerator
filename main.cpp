#include <iostream>
#include "HTMLCalendar.h"

// HTML Calendar Generator - Genrates a 3 year gregorian calendar centered on the provided year.
// Expected usage is ./HTMLCalendarGenerator <year> <output calendar name>
int main(int argc, char* argv[])
{
	std::cout << "HTML Calendar Generator" << std::endl;

	// Default values for year and calendar name
	int year = 2021;
	string htmlName = "calendar";

	// If the first command line parameter is used, capture it as year
	if (argc > 1)
	{
		year = atoi(argv[1]);
	}
	// If the second command line parameter is used, capture it as html name
	if (argc > 2)
	{
		htmlName = argv[2];
	}

	std::cout << "Input year: " << year << " File name: " << htmlName << std::endl;

	if (year <= 1582)
	{
		std::cout << "Chosen year " << year << " is unsupported." << std::endl
			<< "Generating Calendar centred on 1583." << std::endl;
		year = 1583;
	}
	// Create html genertor object
	HTMLCalendar calendar(htmlName, year);
	// Write calendar
	calendar.WriteHTML();

	return 0;
}
