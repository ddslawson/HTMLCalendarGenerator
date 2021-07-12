#pragma once
#include "HTMLMonth.h"

class HTMLYear :
    public IHTMLCalendarComponent
{
public:
	HTMLYear(const int& year) :
		html(),
		year(year)
	{
	};
	virtual ~HTMLYear() {};
	string GetHTML();


	// Returns true if the year is determined to be a leap year
	bool IsLeapYear(const int& year) const;
	// Returns the day of the week that the year starts on.
	HTMLDay::Day FirstDayOfYear(const int& year) const;

	const int year;
private:
	// Useful constants
	static const int daysInYear = 365;
	static const int numberOfMonthRows = 3;

	string html;
};
