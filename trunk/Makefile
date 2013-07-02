OBJ = Huffman.o comprimi.o decomprimi.o
CXXFLAGS=$(DEBUG_FLAGS) -Wall

Huffman: $(OBJ)
	g++ -o Huffman $(OBJ) $(LDFLAGS)
	doxygen

-include dependencies

.PHONY: depend clean cleanall

depend:
	g++ -MM *.cc > dependencies
clean:
	rm *.o
cleanall:
	rm Huffman *.o *~

