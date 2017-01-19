# DankOS
A really simple Real Mode OS, in honour of youtuber Druaga1. http://dankos.org

# Building requirements
Currently, there is no Windows make script, so this only builds on GNU/Linux systems

In order to successfully compile the OS, these packages should be installed:

- build-essential
- nasm

In order to install them, run:

sudo apt-get install build-essential nasm

# Building instructions
The 'make_me.sh' bash script automatically builds the image, and compiles all the
sources from the 'sources' directory (C and Assembly).

All files/dirs from the 'extra' directory will also be copied onto the image.

# Support
To receive support, contribute, report bugs and more, feel free to join the
official DankOS Discord server: https://discord.gg/zzxBagX
