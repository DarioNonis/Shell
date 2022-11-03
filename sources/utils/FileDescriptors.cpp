#include "utils/FileDescriptors.hpp"

FileDescriptors::FileDescriptors()
{
	clear();
}

FileDescriptors::FileDescriptors(int in, int out)
{
	this->in = in;
	this->out = out;
}

int* FileDescriptors::get_ptr()
{
	return &in;
}

void FileDescriptors::clear()
{
	in = STDIN_FILENO;
	out = STDOUT_FILENO;
}

void FileDescriptors::close()
{
	::close(in);
	::close(out);
}
