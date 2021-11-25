#include <sstream>
#include <future>
#include "HTMLCalendar.h"
#include "HTMLYear.h"

using std::stringstream;

// HTMLCalendar Class Constructor
HTMLCalendar::HTMLCalendar(const string& calendarName, const int& year) :
	htmlFile(),
	htmlName(calendarName + ".html"), // Append .html file extension to the calendar name
	years({ {year - 1}, {year}, {year + 1} })
{
}

// HTMLCalendarGenerator Class Destructor
HTMLCalendar::~HTMLCalendar()
{
	if (htmlFile.is_open())
	{
		htmlFile.close();
	}
}

// Populate 3 year html calendar
string HTMLCalendar::GetHTML()
{
	if (!html.empty())
	{
		return html;
	}

	std::cout << "Generating Calendar" << std::endl;
	stringstream htmlStream;

	// Create store for futures for generating each year asynchronously
	std::map<int, std::future<string>> htmlYearTasks;

	// Wrap up steps for getting a year in HTML in lambda
	std::function<string(const int)> getYear = [](const int selectedYear) {
		HTMLYear htmlYear(selectedYear);
		return htmlYear.GetHTML();;
	};

	// Populate and initiate async tasks
	for (int year : years)
	{
		// The future returned by async is stored against the correct year
		// NOTE: The returned future must be stored to avoid internal ~future() being called which is blocking
		htmlYearTasks.emplace(year, std::async(std::launch::async, getYear, year));
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
			<< htmlYearTasks.at(year).get()
			<< "</td>\n";
	}

	htmlStream << "</tr>\n"
		<< "</table>\n"
		<< "</body>\n"
		<< "</html>";

	// Cache output.
	html = htmlStream.str();
	return html;
}


void HTMLCalendar::WriteHTML()
{
	string htmlToWrite = GetHTML();

	try
	{
		std::cout << "Generating file: " << htmlName << std::endl;
		htmlFile.open(htmlName);
		htmlFile << htmlToWrite;
		htmlFile.close();
		std::cout << "Generation Complete" << std::endl;
	}
	catch (const std::ofstream::failure& e)
	{
		std::cout << "Exception thrown when generating the file: " << htmlName << " Exception: " << e.what() << std::endl;
	}
}

string HTMLCalendar::CreateHTMLHeader(const string& title) const
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
