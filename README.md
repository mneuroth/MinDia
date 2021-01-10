## MinDia is a program to create, modify and run multimedia slide shows on a computer

This is an application to make multimedia slide shows on a computer.
There is also support to make presentations with rollei projectors.
The following models are supported: "Rolleivision 35 twin digital P"
and "Rolleivision twin msc 3x0 P". 

<img src="docs/mindia_linux82_full.jpg" alt="MinDia screenshot" >

This application works fine with my dia projector, which is twenty years old.
But there is only limited testing done with the newer versions of the 
Rollei-projectors.

For further information please read the html-documentation.

- [Documentation in german language](src/mindia_de.html)
- [Documentation in english language](src/mindia_en.html)

or visit the [project homepage](http://mindia.sourceforge.net) 
or the [source code repository](https://github.com/mneuroth/MinDia)

### Supported platforms  
This program is written in C++ with the Qt framework. It runs on GNU/Linux, 
Mac OSX, Windows and WASM. The application runs even in a [browser via Webassembly](https://mneuroth.github.io/MinDiaWASM/).
* Windows [![Build status](https://ci.appveyor.com/api/projects/status/s700taf5bv7dwt5f?svg=true)](https://ci.appveyor.com/project/mneuroth/MinDia)
* MacOS and Linux [![Build Status](https://travis-ci.org/mneuroth/MinDia.svg?branch=master)](https://travis-ci.org/mneuroth/MinDia) 

Github Actions status: 

[![Linux CI](https://github.com/mneuroth/MinDia/workflows/Linux%20CI/badge.svg)](https://github.com/mneuroth/MinDia/actions)
[![Windows CI](https://github.com/mneuroth/MinDia/workflows/Windows%20CI/badge.svg)](https://github.com/mneuroth/MinDia/actions)
[![MacOS CI](https://github.com/mneuroth/MinDia/workflows/MacOS%20CI/badge.svg)](https://github.com/mneuroth/MinDia/actions)
[![Webassembly CI](https://github.com/mneuroth/MinDia/workflows/WASM%20CI/badge.svg)](https://github.com/mneuroth/MinDia/actions)

### Building from source  
* You can either open project with QtCreator and press the Build button
* Or build it using a terminal:
``` 
qmake -r mindia.pro
make
```

### License
The program is released under the GPL, see the file COPYING for details.

