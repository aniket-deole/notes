Notes
=======
Notes For Linux (No longer developed or maintained)

A Note taking application for Linux build in Gtk that aims to be an Evernote client.

Please clone the repo recursively as compilation is dependant on another submodule "libevernote" from https://github.com/aniket-deole/libevernote
<img src="http://i62.tinypic.com/27yocu8.png" title="Notes" alt="Notes" />
<img src="http://i60.tinypic.com/6y2s6p.png" title="Notes" alt="Notes" />
<img src="http://i59.tinypic.com/11gpswo.png" title="Notes" alt="Notes" />

This will not work in elementaryOS Luna anymore because of the GTK+3.10 requirement. It will work in any distro having GTK+3.10 and above.
<hr/>
[Ghatage 29-June-2014]  Update: Added the export to PDF feature!
This is the all new immersive mode
<img src="http://i60.tinypic.com/2mblvo.jpg" title="Immersive mode" alt="Notes" />

And here is the export to PDF feature
<img src="http://i62.tinypic.com/35kpmj6.jpg" title="Immersive mode" alt="Notes" />


[Ghatage 28-June-2014]  Update: Adding the export to PDF feature, to build with it, just set env var WITHPDF to 1.
                                unset it to build as usual.
                                Will need wkhtmltopdf (https://github.com/wkhtmltopdf/wkhtmltopdf) installed.
                                
[Ghatage 22-June-2014]  Update: Works well on Ubuntu 14.04 without any theme issues.

Installation Guide
==================

These libraries will be required.
libsqlite3-dev libssl-dev libcrypto++-dev libsigc++-2.0-dev libgtk-3-dev libgtkmm-3.0-dev libwebkitgtk-3.0-dev uuid-dev libuuid1

The application currently connects to the Evernote Sandbox environment which is a testing environment.
So make sure you open an account on sandbox.evernote.com.

Steps for installing prerequisites on Ubuntu 14.04, 14.10, 15.04 & elementaryOS Freya:
---------------------------------------------------
```
sudo apt-get install build-essential

sudo ln -s /usr/include/gtkmm-3.0 /usr/include/gtkmm

sudo apt-get install libsqlite3-dev libssl-dev libcrypto++-dev libsigc++-2.0-dev libgtk-3-dev libgtkmm-3.0-dev libwebkitgtk-3.0-dev uuid-dev libuuid1 libboost-dev

git clone --recursive https://github.com/aniket-deole/notes.git

cd notes/src/libevernote

make

cd ..

make test
```
You can invoke 
```
make clean-db
```
to remove the local db and start over by logging again in the server of your choice (sandbox/prod).

======

Current TODO Stuff: <br/>
1. Create Package for distribution. <br/>
2. Create an icon for the application.<br/>
3. Create a settings window popup to display user related settings.<br/>
4. Add a option to set Notebook as default to Notebook Right click context menu.<br/>

======
I'm currently working on a C++ library that encapsulates the Thrift library so that the implementation is easier and cleaner. Check it out here. https://github.com/aniket-deole/libevernote

