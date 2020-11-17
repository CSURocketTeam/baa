
Sound card modem for Amateur Radio AX25
=======================================

Original Program [extmodem](https://github.com/alejolp/extmodem) By Alejandro Santos, LU4EXT / HB3YNQ. alejolp@gmail.com.

Modified by Kevin Schroeder, KE0SPF. Kmschr@gmail.com

For use in CSU Rocket Team 2020-2021

This is a modem for AFSK AX25 packet (1200 bps APRS compatible). It is capable to send packets using a CLI. It is called 'baa' because this is the sound that rams will output.

Usage
-----

Open a new terminal and ask for the program's help:

        ./baa -h
        
	Allowed options:
	  -h [ --help ]                     produce help message
	  -d [ --debug ]                    produce debug messages
	  -a [ --debugaudio ]               produce audio debug information
	  -L [ --list-devices ]             list audio source devices and exit
	  --config-file arg                 configuration file name
	  --audio-backend arg (=alsa)       Audio backend: alsa,loopback,portaudio,wave
					    ,null
	  --ptt-mode arg (=serial)          PTT mode: serial,parallel,gpio,hamlib,null
	  --ptt-port arg (=/dev/ttyS0)      set serial/parallel port PTT file name
	  --tx-delay arg (=666)             set tx-delay in ms
	  --tx-tail arg (=50)               set tx-tail in ms
	  -s [ --sample-rate ] arg (=22050) sample rate
	  --out-chan arg (=1)               output channel count
	  --frames-per-buffer arg (=8192)   frames per buffer, bigger increases latency
	  --hamlib-model arg (=-1)          hamlib Rig Model, -1 for auto detection
	  --backend-wave-in-file arg        Backend WAVE: input file
	  --alsa-out-dev arg (=default)     ALSA output device string, only for ALSA
					    backend (not PortAudio!)
	  --pa-out-dev arg (=-1)            PortAudio output device id number. See
					    --list-devices
	  -c [ --callsign ] arg             user's amateur radio callsign
	  --lat arg (=0000.00N)             latitude in format ddmm.mmN
	  --lng arg (=00000.00W)            longitude in format dddmm.mmW
	  --course arg                      track angle in degrees
	  --speed arg                       ground speed in knots
	  --alt arg                         altitude in feet


If using PTT,
You should at least set the PTT Port, Usually COMx on Windows and /dev/ttySX on Linux and Unix. You can run extmodem in receive-only mode by setting a dummy PTT mode with the argument --ptt-mode null.

### Selecting the sound device to use.

Using the ALSA backend you can use the --alsa-in-dev and --alsa-out-dev command line options to select specific input and output sound devices, ie, hw:1,0. This option only works with the ALSA backend, not PortAudio backend. To get a list of ALSA PCM input and output devices you can use the aplay and arecord ALSA command line utilities:

    $ arecord -L
    $ aplay -L

Use the complete line header as a device string. For example:

    $ arecord -L
    plughw:CARD=C920,DEV=0
        HD Pro Webcam C920, USB Audio
        Hardware device with all software conversions
    
    # ./baa --audio-backend=alsa --alsa-in-dev "plughw:CARD=C920,DEV=0"

The Portaudio backend uses by default the default sound device, which is usually the default ALSA device. To select a different PortAudio device you can first get a list of devices with the -L option:

    $ ./baa --audio-backend=portaudio --list-devices

Take note of the Device NUMBER and then use that number with the --pa-in-dev and --pa-out-dev options:

    $ ./baa --audio-backend=portaudio --pa-in-dev=44 --pa-out-dev=45

### Example on the RPi

This is my command line on the Raspberry Pi.

    ./baa -c KE0SPF --alt 5031 --lat 1053.45N --lng 10257.24W

Build from source
-----------------

### Linux, get the source

On your Linux terminal write:

    git clone https://github.com/kmschr/baa

The source code should be at the "baa" directory.

### Compile

On Linux systems you can compile with g++ using cmake. You need:

* Modern C++ compiler with C++11 support
* CMAKE
* Boost C++ library development files
* PortAudio VERSION 19 library development files

On Debian and Ubuntu you can install the required packages easily:

    apt-get install cmake build-essential g++ portaudio19-dev libboost-system-dev libboost-thread-dev libboost-program-options-dev libhamlib-dev

Then just type, at the "extmodem" directory:

    mkdir build
    cd build
    cmake .. -DCMAKE_BUILD_TYPE=Release
    make

The executable should be at  `baa/build`

### Raspberry Pi Support

I'm running baa on my RPi. To compile with cmake you can:

    mkdir build
    cd build
    CXX=$(which g++-4.8) CC=$(which gcc-4.8) CFLAGS=-O2 CXXFLAGS=-O2 cmake ..
    make

License
-------

This program borrows code from Sivan Toledo's javAX25 and Thomas Sailer's multimon. Thank you folks!

> 
>     Copyright (C) 2013-2017 Alejandro Santos LU4EXT, alejolp@gmail.com.
> 
>     This program is free software; you can redistribute it and/or modify
>     it under the terms of the GNU General Public License as published by
>     the Free Software Foundation; either version 2 of the License, or
>     (at your option) any later version.
> 
>     This program is distributed in the hope that it will be useful,
>     but WITHOUT ANY WARRANTY; without even the implied warranty of
>     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
>     GNU General Public License for more details.
> 
>     You should have received a copy of the GNU General Public License
>     along with this program; if not, write to the Free Software
>     Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
> 

