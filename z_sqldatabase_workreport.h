#ifndef Z_SQLDATABASE_WORKREPORT_H
#define Z_SQLDATABASE_WORKREPORT_H



/*
 Features:
-Not Implemented:
    When "and" & "or" both inside of the same command,
    will have one extra record output
-Implemented:
    all other features work as expected, no other bug found.

-Partly implemented:
    /

Bugs     :
    When "and" & "or" both inside of the same command,
    will have one extra record output

Reflections:
Using Map to store table, MMap to store the parse tree，
Recognize the command by building a state machine, storing
the command after "where" to condition ptree, then use
Shungting Yard to put operators, relational words, and values
in order, then evaluate them .  Record class is to read and
write the binary files and also print the records.



 */





#endif // Z_SQLDATABASE_WORKREPORT_H
