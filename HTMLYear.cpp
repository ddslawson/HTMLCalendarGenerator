#include "HTMLYear.h"

string HTMLYear::GetHTML()
{
	if (!html.empty())
	{
		return html;
	}

	stringstream yearStream;
	yearStream << "<table width=\"30%\" border=\"0\" bgcolor=\"#2fb0e0\">\n"
		<< "<center>" << year << "</center>\n";

	int weekNum;

	// Get the first day of the calendar year
	HTMLDay::Day firstDay = FirstDayOfYear(year);

	// First week of the year in the Gregorian calendar is considered the week of the first Thursday.
	// Determine if first day of the year lies in the first week or the last week of last year.
	if (firstDay <= HTMLDay::Day::Thursday)
	{
		weekNum = 1;
	}
	else
	{
		weekNum = HTMLMonth::weeksInYear;
	}

	int monthNum = 0;

	// Step through table populating each cell with a month starting with January
	for (int row = 0; row < numberOfMonthRows; ++row)
	{
		yearStream << "<tr bgcolor=\"#2fb0e0\">\n";

		int monthsPerRow = (((HTMLMonth::monthsInYear * 10) / numberOfMonthRows) + 9) / 10; // efficiently round up decimal results

		for (int col = 0; col < monthsPerRow && monthNum < HTMLMonth::monthsInYear; ++col)
		{
			HTMLMonth htmlMonth = HTMLMonth(static_cast<HTMLMonth::Month>(monthNum), IsLeapYear(year), firstDay, weekNum);

			yearStream << "<td width=\"15%\" align=\"center\">\n"
				<< htmlMonth.GetHTML() << "\n"
				<< "</td>\n";

			weekNum = htmlMonth.GetNextWeekNum();
			// Calculate the day of the week the next month starts on
			int nextFirstDay = (static_cast<int>(firstDay) + htmlMonth.GetDaysInMonth()) % HTMLMonth::daysInWeek;
			firstDay = static_cast<HTMLDay::Day>(nextFirstDay);
			++monthNum;
		}
		yearStream << "</tr>\n";
	}

	// Close table
	yearStream << "</table>\n";

	// Cache output.
	html = yearStream.str();
	return html;
}

// Returns true if the year is a leap year
bool HTMLYear::IsLeapYear(const int& year) const
{
	// Leap years are every 4 years unless the year is divisible by 100, except for when it is divisible by 400
	return (((year % 400) == 0) || ((year % 100) != 0 && (year % 4) == 0));
}

// Returns the day of the week that the year starts on.
HTMLDay::Day HTMLYear::FirstDayOfYear(const int& year) const
{
	// First Gregorian year is 1582. Calendar started in October,
	// but in a non leap year January and October start with the same day.
	// For 1582 the Gregorian Calendar is propagated back to January 1st.
	HTMLDay::Day firstDay1582 = HTMLDay::Day::Friday;

	int startYear = 1582;
	int firstDay = static_cast<int>(firstDay1582);

	// Add the number of days in each year since 1582 and determine the new weekday the year starts on
	for (int i = startYear; i < year; ++i)
	{
		if (IsLeapYear(i))
		{
			firstDay += daysInYear + 1;
		}
		else
		{
			firstDay += daysInYear;
		}

		firstDay = firstDay % HTMLMonth::daysInWeek;
	}

	return static_cast<HTMLDay::Day>(firstDay);
}
