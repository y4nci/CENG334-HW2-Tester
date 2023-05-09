# CENG334 HW2 Tester

## To Execute

 - Put your `hw2` executable in the root folder of this tester
 - `make`
 - `./tester-hw2`
     - You can learn more about this command by typing `./tester-hw2 -h` or `./tester-hw2 --help` 

## What It Is

This tester is a program which merely checks the basics like the arithmetic results and thread counts.

There are also two other testing units, which are `THREAD SYNCHRONISATION BETWEEN SUM THREADS` and `THREAD SYNCHRONISATION BETWEEN MUL THREADS AND SUM THREADS`. These two units check whether two instances of additions occured concurrently and addition and multiplication occured concurrently, respectively. Sometimes the results of these test might be `FAILED` but that does not mean your code does not work properly. Your code might work just fine but did not execute additions and multiplication concurrently for some reason, which is the case for small dimensions usually.

## What It Is Not

This tester, in any way, is not an official tester, and the result produced by this tester is not guaranteed to be correct. This tester is only for your convenience and to give you an idea about what could go wrong during the evaluation of your code.
