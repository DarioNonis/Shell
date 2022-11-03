#include "shell/Shell.hpp"

std::list<Job> Shell::job_list = {};

void Shell::show_prompt()
{
	std::cout << "\033[1;36m" << std::string(std::filesystem::current_path()) << ">\033[0m " << std::flush;
}

std::vector<std::string> Shell::separate_parts(std::string command)
{
	// Empty command case
	if (command.empty())
		return {};

	// Add spaces around pipes/redirections
	for (int i = 0; i < command.size(); i++)
	{
		if (command[i] == '>' || command[i] == '<' || command[i] == '|' || command[i] == '&')
		{
			if (i + 1 < command.size() && command[i] == '>' && command[i + 1] == '>')
			{
				command.insert(i + 2, 1, ' ');
				command.insert(i, 1, ' ');
				i += 1;
			}

			else
			{
				command.insert(i + 1, 1, ' ');
				command.insert(i, 1, ' ');
			}

			i += 2;
		}
	}

	command.push_back(' ');
	std::vector<std::string> result = {};

	while (true)
	{
		// Remove start spaces
		while (command.front() == ' ' || command.front() == '\t')
			command = command.substr(1);

		if (command.empty())
			break;

		// Add word in the array
		size_t end = command.find_first_of(" \t");

		// Last word case
		if (end == std::string::npos)
		{
			result.push_back(command);
			break;
		}

		result.push_back(command.substr(0, end));
		command = command.substr(end);

		if (command.empty())
			break;
	}

	return result;
}

void Shell::launch_command(std::vector<std::string> arguments)
{
	bool foreground = true;

	if (std::count(arguments.begin(), arguments.end(), "&") > 0)
	{
		if (std::count(arguments.begin(), arguments.end(), "&") > 1 || arguments.back() != "&")
			throw std::invalid_argument("syntax error");

		foreground = false;
		arguments.pop_back();
	}

	Shell::clear_jobs();
	job_list.emplace_back(arguments, foreground);
}

void Shell::clear_jobs()
{
	for (auto it = job_list.begin(); it != job_list.end(); it++)
		if (it->is_finished())
			it = job_list.erase(it);
}
