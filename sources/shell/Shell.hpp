#ifndef SHELL_HPP
#define SHELL_HPP

#include "utils/libraries.hpp"
#include "job/Job.hpp"
#include "utils/utils.hpp"

/**
 * @brief Class representing the shell
 *
 */
class Shell
{
public:

	static std::list<Job> job_list; // List of active jobs in the shell

	/**
	 * @brief Show the prompt
	 *
	 */
	static void show_prompt(); // Show the prompt

	/**
	 * @brief Separate all key words of an input command
	 *
	 * @param command the input command
	 * @return an array of key words
	 */
	static std::vector<std::string> separate_parts(std::string command);

	/**
	 * @brief Parse and execute the input command
	 *
	 * @param arguments an array of key words (see Shell::separate_parts(std::string))
	 */
	static void launch_command(std::vector<std::string> arguments);

	/**
	 * @brief Clear all finished jobs
	 *
	 */
	static void clear_jobs();
};

#endif
