# Checksum

This program calculates the checksum for the text in a file. It takes two command line parameters. The first parameter is the name of the input file for calculating the checksum. The second parameter is the size of the checksum (8, 16, or 32 bits).  It opens the two files, echos the processed input to the screen, makes the necessary calculations, and then outputs the ciphertext to the console.

Checksum Size
The checksum size is a single integer, passed as the first command line argument. The valid values are the size of the checksum, which can be 8, 16, or 32 bits. Therefore, if the first parameter is not one of the valid values, the program should advise the user that the value is incorrect with a message sent to STDERR.

Format of the Input File
The input file consists of the valid ASCII characters associated with the average text file. This includes punctuation, numbers, special characters, and whitespace.

Included in this repository are sample files.
