#pragma once
#include <string>
#include <vector>

namespace gl_classes {

// https://stackoverflow.com/a/2548212
std::string& replace_string_inplace(std::string& str, const std::string& remove, const std::string& insert);
std::string& replace_strings_inplace(std::string& str, const std::vector<std::string>& remove, const std::vector<std::string>& insert);

} // namespace gl_classes
