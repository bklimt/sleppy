sleppy
======

Simple program for generating white noise on a Raspberry Pi.

# Setup

    sudo apt-get install libasound2-dev libasound2

# Build

    make

# Run

    /bin/sleppy hw:0,0

# Configure

To change the volume, do:

    amixer sset PCM 100%
