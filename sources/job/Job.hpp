#ifndef JOB_HPP
#define JOB_HPP

#include "utils/libraries.hpp"
#include "process/Process.hpp"

/**
 * @brief Class representing a list of process launched
 *
 */
class Job
{
private:

	int							number;			// Id in the background list
	std::string					arguments;		// Arguments of the job
	bool						foreground;		// true if the job is in foreground
	std::list<Process>			process_list;	// List of active process in the job
	std::list<FileDescriptors>	fd_list;		// List of active pipes

public:

	/**
	 * @brief Construct a new job
	 *
	 */
	Job() = default;

	/**
	 * @brief Construct a job from another
	 *
	 * @param other the job to copy
	 */
	Job(const Job& other) = default;

	/**
	 * @brief Construct a job and start it from these arguments
	 *
	 * @param arguments the arguments of the job
	 * @param foreground true if the job is launched in foreground
	 */
	Job(const std::vector<std::string>& arguments, bool foreground);

	/**
	 * @brief Equal operator
	 *
	 * @param other the job to copy
	 * @return a reference to the current job after the copy
	 */
	Job& operator=(const Job& other) = default;

	/**
	 * @brief Start the job from theses arguments
	 *
	 * @param arguments the arguments of the job
	 * @param foreground true if the job is launched in foreground
	 */
	void launch(const std::vector<std::string>& arguments, bool foreground);

	/**
	 * @brief Wait for all the process of the job to stop
	 *
	 */
	void wait();

	/**
	 * @brief Check if the job finished
	 *
	 * @return true if the job finished
	 */
	bool is_finished();

	/**
	 * @brief Close all opened file descriptors
	 *
	 */
	void close_fd();
};

#endif
