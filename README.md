# CENG334 HW2 Tester

See and create [issues](https://github.com/y4nci/CENG334-HW2-Tester/issues) about this tester.

## To Execute

 - Put your `hw2` executable in the root folder of this tester
 - `make`
 - `./tester-hw2`
     - Another usage is `./tester-hw2 <test case count> <minimum dimension of matrices> <maximum dimension of matrices>`
         - `test case count`: The number of test cases. I suggest to keep this number between 1-10, as going higher causes memory errors currently.
         - `minimum dimension of matrices`: The minimum dimension of generated matrices. Does not guarantee that a matrix having this number as a dimension is going to be generated, this number just provides a lower bound for randomly generated dimensions.
         - `maximum dimension of matrices`: The maximum dimension of generated matrices. Does not guarantee that a matrix having this number as a dimension is going to be generated, this number just provides an upper bound for randomly generated dimensions.
     - To learn more about this command, type `./tester-hw2 -h` or `./tester-hw2 --help` 

## INFORMATION ABOUT TEST UNITS:

 - **Sum results**: Tests the expected result of sums J and L with the actual outputs. The outputs are parsed from the thread outputs.
 - **Matching output matrices**: Matching the outputs of thread with `matrix id = 2` outputs with the final output printed by hw2.
 - **Final matrix**: Matching the final output printed by hw2 with the expected output of the final multiplication.
 - **Thread count**: Matching the thread count printed by main with the expected thread count (2N + M). The total number of lines are used for this purpose.
 - **Thread synchronisation**: Detecting concurrent additions of A&B and C&D, Detecting concurrent multiplication and addition. These two units check whether two different matrix additions occured concurrently and addition and multiplication occured concurrently, respectively. Sometimes the results of these test might be `FAILED` but that does not mean your code does not work properly. Your code might work just fine but may not execute additions and multiplication concurrently for some reason in a test case, which usually happens with cases having relatively small matrices.

## What It Is

This tester is a program which merely checks the basics like the arithmetic results and thread counts. This is only for your convenience and to give you an idea about what could go wrong during the evaluation of your code. You are encouraged to test the generated input matrices on your code manually.

## What It Is Not

This tester, in any way, is not an official tester, and the result produced by this tester is not guaranteed to be correct. Moreover, this tester does not test certain situations and conditions (like unnecessary waiting etc.)
