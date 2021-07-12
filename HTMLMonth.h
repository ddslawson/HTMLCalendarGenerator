#pragma once
#include <vector>
#include <map>
#include "HTMLDay.h"

class HTMLMonth :
    public IHTMLCalendarComponent
{
public:
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
	static const std::map<Month, string> monthNameMap;

	HTMLMonth(const HTMLMonth::Month& month, const bool& isLeapYear, const HTMLDay::Day& startDay, const int& startingWeekNum);

	~HTMLMonth() {};
	string GetHTML();

	int GetNextWeekNum() { return nextMonthStartingWeekNum; };
	int GetDaysInMonth() { return daysInMonth; };

	static const int weeksInYear = 52;
	static const int monthsInYear = 12;
	static const int daysInWeek = 7;

	const Month month;
	const HTMLDay::Day startDay;
	const int startingWeekNum;

private:
	// Default days in months
	static const std::vector<int> defaultDaysInMonths;
	static const int weekNumColumn = -1;
	static const int numberOfWeekRows = 6;

	string html;
	int nextMonthStartingWeekNum;
	int daysInMonth;
};
