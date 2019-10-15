

const short int sine180_table[] = {
0x800,0x847,0x88e,0x8d6,0x91d,
0x963,0x9a9,0x9ef,0xa34,0xa78,
0xabc,0xaff,0xb40,0xb81,0xbc1,
0xc00,0xc3d,0xc79,0xcb3,0xcec,
0xd24,0xd5a,0xd8e,0xdc1,0xdf1,
0xe20,0xe4d,0xe78,0xea1,0xec8,
0xeed,0xf10,0xf30,0xf4e,0xf6a,
0xf84,0xf9b,0xfb0,0xfc3,0xfd3,
0xfe0,0xfec,0xff4,0xffb,0xffe,
0xfff,0xffe,0xffb,0xff4,0xfec,
0xfe0,0xfd3,0xfc3,0xfb0,0xf9b,
0xf84,0xf6a,0xf4e,0xf30,0xf10,
0xeed,0xec8,0xea1,0xe78,0xe4d,
0xe20,0xdf1,0xdc1,0xd8e,0xd5a,
0xd24,0xcec,0xcb3,0xc79,0xc3d,
0xc00,0xbc1,0xb81,0xb40,0xaff,
0xabc,0xa78,0xa34,0x9ef,0x9a9,
0x963,0x91d,0x8d6,0x88e,0x847,
0x800,0x7b8,0x771,0x729,0x6e2,
0x69c,0x656,0x610,0x5cb,0x587,
0x543,0x500,0x4bf,0x47e,0x43e,
0x400,0x3c2,0x386,0x34c,0x313,
0x2db,0x2a5,0x271,0x23e,0x20e,
0x1df,0x1b2,0x187,0x15e,0x137,
0x112,0x0ef,0x0cf,0x0b1,0x095,
0x07b,0x064,0x04f,0x03c,0x02c,
0x01f,0x013,0x00b,0x004,0x001,
0x000,0x001,0x004,0x00b,0x013,
0x01f,0x02c,0x03c,0x04f,0x064,
0x07b,0x095,0x0b1,0x0cf,0x0ef,
0x112,0x137,0x15e,0x187,0x1b2,
0x1df,0x20e,0x23e,0x271,0x2a5,
0x2db,0x313,0x34c,0x386,0x3c2,
0x400,0x43e,0x47e,0x4bf,0x500,
0x543,0x587,0x5cb,0x610,0x656,
0x69c,0x6e2,0x729,0x771,0x7b8,
};
// 1 400 4 me, 2 330 la 4 key,3 re mid 330 , 4 la down 330
 // 1: 505 do 470 minner re 450 re 425 minner me 400 me 380 fa 360 sharp fa 340 so 320 sharp so 300 la 285 sharp la 270 si 250 2: 320 minner la 330 sharp so 340 so 360 minner so 375
const short int eatbread_table[] = {
		510,480000000,//do
		455,480000000,//re
		405,480000000,//me6-1

};
const short int die_table[] = {
		510,480000000,//do
		540,480000000,//si
		610,480000000,//la
		685,480000000,//so
		765,480000000,//fa10-1
};
const short int game_table[] = {
510,510,4800000,510,//do
405,405,4800000,405,//me
610,610,4800000,610,//la
510,510,4800000,510,//do
455,455,4800000,455,//re
385,385,4800000,385,//fa
685,685,4800000,685,//so
540,540,4800000,540,//si
510,510,4800000,510,//do
405,405,4800000,405,//me
610,610,4800000,610,//la
510,510,4800000,510,//do
765,765,4800000,765,//fa
610,610,4800000,610,//la
685,685,4800000,685,//so
540,540,4800000,540,//si
4800000,510,510,510,//do
4800000,510,510,510,//do
4800000,510,510,510,//do76-1
/*
4800000,4800000,4800000,4800000,//
4800000,4800000,4800000,4800000,//
510,4800000,540,540,
610,4800000,540,540,
510,4800000,455,455,
4800000,405,405,405,//me
4800000,405,405,405,//me
4800000,405,405,405,//me
4800000,4800000,4800000,4800000,
4800000,4800000,4800000,4800000,
405,4800000,455,455,//me re
405,4800000,385,385,//me fa
340,340,340,340,//so
510,510,510,510,//do 132
305,305,//la
340,340,340,340,//so
385,385,//fa
405,405,405,405,//me
455,455,455,455,//re
510,510,510,510,//do 151
*/
};

const short int start_table[] = {
		455,455,455,455,//re 8
		340,340,480000000,//so
		340,480000000,//so
		305,480000000,//la
		340,480000000,//so
		385,480000000,//fa29
		405,405,480000000,//me
		405,405,480000000,//me
		405,405,480000000,//me
		305,305,480000000,//la49
		305,480000000,//la
		270,480000000,//si
		305,480000000,//la
		340,480000000,//so65
		365,365,480000000,//-fa
		455,455,480000000,//re
		455,455,480000000,//re
		270,270,480000000,//si85
		270,480000000,//si
		255,480000000,//do
		270,480000000,//si
		305,480000000,//la
		340,480000000,//so105
		405,405,480000000,//me
		455,480000000,//re
		455,480000000,//re
		405,405,480000000,//me
		305,305,480000000,//la
		365,365,480000000,//-fa
		340,340,480000000,//so76-1
};
//765,685,610,540,510,455,405,385,365,340,305,270,255,235,215,480000000
//fa, so, la ,si, do, re, me, fa,-fa,so, la, si, do,re, me,space
//0    1   2   3   4   5   6   7   8   9   10  11   12
const short int gameover_table[] = {
		385,385,385,385,480000000,//fa
		405,405,480000000,//me
		385,385,480000000,//fa
		255,255,255,255,480000000,//do
		255,255,255,255,480000000,//do
		385,385,385,385,480000000,//fa
		405,405,480000000,//me
		385,385,480000000,//fa
		305,305,305,305,//la
		305,305,305,305,//la40-1
};


