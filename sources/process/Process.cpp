#include "shell/Shell.hpp"
#include "process/Process.hpp"

Process::Process()
{
	pid = 0;
	status = 0;
	parent = nullptr;
}

Process::Process(const std::vector<std::string>& arguments, bool child, Job* parent)
{
	pid = 0;
	status = 0;
	this->parent = parent;
	launch(arguments, FileDescriptors(), child);
}

Process::Process(const std::vector<std::string>& arguments, FileDescriptors fd, bool child, Job* parent)
{
	pid = 0;
	status = 0;
	this->parent = parent;
	launch(arguments, fd, child);
}

void Process::launch(std::vector<std::string> arguments, bool child)
{
	// Handle redirections
	arguments = redirections(arguments);

	// No argument case
	if (arguments.empty())
		throw std::invalid_argument("no argument");

	// Builtins on parent process
	if (!child)
	{
		// Cd
		if (arguments.front() == "cd")
		{
			cd(arguments, false);
			close_fd();
			return;
		}

		// Cp
		else if (arguments.front() == "cp")
		{
			cp(arguments, false);
			close_fd();
			return;
		}

		// Mkdir
		else if (arguments.front() == "mkdir")
		{
			mkdir(arguments, false);
			close_fd();
			return;
		}
	}

	// Fork
	pid = fork();

	// Fork error case
	if (pid == -1)
	{
		perror("fork");
		throw std::runtime_error("fork failed");
	}

	// Child case
	else if (pid == 0)
	{
		// Change file descriptors of the process
		if (fd.in != STDIN_FILENO && dup2(fd.in, STDIN_FILENO) == -1)
			exit(errno);

		if (fd.out != STDOUT_FILENO && dup2(fd.out, STDOUT_FILENO) == -1)
			exit(errno);

		// Close file descriptors
		close_fd();
		parent->close_fd();

		// Cd
		if (arguments.front() == "cd")
			cd(arguments, true);

		// Cp
		else if (arguments.front() == "cp")
			cp(arguments, true);

		// Mkdir
		else if (arguments.front() == "mkdir")
			mkdir(arguments, true);

		// Binary
		else
			binary(arguments);

		exit(EXIT_SUCCESS);
	}

	// Parent case
	close_fd();
}

void Process::launch(std::vector<std::string> arguments, FileDescriptors fd, bool child)
{
	this->fd = fd;
	launch(arguments, child);
}

std::vector<std::string> Process::redirections(const std::vector<std::string>& arguments)
{
	// Syntax error case
	if (arguments.empty() || arguments.back() == ">" || arguments.back() == "<" || arguments.back() == ">>")
		throw std::invalid_argument("syntax error");

	std::vector<std::string> result = {};

	// Change file descriptors and remove redirections
	for (int i = 0; i < arguments.size(); i++)
	{
		if (arguments[i] == ">")
		{
			if (arguments[i + 1] == ">" || arguments[i + 1] == "<" || arguments[i + 1] == ">>")
				throw std::invalid_argument("syntax error");

			if (fd.out != STDOUT_FILENO)
				close(fd.out);

			fd.out = open(arguments[i + 1].data(), O_CREAT | O_WRONLY | O_TRUNC, 0664);

			if (fd.out == -1)
			{
				perror("open");
				throw std::runtime_error("open");
			}

			i++;
		}

		else if (arguments[i] == "<")
		{
			if (arguments[i + 1] == ">" || arguments[i + 1] == "<" || arguments[i + 1] == ">>")
				throw std::invalid_argument("syntax error");

			if (fd.in != STDIN_FILENO)
				close(fd.in);

			fd.in = open(arguments[i + 1].data(), O_RDONLY);

			if (fd.in == -1)
			{
				perror("open");
				throw std::runtime_error("open");
			}

			i++;
		}

		else if (arguments[i] == ">>")
		{
			if (arguments[i + 1] == ">" || arguments[i + 1] == "<" || arguments[i + 1] == ">>")
				throw std::invalid_argument("syntax error");

			if (fd.out != STDOUT_FILENO)
				close(fd.out);

			fd.out = open(arguments[i + 1].data(), O_CREAT | O_WRONLY | O_APPEND, 0664);

			if (fd.out == -1)
			{
				perror("open");
				throw std::runtime_error("open");
			}

			i++;
		}

		else
			result.push_back(arguments[i]);
	}

	return result;
}

pid_t Process::get_pid() const
{
	return pid;
}

void Process::set_pid(pid_t pid)
{
	this->pid = pid;
}

int Process::get_status() const
{
	return status;
}

void Process::set_status(int status)
{
	this->status = status;
}

void Process::wait()
{
	waitpid(pid, &status, 0);
}

bool Process::is_finished()
{
	return waitpid(pid, &status, WNOHANG);
}

void Process::close_fd()
{
	if (fd.in != STDIN_FILENO)
		close(fd.in);

	if (fd.out != STDOUT_FILENO)
		close(fd.out);
}
