
PROGS = cli_01 serv_01
OBJS = cli_01.o serv_01.o


all:	${PROGS}

cli_01: cli_01.o		
		cc -o $@ cli_01.o -lunp 

serv_01: serv_01.o		
		cc -o $@ serv_01.o -lunp 
clean:
		rm -f ${PROGS} ${OBJS} ${CLEANFILES}
