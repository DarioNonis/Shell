#include "process/Process.hpp"
#include "job/Job.hpp"
#include "shell/Shell.hpp"

int main()
{
	// App loop
	while (true)
	{
		// Show prompt
		Shell::show_prompt();

		// Get the user command
		std::string command;
		getline(std::cin, command);

		// Separate command parts
		std::vector<std::string> arguments = Shell::separate_parts(command);

		// Only enter case
		if (arguments.empty())
			continue;

		// Exit command
		if (arguments.front() == "exit")
			break;

		// Parse and execute the command
		try
		{
			Shell::launch_command(arguments);
		}

		catch (const std::invalid_argument& e)
		{
			if (e.what() != "no argument")
				std::cerr << e.what() << std::endl;
		}

		catch (const std::runtime_error& e) {}
	}

	return EXIT_SUCCESS;
}
