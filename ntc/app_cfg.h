#ifndef _APP_CFG_H_
#define _APP_CFG_H_

#include "ntc3380.h"
//#include "ntc3950.h"

#define NTC(TEMP) NTC_AD(TEMP)

//-50C - +100C  10K
const uint16_t gc_hwTempDefArray[151]={
                                    NTC(N50),NTC(N49),NTC(N48),NTC(N47),NTC(N46),NTC(N45),NTC(N44),NTC(N43),NTC(N42),NTC(N41),
                                    NTC(N40),NTC(N39),NTC(N38),NTC(N37),NTC(N36),NTC(N35),NTC(N34),NTC(N33),NTC(N32),NTC(N31),
                                    NTC(N30),NTC(N29),NTC(N28),NTC(N27),NTC(N26),NTC(N25),NTC(N24),NTC(N23),NTC(N22),NTC(N21),
                                    NTC(N20),NTC(N19),NTC(N18),NTC(N17),NTC(N16),NTC(N15),NTC(N14),NTC(N13),NTC(N12),NTC(N11),
                                    NTC(N10),NTC(N9),NTC(N8),NTC(N7),NTC(N6),NTC(N5),NTC(N4),NTC(N3),NTC(N2),NTC(N1),
                                    NTC(0),
                                    NTC(1),NTC(2),NTC(3),NTC(4),NTC(5),NTC(6),NTC(7),NTC(8),NTC(9),NTC(10),
                                    NTC(11),NTC(12),NTC(13),NTC(14),NTC(15),NTC(16),NTC(17),NTC(18),NTC(19),NTC(20),
                                    NTC(21),NTC(22),NTC(23),NTC(24),NTC(25),NTC(26),NTC(27),NTC(28),NTC(29),NTC(30),
                                    NTC(31),NTC(32),NTC(33),NTC(34),NTC(35),NTC(36),NTC(37),NTC(38),NTC(39),NTC(40),
                                    NTC(41),NTC(42),NTC(43),NTC(44),NTC(45),NTC(46),NTC(47),NTC(48),NTC(49),NTC(50),
                                    NTC(51),NTC(52),NTC(53),NTC(54),NTC(55),NTC(56),NTC(57),NTC(58),NTC(59),NTC(60),
                                    NTC(61),NTC(62),NTC(63),NTC(64),NTC(65),NTC(66),NTC(67),NTC(68),NTC(69),NTC(70),
                                    NTC(71),NTC(72),NTC(73),NTC(74),NTC(75),NTC(76),NTC(77),NTC(78),NTC(79),NTC(80),
                                    NTC(81),NTC(82),NTC(83),NTC(84),NTC(85),NTC(86),NTC(87),NTC(88),NTC(89),NTC(90),
                                    NTC(91),NTC(92),NTC(93),NTC(94),NTC(95),NTC(96),NTC(97),NTC(98),NTC(99),NTC(100),
                                };
                                
#define TEMP_ADMAX  NTC(N50)
#define TEMP_ADMIN  NTC(100)

#endif
