#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include "IHTMLCalendarComponent.h"

using std::string;

// Class will take a name and a year and will provide functionality to generate
// an html calendar covering three years centred on the year passed in the constructor
class HTMLCalendar : public IHTMLCalendarComponent
{
public:
	// Class Constructor - initialises class members, including the name of the generated html file and the years of the calendar.
	HTMLCalendar(const string& calendarName, const int& year);
	~HTMLCalendar();

	// Generates html calendar.
	string GetHTML();

	// Writes HTML to file.
	void WriteHTML();

private:
	// Object used for writing the html file.
	std::ofstream htmlFile;
	// Name of the html file to write
	const string htmlName;
	// The years for which the calendar will be generated
	const std::vector<int> years;

	// Tidy away some of the html header creation.
	string CreateHTMLHeader(const string& title) const;

	string html;
};
