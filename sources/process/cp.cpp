#include "process/Process.hpp"

/**
 * @brief Check if two paths are the same file
 *
 * @param input_file the input file path
 * @param output_file the output file path
 * @param child true if the process is in a child process
 */
void check_same_files(const std::filesystem::path& input_file, const std::filesystem::path& output_file, bool child)
{
	std::string input_path = std::filesystem::absolute(input_file);
	std::string output_path;

	try
	{
		output_path = std::filesystem::absolute(output_file);
	}

	catch (...)
	{
		return;
	}

	if (input_path == output_path)
	{
		if (std::filesystem::is_directory(input_file))
			show_error("cp: input directory is output directory", child);

		else
			show_error("cp: input file is output file", child);
	}
}

/**
 * @brief Copy a file
 *
 * @param input_file the input file name
 * @param output_file the output file name
 */
void copy_file(const std::string& input_file, const std::string& output_file)
{
	// Open source and destination files
	int src_desc = open(input_file.data(), O_RDONLY);
	int dest_desc = open(output_file.data(), O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);

	// Check open errors
	if (src_desc < 0 || dest_desc < 0)
	{
		perror("open");
		exit(errno);
	}

	// Get informations from the source file
	struct stat input_stat;

	if (fstat(src_desc, &input_stat) < 0)
	{
		perror("fstat");
		exit(errno);
	}

	// Copy permissions
	if (fchmod(dest_desc, input_stat.st_mode) < 0)
	{
		perror("fchmod");
		exit(errno);
	}

	while (true)
	{
		// Copy data from the input file
		char buffer[4096];
		int read_count = read(src_desc, buffer, sizeof(buffer));

		// Check read errors
		if (read_count < 0)
		{
			perror("read");
			exit(errno);
		}

		if (read_count == 0)
			break;

		// Paste data in output file
		write(dest_desc, buffer, read_count);
	}

	// Close source and destination files
	close(src_desc);
	close(dest_desc);
}

/**
 * @brief Join a directory path with a file name
 *
 * @param directory the directory path
 * @param file the file name
 * @return the full path to the file
 */
std::string path_join(const std::string& directory, const std::string& file)
{
	return directory + (directory.back() == '/' ? "" : "/") + file;
}

/**
 * @brief Copy a directory (exit on failure)
 *
 * @param input_directory the input directory name
 * @param output_directory the output directory name
 */
void copy_directory(const std::string& input_directory, const std::string& output_directory)
{
	// Open the directory
	struct stat input_stat;
	struct dirent* ent;
	DIR* directory = opendir(input_directory.c_str());

	// Check opendir errors
	if (directory == NULL)
	{
		perror("opendir");
		exit(errno);
	}

	// Get informations from the source directory
	if (stat(input_directory.c_str(), &input_stat) < 0)
	{
		perror("stat");
		exit(errno);
	}

	// Create the output directory
	if (mkdir(output_directory.c_str(), S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IXGRP | S_IROTH | S_IWOTH | S_IXOTH) < 0)
	{
		perror("mkdir");
		exit(errno);
	}

	// Copy permissions
	if (chmod(output_directory.c_str(), input_stat.st_mode) < 0)
	{
		perror("chmod");
		exit(errno);
	}

	// Get the first file/directory in the directory
	ent = readdir(directory);

	// Check readdir errors
	if (ent == NULL)
	{
		perror("readdir");
		exit(errno);
	}

	while (ent != NULL)
	{
		// Skip if the directory is . or ..
		if (strcmp(ent->d_name, ".") && strcmp(ent->d_name, ".."))
		{
			struct stat input_stat;
			std::string input_name = path_join(input_directory, ent->d_name);
			std::string output_name = path_join(output_directory, ent->d_name);
			stat(input_name.data(), &input_stat);

			// Copy the file or the directory
			if (S_ISDIR(input_stat.st_mode))
				copy_directory(input_name, output_name);

			else
				copy_file(input_name, output_name);
		}

		// Get the next file/directory in the directory
		ent = readdir(directory);
	}

	closedir(directory);
}

void Process::cp(const std::vector<std::string>& arguments, bool child)
{
	// Too many arguments case
	if (arguments.size() > 3)
		show_error("cp: too many arguments", child);

	// Missing argument case
	else if (arguments.size() < 3)
		show_error("cp: missing arguments", child);

	if (arguments.size() == 3)
	{
		// Check if source exists
		if (!std::filesystem::exists(arguments[1]))
			show_error("cp: cannot stat '" + arguments[1] + "': No such file or directory", child);

		// Check if source and destination are the sames
		check_same_files(arguments[1], arguments.back(), child);

		// Copy the file/directory
		if (std::filesystem::is_directory(arguments[1]))
			copy_directory(arguments[1], arguments.back());

		else
			copy_file(arguments[1], arguments.back());
	}
}


