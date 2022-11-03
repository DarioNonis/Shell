#include "process/Process.hpp"

void Process::mkdir(const std::vector<std::string>& arguments, bool child)
{
	// Too many arguments case
	if (arguments.size() > 2)
		show_error("mkdir: too many arguments", child);

	// Missing argument case
	else if (arguments.size() < 2)
		show_error("mkdir: missing directory name", child);

	else
		::mkdir(arguments.back().data(), 0777);
}
