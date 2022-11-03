#include "process/Process.hpp"

void Process::cd(const std::vector<std::string>& arguments, bool child)
{
	std::string old_pwd = std::string(std::filesystem::current_path());
	std::string new_pwd;

	// Too many arguments case
	if (arguments.size() > 2)
		show_error("cd: too many arguments", child);

	// Handle cd without argument
	else if (arguments.size() == 1)
	{
		if (std::getenv("HOME") == nullptr)
			show_error("cd: HOME not set", child);

		new_pwd = getenv("HOME");
	}

	// Handle go back
	else if (arguments.back() == "-")
	{
		if (std::getenv("OLDPWD") == nullptr)
			show_error("cd: OLDPWD not set", child);

		new_pwd = getenv("OLDPWD");
		write(fd.out, (new_pwd + '\n').data(), new_pwd.size() + 1);
	}

	// Handle going in a new directory
	else
		new_pwd = arguments.back();

	try
	{
		std::filesystem::current_path(new_pwd);
	}

	catch (...)
	{
		show_error("cd: " + arguments.back() + ": No such file or directory", child);
	}

	setenv("OLDPWD", old_pwd.data(), 1);
	setenv("PWD", std::string(std::filesystem::current_path()).data(), 1);
}
