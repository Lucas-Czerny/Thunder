#pragma once

#include <string>

#define FILE_DIALOG_FILTER(tag, extension) #tag "(*" extension ")\0*" extension "\0)"

namespace Thunder { namespace FileDialog
{
	fs::path OpenFileDialog(const char* filter);
	fs::path SaveFileDialog(const char* filter);

	fs::path OpenFolderDialog();
} }