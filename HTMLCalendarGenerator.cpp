#include <sstream>
#include <future>
#include "HTMLCalendarGenerator.h"

using std::stringstream;

// HTMLCalendarGenerator Class Constructor
HTMLCalendarGenerator::HTMLCalendarGenerator(const string& calendarName, const int& year) :
	htmlFile(),
	htmlName(calendarName + ".html"), // Append .html file extension to the calendar name
	years({{year - 1}, {year}, {year + 1}})
{
}

// HTMLCalendarGenerator Class Destructor
HTMLCalendarGenerator::~HTMLCalendarGenerator()
{
	if (htmlFile.is_open())
	{
		htmlFile.close();
	}
}

// Populate 3 year html calendar
void HTMLCalendarGenerator::PopulateCalendar()
{
	std::cout << "Generating Calendar" << std::endl;
	stringstream htmlStream;

	// Create tasks for generating each year asynchronously
	std::map<int, std::packaged_task<string(int)>> htmlYearTasks;

	// Populate and initiate tasks
	for (int year : years)
	{
		htmlYearTasks.emplace(year, [&](int selectedYear) {
			return GenerateYear(selectedYear);
			});
		htmlYearTasks.at(year)(year);
	}

	// Populate start of html file
	htmlStream << "<html>\n"
		<< CreateHTMLHeader(htmlName)
		<< "<body>\n"
		<< "<table border=\"0\" bgcolor=\"#2fb0e0\">\n"
		<< "<tr bgcolor=\"#ffffff\">\n";

	// Get output of html year tasks in order
	for (auto year : years)
	{
		htmlStream << "<td>\n"
			<< htmlYearTasks.at(year).get_future().get()
			<< "</td>\n";
	}

	htmlStream << "</tr>\n"
		<< "</table>\n"
		<< "</body>\n"
		<< "</html>";

	try
	{
		std::cout << "Generating file: " << htmlName << std::endl;
		htmlFile.open(htmlName);
		htmlFile << htmlStream.str();
		htmlFile.close();
		std::cout << "Generation Complete" << std::endl;
	}
	catch (const std::ofstream::failure& e)
	{
		std::cout << "Exception thrown when generating the file: " << htmlName << " Exception: " << e.what() << std::endl;
	}
}

// Generate html for year and return as string
string HTMLCalendarGenerator::GenerateYear(const int& year) const
{
	stringstream yearStream;

	std::vector<int> daysInMonths(defaultDaysInMonths);

	// If year is a leap year, change the number of days in February to 29
	if (IsLeapYear(year))
	{
		daysInMonths.at(static_cast<int>(Month::February)) = 29;
	}
	else
	{
		daysInMonths.at(static_cast<int>(Month::February)) = 28;
	}

	// Get the first day of the calendar year
	Day firstDay = FirstDayOfYear(year);

	yearStream << "<table width=\"30%\" border=\"0\" bgcolor=\"#2fb0e0\">\n"
		<< "<center>" << year << "</center>\n";

	int weekNum;

	// First week of the year in the Gregorian calendar is considered the week of the first Thursday.
	// Determine if first day of the year lies in the first week or the last week of last year.
	if (firstDay <= Day::Thursday)
	{
		weekNum = 1;
	}
	else
	{
		weekNum = weeksInYear;
	}

	int monthNum = 0;
	// Step through table populating each cell with a month starting with January
	for (int row = 0; row < numberOfMonthRows; ++row)
	{
		yearStream << "<tr bgcolor=\"#2fb0e0\">\n";

		int monthsPerRow = (((monthsInYear * 10)/ numberOfMonthRows) + 9) / 10; // efficiently round up decimal results

		for (int col = 0; col < monthsPerRow && monthNum < monthsInYear; ++col)
		{
			yearStream << "<td width=\"15%\" align=\"center\">\n"
					<< GenerateMonth(static_cast<Month>(monthNum), firstDay, daysInMonths.at(monthNum), weekNum) << "\n"
					<< "</td>\n";
			// Calculate the day of the week the next month starts on
			int nextFirstDay = (static_cast<int>(firstDay) + daysInMonths.at(monthNum)) % daysInWeek;
			firstDay = static_cast<Day>(nextFirstDay);
			++monthNum;
		}
		yearStream << "</tr>\n";
	}

	// Close table
	yearStream << "</table>\n";

	return yearStream.str();
}

// Generate html for month and return as string
string HTMLCalendarGenerator::GenerateMonth(const Month& month, const Day& startDay, const int& numOfDays, int& startingWeekNum) const
{
	stringstream monthStream;

	string monthName = monthNameMap.at(month);

	// Populate first row of table with week number and day headings
	monthStream  << "<table width = \"100%\" bgcolor = \"#dbf3fc\">\n"
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

	for (int row = 0, gridPos = 0, day = 1; row < numberOfWeekRows; ++row)
	{
		monthStream << "<tr valign=\"top\">\n";
		for (int col = weekNumColumn; col < daysInWeek; ++col)
		{
			// If column is week number column populate with week number
			if (col == weekNumColumn)
			{
				monthStream << "<td bgcolor=\"#2fb0e0\">" << startingWeekNum << "</td>\n";
				// Check if week finishes in the middle of the week
				if (numOfDays - day >= daysInWeek - 1)
				{
					++startingWeekNum;
					if (startingWeekNum > weeksInYear)
					{
						startingWeekNum = 1;
					}
				}
			}
			else if (gridPos < static_cast<int>(startDay) || day > numOfDays)
			{
				// For cells not occupied by days, make them white
				monthStream << "<td bgcolor=\"#ffffff\">&nbsp;</td>\n";
				++gridPos;
			}
			else
			{
				// This cell is a day of the month

				monthStream << "<td";
				// Apply additional colour to weekend days
				if (col == static_cast<int>(Day::Saturday))
				{
					monthStream << " bgcolor=\"#62e0dc\">";
				}
				else if (col == static_cast<int>(Day::Sunday))
				{
					monthStream << " bgcolor=\"#42afac\">";
				}
				else
				{
					monthStream << ">";
				}

				monthStream << day << "</td>\n";
				++gridPos;
				++day;
			}
		}
		monthStream << "</tr>\n";
	}
	// Close table
	monthStream << "</table>\n";

	return monthStream.str();
}

// Returns true if the year is a leap year
bool HTMLCalendarGenerator::IsLeapYear(const int& year) const
{
	// Leap years are every 4 years unless the year is divisible by 100, except for when it is divisible by 400
	return (((year % 400) == 0) || ((year % 100) != 0 && (year % 4) == 0));
}

// Returns the day of the week that the year starts on.
HTMLCalendarGenerator::Day HTMLCalendarGenerator::FirstDayOfYear(const int& year) const
{
	// First Gregorian year is 1582. Calendar started in October,
	// but in a non leap year January and October start with the same day.
	// For 1582 the Gregorian Calendar is propagated back to January 1st.
	Day firstDay1582 = Day::Friday;

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

		firstDay = firstDay % daysInWeek;
	}

	return static_cast<Day>(firstDay);
}

string HTMLCalendarGenerator::CreateHTMLHeader(const string& title) const
{
	stringstream htmlStream;

	htmlStream << "<head>\n"
		<< "<meta content=\"text/html; charset=ISO-8859-1\" http-equiv=\"content-type\">\n"
		<< "<title>" << title << "</title>\n"
		<< "</head>\n"
		<< "<style>\n"
		<< "table{border-collapse: collapse;}"
		<< "h1, th, td{ border: 1px solid blue; padding: 2px; text-align: center; font-family: helvetica;}\n"
		<< "</style>\n";

	return htmlStream.str();
}
