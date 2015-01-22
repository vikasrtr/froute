# froute Make file
# Builds individula daemons

export CXXFLAGS
export CFLAGS

SUB_DIRS = frouted

#####################
all: daemons
	@echo building daemons...

#####################
daemons:
	$(MAKE) -C $(SUB_DIRS)
	
#####################
clean:
	$(MAKE) -C $(SUB_DIRS) clean
