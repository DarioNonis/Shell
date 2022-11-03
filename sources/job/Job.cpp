#include "job/Job.hpp"
#include "shell/Shell.hpp"

Job::Job(const std::vector<std::string>& arguments, bool foreground)
{
	process_list.clear();
	launch(arguments, foreground);
}

void Job::launch(const std::vector<std::string>& arguments, bool foreground)
{
	this->foreground = foreground;

	// Syntax error case
	if (arguments.empty() || arguments.front() == "|" || arguments.back() == "|")
		throw std::invalid_argument("syntax error");

	// Set arguments
	for (int i = 0; i < arguments.size(); i++)
		this->arguments += arguments[i] + " ";

	this->arguments.pop_back();

	int nb_pipes = std::count(arguments.begin(), arguments.end(), "|");
	std::vector<FileDescriptors> fd_list = {};

	// Pipes case
	if (nb_pipes > 0)
	{
		// Open all pipes
		for (int i = 0; i < nb_pipes; i++)
		{
			fd_list.emplace_back();

			if (pipe(fd_list.back().get_ptr()) == -1)
			{
				perror("pipe");
				throw std::runtime_error("pipe");
			}
		}

		std::vector<std::string> process_arguments = {};
		int pipe_i = 0;

		// Launch all process
		for (int i = 0; i < arguments.size(); i++)
		{
			if (arguments[i] == "|")
			{
				if (process_arguments.empty())
					throw std::invalid_argument("syntax error");

				int fd_in = (pipe_i == 0 ? STDIN_FILENO : fd_list[pipe_i - 1].in);
				int fd_out = fd_list[pipe_i].out;

				process_list.emplace_back(process_arguments, FileDescriptors(fd_in, fd_out), true, this);
				process_arguments.clear();
				pipe_i++;
			}

			else
				process_arguments.push_back(arguments[i]);
		}

		process_list.emplace_back(process_arguments, FileDescriptors(fd_list.back().in, STDOUT_FILENO), true, this);
		close_fd();
	}

	// No pipes case
	else
		process_list.emplace_back(arguments, !foreground, this);

	// Wait until all process finished if in foreground
	if (foreground)
		wait();

	// Show pid of background process
	else
	{
		number = Shell::job_list.size() + 1;
		std::cout << "[" << number << "] (";
		int i = 0;

		for (Process& process : process_list)
		{
			if (i == process_list.size() - 1)
				std::cout << process.get_pid() << ")" << std::endl;

			else
				std::cout << process.get_pid() << ", ";

			i++;
		}
	}
}

void Job::wait()
{
	for (Process process : process_list)
		process.wait();
}

bool Job::is_finished()
{
	// Foreground process is always finished in this context
	if (foreground)
		return true;

	// Check all background process
	for (Process process : process_list)
		if (!process.is_finished())
			return false;

	std::cout << "[" << number << "]+ Done          " << arguments << std::endl;
	return true;
}

void Job::close_fd()
{
	for (auto fd : fd_list)
		fd.close();
}
