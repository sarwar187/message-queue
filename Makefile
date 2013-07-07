CC=gcc
OBJS=msgsrv.o msgclient.o
EXECUTABLE=msgsrv msgclient
RM=rm
all: $(EXECUTABLE)
msgsrv: msgsrv.o  
  @echo 'Building target $@'
	@echo 'Invoking: GCC C Linker'
	$(CC)  msgsrv.o  -o $@
	@echo 'Finished building target: $@'
	@echo ' '
msgsrv.o: msgsrv.c
	@echo 'Building target $@'
	@echo 'Invoking: GCC C Linker'
	$(CC)  -c msgsrv.c -o $@
	@echo 'Finished building target: $@'
	@echo ' '
msgclient: msgclient.o  
	@echo 'Building target $@'
	@echo 'Invoking: GCC C Linker'
	$(CC)  msgclient.o  -o $@
	@echo 'Finished building target: $@'
	@echo ' '
msgclient.o: msgclient.c
	@echo 'Building target $@'
	@echo 'Invoking: GCC C Linker'
	$(CC)  -c msgclient.c -o $@
	@echo 'Finished building target: $@'
	@echo ' '

clean:
	$(RM) $(OBJS) $(EXECUTABLE)
