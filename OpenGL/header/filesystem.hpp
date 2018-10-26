#ifndef FILESYSTEM_HPP_
#define FILESYSTEM_HPP_

#include <iostream>
#include <fstream>
#include <sstream>

#define BOOST_FILESYSTEM_NO_DEPRECATED
#include <boost/filesystem.hpp>
namespace bf = boost::filesystem;

class FileSystem {
protected:
	static bool IsDirectory(bf::path path);
	static bool IsDirectory(const std::string &path);

	static bool IsFile(bf::path path);
	static bool IsFile(const std::string &path);

	static bool Exists(bf::path path);
	static bool Exists(const std::string &path);
public:
	FileSystem() = delete;
	static bf::path GetShaderPath();
	static std::string GetString(bf::path path);
};
#endif // FILESYSTEM_HPP_
