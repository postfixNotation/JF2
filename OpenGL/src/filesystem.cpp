#include <filesystem.hpp>

bool FileSystem::IsDirectory(bf::path path) { return bf::is_directory(path); }
bool FileSystem::IsDirectory(const std::string &path) { return bf::is_directory(bf::path(path)); }

bool FileSystem::IsFile(bf::path path) { return bf::is_regular_file(path); }
bool FileSystem::IsFile(const std::string &path) { return bf::is_regular_file(bf::path(path)); }

bool FileSystem::Exists(bf::path path) { return bf::exists(path); }
bool FileSystem::Exists(const std::string &path) { return bf::exists(bf::path(path)); }

bf::path FileSystem::GetShaderPath() {
	bf::path path = bf::current_path().parent_path();
	path /=bf::path("shader"); 
	return path;
}

std::string FileSystem::GetString(bf::path path) {
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
