hidclient: hidclient.c asciimap.h
	gcc -o hidclient -I. hidclient.c -O2 -lbluetooth -Wall

asciimap.h: asciimap
	$(RM) $@
	./asciimap > $@

asciimap: asciimap.c
	gcc -o $@ asciimap.c

clean:
	$(RM) hidclient asciimap asciimap.h
