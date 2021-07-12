#include "HTMLDay.h"

string HTMLDay::GetHTML()
{
	if (!html.empty())
	{
		return html;
	}

	stringstream dayHTML;
	dayHTML << "<td";
	// Apply additional colour to weekend days
	if (day == Day::Saturday)
	{
		dayHTML << " bgcolor=\"#62e0dc\">";
	}
	else if (day == Day::Sunday)
	{
		dayHTML << " bgcolor=\"#42afac\">";
	}
	else
	{
		dayHTML << ">";
	}

	dayHTML << date << "</td>\n";
	
	// Cache output.
	html = dayHTML.str();
	return html;
}
