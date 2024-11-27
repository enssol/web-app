.POSIX:

include ../common-rules.am

DISTFILES = ../bin/web-app ../docs/* ../etc/config.ini ../LICENSE ../README.md
dist_ARCHIVES = web-app.tar.xz

all: dist

dist: $(DISTFILES)
	tar -cJf $(dist_ARCHIVES) $(DISTFILES)

clean:
	rm -f $(dist_ARCHIVES)
