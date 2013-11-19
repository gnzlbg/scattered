cmake_minimum_required(VERSION 2.8.8)
project(catch_builder CXX)
include(ExternalProject)

ExternalProject_Add(catch
  GIT_REPOSITORY https://github.com/philsquared/Catch.git
  TIMEOUT 5
  INSTALL_COMMAND "" # Disable install step
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
)

# Specify include dir
ExternalProject_Get_Property(catch source_dir)
set(CATCH_INCLUDE_DIR ${source_dir}/include PARENT_SCOPE)
