#ifndef Z_SQLDATABASE_OUTPUT_H
#define Z_SQLDATABASE_OUTPUT_H




/*
---------SQL---------
//****************************************************************************
//		CREATE AND INSERT
//****************************************************************************

//---- employee table ----------
[0] Command: make table employee fields  last, 		first, 			dep





====SQL DONE!======


[1] Command: insert into employee values Blow, 		Joe, 			CS





====SQL DONE!======


[2] Command: insert into employee values Johnson, 	"Jimmy", 		Chemistry





====SQL DONE!======


[3] Command: insert into employee values Yang, 		Bo, 			CS





====SQL DONE!======


[4] Command: insert into employee values "Jackson",	Billy, 			Math





====SQL DONE!======


[5] Command: insert into employee values Johnson,	Billy, 			"Phys Ed"





====SQL DONE!======


[6] Command: insert into employee values "Van Gogh",	"Jim Bob", 		"Phys Ed"





====SQL DONE!======



[7] Command: select * from employee




Table name: employee:
record          last          first          dep
0 / 6            Blow            Joe             CS
1 / 6         Johnson          Jimmy      Chemistry
2 / 6            Yang             Bo             CS
3 / 6         Jackson          Billy           Math
4 / 6         Johnson          Billy        Phys Ed
5 / 6        Van Gogh        Jim Bob        Phys Ed



====SQL DONE!======



//----- student table ----------
[8] Command: make table student fields 	fname, 			lname, 		major, 				age





====SQL DONE!======


[9] Command: insert into student values 	Flo, 			Yao, 	CS, 				20





====SQL DONE!======


[10] Command: insert into student values 	"Flo", 			"Jackson", 	Math,	 			21





====SQL DONE!======


[11] Command: insert into student values 	Calvin, 		Woo, 	Physics,			22





====SQL DONE!======


[12] Command: insert into student values 	"Anna Grace", 	"Del Rio", 	CS,	 				22





====SQL DONE!======


[13] Command: select * from student




Table name: student:
record          fname          lname          major          age
0 / 4             Flo            Yao             CS             20
1 / 4             Flo        Jackson           Math             21
2 / 4          Calvin            Woo        Physics             22
3 / 4      Anna Grace        Del Rio             CS             22



====SQL DONE!======





//****************************************************************************
//		SIMPLE SELECT
//****************************************************************************

[14] Command: select * from student




Table name: student:
record          fname          lname          major          age
0 / 4             Flo            Yao             CS             20
1 / 4             Flo        Jackson           Math             21
2 / 4          Calvin            Woo        Physics             22
3 / 4      Anna Grace        Del Rio             CS             22



====SQL DONE!======



//------- simple strings -------------------
[15] Command: select * from student where lname = Jackson



output_q:  {lname}->[Jackson]->[=]-> |||


Table name: student16807:
record          *
0 / 1             Flo        Jackson           Math             21



====SQL DONE!======



//----- quoted strings ---------------------
[16] Command: select * from student where lname = "Del Rio"



output_q:  {lname}->[Del Rio]->[=]-> |||


Table name: student282475249:
record          *
0 / 1      Anna Grace        Del Rio             CS             22



====SQL DONE!======



//-------- non-existing string ------------------
[17] Command: select * from student where lname = "Does Not Exist"



output_q:  {lname}->[Does Not Exist]->[=]-> |||


Table name: student1622650073:
record          *



====SQL DONE!======



//****************************************************************************
//		RELATIONAL OPERATORS:
//****************************************************************************

//.................
//:Greater Than   :
//.................
[18] Command: select * from student where lname > Yang



output_q:  {lname}->[Yang]->[>]-> |||

[{Woo:[2,]} ]
Table name: student984943658:
record          *



====SQL DONE!======




//. . . . . . . . . . . . . (Greater Than: Non-existing) . . . . . . . . . . . . . . . . . . . . .
[19] Command: select * from student where age > 50



output_q:  {age}->[50]->[>]-> |||

[{21:[1,]} ]
Table name: student1144108930:
record          *



====SQL DONE!======



//. . . . . . . . . . . . . (Greater than: last item) . . . . . . . . . . . . . . . . . .
[20] Command: select * from student where age  > 53



output_q:  {age}->[53]->[>]-> |||

[{21:[1,]} ]
Table name: student470211272:
record          *



====SQL DONE!======



//. . . . . . . . . . . . . (Greater Than: Passed last item) . . . . . . . . . . . . . . . . . . . . .
[21] Command: select * from student where age > 54



output_q:  {age}->[54]->[>]-> |||

[{21:[1,]} ]
Table name: student101027544:
record          *



====SQL DONE!======



//.................
//:Greater Equal  :
//.................
[22] Command: select * from student where lname >= Yang



output_q:  {lname}->[Yang]->[>=]-> |||

[{Woo:[2,]} ]
Table name: student1457850878:
record          *
0 / 1             Flo            Yao             CS             20



====SQL DONE!======


//. . . . . .  (Greater Equal non-existing: ) . . . . . . . . . . .
[23] Command: select * from employee where last >= Jack



output_q:  {last}->[Jack]->[>=]-> |||

[{Johnson:[1,]} {Van Gogh:[5,]} ]
Table name: employee1458777923:
record          *
0 / 5         Jackson          Billy           Math
1 / 5         Johnson          Jimmy      Chemistry
2 / 5         Johnson          Billy        Phys Ed
3 / 5        Van Gogh        Jim Bob        Phys Ed
4 / 5            Yang             Bo             CS



====SQL DONE!======




//.................
//:Less Than      :
//.................


//. . . . . . . . . . . . . (Less Than: Non-existing) . . . . . . . . . . . . . . . . . . . . .
[24] Command: select * from student where lname < Jackson



output_q:  {lname}->[Jackson]->[<]-> |||

[{Woo:[2,]} ]
Table name: student2007237709:
record          *
0 / 1      Anna Grace        Del Rio             CS             22



====SQL DONE!======



//. . . . . . . . . . . . . (Less than: first item) . . . . . . . . . . . . . . . . . .
[25] Command: select * from student where age  < 20



output_q:  {age}->[20]->[<]-> |||

[{21:[1,]} ]
Table name: student823564440:
record          *



====SQL DONE!======




//. . . . . . . . . . . . . (Less Than: before first item) . . . . . . . . . . . . . . . . . . . . .

[26] Command: select * from student where age  < 19



output_q:  {age}->[19]->[<]-> |||

[{21:[1,]} ] [{21:[1,]} ]
Table name: student1115438165:
record          *



====SQL DONE!======




//.................
//:Less Equal     :
//.................

[27] Command: select * from student where lname <= Smith



output_q:  {lname}->[Smith]->[<=]-> |||

[{Woo:[2,]} ] [{Woo:[2,]} ]
Table name: student1784484492:
record          *
0 / 3      Anna Grace        Del Rio             CS             22
1 / 3             Flo        Jackson           Math             21
2 / 3          Calvin            Woo        Physics             22



====SQL DONE!======



//. . . . . .  (Less Equal non-existing: ) . . . . . . . . . . .
[28] Command: select * from employee where last <= Peach



output_q:  {last}->[Peach]->[<=]-> |||

[{Johnson:[1,]} {Van Gogh:[5,]} ] [{Johnson:[1,]} {Van Gogh:[5,]} ]
Table name: employee74243042:
record          *
0 / 5            Blow            Joe             CS
1 / 5         Jackson          Billy           Math
2 / 5         Johnson          Jimmy      Chemistry
3 / 5         Johnson          Billy        Phys Ed
4 / 5        Van Gogh        Jim Bob        Phys Ed



====SQL DONE!======



//****************************************************************************
//		LOGICAL OPERATORS
//****************************************************************************


//.................
//:AND            :
//.................

[29] Command: select * from student where fname = "Flo" and lname = "Yao"



output_q:  {fname}->[Flo]->[=]->[lname]->[Yao]->[=]->[and]-> |||


Table name: student114807987:
record          *
0 / 1             Flo            Yao             CS             20



====SQL DONE!======




//.................
//:OR            :
//.................
[30] Command: select * from student where fname = Flo or lname = Jackson



output_q:  {fname}->[Flo]->[=]->[lname]->[Jackson]->[=]->[or]-> |||

====inside case or====

Table name: student1137522503:
record          *
0 / 2             Flo            Yao             CS             20
1 / 2             Flo        Jackson           Math             21



====SQL DONE!======




//.................
//:OR AND         :
//.................
[31] Command: select * from student where fname = Flo or major = CS and age <= 23



output_q:  {fname}->[Flo]->[=]->[major]->[CS]->[=]->[age]->[23]->[<=]->[and]->[or]-> |||


Table name: student1441282327:
record          *
0 / 4             Flo            Yao             CS             20
1 / 4             Flo        Jackson           Math             21
2 / 4          Calvin            Woo        Physics             22
3 / 4      Anna Grace        Del Rio             CS             22



====SQL DONE!======




//.................
//:AND OR AND     :
//.................

[32] Command: select * from student where age <30 and major=CS or major = Physics and lname = Jackson



output_q:  {age}->[30]->[<]->[major]->[CS]->[=]->[and]->[major]->[Physics]->[=]->[lname]->[Jackson]->[=]->[and]->[or]-> |||


Table name: student16531729:
record          *
0 / 3             Flo            Yao             CS             20
1 / 3             Flo        Jackson           Math             21
2 / 3      Anna Grace        Del Rio             CS             22



====SQL DONE!======




//.................
//:OR AND OR      :
//.................

[33] Command: select * from student where lname = Yang or major = CS and age < 23 or lname = Jackson



output_q:  {lname}->[Yang]->[=]->[major]->[CS]->[=]->[age]->[23]->[<]->[and]->[or]->[lname]->[Jackson]->[=]->[or]-> |||


Table name: student823378840:
record          *
0 / 4             Flo            Yao             CS             20
1 / 4             Flo        Jackson           Math             21
2 / 4          Calvin            Woo        Physics             22
3 / 4      Anna Grace        Del Rio             CS             22



====SQL DONE!======














======THIS IS THE END======

*/


#endif // Z_SQLDATABASE_OUTPUT_H
