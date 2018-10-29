#include <filesystem.hpp>

std::map<std::string, bf::path> FileSystem::directories;
bf::path FileSystem::resource_root_dir{ bf::current_path().parent_path() };

bf::path FileSystem::GetPath(const std::string &subdirname) {
	assert(directories.find(subdirname) != directories.end());
	return directories[subdirname];
}

std::string FileSystem::GetPathString(const std::string &subdirname) {
	return GetPath(subdirname).generic_string();
}

std::string FileSystem::GetContent(bf::path path) {
	if (IsFile(path) && Exists(path)) {
		std::ifstream ifs;
		std::ostringstream ss;
		try {
			ifs.open(path.generic_string(), std::ifstream::in);

			if (!ifs.fail()) {
				ss << ifs.rdbuf();
			}

			ifs.close();
		}
		catch (const std::exception ex) {
			std::cout << "EXCEPTION: LOADING SHADER SOURCE" << std::endl;
			std::cout << ex.what() << std::endl;
		}
		return ss.str();
	}
	return "";
}

std::string FileSystem::GetContent(const std::string &path) {
	return GetContent(bf::path(path));
}

const bf::path& FileSystem::SetResourceRootDir(bf::path path) {
	resource_root_dir /= path;
	assert(Exists(resource_root_dir) && IsDirectory(resource_root_dir));
	return resource_root_dir;
}

const bf::path& FileSystem::SetResourceSubDir(const std::string &name) {
	directories[name] = resource_root_dir;
	directories[name] /= (bf::path(name)+="/");
	assert(Exists(directories[name]) && IsDirectory(directories[name]));
	return directories[name];
}

void FileSystem::InitSubDirs(const std::vector<std::string> &sub_dir_list) {
	for (const std::string &subdir : sub_dir_list) {
		SetResourceSubDir(subdir);
	}
}