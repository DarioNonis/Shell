#ifndef FILEDESCRIPTORS_HPP
#define FILEDESCRIPTORS_HPP

#include "utils/libraries.hpp"

/**
 * @brief Structure representing an input and output file descriptor
 *
 */
struct FileDescriptors
{
	int in;		// Input file descriptor
	int out;	// Output file descriptor

	/**
	 * @brief Construct file descriptors structure
	 *
	 */
	FileDescriptors();

	/**
	 * @brief Construct file descriptors structure from other
	 *
	 * @param fd the file descriptors structure to copy
	 */
	FileDescriptors(const FileDescriptors& fd) = default;

	/**
	 * @brief Construct file descriptors structure from input and output file descriptors
	 *
	 * @param in the input file descriptor
	 * @param out the output file descriptor
	 */
	FileDescriptors(int in, int out);

	/**
	 * @brief Equal operator
	 *
	 * @param fd the file descriptors structure to copy
	 * @return a reference to the current file descriptors structure
	 */
	FileDescriptors& operator=(const FileDescriptors& fd) = default;

	/**
	 * @brief Get a pointer to the structure data (for pipe(int*))
	 *
	 * @return the pointer to the structure data
	 */
	int* get_ptr();

	/**
	 * @brief Set the default file descriptors
	 *
	 */
	void clear();

	/**
	 * @brief Close the files descriptors
	 *
	 */
	void close();
};

#endif
