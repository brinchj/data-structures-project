CXXFLAGS = -DNDEBUG -Wall -std=c++0x -pedantic -x c++ -O3 -g
CPHSTLSRC = ../../
V1SRC=${CPHSTLSRC}/../../v1
IFLAGS = -I $(CPHSTLSRC)/Assert/Code -I $(CPHSTLSRC)/Meldable-priority-queue/Code -I ../Code -I $(CPHSTLSRC)/Priority-queue-framework/Code -I $(CPHSTLSRC)/Iterator/Code -I $(CPHSTLSRC)/Type/Code -I ${V1SRC} -I$(CPHSTLSRC)/List/Code -I .
CXX = g++

implementation-files:= $(wildcard *.i++)
data-structures:= $(basename $(implementation-files))
time-tests = $(addsuffix .time, $(data-structures)) 
comp-tests = $(addsuffix .comp, $(data-structures)) 
push-time-tests:= $(addsuffix .push, $(time-tests)) 
push-comp-tests:= $(addsuffix .push, $(comp-tests)) 
increase-time-tests:= $(addsuffix .increase, $(time-tests)) 
increase-comp-tests:= $(addsuffix .increase, $(comp-tests)) 
increase2-time-tests:= $(addsuffix .increase2, $(time-tests)) 
increase2-comp-tests:= $(addsuffix .increase2, $(comp-tests)) 
increase3-time-tests:= $(addsuffix .increase3, $(time-tests)) 
increase3-comp-tests:= $(addsuffix .increase3, $(comp-tests)) 
erase-comp-tests:= $(addsuffix .erase, $(comp-tests)) 
erase-time-tests:= $(addsuffix .erase, $(time-tests)) 
pop-time-tests:= $(addsuffix .pop, $(time-tests)) 
pop-comp-tests:= $(addsuffix .pop, $(comp-tests)) 
pqsort-comp-tests:= $(addsuffix .pqsort, $(comp-tests)) 
pqsort-time-tests:= $(addsuffix .pqsort, $(time-tests)) 
dijkstra-comp-tests:= $(addsuffix .dijkstra, $(comp-tests))
dijkstra-time-tests:= $(addsuffix .dijkstra, $(time-tests))

$(time-tests): %.time: %.i++ 
	@cp $*.i++ data-structure.i++
	@make -s $*.time.push
	@make -s $*.time.increase
	@make -s $*.time.increase2
	@make -s $*.time.increase3
	@make -s $*.time.erase
	@make -s $*.time.pop
	@rm data-structure.i++

$(comp-tests): %.comp: %.i++ 
	@cp $*.i++ data-structure.i++
	@make -s $*.comp.push
	@make -s $*.comp.increase
	@make -s $*.comp.increase2
	@make -s $*.comp.increase3
	@make -s $*.comp.erase
	@make -s $*.comp.pop
	@rm data-structure.i++

list = 10000 #100000 500000

$(push-time-tests): %.time.push : %.i++
	@echo $* "time per push" 
	@for x in $(list) ; do \
	  $(CXX) $(CXXFLAGS) -DNUMBER=$$x $(IFLAGS) push-time.c++;\
	  ./a.out; \
	  rm -f ./a.out ; \
	done

$(increase2-time-tests): %.time.increase2 : %.i++
	@echo $* "time per increase2" 
	@for x in $(list) ; do \
	  $(CXX) $(CXXFLAGS) -DNUMBER=$$x $(IFLAGS) increase2-time.c++;\
	  ./a.out; \
	  rm -f ./a.out ; \
	done

$(increase3-time-tests): %.time.increase3 : %.i++
	@echo $* "time per increase3" 
	@for x in $(list) ; do \
	  $(CXX) $(CXXFLAGS) -DNUMBER=$$x $(IFLAGS) increase3-time.c++;\
	  ./a.out; \
	  rm -f ./a.out ; \
	done

$(dijkstra-time-tests): %.time.dijkstra : %.i++
	@echo $* "time per dijkstra ;-)" 
	@for x in $(list) ; do \
	  $(CXX) $(CXXFLAGS) -DNUMBER=$$x $(IFLAGS) dijkstra-time.c++ && \
	  ./a.out && \
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

$(increase2-comp-tests): %.comp.increase2 : %.i++
	@echo $* "#comp per increase2" 
	@for x in $(list) ; do \
	  $(CXX) $(CXXFLAGS) -DNUMBER=$$x $(IFLAGS) increase2-comp.c++;\
	  ./a.out; \
	  rm -f ./a.out ; \
	done

$(increase3-comp-tests): %.comp.increase3 : %.i++
	@echo $* "#comp per increase3" 
	@for x in $(list) ; do \
	  $(CXX) $(CXXFLAGS) -DNUMBER=$$x $(IFLAGS) increase3-comp.c++;\
	  ./a.out; \
	  rm -f ./a.out ; \
	done

$(dijkstra-comp-tests): %.comp.dijkstra : %.i++
	@echo $* "#comp per dijkstra ;-)" 
	@for x in $(list) ; do \
	  $(CXX) $(CXXFLAGS) -DNUMBER=$$x $(IFLAGS) dijkstra-comp.c++;\
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

$(pqsort-comp-tests): %.comp.pqsort : %.i++
	@echo $* "#comp per element" 
	@for x in $(list) ; do \
	  $(CXX) $(CXXFLAGS) -DNUMBER=$$x $(IFLAGS) pqsort-comp.c++;\
	  ./a.out; \
	done

$(pqsort-time-tests): %.time.pqsort : %.i++
	@echo $* "#time per element" 
	@for x in $(list) ; do \
	  $(CXX) $(CXXFLAGS) -DNUMBER=$$x $(IFLAGS) pqsort-time.c++;\
	  ./a.out; \
	done

clean:
	@rm -vf *~ a.out temp
