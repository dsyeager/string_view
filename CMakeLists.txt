cmake_minimum_required(VERSION 3.1...3.29)

project(
  StringView
  VERSION 1.0
  LANGUAGES CXX)

# purposely not adding the misc submodule here
# since misc also links to string_view

add_subdirectory(tests)
