CXXFLAGS = -DNDEBUG -Wall -std=c++0x -pedantic -x c++ -O3 -g
CPHSTLSRC = $(HOME)/diku/data/src/new_CPHSTL/
IFLAGS = -I $(CPHSTLSRC)/Source/Assert/Code -I $(CPHSTLSRC)/Source/Meldable-priority-queue/Code -I ../Code -I $(CPHSTLSRC)/Source/Priority-queue-framework/Code -I $(CPHSTLSRC)/Source/Iterator/Code -I $(CPHSTLSRC)/Source/Type/Code -I $(HOME)/diku/data/src/v1 -I$(CPHSTLSRC)/Source/List/Code -I .
CXX = g++

implementation-files:= $(wildcard *.i++)
data-structures:= $(basename $(implementation-files))
time-tests = $(addsuffix .time, $(data-structures)) 
comp-tests = $(addsuffix .comp, $(data-structures)) 
push-time-tests:= $(addsuffix .push, $(time-tests)) 
push-comp-tests:= $(addsuffix .push, $(comp-tests)) 
increase-time-tests:= $(addsuffix .increase, $(time-tests)) 
increase-comp-tests:= $(addsuffix .increase, $(comp-tests)) 
erase-comp-tests:= $(addsuffix .erase, $(comp-tests)) 
erase-time-tests:= $(addsuffix .erase, $(time-tests)) 
pop-time-tests:= $(addsuffix .pop, $(time-tests)) 
pop-comp-tests:= $(addsuffix .pop, $(comp-tests)) 

$(time-tests): %.time: %.i++ 
	@cp $*.i++ data-structure.i++
	@make -s $*.time.push
	@make -s $*.time.increase
	@make -s $*.time.erase
	@make -s $*.time.pop
	@rm data-structure.i++

$(comp-tests): %.comp: %.i++ 
	@cp $*.i++ data-structure.i++
	@make -s $*.comp.push
	@make -s $*.comp.increase
	@make -s $*.comp.erase
	@make -s $*.comp.pop
	@rm data-structure.i++

list = 100 # 100000 # 500000

$(push-time-tests): %.time.push : %.i++
	@echo $* "time per push" 
	@for x in $(list) ; do \
	  $(CXX) $(CXXFLAGS) -DNUMBER=$$x $(IFLAGS) push-time.c++;\
	  ./a.out; \
	  rm -f ./a.out ; \
	done

$(increase-time-tests): %.time.increase : %.i++
	@echo $* "time per increase" 
	@for x in $(list) ; do \
	  $(CXX) $(CXXFLAGS) -DNUMBER=$$x $(IFLAGS) increase-time.c++;\
	  ./a.out; \
	  rm -f ./a.out ; \
	done

$(erase-time-tests): %.time.erase : %.i++
	@echo $* "time per erase" 
	@for x in $(list) ; do \
	  $(CXX) $(CXXFLAGS) -DNUMBER=$$x $(IFLAGS) erase-time.c++;\
	  ./a.out; \
	  rm -f ./a.out ; \
	done

$(pop-time-tests): %.time.pop : %.i++
	@echo $* "time per pop" 
	@for x in $(list) ; do \
	  $(CXX) $(CXXFLAGS) -DNUMBER=$$x $(IFLAGS) pop-time.c++;\
	  ./a.out; \
	  rm -f ./a.out ; \
	done

$(push-comp-tests): %.comp.push : %.i++
	@echo $* "#comp per push" 
	@for x in $(list) ; do \
	  $(CXX) $(CXXFLAGS) -DNUMBER=$$x $(IFLAGS) push-comp.c++;\
	  ./a.out; \
	  rm -f ./a.out ; \
	done

$(increase-comp-tests): %.comp.increase : %.i++
	@echo $* "#comp per increase" 
	@for x in $(list) ; do \
	  $(CXX) $(CXXFLAGS) -DNUMBER=$$x $(IFLAGS) increase-comp.c++;\
	  ./a.out; \
	  rm -f ./a.out ; \
	done

$(erase-comp-tests): %.comp.erase : %.i++
	@echo $* "#comp per erase" 
	@for x in $(list) ; do \
	  $(CXX) $(CXXFLAGS) -DNUMBER=$$x $(IFLAGS) erase-comp.c++;\
	  ./a.out; \
	  rm -f ./a.out ; \
	done

$(pop-comp-tests): %.comp.pop : %.i++
	@echo $* "#comp per pop" 
	@for x in $(list) ; do \
	  $(CXX) $(CXXFLAGS) -DNUMBER=$$x $(IFLAGS) pop-comp.c++;\
	  ./a.out; \
	done

clean:
	@rm -vf *~ a.out temp