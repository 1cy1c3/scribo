scribo
============

# Description
A word processor which allows to encrypt and decrypt text.

## Prerequisites
+ C/C++ libraries
+ Qt libraries
+ Doxygen
+ IDE Qt creator with Qt designer

## Installation
At first, clone or download this project. Afterwards, double click on the pro-files of the client and server. Now, the Qt creator loads the projects and you're able to edit, compile and execute it.

## Usage
At first, compile and execute it. After this processes, the program is started. Navigate to the menu item `Help` which presents a video regarding the usage of this program. In addition to the desktop applications, it's possible to compile and execute the code for `Android` or `iOS`. Everything you need is to satisfy the conditions. Read more at http://doc.qt.io/qt-5/android-support.html and http://doc.qt.io/qt-5/ios-support.html.

## Note
In addition to the client, there is a server which acts as an update server. For example, start the server with these commands:
```
$ cd scribo-server
$ ./scribo-server
```
Afterwards, it's possible with the client to check whether updates are available. By the way, the default port is `1234`.

## More information
Generate the documentation regarding the special comments with a command in your terminal, for example:
```
$ cd scribo
$ doxygen doxygen.config
```
Afterwards, you will get a website with helpful informations about the code. Furthermore, read the documentation about the framework `Qt` at http://doc.qt.io/ or in the IDE `Qt creator`. 
