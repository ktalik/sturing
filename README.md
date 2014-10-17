[![Build Status](https://travis-ci.org/ktalik/Sturing.svg?branch=dev)](https://travis-ci.org/ktalik/Sturing)

Sturing
=======

Simple Turing machine interpreter and simulator.

1. Compilation
--------------

To compile Sturing, simply run `make`.

2. Usage
--------

Usage of `sturing` looks as follows:

	sturing [OPTIONS] [SOURCE FILE] [TAPE FILE]

If no SOURCE FILE is given, **interactive mode** is started.

If there is no TAPE FILE, the tape will be initialized as empty.

To list all possible options write:

	sturing --help

### 2.1. Syntax

The syntax of Simple Turing machine source code is simple. Actually it follows a defined pattern:

	[state name]:
	[encountered character] [character to be written] [move] [jump]

What you see above we will call a *rule*. A rule is a set of five characteristics. First two of them are conditions required to perform a particular rule and last three of them are actions to perform within this conditions. **State name** indicates in which state we will perform those actions. **Encountered character** tells the machine that those actions had to be taken when given character in given state will be met. The third characteristic - **character to be written** - indicates which character will be written on the current place of the head of the machine. **Move** action means which "physical" action of the machine's head should be performed (move left, move right or stay). At last, **jump** means to which state our machine should pass within this rule.

State names and characters can be named by more than one UTF-8 characters, e.g.:

	StateA:
	// Write a new_character, move right and jump into the StateB
	character     new_character > StateB
	new_character character     > StateB
	...

Sturing syntax provides C-like comments, as one above. As you can observe, there is a special *keyword* used to indicate "move right" action.

There are various keywords used to describe various characteristics:

* character to be written

	It can be specific character (its name) or one of keywords:

	* `\=`, which means writting the same character as met (*NOTE: currently under "construction"*),
	* `\tape`, which means a *blank* tape character (printed in the simulator as "_" (without the quote marks).

* move

	It can be "left", "right" or "stay", determined by following keywords:

	* *right* has following keywords:

		`\right`, `r`, `R`, `p`, `P`, `>`

	* *left* has following keywords:

		`\left`, `l`, `L`, `<`

	* for *stay* that are:

		`\stay`, `s`, `S`, `=`

* jump

	In this action we can specify a concrete state (its name) or use following keywords:

	`\done`, `^`, `;`

	which means that the machine process will be end within this rule.

### 2.2. Running

To start interpreting your code, prepare rules and start the machine simulator simply run:

	sturing mycode.stu

If you want to use a specific tape written in a file, simply write:

	sturing mycode.t tape.tap

The contents of every tape should looks as follows:

	[character]<space>[character]<space>[character]<space>...

Which means that every character should be separated with any other using whitespace character (space, tab, new line or any other).

"Tapes" like these are being loaded to interpreter and placed on an "infinite" tape of the machine. The starting position of the machine's head is a leftmost character of loaded tape.

Examples:

* `1 2 3 4 5 6 7 8 9 0`

* `1 0 0 1 1 0 1 0 1 1 1 0 1 1 0 0 0 1 0 1`

* `Lorem ipsum dolor sit amet`

* `foo bar baz quux corge grault garply`

### 2.3. Options use

Useful options are intended to be helpful with some cases:

1. You are writing in Sturing for the first time. You want to run interactive mode with helpful messages, which will describe what is going on inside the machine:

		sturing -v

2. You are a little bit experienced Turing machine programmer, but interpreter founds an error in your code and your source file is big. You want to take a look at line in which error takes place and see whole source code with line numbers:

		sturing --echo-lines mycode.stu mytape.tap

	(the error message will be right below its line)

3. You have performed some computations on binary numbers (or other code) and want to print the results as one word:

		sturing --no-spaces mycode.stu mytape.tap

*NOTE:* The `--board` option is currently under development and works nice only with one-sign state names and character names.

3. Examples
-----------

Examples placed in the `examples` folder:

*A simplest Sturing Hello World program:*

	A:
	_ Hello,World! > ^

*Binary negation with comments usage example:*

	A: // First state.
	0 1 // We negate the character.
	    > A // We continue.
	1 0 // As above.
	    > A
	_ _ = ^

*Example of a program answering a simple question:*

	Jump:
	Tell	_ > Jump
	me	_ > Jump
	if	_ > Jump
	a	_ > Jump
	apple	_ > Fruit
	banana	_ > Fruit
	orange	_ > Fruit
	potato	_ > Vegatable
	tomato	_ > Vegetable
	pumpkin	_ > Vegetable
	_	_ = NoQuestion
	
	Fruit:
	is	_ > Fruit
	a	_ > Fruit
	fruit?	_ > Yes
	_	_ > NoQuestion
	
	Vegetable:
	is	_ > Vegetable
	a	_ > Vegatable
	fruit?	_ > No
	_	_ > NoQuestion
	
	Yes:
	_ YES = ^
	
	No:
	_ NO = ^
	
	NoQuestion:
	_ You_haven't_given_any_question! = ^

*Exemplary tape:*

	Tell me if orange is a fruit?

*Result:*

	_ _ _ _ _ _ _ _ YES

4. License
----------

Copyright 2013 Konrad Talik <konradtalik@gmail.com>

This software is free; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
