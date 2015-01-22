# make for froute program

# give lib path
VPATH = ../../lib

#######################
all: frouted
	@echo Building 'frouted' daemon

#######################
frouted: main.o
	$(CXX) $< -o $@

main.o: constants.hh

########################
clean:
	rm -f frouted *.o