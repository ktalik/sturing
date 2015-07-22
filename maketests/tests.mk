# Make Tests
# Konrad Talik <konrad.talik@slimak.matinf.uj.edu.pl>

#/ In order to make tests, set up following variables:
#/  TEXEC           target executable
#/
#/ Optional parameters (with default values)
#/  TPARAMS="[INPUT_FILE]"  executable params for all tests
#/  TINPUT=tests            input files root directory
#/  TOUTPUT=tests           expected outputs root directory
#/  TACTUAL=tests           actual outputs root directory
#/  TIN=.in                 test input extension (start with dot if present)
#/  TOUT=.out               expected outputs extension
#/  TACT=.act               actual outputs extension

###
# Default constans
###

# Tests default dir
TDDIR=tests

SHELL = /bin/bash

ifeq ($(origin TPARAMS), undefined)
TPARAMS="[INPUT_FILE]"
endif

ifeq ($(origin TINPUT), undefined)
TINPUT=$(TDDIR)
endif

ifeq ($(origin TOUTPUT), undefined)
TOUTPUT=$(TDDIR)
endif

ifeq ($(origin TACTUAL), undefined)
TACTUAL=$(TDDIR)
endif

ifeq ($(origin TIN), undefined)
TIN=.in
endif

ifeq ($(origin TOUT), undefined)
TOUT=.out
endif

ifeq ($(origin TACT), undefined)
TACT=.act
endif

INPUTS = $(wildcard $(TINPUT)/*$(TIN))
ACTUALS = $(patsubst %$(TIN), %$(TACT), $(INPUTS))
DIFF = git diff --no-index --no-prefix --ignore-space-change

###
# Recipes
###

# Make Tests
.PHONY: tests
tests:
	@printf "Tests: params check...\n"
	$(MAKE) --silent tests_params_check
	@printf "Tests: start.\n"
	$(MAKE) init_tests
	$(MAKE) --silent run_tests
	@printf "Tests: DONE.\n"
	$(MAKE) --silent sumup_tests
	$(MAKE) --silent status_tests

# Params check
.PHONY: tests_params_check
tests_params_check:
ifeq ($(origin TEXEC), undefined)
	$(error TEXEC: Please provide executable filename/path!)
endif

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
			out="$${act/$(TACT)/$(TOUT)}" ;\
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
.PHONY: %$(TACT)
%$(TACT): %$(TIN) %$(TOUT)
	$(MAKE) --silent test \
		params="$(subst [INPUT_FILE], $*$(TIN), $(subst [FILE_STEM], $*, $(TPARAMS)))"\
		act=$@
	$(MAKE) --silent compare act="$@" out="$(word 2, $^)" verbose=true

# Test exec with params and create actuals
#
# Parameters:
#   params      executable parameters for test
#   act			actual output filename 
#   verbose     flag: print test descrption
#   
.PHONY: test
test:
	@if [ -n $(verbose) ] ; then \
		printf "Test: $(TEXEC) $(params)"; \
	fi;
	$(shell $(TEXEC) $(params) > $(act))

# Compare expected outputs and actuals
#
# Parameters:
#   verbose         flag: print statuses as PASSED or FAILED
#                   if ommited, status will have format: . or x respectively
#   silent          flag: do not print any output
#   fail_details    flag: print FAILED tests details
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
