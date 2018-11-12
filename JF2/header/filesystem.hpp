#ifndef FILESYSTEM_HPP_
#define FILESYSTEM_HPP_

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <memory>
#include <map>

#define BOOST_FILESYSTEM_NO_DEPRECATED
#include <boost/filesystem.hpp>
namespace bf = boost::filesystem;

class FileSystem {
public:
	static FileSystem& Instance() {
		static std::unique_ptr<FileSystem> instance { std::unique_ptr<FileSystem>(new FileSystem()) };
		return *instance;
	}
	bf::path GetPath(const std::string &subdirname);
	std::string GetPathString(const std::string &subdirname);

	std::string GetContent(bf::path path);
	std::string GetContent(const std::string &path);

	const bf::path& SetResourceRootDir(bf::path path);
	const bf::path& SetResourceSubDir(const std::string &name);

	void InitSubDirs(const std::vector<std::string> &sub_dir_list);

private:
	FileSystem() {}
	std::map<std::string, bf::path> directories_;
	bf::path resource_root_dir_{ bf::current_path().parent_path() };

	bool IsDirectory(const bf::path &path) { return bf::is_directory(path); }

	bool IsFile(const bf::path &path) { return bf::is_regular_file(path); }

	bool Exists(const bf::path &path) { return bf::exists(path); }

};
#endif // FILESYSTEM_HPP_
