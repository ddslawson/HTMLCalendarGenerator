#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>

using std::string;

// Class will take a name and a year and will provide functionality to generate
// an html calendar covering three years centred on the year passed in the constructor
class HTMLCalendarGenerator
{
public:
	// Class Constructor - initialises class members, including the name of the generated html file and the years of the calendar.
	HTMLCalendarGenerator(const string& calendarName, const int& year);
	~HTMLCalendarGenerator();

	// Generates and writes the html calendar file.
	void PopulateCalendar();

private:
	enum class Day
	{
		Monday,
		Tuesday,
		Wednesday,
		Thursday,
		Friday,
		Saturday,
		Sunday
	};

	enum class Month
	{
		January,
		February,
		March,
		April,
		May,
		June,
		July,
		August,
		September,
		October,
		November,
		December
	};

	// Lookup for month names
	const std::map<Month, string> monthNameMap {
		{Month::January, "January"},
		{Month::February, "February"},
		{Month::March, "March"},
		{Month::April, "April"},
		{Month::May, "May"},
		{Month::June, "June"},
		{Month::July, "July"},
		{Month::August, "August"},
		{Month::September, "September"},
		{Month::October, "October"},
		{Month::November, "November"},
		{Month::December, "December"}
	};

	// Default days in months
	const std::vector<int> defaultDaysInMonths{ 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	// Useful constants
	static const int weekNumColumn = -1;
    static const int monthsInYear = 12;
    static const int weeksInYear = 52;
	static const int daysInYear = 365;
	static const int daysInWeek = 7;
	static const int numberOfMonthRows = 3;
	static const int numberOfWeekRows = 6;
	
	// Object used for writing the html file.
	std::ofstream htmlFile;
	// Name of the html file to write
	const string htmlName;
	// The years for which the calendar will be generated
	const std::vector<int> years;

	// Generates the html for a single calendar year based on the year passed as a parameter.
	// It then returns the html as a string.
	string GenerateYear(const int& year) const;
	// Generates the html for a single calendar month based on the month, start day and number of the days in the month.
	// It then returns the html as a string.
	string GenerateMonth(const Month& month, const Day& startDay, const int& numOfDays, int& startingWeekNum) const;

	// Returns true if the year is determined to be a leap year
	bool IsLeapYear(const int& year) const;
	// Returns the day of the week that the year starts on.
	Day FirstDayOfYear(const int& year) const;

	// Tidy away some of the html header creation.
	string CreateHTMLHeader(const string& title) const;
};

