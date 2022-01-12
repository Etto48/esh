CC 				:=	g++

MODE			:=	DEBUG

DEFINE_LIST		:=	$(MODE)
CARGS			:=	-Wall $(addprefix -D, $(DEFINE_LIST)) -std=c++2a -lncurses

ifeq ($(MODE),DEBUG)
	CARGS		+=	-ggdb -Wextra -pedantic
else ifeq ($(MODE),RELEASE)
	CARGS		+=	-O3
endif

CFILES			:= 	$(shell find ./ -name "*.cpp")
OBJ				:=	$(patsubst %.cpp, %.o, $(CFILES))
HEADERS			:=	$(shell find ./ -name "*.hpp")
ALL_OBJ			:=	$(shell find ./ -name "*.o")
THIS			:=	./Makefile

PROJECT			:=	esh

all: $(PROJECT)

$(PROJECT): $(OBJ)
	@echo Linking $@
	@$(CC) $(OBJ) -o $@ $(CARGS)

%.o: %.cpp $(HEADERS) $(THIS)
	@echo Compiling $@
	@$(CC) -c $< -o $@ $(CARGS)

.PHONY: clean clean-data dry-run
clean:
	@echo Removing object files and executables
	@rm -f $(ALL_OBJ) $(PROJECT)

dry-run: 
	@$(CC) $(CFILES) -o /dev/null $(CARGS)