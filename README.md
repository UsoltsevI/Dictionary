# Brief description
In order to start using the program install the repository on your computer. In order to compile the program tap `make` on your keyboard. The executable file is called `hd.x` for double hashing principle, `hsc.x` for chaining principle, `hc.x` for single-chaining principle (the principle, in which there is only one chain for the entire table) by default.

Input data requirements: `<the_number_of_expected_responses> <dictionary_length_including_spaces> <the_dictionary_line_itself> <the_length_of_the_string_of_the_searched_words,_including_spaces> <a_string_containing_the_words_you_are_looking_for>`

If you want to check answers, tap `check` after ./hsc.x. For example: `./hsc.x check < Tests/Test1.txt`
If you don't, tap nonhting: `./hsc.x < Tests/Test1.txt`

To test the program using my tests from Tests/ folder tap `make test`.
If you want to using my tests from Tests/ folder for only one hashmap implementation tap `make testd` (double hashing) or  `make testsc` (chaining) or `make testc` (single-chaining).

Note 1: the program does not support searching for strings where there are several spaces in a row.
Note 2: the sepchhmap.c file does not have an implementation of resizing function, but the main.c file requires calling it (in the latest implementation).

To learn more about "uhashmap", follow the link [UHmap](https://github.com/UsoltsevI/UHmap)
