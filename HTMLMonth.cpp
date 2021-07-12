#include "HTMLMonth.h"

const std::map<HTMLMonth::Month, string> HTMLMonth::monthNameMap({
	{ Month::January, "January"},
	{ Month::February, "February" },
	{ Month::March, "March" },
	{ Month::April, "April" },
	{ Month::May, "May" },
	{ Month::June, "June" },
	{ Month::July, "July" },
	{ Month::August, "August" },
	{ Month::September, "September" },
	{ Month::October, "October" },
	{ Month::November, "November" },
	{ Month::December, "December" }
	});

const std::vector<int> HTMLMonth::defaultDaysInMonths({ 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 });

HTMLMonth::HTMLMonth(const HTMLMonth::Month& month, const bool& isLeapYear, const HTMLDay::Day& startDay, const int& startingWeekNum) :
	html(),
	month(month),
	startDay(startDay),
	daysInMonth(defaultDaysInMonths[static_cast<int>(month)]),
	startingWeekNum(startingWeekNum),
	nextMonthStartingWeekNum(-1)
{
	if (isLeapYear && month == Month::February)
	{
		daysInMonth = 29;
	}
};

string HTMLMonth::GetHTML()
{
	if (!html.empty())
	{
		return html;
	}

	stringstream monthStream;

	string monthName = HTMLMonth::monthNameMap.at(month);

	// Populate first row of table with week number and day headings
	monthStream << "<table width = \"100%\" bgcolor = \"#dbf3fc\">\n"
		<< "<center>" << monthName << "</center>\n"
		<< "<tr bgcolor=\"#2fb0e0\">\n"
		<< "<td width=\"14.28%\" align=\"center\">Wk</td>\n"
		<< "<td width=\"14.28%\" align=\"center\">Mo</td>\n"
		<< "<td width=\"14.28%\" align=\"center\">Tu</td>\n"
		<< "<td width=\"14.28%\" align=\"center\">We</td>\n"
		<< "<td width=\"14.28%\" align=\"center\">Th</td>\n"
		<< "<td width=\"14.28%\" align=\"center\">Fr</td>\n"
		<< "<td bgcolor=\"#62e0dc\" width=\"14.28%\" align=\"center\">Sa</td>\n"
		<< "<td bgcolor=\"#42afac\" width=\"14.28%\" align=\"center\">Su</td>\n"
		<< "</tr>\n";

	nextMonthStartingWeekNum = startingWeekNum;

	for (int row = 0, gridPos = 0, day = 1; row < numberOfWeekRows; ++row)
	{
		monthStream << "<tr valign=\"top\">\n";
		for (int col = weekNumColumn; col < daysInWeek; ++col)
		{
			// If column is week number column populate with week number
			if (col == weekNumColumn)
			{
				monthStream << "<td bgcolor=\"#2fb0e0\">" << nextMonthStartingWeekNum << "</td>\n";
				// Check if week finishes in the middle of the week
				if (daysInMonth - day >= daysInWeek - 1)
				{
					++nextMonthStartingWeekNum;
					if (nextMonthStartingWeekNum > weeksInYear)
					{
						nextMonthStartingWeekNum = 1;
					}
				}
			}
			else if (gridPos < static_cast<int>(startDay) || day > daysInMonth)
			{
				// For cells not occupied by days, make them white
				monthStream << "<td bgcolor=\"#ffffff\">&nbsp;</td>\n";
				++gridPos;
			}
			else
			{
				// This cell is a day of the month
				HTMLDay htmlDay = HTMLDay(static_cast<HTMLDay::Day>(col), day);
				monthStream << htmlDay.GetHTML();
				++gridPos;
				++day;
			}
		}
		monthStream << "</tr>\n";
	}
	// Close table
	monthStream << "</table>\n";

	// Cache output.
	html = monthStream.str();
	return html;
}
