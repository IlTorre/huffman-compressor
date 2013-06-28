OBJ = Huffman.o comprimi.o decomprimi.o
CXXFLAGS=$(DEBUG_FLAGS) -Wall -g

Huffman: $(OBJ)
	g++ -o Huffman -g $(OBJ) $(LDFLAGS)

-include dependencies

.PHONY: depend clean cleanall

depend:
	g++ -MM *.cc > dependencies
clean:
	rm *.o
cleanall:
	rm Huffman *.o *~

