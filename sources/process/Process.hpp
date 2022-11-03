#ifndef PROCESS_HPP
#define PROCESS_HPP

#include "utils/libraries.hpp"
#include "utils/utils.hpp"
#include "utils/FileDescriptors.hpp"

class Job;

/**
 * @brief Class representing a process launched
 *
 */
class Process
{
private:

	Job*			parent;	// The parent Job
	pid_t			pid;	// Process ID
	int				status;	// Reported status value
	FileDescriptors	fd;		// File descriptors

public:

	/**
	 * @brief Construct a process
	 *
	 */
	Process();

	/**
	 * @brief Construct a process form another
	 *
	 * @param other the process to copy
	 */
	Process(const Process& other) = default;

	/**
	 * @brief Construct a process from these arguments
	 *
	 * @param arguments the input arguments
	 * @param child true if the process will be launch in a child process
	 * @param parent the Job parent
	 */
	Process(const std::vector<std::string>& arguments, bool child, Job* parent);

	/**
	 * @brief Construct a process from theses arguments and file descriptors
	 *
	 * @param arguments the input arguments
	 * @param fd the file descriptors
	 * @param child true if the process will be launch in a child process
	 * @param parent the Job parent
	 */
	Process(const std::vector<std::string>& arguments, FileDescriptors fd, bool child, Job* parent);

	/**
	 * @brief Equal operator
	 *
	 * @param other the process to copy
	 * @return A reference to the current process
	 */
	Process& operator=(const Process& other) = default;

	/**
	 * @brief Launch the process from these arguments
	 *
	 * @param arguments the input arguments
	 * @param child true if the process will be launch in a child process
	 */
	void launch(std::vector<std::string> arguments, bool child);

	/**
	 * @brief Launch the process from theses arguments and file descriptors
	 *
	 * @param arguments the input arguments
	 * @param fd the file descriptors
	 * @param child true if the process will be launch in a child process
	 */
	void launch(std::vector<std::string> arguments, FileDescriptors fd, bool child);

	/**
	 * @brief Change file descriptors to handle redirections in the command
	 *
	 * @param arguments the input arguments
	 * @return the arguments without redirections
	 */
	std::vector<std::string> redirections(const std::vector<std::string>& arguments);

	/**
	 * @brief Get the pid of the process
	 *
	 * @return the pid of the process
	 */
	pid_t get_pid() const;

	/**
	 * @brief Set the pid of the process
	 *
	 * @param pid the new pid
	 */
	void set_pid(pid_t pid);

	/**
	 * @brief Get the status of the process
	 *
	 * @return the status of the process
	 */
	int get_status() const;

	/**
	 * @brief Set the status of the process
	 *
	 * @param status the new status
	 */
	void set_status(int status);

	/**
	 * @brief The cd command of Shell
	 *
	 * @param arguments "cd" and the file to move
	 * @param child true if it's in the child process, false otherwise
	 * @return false on failure and true otherwise
	 */
	void cd(const std::vector<std::string>& arguments, bool child);

	/**
	 * @brief The cp command of Shell
	 *
	 * @param arguments "cp" and the file to copy
	 * @param child true if it's in the child process, false otherwise
	 * @return false on failure and true otherwise
	 */
	void cp(const std::vector<std::string>& arguments, bool child);

	/**
	 * @brief The mkdir command of Shell
	 *
	 * @param arguments "mkdir" and the directory to create
	 * @param child true if it's in the child process, false otherwise
	 * @return false on failure and true otherwise
	 */
	void mkdir(const std::vector<std::string>& arguments, bool child);

	/**
	 * @brief Run a binary
	 *
	 * @param arguments the name of the binary and its arguments
	 * @return false on failure and true otherwise
	 */
	void binary(const std::vector<std::string>& arguments);

	/**
	 * @brief Wait until the child process stop
	 *
	 */
	void wait();

	/**
	 * @brief Check if the process finished
	 *
	 * @return true if the process finished
	 */
	bool is_finished();

	/**
	 * @brief Close file descriptors
	 *
	 */
	void close_fd();
};

#endif
