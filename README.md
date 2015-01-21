#Froute
=======
##A Software based IP Router
============================

Froute is a experimental x86-based software IP Router
Froute ustilises a linux kernel module to implement FIB, and user-space daemons for working with RIB and Routing Protocols

###Structure

froute/										-- Main project folder

		/doc								-- Project Documentation

		/etc								-- Kernel Patches (Linux only)

		/lib								-- Shared Libraries

		/froute 							-- Folder for daemons and other modules

				/bgpd						-- BGP Implementation

				/frouted					-- RIB daemon and front-end
				
				/staticd					-- Static Routes Daemon

