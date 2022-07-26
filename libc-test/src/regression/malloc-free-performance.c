#include <malloc.h>
#include <time.h>
#include <sched.h>
#include <errno.h>
#include <string.h>
#include "test.h"

#define MALLOC_TIME 131072
#define CLUSTER_SIZE 16
#define SIZE_ARR_SIZE 2048
#define NANOSEC_PER_SEC 1e9

/* A random generated size array for performance test
 * The sizes array is roughly uniformed distributed in every bin
 */
size_t sizes[SIZE_ARR_SIZE] = {
 70398,   5870,    812,    782, 132092,     44,  10608,    814,   4523,    140,   3989,   2872,  71794,   1503,   3682,
199068,     75,  42231,    682,    622, 154292, 130053,    237,    839,    324,  99182, 140794,   4150,    969,   1080,
  4392,   2978,    202,    174,   4236,  11422,   9935,    449, 164211,   1647,    264,  85939, 125428,  68810,   7330,
  1077,  50947, 224883,   1211,    775,    899,  13313,  13711, 153171,   2629,  29524, 121541,  22415,    462,    547,
    39,    422,  52062,  36162,    517,   1259,    585,  46209,    332,   9488,    555,    940,    395,    108,  70242,
   652,    165,  24921,  10310,    611,  30317,   7061,     43,    170,    553, 127383,    585,  15952,   4790,  82906,
   235,    836,  12391,    267,  28743,    802,    455,   7593,   1992,    865,   1775,    750,  14738,    617,  81735,
 66408,    802,    388,    999,   2000,  18975,  67893,      3,    451,    616,    778, 239766,   6633,  45054,    197,
   303,  12049,    555,   6787,    429,     68,    333, 132413,    937,  12291,  30571,   3302,  44731,   1402,    460,
   772,   3274,  99137, 220195,   3751,  28575,    299,   1893,    229,    487,  21691, 154058,  40029,   4335,    910,
214958,    485,  88161,    900,  14042,    107,  11356,    906,    553,    559,    847,   1593,    815,    271,   8454,
 95479,    814,    486,    556,    325,  13687,    995,    805,   8407,   1179,    332, 244198,    525,    105,    838,
204603,    841,    874,   6653,    647,   9133,     79,    963,    259,    193,    238,   1626, 184019,    968,    360,
 20162,    388,   1368,  31575,    674,      1,    303,    418,    995,  23078, 183823,  56425, 208634,    577,  36907,
   705,   3981,  12763,    328,    621,   4939,   1340,    742,    805,    137,    867,    109,   1466,    205,   1002,
   516,   1007,    742, 258449,  15950,     38,   3818,    295,    552,  39173,  12387,    711,   1376,  29945,   7137,
  1621,    236,  27868,    452,     46,    171,    683,   2843,   6833,     43,    356,  11609,   1305,    805,  53323,
  3832, 198683,    808,    429,   1895,   2713,    558,    654,    813,   9032,    905,    333,    773,    781, 247876,
 47654,   3777,    686,   2506,   5770,  22422,  15557,    292,  79991,     47, 137903,    879,   3736,  20517,   2879,
   460,    749,    423,  61714,    493,  95574,  13594,     69,  10209,    806,    975,    357,    905,   9232,  27054,
235843,   7299,  56998,    106,   7697,    422,    577,    428,  32538,    455,   4304,   1942,   2946,   3531,    424,
  1834,    780,   2013,    228, 159815,  79256,    751,    204,    973,    965,    194,    776,  98967,   5460,    462,
 70645,    111,      6,   1003,    389, 101973,   3200,    463,  43516,   3029,    783,    837,    683,  11671,  33303,
 47768,  10458, 179347,    973,  38940,  75350,  14263, 261886,    333,   4954,    300,    907,  17111,  68577,  19266,
   782,    802,    937,  26320,  28570,   2830,    686,    292,    484,    334,    143,  64594,  54629,    552,    137,
   871,   3248,   1684,    327, 203105, 128726,   3817,  55839,   3382,    163, 151956,     98,   5776,    841,   4906,
138722, 130420,  57664,   3453,  20918, 166702,    260, 226203,    715,    134,    655,   2196,  33196,    515,    233,
 11594, 116927,   3203,    164,  30898,  17118,   1816,    715,  58259,    173,    226, 122801,  17628,  80971, 217244,
 83675,    103,    616,   3842,  47968,    452,    385,    975,    617,    449,    514,  27115,      3, 117059,   2465,
 64951,   2461,    588,   2003,    482,    682, 216424,    488,    101,  20908, 134882,   3347,    388,    547,    975,
     5,    616,    513,    876,   1001,    389,    259,    684,   3288, 106796,   5408,    769,   2984,  45442,   8939,
  9359,   5672,    553,   2258,   9126,    738,  67280,     65,  53874,   3215, 246562,   3820,    996, 156079,    101,
  4231,    651,  12558,    324,    577,    716,  16016,    269,    934,    841,  14866,    609,  21962,   1920,    267,
   674,    676,     70,    488,    707,     99,    322, 129879,   2967,    550,    903,    810,    741,   1739,  76817,
  1514,    875,  75663, 102461,   5887,   5522, 171815,   1220,    206,    559,    134,    744,    900,   5656,   6482,
  5913,  15317,     69,   1500,  14341,    423,  78476,    362,    107,    939,   4963,     47, 240406,  14778,    903,
   330,   2130,    323,   1320,   2378,  64207,   4861, 160245,    231,    171,    609,    161,  11773,  22211,  10458,
 47327,    647,  23595,     41,    333,   1610,    709,  18849, 195435,    558,    866,  18759,  72108,   3244, 102716,
 73862,    353,    302,   6803,    455,  15788,  53140, 109524,    777,    969, 113077,    454,    875,    235,  23856,
   644,  21337,  45424,    873,    161,    652,    807,  99721,  27553,    357, 190179,    330,  14428,    589,   3347,
 65978,   9364,    835,   2002,     47,    995,    930,   1253,    387,   6780,   1998,    905,  29723,    610,     79,
   290,  37169,   1971,    743,    172,  14856,  11888,    943,   1859,   1208,     40,  47201,  23335,   5233,  12980,
171863,   3036,   9107,     45,    490,    869,   6588,    333,    746,  26871, 118844, 127074,    551,    813,  20159,
  3136, 197941,    996,    654,  12931,   3141,   1300,    610,    804,    805,   1345, 197261,    168,    263,    362,
  3293,   3166, 178245,    521,   3116,  43376,    262,    933,     72,    170,     34,  19267,    104,    524,    649,
   139,    387,   1138,  16827,    911,  13562,   1722,   4281,    591,  52262,    193, 238772,  70881,   8430,    453,
  1945, 126322, 164070,  97918,    354,     98,   3662,  22224,   3309, 102266,    877,    645,  59699,    931,    648,
  2134, 214595,   4001, 129563,   5792,  52854,     13,    611,   9275,  45407,    679,   4802,   7668,  53087,     97,
 30461,     73,    203,  14303,    324,    261,    140,    940, 100389,    518,  18613, 253547,    519,  71172,  26630,
   551,   1739,    769, 167627,   6280,    931,    229, 108315,  89014,  12596,   1953,  19134,  57947,    427,    206,
  6173,   3532,   8619,  38561,    138,  22338,    519,    106,    932, 259336,    232,   5748,    207,  61851,    487,
   616,    680,    163,  64600,   7255,    780,    936,    647,  33859,    942, 136598,    708,    866,    196,  17475,
   524,    492, 182291,   3859,    484,  16731,  13992,    513,  10509,  40595,  77200,  15873,    715,   8122,    929,
 31017,    962,    616,    739,    517,    397,    452,    425, 146686,   2005,    173,   1348,   5228,      4,    652,
 14700,    359,    321,   5388,    778,    271,  84182,    966,  34596,     35,    356,  28288, 121659,   1796,  17260,
   808, 243982,  12292,     75,   7399,    548, 217421,    289,    559,  50038,    673,    769,  10769,   1934,    488,
   298,  36678,    389,    581,  89654,   4883,  18525,    676,     65,  55459,  74196, 109744,    623,   2527,  94923,
   328,    900,    328,   1101,  64821,    801,   2197,    201,     68,   1004,     72,    143,   4917,   2409,    449,
     8,   1000,    745,    201,    195,    546,    930,   5793,  15314,  11095,   2756,   3127,   8121,     38,    870,
 11231,   3838,  74228,  63376,  56633, 159022,    942, 174037, 182329,  30664, 150490,    801,     40,  24289,    675,
   547,   4046,    481,  61835,    964,    133,  45929,  35021,   4224,   1000,    910,  30014, 216676, 107110,  59468,
127074,    586,     67,     68,   4344,   3758, 129795,    426,    458,   1466,    556,    295,  26140,  98849,  56438,
161433, 108530,    649,  16593,  54624,  58346,    131,  83535,   1549,    227,  45126,     15,    967,    196,   1031,
   289,  12894,   7347,     11,    808,  29130,    619,    101,    264,    652,   6118,   5445,    270,    938,   1001,
   398,    810,   9316, 194708,    993,   1902,   3191,    293,     98,    527,   1645,   1512,   1988,    586,  76639,
182172,    804, 254145,    866,  30406,    588,    418,    428,   1594,   5151,   4881,   8454,    714,  29323,    515,
   806,  20037,   2379,  77376,   7505, 122331,     67,  46036,   1000,   1046,    333,    621,    582,    207,    555,
 23285,   1917,    937,    519,    267,     35,  77624,    358,    587,    162,  29879, 141182,    194,  20079,    654,
  1717,  10064,   3432,    462,   1001,  23544,    804,   1872,    710,    494,   4163,   8624,    325,  29374, 237775,
161384,    392,   3701,  27911,   4655,    361,  24420,     78,    300,    321,  36528,    876,    739,    620,   1149,
   550,  30840,    164,    484,    525,    292,    520,      2,    485,   6254,   1723,    394, 260413,   1940,  11780,
  2444,    993,    869,  32460,    942,   1384,   1004,   1479,     99,   5976,    335,    389,    299,  90270,    962,
 15520,   2144,    869,  32604,   5137,    549,  63247, 122549,    557,    303,   8963,    206,   6963,    130, 123812,
   332,   3419,  74006,  12617,    484, 163211,   1551,    545,    395,    911,    939,   5195,     42,  26251,  80176,
  4982,    746,  24097,    490, 124671,  70629, 236138,   7829,  79143,  32527,    430,    929,  29291,    335,   2397,
   589,    166,    868,  66068,    709,    298,   8929,   5659,    778,    484,  53316,    872,    591,  27338,  29233,
  1002,   1914,  13784,    878,   4677,    870,      7,  93312,    966,    334,    931,    291,    680,    229,  91335,
   974,      1, 163843, 232509,    719,    295,    815,  66563,  25897,   2112,    137, 206975, 122126,   5636,    840,
178041, 121125,  96929,  12271,    525,   1194,  39935,   1812,    359,     70,    586,  28109,    655,   1823,  15971,
  1165,  50539,   7517,    613,   4334,    869, 143267,     33, 120769,  41743, 181472,    136,    775,    898,    687,
 87489,  81862,    526,   1041, 119803,    203,    556,   2830,   1753,    995,    968,   1000,    362, 105558,  25289,
 30313,    167,    897,    483,  87996,    131,   1286,     73,   4778,   3765, 162611,   2258,   4071,    577,    740,
 19328,  23181,  44833,    356,    300,    481,    101,   1945,  55445,   7862,    586,   1690,    108,   3179,   6265,
   833,    232,    513,    774,     38,   1006,  16313,      2,   5722,    929,    648, 124091,   3128,     12,    653,
   289,    168, 113025,    394,  22275,  36138,    934,  33296,  44428,    648,  23347,     36,    966,   7656,  30616,
   303,  12065,    395,   2175, 110915,   3400,   3985,    547,   1457,  77770,    775,    942,    677,     47,    845,
261480,  20514,  26542,    644,    709,   8629, 160291,     43,  12951,    613,    771,    490,    172,  23687,   2678,
112194,  15152,    583,   6109,  76361,  48932, 138769,  68824, 256665,   1214,    427, 197349,  11553,     35,    292,
  7826,    386,  15287, 143431,    139,    742,    783,   7933,    550,  61036, 153864,   5527,   1220, 179326,  52538,
   297,    331, 116679,  32116,    548,   7180,  24432, 150458,    647,    198, 230679,    492, 183327,     10,    582,
    46,    742,  19932,    139,    333,    838, 190324,  82554,   3673,    263,    810,  20105,  18894,    801,   3902,
  3452,    203,  30556,    521,    805,    399,    491, 261614,    132,    745,    646,    621,    815,  21428,  32822,
   941,     35,    775,    803,   9277,     77,   3531,     73,    877,    353,    582,   1561,   1794,   6272,  14250,
 10258,  87923,  16317,   1574, 107483, 118292,    230,    492,    193,    522,    903,  21672,    259, 101987,   5379,
   547,  42742,    131,    909,  11141,  45670,    650,    327,    203,    417,   1822,  73206,   4047,   5651,  31863,
   364,   6056,    482,   2414,    686,     67, 116497,    233,  89151,    712,   1960,  15023,    457,    911,  87709,
   326,    647,    131,   1544,    493, 103295,     35,   4201, 120600,    679,   4536,    354,    649,  12877,    514,
    11,   7599,    492,  66894,    779,   5012,    803,    270,  27587,   1716,    653,     15,   1745,  81311,    142,
 74662,   2476,    847,    621,   2326,    842, 183990,  48969,  37165,    258,  15920,   1450, 139424,     97,    622,
217730,    389,  14629,    682,  82614,  10637,    170,    205,  16924,   1684,    525,    675,      2,  39617,    237,
   324,    623,   3998, 171709,    130,  44293,    301,    900,  36268,    297,    130,   3905,    364,   8500,   6175,
   267,  74017,    387,    963,    869,      0,    653,    324,    813, 138388,    673, 132383,    835,   2206,    259,
 87218,     99,   7984,    521,    808,   9500, 126004,   1614,  20344,    142,    972,   9619,    684,   1709,    430,
   260, 112449,  73506,    867,  15030,  37867,    417,    874,  43752,   7460,  63448,    582,   1796,   8038,   1495,
 19720, 234481,  43216,    554,     33,    137,    908,   2959,     11,    106,    642,  37055,    973,    495,    969,
    73,    836,  84397,  94851,   1000,    354,  18329,  38597,    970,  16503,   1215,    900,   9823,   9582,    132,
  8123,    801,    236,    642,    929, 120593,    713,    486,     34, 241685,    492,  49871,  30206,   2905,    936,
  6818,  14692,    461,    714,  70500,    718,    868,    996,  65063,    737,    559,    677,    555,   6234,   5245,
  1136,    553,    751,    236,     67,   1167,  28052,    199,   2634,  22830,  38653,   1874,    366,    579,   7451,
194153,    488,    907,  13417,    616,   1593,    293,  14217,   3553,    419,    492,    835,    334,   3730,    875,
   775,    969,   3959,    430,    930,    167,    616,    651,    968,    644,    556,   8349,    710,    551,    647,
   527,    170,    652,   5834,     43,     78,    205,    678,    748,    650,    998,    495,    556,    108,   1917,
   484,    111,   1540,     11,  86819,    547,    267,    527,  21645,    549,    611,   7878, 163845,    269,   1004,
   750,    994,    101,  28372,    386,    173,   3086,  48514,   8148,    961,   9005,  12271,    619,    641,   7038,
   263,   1768,    910,   6053,   6276,  88873,    623, 170360,    685,  16800,   2299,   2431,  35377, 147880,    643,
128403,   5086,     97,    131,    583,   1690,    939,   6601, 221042,     79,  30789,    357,  87455,    522,    239,
   904,      5,   1174,    293,  18785,    587,   3587,    238,    335,    581,  36976,  40494,    933, 198961,   1719,
   554,    267,    132,    674,    421,   5792,    745,    492,   2013,   2290,   3834, 106626,    773,    270,    175,
   901,   6109,   4583,    422, 127089,     73,  88055,  43610,  72305,   1878,    425,  44179,   2765,   1200,   2753,
   749, 150927,   3509,  14040,   1549,    424,    454, 219113,    491,    910,    622, 160490,  21886,    715,    457,
 63071, 178200,   3760,    770,    705, 255855,   1254,   5798,    527,    676,   7920,    716,  17543,    294,    813,
    68, 221793,    100,   1636, 106218,    747,    877,    193,    686, 203251,    100,    587,    554,   1477,    335,
 31222,    324,    398,    711,   5926,   4036,    326,  60222,    233,   3716,  65404,  16007,    142,    419,    874,
  1001,   1829, 253309,  59056,    973,   3390,   1432,    236,     15,   2726,    642,    617,   3782,    135, 186049,
   359,  14274,    193,    972,    105,    934,     47,    810,    746,    744,     41,    813,  16071,   8974,  36929,
   610,    169,  12477,  13763,    259,    259,    845,   2367,    549,    430,  70141,    685,   1962,    580,  68783,
 66713,    196,    387,    292,      1,   9189,  61834,    558, 107624,    750,    367,    678,   1327,   2686,    229,
 91802,   4874, 173587,  31323,  13180,    740, 124601,   1433,   7331,  15637,   1713,    579,    876,  16730,   3138,
   936,  61890,    993,  91003, 102014,    195,     78,   1178,    463, 221454,    451,  59083,    742,    615,    586,
255579,    774,  32922,   6243,    325,    675,  44268,    553, 112234,   1506,    903,    778,    492,   3168,  63549,
   879,   4683,   8721,   7697,    327, 105304,    164,  50339,    171,  55374,   5043,    705,   3741,  16724,    271,
   196,    932,    771,    391,    999,  85846,  51146,    257,    686,    937,  41264,   1003,    197,  20528,    614,
   929,    965,    299,    109,  84938,   7700, 126923,  24435,    460,    551,    591,  18712,    589,  57468,   8396,
172592,   2621,    105,    557,    101,   4858, 187139,    520,   3930,  47360,    296, 259193,  88797,  18147,    642,
122056,   3308,  11404,    452,  20060,    334,      7, 140626,  52316,  13883,    868,  24962,  19392,   7411,    901,
191194,    581,  78399,    386, 200180,    774,    684,    514,   5030,  33323,  21820,    421,   7067,    484,    962,
   683,     75, 209451,   3966,     38,    237,  13130,  23696
};


int main(int argc, char *argv[])
{
	cpu_set_t mask;
	void *ptr[CLUSTER_SIZE] = {0};
	struct timespec ts[2];

	CPU_ZERO(&mask);
	CPU_SET(0, &mask);
	if (sched_setaffinity(0, sizeof(mask), &mask) < 0) {
		t_error("Set CPU affinity failure, ERROR:%s\n", strerror(errno));
		return -1;
	}

	clock_gettime(CLOCK_REALTIME, ts);
	size_t index = 0;
	for (int i = 0; i < MALLOC_TIME / CLUSTER_SIZE; ++i) {
		for (int j = 0; j < CLUSTER_SIZE; ++j) {
			ptr[j] = malloc(sizes[index]);
			if (!ptr[j]) {
				t_error("Malloc size of %u byte(s) failed: %s\n", sizes[index], strerror(errno));
				return -1;
			}
			++index;
			index %= SIZE_ARR_SIZE;
		}
		for (int j = 0; j < CLUSTER_SIZE; ++j) {
			free(ptr[j]);
		}
	}
	clock_gettime(CLOCK_REALTIME, ts + 1);

	double cost = (ts[1].tv_sec - ts[0].tv_sec) * NANOSEC_PER_SEC +
		(ts[1].tv_nsec - ts[0].tv_nsec);

	t_printf("Malloc and free %d times cost %lf s\n", MALLOC_TIME, cost / NANOSEC_PER_SEC);

	return t_status;
}
