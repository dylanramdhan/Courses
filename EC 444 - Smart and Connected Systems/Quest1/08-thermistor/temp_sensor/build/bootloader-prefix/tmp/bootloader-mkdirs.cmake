# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/Users/dylanramdhan/esp/esp-idf/components/bootloader/subproject"
  "/Users/dylanramdhan/Documents/Visual Studio Code/EC444/Quest1/08-thermistor/temp_sensor/build/bootloader"
  "/Users/dylanramdhan/Documents/Visual Studio Code/EC444/Quest1/08-thermistor/temp_sensor/build/bootloader-prefix"
  "/Users/dylanramdhan/Documents/Visual Studio Code/EC444/Quest1/08-thermistor/temp_sensor/build/bootloader-prefix/tmp"
  "/Users/dylanramdhan/Documents/Visual Studio Code/EC444/Quest1/08-thermistor/temp_sensor/build/bootloader-prefix/src/bootloader-stamp"
  "/Users/dylanramdhan/Documents/Visual Studio Code/EC444/Quest1/08-thermistor/temp_sensor/build/bootloader-prefix/src"
  "/Users/dylanramdhan/Documents/Visual Studio Code/EC444/Quest1/08-thermistor/temp_sensor/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/dylanramdhan/Documents/Visual Studio Code/EC444/Quest1/08-thermistor/temp_sensor/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/dylanramdhan/Documents/Visual Studio Code/EC444/Quest1/08-thermistor/temp_sensor/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
