#pragma once
#include "IHTMLCalendarComponent.h"

class HTMLDay :
    public IHTMLCalendarComponent
{
public:
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

    HTMLDay(const Day& day, const int& date) :
		html(),
		day(day),
		date(date)
	{
	};
	virtual ~HTMLDay() {};
    string GetHTML();

    const Day day;
	const int date;
private:
	string html;
};
