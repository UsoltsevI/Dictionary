# Brief description
In order to start using the program install the repository on your computer. In order to compile the program tap `make` on your keyboard. The executable file is called `h.x` by default.

Input data requirements: `<the_number_of_expected_responses> <dictionary_length_including_spaces> <the_dictionary_line_itself> <the_length_of_the_string_of_the_searched_words,_including_spaces> <a_string_containing_the_words_you_are_looking_for>`

If you want to check answers, tap `check` after ./h.x. For example: `./h.x check < Tests/Test1.txt`
If you don't, tap nonhting: `./h.x < Tests/Test1.txt`

To test the program using my tests from Tests/ folder tap `make test`.

Note: the program does not support searching for strings where there are several spaces in a row.

To learn more about "uhashmap", follow the link [UHmap](https://github.com/UsoltsevI/UHmap)
