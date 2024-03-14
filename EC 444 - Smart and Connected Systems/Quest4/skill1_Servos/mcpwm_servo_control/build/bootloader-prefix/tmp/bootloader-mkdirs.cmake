# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/Users/dylanramdhan/esp/esp-idf/components/bootloader/subproject"
  "/Users/dylanramdhan/Documents/Visual Studio Code/EC444/Quest 4/Skill 1 - Servos/mcpwm_servo_control/build/bootloader"
  "/Users/dylanramdhan/Documents/Visual Studio Code/EC444/Quest 4/Skill 1 - Servos/mcpwm_servo_control/build/bootloader-prefix"
  "/Users/dylanramdhan/Documents/Visual Studio Code/EC444/Quest 4/Skill 1 - Servos/mcpwm_servo_control/build/bootloader-prefix/tmp"
  "/Users/dylanramdhan/Documents/Visual Studio Code/EC444/Quest 4/Skill 1 - Servos/mcpwm_servo_control/build/bootloader-prefix/src/bootloader-stamp"
  "/Users/dylanramdhan/Documents/Visual Studio Code/EC444/Quest 4/Skill 1 - Servos/mcpwm_servo_control/build/bootloader-prefix/src"
  "/Users/dylanramdhan/Documents/Visual Studio Code/EC444/Quest 4/Skill 1 - Servos/mcpwm_servo_control/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/dylanramdhan/Documents/Visual Studio Code/EC444/Quest 4/Skill 1 - Servos/mcpwm_servo_control/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/dylanramdhan/Documents/Visual Studio Code/EC444/Quest 4/Skill 1 - Servos/mcpwm_servo_control/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
