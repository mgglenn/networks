include Make.defines

PROGS =	 getfile webserver

OPTIONS = -DUNIX  -DUSE_SIGACTION -DANSI


COBJECTS = getfileHelper.o	
CSOURCES = getfileHelper.c

CPLUSOBJECTS = 

COMMONSOURCES =

CPLUSSOURCES =

all:	${PROGS}


getfile:	getfile.o $(CPLUSOBJECTS) $(COBJECTS) $(LIBS) $(COMMONSOURCES) $(CSOURCES)
		${CC} $(INCLUDES) ${LINKOPTIONS}  $@ getfile.o $(CPLUSOBJECTS) $(COBJECTS) $(LIBS) $(LINKFLAGS)


webserver:	webserver.o $(CPLUSOBJECTS) $(COBJECTS)
		${CC} $(INCLUDES) ${LINKOPTIONS} $@ webserver.o $(CPLUSOBJECTS) $(COBJECTS) $(LIBS) $(LINKFLAGS)


.cc.o:	$(HEADERS)
	$(CPLUS) $(CPLUSFLAGS) $(OPTIONS) $<

.c.o:	$(HEADERS)
	$(CC) $(CFLAGS) $(OPTIONS) $<

backup:
	rm -f mgglenn-hw2.tar.gz
	rm -f mgglenn-hw2.tar
	tar -cf mgglenn-hw2.tar *
	gzip -f mgglenn-hw2.tar

clean:
		rm -f ${PROGS} ${CLEANFILES}

depend:
		makedepend getfile.c webserver.c $(INCLUDES) $(CFLAGS) $(HEADERS) $(SOURCES) $(COMMONSOURCES) $(CSOURCES)
#		mkdep $(CFLAGS) $(HEADERS) $(SOURCES) $(COMMONSOURCES) $(CSOURCES)

# DO NOT DELETE
