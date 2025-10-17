COMP = g++
# SOURCES = $(wildcard *.cpp) $(wildcard ../Calculator/Parse/*.cpp)
#CRFLAGS = -std=c++17 -DNDEBUG  -O3 -I. -I../Calculator/Parse

# TARGET = solver

ASM_DIR = Assembler
PROC_DIR = Processor
DISASM_DIR = Disassembler

ASM_SRC = $(wildcard $(ASM_DIR)/*.cpp)
PROC_SRC = $(wildcard $(PROC_DIR)/*.cpp)
DISASM_SRC = $(wildcard $(DISASM_DIR)/*.cpp)

STACK_SRC = $(wildcard StackFunctions/*.cpp)
FILEOP_SRC = $(wildcard FileOperations/*.cpp)
PARSE_SRC = $(wildcard Parse/*.cpp)
CALC_SRC = $(wildcard Processor/Calc/*.cpp)
PROCFUNC_SRC = $(wildcard Processor/ProcessorFunctions/*.cpp)
ALLLOG_SRC = $(wildcard AllLogFile/*.cpp)

CFLAGS = -D_DEBUG -ggdb3 -g -std=c++17 -O0 -Wall -Wextra -Weffc++ -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat=2 -Winline -Wnon-virtual-dtor -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-overflow=2 -Wsuggest-override -Wswitch-default -Wswitch-enum -Wundef -Wunreachable-code -Wunused -Wvariadic-macros -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -fno-omit-frame-pointer -Wlarger-than=8192 -fPIE -Werror=vla -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,nonnull-attribute,null,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr -I. -IAssembler -IProcessor -IDisasassembler -IFileOperations -IStackFunctions -IAllLogFile -IParse -IProcessor/ProcessorFunctions -IProcessor/Calc

asm: $(ASM_SRC)
	@$(COMP) $(CFLAGS) $(ASM_SRC) $(FILEOP_SRC) $(PARSE_SRC) $(ALLLOG_SRC) $(STACK_SRC) -o asm.out
	@rm -rf *.dSYM

proc: $(PROC_SRC)
	@$(COMP) $(CFLAGS) $(PROC_SRC) $(FILEOP_SRC) $(PARSE_SRC) $(CALC_SRC) $(STACK_SRC) $(PROCFUNC_SRC) $(ALLLOG_SRC) -o proc.out
	@rm -rf *.dSYM

disasm: $(DISASM_SRC)
	@$(COMP) $(CFLAGS) $(DISASM_SRC) $(FILEOP_SRC) $(PARSE_SRC) $(STACK_SRC) $(PROCFUNC_SRC) -o disasm.out
	@rm -rf *.dSYM

# debug: $(SOURCES)
# 	@$(COMP) $(CFLAGS) $(SOURCES) -o $(TARGET)

# release: $(SOURCES)
# 	@$(COMP) $(CRFLAGS) $(SOURCES) -o $(TARGET)

# %.o: %.cpp
# 	@$(COMP) $(CFLAGS) -c $< -o $@

# %.out: %.cpp
# 	@$(COMP) $< -o $@

# clean: 
# 	rm *.o *.out $(TARGET)