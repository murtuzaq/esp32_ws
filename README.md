# esp32_ws


#### Development Environment
# Virtual Machine VMWARE Workstation 15.x
# Operating System Ubuntu 18.04 64 bit

##(1) ESP IDF Prerequisites
# ESP-IDF Programming Guide, Standard Setup of Toolchain for Linux 
# https://docs.espressif.com/projects/esp-idf/en/latest/get-started/linux-setup.html 
sudo apt-get install git wget flex bison gperf python python-pip python-setuptools python-serial python-click python-cryptography python-future python-pyparsing python-pyelftools cmake ninja-build ccache libffi-dev libssl-dev

##(2) ESP IDF
# ESP-IDF Programming Guide, Step 2. Get ESP-IDF
# Linux and macOS
# https://docs.espressif.com/projects/esp-idf/en/latest/get-started/index.html 
cd ~/esp
git clone --recursive https://github.com/espressif/esp-idf.git

## (3) ECLIPSE Prerequisites 
# Espressif IDF Eclipse Plugins, Installing Prerequisites
# https://github.com/espressif/idf-eclipse-plugin/blob/master/README.md
# Get Java 8 JRE
sudo apt install openjdk-8-jre-headless
sudo apt install openjdk-8-jdk
# Download Eclipse 2018-12 IDE Tar ball for linux
# https://www.eclipse.org/downloads/packages/release/2018-12/r/eclipse-ide-cc-developers

## (4) ECLIPSE CDT IDE FOR ESPRESSIF ESP32 IDF Integration
# https://github.com/espressif/idf-eclipse-plugin/blob/master/README.md
# Install Espressif IDF Plugins for Eclipse
# Espressif IDF Tool Manager, Install Tools.

## (5) Adding user to dialout on Linux for Serial Flashing, and Serial Monitoring
sudo usermod -a -G dialout $USER
