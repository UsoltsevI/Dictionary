# Brief description
In order to start using the program install the repository on your computer. In order to compile the program tap `make` on your keyboard. The executable file is called `hd.x` for double hashing principle, `hsc.x` for chaining principle, `hc.x` for single-chaining principle (the principle, in which there is only one chain for the entire table) by default.

Input data requirements: `<the_number_of_expected_responses> <dictionary_length_including_spaces> <the_dictionary_line_itself> <the_length_of_the_string_of_the_searched_words,_including_spaces> <a_string_containing_the_words_you_are_looking_for>`

If you want to check answers, tap `check` after ./hsc.x. For example: `./hsc.x check < Tests/Test1.txt`
If you don't, tap nonhting: `./hsc.x < Tests/Test1.txt`

To test the program using my tests from Tests/ folder tap `make test`.
If you want to using my tests from Tests/ folder for only one hashmap implementation tap `make testd` (double hashing) or  `make testsc` (chaining) or `make testc` (single-chaining).

# Some notes
Note 1: the program does not support searching for strings where there are several spaces in a row.
Note 2: the sepchhmap.c file does not have an implementation of resizing function, but the main.c file requires calling it (in the latest implementation).

# Tests results
After testing this three hash map implementations using my tests, the following results were obtained: 

If we use a large hashmap (for example, size = number_of_data / 2), the fastest principle is a simple chaining. This is because it has a simple alghoritm and there are no long chains in it. The implementations with double hashing and single-chaining are about the same.

If we use a small hashmap (for example, size = number_of_data / 16), the fastest principle is a single-chaining, and the lowest is a double hashing. It causes by a principle of double hashing, which requires a resize in this case.

If we call the resizing function, single-chaining principle is faster than double hashing. This is because single-chaining principle uses previous data chain.

In my implementation the fastest princilpe is a single-chaining. 

# Links
To learn more about "uhashmap", follow the link [UHmap](https://github.com/UsoltsevI/UHmap)

[Hash chain](https://en.wikipedia.org/wiki/Hash_chain)
