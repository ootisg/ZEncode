main: plaintext default_encoder ZEncode_header cbuf search_window
	gcc encoder.c cbuf.o ZEncode_header.o search_window.o plaintext_encoder.o default_binary_encoder.o -o ZEncode
default_encoder: default_binary_encoder.c default_binary_encoder.h
	gcc default_binary_encoder.c -c
plaintext: plaintext_encoder.c plaintext_encoder.h
	gcc plaintext_encoder.c -c
ZEncode_header: ZEncode_header.c ZEncode_header.h
	gcc ZEncode_header.c -c
cbuf: data_structures/cbuf.c data_structures/cbuf.h
	gcc data_structures/cbuf.c -c
search_window: data_structures/search_window.c data_structures/search_window.h
	gcc data_structures/search_window.c -c