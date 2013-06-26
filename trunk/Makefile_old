OBJ = Huffman.o comprimi.o decomprimi.o
CXXFLAGS=$(DEBUG_FLAGS) -Wall `pkg-config --cflags gtk+-3.0`
LDFLAGS=-export-dynamic `pkg-config --libs gtk+-3.0`

Huffman: $(OBJ)
	g++ -o Huffman $(OBJ) $(LDFLAGS)

-include dependencies

.PHONY: depend clean cleanall

depend:
	g++ -MM *.cc > dependencies
clean:
	rm *.o
cleanall:
	rm Huffman *.o *~

