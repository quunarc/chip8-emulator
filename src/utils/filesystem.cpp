#include "filesystem.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

#ifdef K_PLATFORM_WINDOWS
#include <windows.h>
#endif

namespace Klein {

namespace FileSystem
{

static std::filesystem::path project_directory_cached;

std::optional<std::filesystem::path> get_executable_directory()
{
#ifdef K_PLATFORM_WINDOWS
    wchar_t path[MAX_PATH];
    DWORD length = GetModuleFileNameW(NULL, path, MAX_PATH);
    if (length == 0 || length == MAX_PATH) return std::nullopt;
    return std::filesystem::path(path).parent_path();
#else
    return std::filesystem::read_symlink("/proc/self/exe").parent_path();
#endif
}

std::optional<std::filesystem::path> get_project_directory()
{
    if (!project_directory_cached.empty())
        return project_directory_cached;

    std::filesystem::path current = get_executable_directory().value_or("");

    // there must be a engine.toml
    while (current.has_relative_path()) {
        if (std::filesystem::exists(current / "engine.toml")) {
            project_directory_cached = current;
            return current;
        }
        if (!current.has_parent_path()) break;
        current = current.parent_path();
    }

    return std::nullopt;
}

std::optional<std::string> read(std::string path)
{
    std::filesystem::path file_path = path;
    std::filesystem::path project_directory = get_project_directory().value_or("");
    std::filesystem::path file_path_absolute;

    if (file_path.is_relative())
        file_path_absolute = project_directory / file_path;
    else if (file_path.is_absolute())
        file_path_absolute = file_path;

    if (!std::filesystem::exists(file_path_absolute)) {
        return std::nullopt;
    }
    // binary mode for better byte accuracy
    // ate to seek to end of file
    std::ifstream file(file_path_absolute, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        return std::nullopt;
    }

    // Current position is at the end of file due to opening with `std::ios::ate`
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::string buffer;
    buffer.resize(static_cast<size_t>(size));

    if (file.read(buffer.data(), size)) {
        file.close();
        return buffer;
    }

    file.close();
    return std::nullopt;
}

std::optional<std::filesystem::path> get_absolute_path(std::filesystem::path file)
{
    if (file.is_relative())
        return (get_project_directory().value_or("") / file);
    else return file;
}

bool exists(std::filesystem::path file)
{
    return std::filesystem::exists(get_absolute_path(file).value_or(""));
}

void new_file(std::string name) {
    std::ofstream file(get_absolute_path(name)->string());
}

void new_directory(std::string name) {
    if (std::filesystem::create_directories(get_absolute_path(name)->string())) {
        std::cout << "Directory created successfully\n";
    } else {
        std::cout << "Directory already exists or could not be created\n";
    }
}


}

}
