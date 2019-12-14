all:
        gcc -o count `pkg-config --cflags --libs glib-2.0` count.c `pkg-config --libs glib-2.0` ../lib/libstemmer.o
        gcc -o predictor `pkg-config —cflags —libs glib-2.0` -I../include predictor.c ../lib/libstemmer.o `pkg-config —libs glib-2.0` -lm
