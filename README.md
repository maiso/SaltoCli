# SaltoCli
Assignment to write a CLI tool for salto

## How to build the project

1. First install python3 and ninja: `sudo apt-get install python3 python3-pip python3-setuptools python3-wheel ninja-build libboost-all-dev`

2. Then install the buildsstem meson: `pip3 install --user meson`

3. Navigate to the root directory of the project and execute: 
    1. To install gtest: `meson wrap install gtest`
    2. To install json lib: `meson wrap install nlohmann_json`
    3. To configure meson: `meson builddir`

4. Navigate to the builddir and execute `ninja`

5. The application will now be build and can be run by executing: `./src/SaltoCLI`

6. Running the unit tests can be done by `ninja test`

