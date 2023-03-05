# SVG-Editor
A web application that can edit SVG files with a C backend

This project involves a C backend that is used to parse SVG files and display them on a web application. The backend utilizes the libxml2 software library to parse XML files. It contains functions for creating, editing, deleting, and retrieving data on SVG files.

To facilitate the use of C functions in the JavaScript frontend, the makefile employs dynamic linking and generates a shared object file named libsvgparse.so. This shared object file is read by the Node library, ffi-napi.

Tools Used: C, Javascript, Make, Express, Ajax, Node, libxml2, HTML, CSS, Bootstrap 4
