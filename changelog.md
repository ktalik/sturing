STuring - Turing Machine Interpreter - Version Changelog
========================================================

sturing 0.1212 - Basic functionalities
--------------------------------------

Basic functionalities have been addedd:

1. Reading machine states in specific syntax.
	* Dynamic interpretation can recognize and throw simple syntax errors.
	* Verbose mode for informational use.
	* States and characters dynamic creation (without knowing them all in the beginning).
	* Virtual Turing Machine containing all states with defined rules.

2.  Source can be read from file or in the interactive mode.
	* Errors stop interpretation when interpreting a file, unlike in the interactive mode.

3. Simple useful options including line echo and state board printing.
	* For now, board viewing only makes sense with one-character states or alphabet members.
