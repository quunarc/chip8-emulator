#pragma once
#include <filesystem>
#include <optional>

namespace FileSystem
{
    namespace fs = std::filesystem;

    // File Handling
    std::optional<fs::path>     get_project_directory();
    // return contents of file at path
    std::optional<std::string>  read(std::string path);
    std::optional<fs::path>     get_absolute_path(fs::path file);
    std::optional<fs::path>     get_executable_directory();
    bool                        exists(fs::path file);

    void                        new_file(std::string name);
    void                        new_directory(std::string name);

    // Strings



    // TODO: Implement this method
    void        write(std::string path, std::string content);
}
