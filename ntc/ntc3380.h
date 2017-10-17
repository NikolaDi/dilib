/******************************************************************************/
#ifndef     _NTC3380_H_
#define     _NTC3380_H_
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
 * #define  STAND_TEMPE     NTC_AD10(B3380_25)
 * if(g_Tempeture == STAND_TEMPE)
 * --------------------------------------
 */
/******************************************************************************/
#define NTC_AD8(x)          x/256
#define NTC_AD10(x)         x/64
#define NTC_AD12(x)         x/16
#define NTC_AD(x)           B3380_##x/64///////////////////////////////////////
/******************************************************************************/
//16bit
#define B3380_N50       0xF874  //329.2K
#define B3380_N49       0xF804  //310.7K
#define B3380_N48       0xF78F  //293.3K
#define B3380_N47       0xF715  //277.0K
#define B3380_N46       0xF690  //261.3K
#define B3380_N45       0xF60F  //247.5K
#define B3380_N44       0xF583  //234.1K
#define B3380_N43       0xF4F2  //221.6K
#define B3380_N42       0xF45A  //209.8K
#define B3380_N41       0xF3BC  //198.7K
#define B3380_N40       0xF319  //188.4K
#define B3380_N39       0xF268  //178.3K
#define B3380_N38       0xF1B1  //168.9K
#define B3380_N37       0xF0F3  //160.1K
#define B3380_N36       0xF02E  //151.8K
#define B3380_N35       0xEF60  //144.0K
#define B3380_N34       0xEE8A  //136.6K
#define B3380_N33       0xEDAD  //129.7K
#define B3380_N32       0xECC8  //123.2K
#define B3380_N31       0xEBDC  //117.1K
#define B3380_N30       0xEAE5  //111.3K
#define B3380_N29       0xE9E0  //105.7K
#define B3380_N28       0xE8D0  //100.4K
#define B3380_N27       0xE7BA  //95.47K
#define B3380_N26       0xE69A  //90.80K
#define B3380_N25       0xE571  //86.39K
#define B3380_N24       0xE43E  //82.22K
#define B3380_N23       0xE301  //78.29K
#define B3380_N22       0xE1BC  //74.58K
#define B3380_N21       0xE06C  //71.07K
#define B3380_N20       0xDF12  //67.74K
#define B3380_N19       0xDDA8  //64.54K
#define B3380_N18       0xDC35  //61.52K
#define B3380_N17       0xDAB6  //58.65K
#define B3380_N16       0xD92F  //55.95K
#define B3380_N15       0xD79D  //53.39K
#define B3380_N14       0xD600  //50.95K
#define B3380_N13       0xD45C  //48.66K
#define B3380_N12       0xD2AD  //46.48K
#define B3380_N11       0xD0FA  //44.44K
#define B3380_N10       0xCF31  //42.45K
#define B3380_N9        0xCD5E  //40.56K
#define B3380_N8        0xCB7F  //38.76K
#define B3380_N7        0xC997  //37.05K
#define B3380_N6        0xC7A6  //35.43K
#define B3380_N5        0xC5AC  //33.89K
#define B3380_N4        0xC3AA  //32.43K
#define B3380_N3        0xC19F  //31.04K
#define B3380_N2        0xBF8D  //29.72K
#define B3380_N1        0xBD74  //28.47K
#define B3380_0         0xBB55  //27.28K
#define B3380_1         0xB925  //26.13K
#define B3380_2         0xB6EB  //25.03K
#define B3380_3         0xB4AF  //23.99K
#define B3380_4         0xB267  //22.99K
#define B3380_5         0xB020  //22.05K
#define B3380_6         0xADD1  //21.15K
#define B3380_7         0xAB83  //20.30K
#define B3380_8         0xA929  //19.48K
#define B3380_9         0xA6CD  //18.70K
#define B3380_10        0xA471  //17.96K
#define B3380_11        0xA205  //17.24K
#define B3380_12        0x9F94  //16.55K
#define B3380_13        0x9D29  //15.90K
#define B3380_14        0x9ABC  //15.28K
#define B3380_15        0x9846  //14.68K
#define B3380_16        0x95DD  //14.12K
#define B3380_17        0x9363  //13.57K
#define B3380_18        0x90FC  //13.06K
#define B3380_19        0x8E86  //12.56K
#define B3380_20        0x8C1C  //12.09K
#define B3380_21        0x89A5  //11.63K
#define B3380_22        0x873F  //11.20K
#define B3380_23        0x84CE  //10.78K
#define B3380_24        0x8263  //10.38K
#define B3380_25        0x8000  //10.00K
#define B3380_26        0x7D9A  //9.632K
#define B3380_27        0x7B3A  //9.281K
#define B3380_28        0x78DD  //8.944K
#define B3380_29        0x7687  //8.622K
#define B3380_30        0x7435  //8.313K
#define B3380_31        0x71E5  //8.015K
#define B3380_32        0x6F92  //7.725K
#define B3380_33        0x6D56  //7.455K
#define B3380_34        0x6B18  //7.192K
#define B3380_35        0x68E3  //6.941K
#define B3380_36        0x66B3  //6.699K
#define B3380_37        0x648C  //6.468K
#define B3380_38        0x626C  //6.246K
#define B3380_39        0x6054  //6.033K
#define B3380_40        0x5E46  //5.829K
#define B3380_41        0x5C36  //5.630K
#define B3380_42        0x5A30  //5.439K
#define B3380_43        0x5832  //5.256K
#define B3380_44        0x563D  //5.080K
#define B3380_45        0x5454  //4.912K
#define B3380_46        0x526E  //4.749K
#define B3380_47        0x5096  //4.594K
#define B3380_48        0x4EC4  //4.444K
#define B3380_49        0x4CFB  //4.300K
#define B3380_50        0x4B39  //4.161K
#define B3380_51        0x497B  //4.026K
#define B3380_52        0x47CA  //3.897K
#define B3380_53        0x461E  //3.772K
#define B3380_54        0x447B  //3.652K
#define B3380_55        0x42E4  //3.537K
#define B3380_56        0x4153  //3.426K
#define B3380_57        0x3FCB  //3.319K
#define B3380_58        0x3E4C  //3.216K
#define B3380_59        0x3CD2  //3.116K
#define B3380_60        0x3B65  //3.021K
#define B3380_61        0x39FB  //2.928K
#define B3380_62        0x3898  //2.838K
#define B3380_63        0x373F  //2.752K
#define B3380_64        0x35EF  //2.669K
#define B3380_65        0x34A6  //2.589K
#define B3380_66        0x3365  //2.512K
#define B3380_67        0x322A  //2.437K
#define B3380_68        0x30F7  //2.365K
#define B3380_69        0x2FCD  //2.296K
#define B3380_70        0x2EA9  //2.229K
#define B3380_71        0x2D87  //2.163K
#define B3380_72        0x2C72  //2.101K
#define B3380_73        0x2B60  //2.040K
#define B3380_74        0x2A54  //1.981K
#define B3380_75        0x294F  //1.924K
#define B3380_76        0x2855  //1.870K
#define B3380_77        0x275D  //1.817K
#define B3380_78        0x266D  //1.766K
#define B3380_79        0x257F  //1.716K
#define B3380_80        0x249E  //1.669K
#define B3380_81        0x23BA  //1.622K
#define B3380_82        0x22DF  //1.577K
#define B3380_83        0x220C  //1.534K
#define B3380_84        0x213D  //1.492K
#define B3380_85        0x2070  //1.451K
#define B3380_86        0x1FAD  //1.412K
#define B3380_87        0x1EED  //1.374K
#define B3380_88        0x1E31  //1.337K
#define B3380_89        0x1D79  //1.301K
#define B3380_90        0x1CC5  //1.266K
#define B3380_91        0x1C1A  //1.233K
#define B3380_92        0x1B6E  //1.200K
#define B3380_93        0x1ACB  //1.169K
#define B3380_94        0x1A28  //1.138K
#define B3380_95        0x1989  //1.108K
#define B3380_96        0x18F4  //1.080K
#define B3380_97        0x185E  //1.052K
#define B3380_98        0x17CD  //1.025K
#define B3380_99        0x173F  //0.9988K
#define B3380_100       0x16B6  //0.9735K
#define B3380_101       0x162F  //0.9488K
#define B3380_102       0x15AD  //0.9250K
#define B3380_103       0x152D  //0.9018K
#define B3380_104       0x14B1  //0.8793K
#define B3380_105       0x1438  //0.8575K
#define B3380_106       0x13C2  //0.8364K
#define B3380_107       0x134F  //0.8158K
#define B3380_108       0x12E0  //0.7960K
#define B3380_109       0x1273  //0.7766K
#define B3380_110       0x1209  //0.7579K
#define B3380_111       0x11A1  //0.7396K
#define B3380_112       0x113D  //0.7219K
#define B3380_113       0x10DA  //0.7047K
#define B3380_114       0x107B  //0.6880K
#define B3380_115       0x101E  //0.6718K
#define B3380_116       0x0FC2  //0.6560K
#define B3380_117       0x0F6A  //0.6407K
#define B3380_118       0x0F14  //0.6258K
#define B3380_119       0x0EBF  //0.6113K
#define B3380_120       0x0E6D  //0.5972K
/******************************************************************************/
#endif      //_NTC3380_H_
/******************************************************************************/
