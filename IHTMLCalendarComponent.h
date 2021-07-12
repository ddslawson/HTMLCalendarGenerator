#pragma once

#include <string>
#include <sstream>

using std::string;
using std::stringstream;

class IHTMLCalendarComponent
{
public:
	virtual ~IHTMLCalendarComponent() {};
	virtual string GetHTML() = 0;
};
