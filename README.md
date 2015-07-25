# Froute
=======

### A Software based IP Router

Froute is a experimental x86-based software IP Router
Froute ustilises a linux kernel module to implement FIB, and user-space daemons for working with RIB and Routing Protocols.

**NOTE**: Supports Linux Kernel 3.12 only

### Structure

froute/										-- Main project folder

		/etc								-- Kernel Patches (Linux only)

		/lib								-- Shared Libraries

		/froute 							-- Folder for daemons and other modules

			/linuxmodule					-- Implementation of Kernel Module to sniff packets

### Dependencies
 - `Kernel Module ` requires UML (User Mode Linux) to compile
 - GCC 4.8+

### Usage
 - Patch the Kernel with `final.patch` given in `/etc`
 - `insmod` the generated module and packet sniffing starts.

# Disclaimer

The project is for academic use only and may contain incomplete, buggy or dangerous behaviour, that may harm the system.
No thorough tests have been done on software.
The author assumes no responsibilty for any damage, of any kind, caused by use of software.
