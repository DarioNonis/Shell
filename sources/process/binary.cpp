#include "process/Process.hpp"

void Process::binary(const std::vector<std::string>& arguments)
{
	// Replace the process by the binary
	execvp(arguments.front().data(), to_char_array(arguments));

	if (errno == ENOENT)
		std::cerr << arguments.front() << ": command not found" << std::endl;

	else
		perror(arguments.front().data());

	exit(EXIT_FAILURE);
}
