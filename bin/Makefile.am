.POSIX:

include ../common-rules.am

bin_PROGRAMS = web-app$(EXEEXT)
web_app_SOURCES = ../objects/*.o

web-app$(EXEEXT): $(web_app_SOURCES)
	$(CC) $(LDFLAGS) -o $@ $(web_app_SOURCES)

clean:
	rm -f web-app
