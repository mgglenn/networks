include Make.defines

PROGS =	 getfile webserver

OPTIONS = -DUNIX  -DUSE_SIGACTION -DANSI


COBJECTS =	DieWithError.o getfile.o 
CSOURCES =	DieWithError.c getfile.c

CPLUSOBJECTS = 

COMMONSOURCES =

CPLUSSOURCES =

all:	${PROGS}


getfile:	webserver.o $(CPLUSOBJECTS) $(COBJECTS) $(LIBS) $(COMMONSOURCES) $(SOURCES)
		${CC} $(INCLUDES) ${LINKOPTIONS}  $@ webserver.o $(CPLUSOBJECTS) $(COBJECTS) $(LIBS) $(LINKFLAGS)


webserver:	webserver.o $(CPLUSOBJECTS) $(COBJECTS)
		${CC} $(INCLUDES) ${LINKOPTIONS} $@ webserver.o $(CPLUSOBJECTS) $(COBJECTS) $(LIBS) $(LINKFLAGS)


.cc.o:	$(HEADERS)
	$(CPLUS) $(CPLUSFLAGS) $(OPTIONS) $<

.c.o:	$(HEADERS)
	$(CC) $(CFLAGS) $(OPTIONS) $<



backup:
	rm -f tcpecho.tar.gz
	rm -f tcpecho.tar
	tar -cf tcpecho.tar *
	gzip -f tcpecho.tar

clean:
		rm -f ${PROGS} ${CLEANFILES}

depend:
		makedepend getfile.c webserver.c $(INCLUDES) $(CFLAGS) $(HEADERS) $(SOURCES) $(COMMONSOURCES) $(CSOURCES)
#		mkdep $(CFLAGS) $(HEADERS) $(SOURCES) $(COMMONSOURCES) $(CSOURCES)

# DO NOT DELETE

webserver.o: /usr/include/stdio.h /usr/include/features.h
webserver.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
webserver.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
webserver.o: /usr/include/linux/stddef.h /usr/include/bits/types.h
webserver.o: /usr/include/bits/typesizes.h /usr/include/libio.h
webserver.o: /usr/include/_G_config.h /usr/include/wchar.h
webserver.o: /usr/include/bits/wchar.h /usr/include/gconv.h
webserver.o: /usr/lib/gcc/i386-redhat-linux/4.1.1/include/stdarg.h
webserver.o: /usr/include/bits/stdio_lim.h
webserver.o: /usr/include/bits/sys_errlist.h /usr/include/sys/socket.h
webserver.o: /usr/include/sys/uio.h /usr/include/sys/types.h
webserver.o: /usr/include/linux/time.h /usr/include/linux/types.h
webserver.o: /usr/include/linux/posix_types.h /usr/include/linux/stddef.h
webserver.o: /usr/include/asm/posix_types.h /usr/include/asm/types.h
webserver.o: /usr/include/endian.h /usr/include/bits/endian.h
webserver.o: /usr/include/sys/select.h /usr/include/bits/select.h
webserver.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
webserver.o: /usr/include/sys/sysmacros.h
webserver.o: /usr/include/bits/pthreadtypes.h /usr/include/bits/uio.h
webserver.o: /usr/include/bits/socket.h /usr/include/linux/limits.h
webserver.o: /usr/include/bits/sockaddr.h /usr/include/asm/socket.h
webserver.o: /usr/include/asm/sockios.h /usr/include/arpa/inet.h
webserver.o: /usr/include/netinet/in.h /usr/include/stdint.h
webserver.o: /usr/include/bits/in.h /usr/include/bits/byteswap.h
webserver.o: /usr/include/netdb.h /usr/include/rpc/netdb.h
webserver.o: /usr/include/bits/netdb.h /usr/include/stdlib.h
webserver.o: /usr/include/alloca.h /usr/include/string.h
webserver.o: /usr/include/linux/unistd.h /usr/include/asm/unistd.h
webserver.o: /usr/include/stdio.h /usr/include/features.h
webserver.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
webserver.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
webserver.o: /usr/include/linux/stddef.h /usr/include/bits/types.h
webserver.o: /usr/include/bits/typesizes.h /usr/include/libio.h
webserver.o: /usr/include/_G_config.h /usr/include/wchar.h
webserver.o: /usr/include/bits/wchar.h /usr/include/gconv.h
webserver.o: /usr/lib/gcc/i386-redhat-linux/4.1.1/include/stdarg.h
webserver.o: /usr/include/bits/stdio_lim.h
webserver.o: /usr/include/bits/sys_errlist.h /usr/include/sys/socket.h
webserver.o: /usr/include/sys/uio.h /usr/include/sys/types.h
webserver.o: /usr/include/linux/time.h /usr/include/linux/types.h
webserver.o: /usr/include/linux/posix_types.h /usr/include/linux/stddef.h
webserver.o: /usr/include/asm/posix_types.h /usr/include/asm/types.h
webserver.o: /usr/include/endian.h /usr/include/bits/endian.h
webserver.o: /usr/include/sys/select.h /usr/include/bits/select.h
webserver.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
webserver.o: /usr/include/sys/sysmacros.h
webserver.o: /usr/include/bits/pthreadtypes.h /usr/include/bits/uio.h
webserver.o: /usr/include/bits/socket.h /usr/include/linux/limits.h
webserver.o: /usr/include/bits/sockaddr.h /usr/include/asm/socket.h
webserver.o: /usr/include/asm/sockios.h /usr/include/arpa/inet.h
webserver.o: /usr/include/netinet/in.h /usr/include/stdint.h
webserver.o: /usr/include/bits/in.h /usr/include/bits/byteswap.h
webserver.o: /usr/include/stdlib.h /usr/include/alloca.h
webserver.o: /usr/include/string.h /usr/include/linux/unistd.h
webserver.o: /usr/include/asm/unistd.h
DieWithError.o: /usr/include/stdio.h /usr/include/features.h
DieWithError.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
DieWithError.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
DieWithError.o: /usr/include/linux/stddef.h /usr/include/bits/types.h
DieWithError.o: /usr/include/bits/typesizes.h /usr/include/libio.h
DieWithError.o: /usr/include/_G_config.h /usr/include/wchar.h
DieWithError.o: /usr/include/bits/wchar.h /usr/include/gconv.h
DieWithError.o: /usr/lib/gcc/i386-redhat-linux/4.1.1/include/stdarg.h
DieWithError.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
DieWithError.o: /usr/include/stdlib.h /usr/include/sys/types.h
DieWithError.o: /usr/include/linux/time.h /usr/include/linux/types.h
DieWithError.o: /usr/include/linux/posix_types.h /usr/include/linux/stddef.h
DieWithError.o: /usr/include/asm/posix_types.h /usr/include/asm/types.h
DieWithError.o: /usr/include/endian.h /usr/include/bits/endian.h
DieWithError.o: /usr/include/sys/select.h /usr/include/bits/select.h
DieWithError.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
DieWithError.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
DieWithError.o: /usr/include/alloca.h
Handlegetfile.o: /usr/include/stdio.h /usr/include/features.h
Handlegetfile.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
Handlegetfile.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
Handlegetfile.o: /usr/include/linux/stddef.h /usr/include/bits/types.h
Handlegetfile.o: /usr/include/bits/typesizes.h /usr/include/libio.h
Handlegetfile.o: /usr/include/_G_config.h /usr/include/wchar.h
Handlegetfile.o: /usr/include/bits/wchar.h /usr/include/gconv.h
Handlegetfile.o: /usr/lib/gcc/i386-redhat-linux/4.1.1/include/stdarg.h
Handlegetfile.o: /usr/include/bits/stdio_lim.h
Handlegetfile.o: /usr/include/bits/sys_errlist.h /usr/include/sys/socket.h
Handlegetfile.o: /usr/include/sys/uio.h /usr/include/sys/types.h
Handlegetfile.o: /usr/include/linux/time.h /usr/include/linux/types.h
Handlegetfile.o: /usr/include/linux/posix_types.h
Handlegetfile.o: /usr/include/linux/stddef.h /usr/include/asm/posix_types.h
Handlegetfile.o: /usr/include/asm/types.h /usr/include/endian.h
Handlegetfile.o: /usr/include/bits/endian.h /usr/include/sys/select.h
Handlegetfile.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
Handlegetfile.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
Handlegetfile.o: /usr/include/bits/pthreadtypes.h /usr/include/bits/uio.h
Handlegetfile.o: /usr/include/bits/socket.h /usr/include/linux/limits.h
Handlegetfile.o: /usr/include/bits/sockaddr.h /usr/include/asm/socket.h
Handlegetfile.o: /usr/include/asm/sockios.h /usr/include/linux/unistd.h
Handlegetfile.o: /usr/include/asm/unistd.h
