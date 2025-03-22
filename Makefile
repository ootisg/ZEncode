main: plaintext cbuf search_window
	gcc encoder.c cbuf.o search_window.o plaintext_encoder.o -o ZEncode
plaintext: plaintext_encoder.c plaintext_encoder.h
	gcc plaintext_encoder.c -c
cbuf: data_structures/cbuf.c data_structures/cbuf.h
	gcc data_structures/cbuf.c -c
search_window: data_structures/search_window.c data_structures/search_window.h
	gcc data_structures/search_window.c -c