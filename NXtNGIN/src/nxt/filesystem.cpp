#include "filesystem.hpp"

namespace nxt {
	FileSystem& FileSystem::Instance() {
		static std::unique_ptr<FileSystem> instance{ std::unique_ptr<FileSystem>(new FileSystem()) };
		return *instance;
	}

	FileSystem::FileSystem() : resource_root_dir_{ bf::current_path().parent_path() } {}

	bool FileSystem::IsDirectory(const bf::path &path) { return bf::is_directory(path); }

	bool FileSystem::IsFile(const bf::path &path) { return bf::is_regular_file(path); }

	bool FileSystem::Exists(const bf::path &path) { return bf::exists(path); }

	bf::path FileSystem::GetPath(const std::string &subdirname) {
		assert(directories_.find(subdirname) != directories_.end());
		return directories_[subdirname];
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
		resource_root_dir_ /= path;
		assert(Exists(resource_root_dir_) && IsDirectory(resource_root_dir_));
		return resource_root_dir_;
	}

	const bf::path& FileSystem::SetResourceSubDir(const std::string &name) {
		directories_[name] = resource_root_dir_;
		directories_[name] /= (bf::path(name) += "/");
		assert(Exists(directories_[name]) && IsDirectory(directories_[name]));
		return directories_[name];
	}

	void FileSystem::InitSubDirs(const std::vector<std::string> &sub_dir_list) {
		for (const std::string &subdir : sub_dir_list) {
			SetResourceSubDir(subdir);
		}
	}
}
