#ifndef Z_OUTPUT_BTREE_H
#define Z_OUTPUT_BTREE_H


/*
**************************************************************************
**************************************************************************
             E N D     T E S T: 500 tests of 5000 items: VERIFIED
**************************************************************************
**************************************************************************
**************************************************************************


---------------------------------------------------------------------------

Original:

           [110]
     [100]
           [90]
[75]
           [60]
     [50]
           [30]
     [25]
           [12]

---------------------------------------

Insert 80:
           [110]
     [100]
           [90]
           [80]
[75]
           [60]
     [50]
           [30]
     [25]
           [12]

---------------------------------------

Insert 91:
           [110]
     [100]
           [91]
     [90]
           [80]
[75]
           [60]
     [50]
           [30]
     [25]
           [12]

---------------------------------------

Insert 109:
           [110]
           [109]
     [100]
           [91]
     [90]
           [80]
[75]
           [60]
     [50]
           [30]
     [25]
           [12]

---------------------------------------

Insert 108:
           [110]
     [109]
           [108]
[100]
           [91]
     [90]
           [80]
[75]
           [60]
     [50]
           [30]
     [25]
           [12]


------------------------------------------------


     [(key: Pan  value: 6)]
[(key: Joe  value: 4)]
     [(key: Bob  value: 2)]


     [{Pan:[6,]}]
[{Joe:[4,]}]
     [{Bob:[2,8,]}]


----------------------------------------------







*/




#endif // Z_OUTPUT_BTREE_H
