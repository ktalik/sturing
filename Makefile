# STuring Makefile
# Konrad Talik <konrad.talik@slimak.matinf.uj.edu.pl>

SOURCES = $(wildcard src/*.cpp)
OBJECTS = $(patsubst src/%.cpp, bin/%.o, $(SOURCES)) main.o
INCLUDES = -I include
CXXFLAGS = -std=c++11 -Wall -pedantic -g $(INCLUDES)
SHELL = /bin/bash
PROGRAM = sturing

$(PROGRAM): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@

bin/%.o: src/%.cpp
	mkdir -p bin
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm $(PROGRAM) $(OBJECTS)

# Testing

INPUTS = $(wildcard examples/*.tap)
ACTUALS = $(patsubst %.tap, %.act, $(INPUTS))
DIFF = git diff --no-index --no-prefix --ignore-space-change
EXEC = ./sturing
PARAMS = -x

# Test all
.PHONY: tests
tests:
	@printf "Tests: start.\n"
	$(MAKE) init_tests
	$(MAKE) --silent run_tests
	@printf "Tests: DONE.\n"
	$(MAKE) --silent sumup_tests
	$(MAKE) --silent status_tests

# Init tests
.PHONY: init_tests
init_tests:
	@echo "Tests: init..."
	for act in $(ACTUALS) ; do \
		rm -f $$act; \
		rm -f "$$act.failed"; \
	done

# Run tests
.PHONY: run_tests
run_tests:
	@echo "Tests: running..."
	@for act in $(ACTUALS) ; do \
		$(MAKE) --silent $$act ; \
	done

# Print summary with exit code
.PHONY: sumup_tests
sumup_tests:
	@echo "Tests summary..."
	@for act in $(ACTUALS) ; do \
		$(MAKE) --silent compare \
			act="$$act" \
			out="$${act/.act/.out}" ;\
	done
	@echo

# Return tests status code
.PHONY: status_tests
status_tests:
	@echo "Tests: exiting with tests status..."
	@for act in $(ACTUALS) ; do \
		if [ -a $$act.failed ] ; then exit 2 ; fi ; \
	done

# Test exec with input and compare actuals
.PHONY: %.act
%.act: %.stu %.tap %.out
	$(MAKE) --silent test params="$(PARAMS) $(word 1, $^) $(word 2, $^)" act=$@
	$(MAKE) --silent compare act="$@" out="$(word 3, $^)" verbose=true

# Test exec with params and create actuals
.PHONY: test
test:
	@if [ -n $(verbose) ] ; then \
		printf "Test: $(EXEC) $(params)"; \
	fi;
	$(shell $(EXEC) $(params) > $(act))

# Compare expected outputs and actuals
#
# Parameters:
#	verbose			flag: print statuses as PASSED or FAILED
#					if ommited, status will have format: . or x respectively
#	silent			flag: do not print any output
#	fail_details	flag: print FAILED tests details
.PHONY: compare
compare:
	@if [ "$$($(DIFF) $(act) $(out))" == "" ] ; then\
		if [ $(verbose) ] ; then \
			printf " ...\e[0;32mPASSED\e[0m.\n"; \
		    $(DIFF) $(act) $(out); \
		elif [ ! $(silent) ] ; then \
			printf "\e[0;32m.\e[0m"; \
		fi; \
	else \
		if [ "$(verbose)" -o "$(fail_details)"  ] ; then \
			printf " ...\e[0;31mFAILED\e[0m:\n"; \
		    $(DIFF) $(act) $(out); \
		elif [ ! $(silent) ] ; then \
			printf "\e[0;31mx\e[0m"; \
			touch $(act).failed; \
		fi; \
	fi;
