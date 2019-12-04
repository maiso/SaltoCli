# SaltoCli
Assignment to write a CLI tool for salto

## How to build the project

First install python3 and ninja: `sudo apt-get install python3 python3-pip python3-setuptools python3-wheel ninja-build`

Then install the buildsstem meson: `pip3 install --user meson`

Navigate to the root directory of the project and execute: `meson builddir`

Navigate to the builddir and execute `ninja`

The application will now be build and can be run by executing: `./src/SaltoCLI`

