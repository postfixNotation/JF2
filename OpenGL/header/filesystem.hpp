#ifndef FILESYSTEM_HPP_
#define FILESYSTEM_HPP_

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

#define BOOST_FILESYSTEM_NO_DEPRECATED
#include <boost/filesystem.hpp>
namespace bf = boost::filesystem;

class FileSystem {
protected:
	static std::map<std::string, bf::path> directories;
	static bf::path resource_root_dir;

	static bool IsDirectory(const bf::path &path) {
		return bf::is_directory(path);
	}

	static bool IsFile(const bf::path &path) {
		return bf::is_regular_file(path);
	}

	static bool Exists(const bf::path &path) {
		return bf::exists(path);
	}

public:
	FileSystem() = delete;
	static bf::path GetPath(const std::string &subdirname);
	static std::string GetPathString(const std::string &subdirname);

	static std::string GetContent(bf::path path);
	static std::string GetContent(const std::string &path);

	static const bf::path& SetResourceRootDir(bf::path path);
	static const bf::path& SetResourceSubDir(const std::string &name);
};
#endif // FILESYSTEM_HPP_
