SRC=main.C image.C sink.C source.C PNMreader.C PNMwriter.C filter.C
OBJ=$(SRC:.C=.o)

prog: $(OBJ)
	g++ $(OBJ) -o photoEditor

.C.o: $<
	g++ -I. -c $<

clean:
	rm *.o proj3E
