/******************************************************************************/
#ifndef     _NTC3950_H_
#define     _NTC3950_H_
/******************************************************************************/
/*
 * --------------------------------------
 * XXX for 10K PullUp Res Only XXX
 * usage:
 * --------------------------------------
 * #define NTC_AD8(x)          x/256
 * #define NTC_AD10(x)         x/64
 * #define NTC_AD12(x)         x/16
 * --------------------------------------
 * #define  STAND_TEMPE     NTC_AD10(B3950_25)
 * if(g_Tempeture == STAND_TEMPE)
 * --------------------------------------
 */
/******************************************************************************/
#define NTC_AD8(x)          x/256
#define NTC_AD10(x)         x/64
#define NTC_AD12(x)         x/16
#define NTC_AD(x)           B3950_##x/64
/******************************************************************************/
//16bit
#define B3950_N50       0xFBDE
#define B3950_N49       0xFB9E
#define B3950_N48       0xFB57
#define B3950_N47       0xFB0B
#define B3950_N46       0xFAB7
#define B3950_N45       0xFA5A
#define B3950_N44       0xF9F5
#define B3950_N43       0xF985
#define B3950_N42       0xF907
#define B3950_N41       0xF878
#define B3950_N40       0xF7D4
#define B3950_N39       0xF721
#define B3950_N38       0xF666
#define B3950_N37       0xF5A7
#define B3950_N36       0xF4F3
#define B3950_N35       0xF431
#define B3950_N34       0xF35F
#define B3950_N33       0xF28F
#define B3950_N32       0xF1BC
#define B3950_N31       0xF0E6
#define B3950_N30       0xF01E
#define B3950_N29       0xEF4D
#define B3950_N28       0xEE77
#define B3950_N27       0xED9B
#define B3950_N26       0xECD3
#define B3950_N25       0xEBF4
#define B3950_N24       0xEB06
#define B3950_N23       0xEA2D
#define B3950_N22       0xE91D
#define B3950_N21       0xE81D
#define B3950_N20       0xE715
#define B3950_N19       0xE5F5
#define B3950_N18       0xE4CD
#define B3950_N17       0xE38C
#define B3950_N16       0xE233
#define B3950_N15       0xE0C4
#define B3950_N14       0xDF41
#define B3950_N13       0xDDAA
#define B3950_N12       0xDC00
#define B3950_N11       0xDA47
#define B3950_N10       0xD87F
#define B3950_N9        0xD6A9
#define B3950_N8        0xD4C8
#define B3950_N7        0xD2DD
#define B3950_N6        0xD0EA
#define B3950_N5        0xCEEF
#define B3950_N4        0xCCBC
#define B3950_N3        0xCA8D
#define B3950_N2        0xC85F
#define B3950_N1        0xC630
#define B3950_0         0xC3FC
#define B3950_1         0xC1C2
#define B3950_2         0xBF7E
#define B3950_3         0xBD2E
#define B3950_4         0xBAD0
#define B3950_5         0xB863
#define B3950_6         0xB5E6
#define B3950_7         0xB357
#define B3950_8         0xB0B7
#define B3950_9         0xAE05
#define B3950_10        0xAB41
#define B3950_11        0xA865
#define B3950_12        0xA586
#define B3950_13        0xA2A5
#define B3950_14        0x9FC2
#define B3950_15        0x9CDC
#define B3950_16        0x99F4
#define B3950_17        0x970C
#define B3950_18        0x9422
#define B3950_19        0x9138
#define B3950_20        0x8E4F
#define B3950_21        0x8B68
#define B3950_22        0x8885
#define B3950_23        0x85A7
#define B3950_24        0x82CF
#define B3950_25        0x7FFF
#define B3950_26        0x7D39
#define B3950_27        0x7A7E
#define B3950_28        0x77D0
#define B3950_29        0x7531
#define B3950_30        0x72A3
#define B3950_31        0x7002
#define B3950_32        0x6D60
#define B3950_33        0x6ABC
#define B3950_34        0x6818
#define B3950_35        0x6574
#define B3950_36        0x62D2
#define B3950_37        0x6035
#define B3950_38        0x5D9F
#define B3950_39        0x5B14
#define B3950_40        0x5897
#define B3950_41        0x562B
#define B3950_42        0x53D5
#define B3950_43        0x5198
#define B3950_44        0x4F78
#define B3950_45        0x4D7A
#define B3950_46        0x4B6F
#define B3950_47        0x496D
#define B3950_48        0x4773
#define B3950_49        0x4582
#define B3950_50        0x4398
#define B3950_51        0x41B8
#define B3950_52        0x3FE2
#define B3950_53        0x3E15
#define B3950_54        0x3C53
#define B3950_55        0x3A9A
#define B3950_56        0x38ED
#define B3950_57        0x374B
#define B3950_58        0x35B5
#define B3950_59        0x342A
#define B3950_60        0x32AC
#define B3950_61        0x313D
#define B3950_62        0x2FD1
#define B3950_63        0x2E6A
#define B3950_64        0x2D08
#define B3950_65        0x2BAC
#define B3950_66        0x2A58
#define B3950_67        0x290D
#define B3950_68        0x27CB
#define B3950_69        0x2695
#define B3950_70        0x2569
#define B3950_71        0x2449
#define B3950_72        0x2336
#define B3950_73        0x2231
#define B3950_74        0x2139
#define B3950_75        0x2051
#define B3950_76        0x1F7B
#define B3950_77        0x1EAA
#define B3950_78        0x1DDD
#define B3950_79        0x1D14
#define B3950_80        0x1C4F
#define B3950_81        0x1B8D
#define B3950_82        0x1ACE
#define B3950_83        0x1A13
#define B3950_84        0x195B
#define B3950_85        0x18A5
#define B3950_86        0x17F2
#define B3950_87        0x1741
#define B3950_88        0x1693
#define B3950_89        0x15E7
#define B3950_90        0x153E
#define B3950_91        0x14C4
#define B3950_92        0x1444
#define B3950_93        0x13BF
#define B3950_94        0x1338
#define B3950_95        0x12AE
#define B3950_96        0x1224
#define B3950_97        0x119B
#define B3950_98        0x1114
#define B3950_99        0x1090
#define B3950_100       0x100F
#define B3950_101       0x0F93
#define B3950_102       0x0F1C
#define B3950_103       0x0EAC
#define B3950_104       0x0E42
#define B3950_105       0x0DDF
#define B3950_106       0x0D81
#define B3950_107       0x0D26
#define B3950_108       0x0CCD
#define B3950_109       0x0C76
#define B3950_110       0x0C21
#define B3950_111       0x0BCE
#define B3950_112       0x0B7D
#define B3950_113       0x0B2E
#define B3950_114       0x0AE2
#define B3950_115       0x0A98
#define B3950_116       0x0A4F
#define B3950_117       0x0A0A
#define B3950_118       0x09C6
#define B3950_119       0x0984
#define B3950_120       0x0945
/******************************************************************************/
#endif      //_NTC3950_H_
/******************************************************************************/
