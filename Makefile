SOURCE=parserCSV.cpp
BINNAME=parser

all:
	g++ -o $(BINNAME) $(SOURCE)
clean:
	rm -f $(BINNAME)
rebuild: clean all
