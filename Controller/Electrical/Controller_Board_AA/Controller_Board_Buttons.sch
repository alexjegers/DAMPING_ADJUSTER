EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 2
Title "Damping Adjuster Controller Board"
Date "9-2-2022"
Rev "AA"
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Switch:SW_Push SW1
U 1 1 63A83E96
P 3850 2600
F 0 "SW1" V 3900 3000 50  0000 R CNN
F 1 "SW_Push" V 3800 3050 50  0000 R CNN
F 2 "Button_Switch_THT:SW_PUSH_6mm_H5mm" H 3850 2800 50  0001 C CNN
F 3 "~" H 3850 2800 50  0001 C CNN
	1    3850 2600
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R21
U 1 1 63A864CE
P 4100 2200
F 0 "R21" H 3850 2250 50  0000 L CNN
F 1 "330" H 3850 2150 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 4030 2200 50  0001 C CNN
F 3 "~" H 4100 2200 50  0001 C CNN
F 4 "JLC" H 4100 2200 50  0001 C CNN "Mfg"
	1    4100 2200
	1    0    0    -1  
$EndComp
Text HLabel 5900 1700 2    50   Input ~ 0
+3.3V
$Comp
L Device:R R22
U 1 1 63AB6CD3
P 4400 2400
F 0 "R22" V 4200 2400 50  0000 C CNN
F 1 "10k" V 4300 2400 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 4330 2400 50  0001 C CNN
F 3 "~" H 4400 2400 50  0001 C CNN
F 4 "JLC" H 4400 2400 50  0001 C CNN "Mfg"
	1    4400 2400
	0    1    1    0   
$EndComp
$Comp
L Device:C C16
U 1 1 63AB726F
P 4700 2650
F 0 "C16" H 4815 2696 50  0000 L CNN
F 1 "1u" H 4815 2605 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 4738 2500 50  0001 C CNN
F 3 "~" H 4700 2650 50  0001 C CNN
F 4 "JLC" H 4700 2650 50  0001 C CNN "Mfg"
	1    4700 2650
	1    0    0    -1  
$EndComp
Wire Wire Line
	4550 2400 4700 2400
Wire Wire Line
	4700 2400 4700 2500
Wire Wire Line
	3850 2800 3850 2950
Wire Wire Line
	3850 2950 4300 2950
Wire Wire Line
	4700 2950 4700 2800
$Comp
L power:+3.3V #PWR045
U 1 1 63AB808E
P 5800 1650
F 0 "#PWR045" H 5800 1500 50  0001 C CNN
F 1 "+3.3V" H 5815 1823 50  0000 C CNN
F 2 "" H 5800 1650 50  0001 C CNN
F 3 "" H 5800 1650 50  0001 C CNN
	1    5800 1650
	1    0    0    -1  
$EndComp
Wire Wire Line
	5800 1650 5800 1700
Wire Wire Line
	5800 1700 5900 1700
$Comp
L power:+3.3V #PWR047
U 1 1 63AB84EA
P 4100 2000
F 0 "#PWR047" H 4100 1850 50  0001 C CNN
F 1 "+3.3V" H 4115 2173 50  0000 C CNN
F 2 "" H 4100 2000 50  0001 C CNN
F 3 "" H 4100 2000 50  0001 C CNN
	1    4100 2000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR046
U 1 1 63AB8A88
P 5800 1900
F 0 "#PWR046" H 5800 1650 50  0001 C CNN
F 1 "GND" H 5805 1727 50  0000 C CNN
F 2 "" H 5800 1900 50  0001 C CNN
F 3 "" H 5800 1900 50  0001 C CNN
	1    5800 1900
	1    0    0    -1  
$EndComp
Text HLabel 5900 1800 2    50   Input ~ 0
GND
Wire Wire Line
	5900 1800 5800 1800
Wire Wire Line
	5800 1800 5800 1900
$Comp
L power:GND #PWR048
U 1 1 63ABA239
P 4300 2950
F 0 "#PWR048" H 4300 2700 50  0001 C CNN
F 1 "GND" H 4305 2777 50  0000 C CNN
F 2 "" H 4300 2950 50  0001 C CNN
F 3 "" H 4300 2950 50  0001 C CNN
	1    4300 2950
	1    0    0    -1  
$EndComp
Connection ~ 4300 2950
Wire Wire Line
	4300 2950 4700 2950
Wire Wire Line
	4700 2400 4900 2400
Connection ~ 4700 2400
Text HLabel 4900 2400 2    50   Input ~ 0
BT_1
Wire Wire Line
	3850 2400 4100 2400
Wire Wire Line
	4100 2000 4100 2050
Wire Wire Line
	4100 2350 4100 2400
Connection ~ 4100 2400
Wire Wire Line
	4100 2400 4250 2400
$Comp
L Switch:SW_Push SW2
U 1 1 63ABE85E
P 3850 4050
F 0 "SW2" V 3900 4450 50  0000 R CNN
F 1 "SW_Push" V 3800 4500 50  0000 R CNN
F 2 "Button_Switch_THT:SW_PUSH_6mm_H5mm" H 3850 4250 50  0001 C CNN
F 3 "~" H 3850 4250 50  0001 C CNN
	1    3850 4050
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R25
U 1 1 63ABE864
P 4100 3650
F 0 "R25" H 3850 3700 50  0000 L CNN
F 1 "330" H 3850 3600 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 4030 3650 50  0001 C CNN
F 3 "~" H 4100 3650 50  0001 C CNN
F 4 "JLC" H 4100 3650 50  0001 C CNN "Mfg"
	1    4100 3650
	1    0    0    -1  
$EndComp
$Comp
L Device:R R26
U 1 1 63ABE86A
P 4400 3850
F 0 "R26" V 4200 3850 50  0000 C CNN
F 1 "10k" V 4300 3850 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 4330 3850 50  0001 C CNN
F 3 "~" H 4400 3850 50  0001 C CNN
F 4 "JLC" H 4400 3850 50  0001 C CNN "Mfg"
	1    4400 3850
	0    1    1    0   
$EndComp
$Comp
L Device:C C18
U 1 1 63ABE870
P 4700 4100
F 0 "C18" H 4815 4146 50  0000 L CNN
F 1 "1u" H 4815 4055 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 4738 3950 50  0001 C CNN
F 3 "~" H 4700 4100 50  0001 C CNN
F 4 "JLC" H 4700 4100 50  0001 C CNN "Mfg"
	1    4700 4100
	1    0    0    -1  
$EndComp
Wire Wire Line
	4550 3850 4700 3850
Wire Wire Line
	4700 3850 4700 3950
Wire Wire Line
	3850 4250 3850 4400
Wire Wire Line
	3850 4400 4300 4400
Wire Wire Line
	4700 4400 4700 4250
$Comp
L power:+3.3V #PWR050
U 1 1 63ABE87B
P 4100 3450
F 0 "#PWR050" H 4100 3300 50  0001 C CNN
F 1 "+3.3V" H 4115 3623 50  0000 C CNN
F 2 "" H 4100 3450 50  0001 C CNN
F 3 "" H 4100 3450 50  0001 C CNN
	1    4100 3450
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR053
U 1 1 63ABE881
P 4300 4400
F 0 "#PWR053" H 4300 4150 50  0001 C CNN
F 1 "GND" H 4305 4227 50  0000 C CNN
F 2 "" H 4300 4400 50  0001 C CNN
F 3 "" H 4300 4400 50  0001 C CNN
	1    4300 4400
	1    0    0    -1  
$EndComp
Connection ~ 4300 4400
Wire Wire Line
	4300 4400 4700 4400
Wire Wire Line
	4700 3850 4900 3850
Connection ~ 4700 3850
Text HLabel 4900 3850 2    50   Input ~ 0
BT_2
Wire Wire Line
	3850 3850 4100 3850
Wire Wire Line
	4100 3450 4100 3500
Wire Wire Line
	4100 3800 4100 3850
Connection ~ 4100 3850
Wire Wire Line
	4100 3850 4250 3850
$Comp
L Switch:SW_Push SW3
U 1 1 63ABF9F4
P 3850 5500
F 0 "SW3" V 3900 5900 50  0000 R CNN
F 1 "SW_Push" V 3800 5950 50  0000 R CNN
F 2 "Button_Switch_THT:SW_PUSH_6mm_H5mm" H 3850 5700 50  0001 C CNN
F 3 "~" H 3850 5700 50  0001 C CNN
	1    3850 5500
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R29
U 1 1 63ABF9FA
P 4100 5100
F 0 "R29" H 3850 5150 50  0000 L CNN
F 1 "330" H 3850 5050 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 4030 5100 50  0001 C CNN
F 3 "~" H 4100 5100 50  0001 C CNN
F 4 "JLC" H 4100 5100 50  0001 C CNN "Mfg"
	1    4100 5100
	1    0    0    -1  
$EndComp
$Comp
L Device:R R30
U 1 1 63ABFA00
P 4400 5300
F 0 "R30" V 4200 5300 50  0000 C CNN
F 1 "10k" V 4300 5300 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 4330 5300 50  0001 C CNN
F 3 "~" H 4400 5300 50  0001 C CNN
F 4 "JLC" H 4400 5300 50  0001 C CNN "Mfg"
	1    4400 5300
	0    1    1    0   
$EndComp
$Comp
L Device:C C20
U 1 1 63ABFA06
P 4700 5550
F 0 "C20" H 4815 5596 50  0000 L CNN
F 1 "1u" H 4815 5505 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 4738 5400 50  0001 C CNN
F 3 "~" H 4700 5550 50  0001 C CNN
F 4 "JLC" H 4700 5550 50  0001 C CNN "Mfg"
	1    4700 5550
	1    0    0    -1  
$EndComp
Wire Wire Line
	4550 5300 4700 5300
Wire Wire Line
	4700 5300 4700 5400
Wire Wire Line
	3850 5700 3850 5850
Wire Wire Line
	3850 5850 4300 5850
Wire Wire Line
	4700 5850 4700 5700
$Comp
L power:+3.3V #PWR054
U 1 1 63ABFA11
P 4100 4900
F 0 "#PWR054" H 4100 4750 50  0001 C CNN
F 1 "+3.3V" H 4115 5073 50  0000 C CNN
F 2 "" H 4100 4900 50  0001 C CNN
F 3 "" H 4100 4900 50  0001 C CNN
	1    4100 4900
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR056
U 1 1 63ABFA17
P 4300 5850
F 0 "#PWR056" H 4300 5600 50  0001 C CNN
F 1 "GND" H 4305 5677 50  0000 C CNN
F 2 "" H 4300 5850 50  0001 C CNN
F 3 "" H 4300 5850 50  0001 C CNN
	1    4300 5850
	1    0    0    -1  
$EndComp
Connection ~ 4300 5850
Wire Wire Line
	4300 5850 4700 5850
Wire Wire Line
	4700 5300 4900 5300
Connection ~ 4700 5300
Text HLabel 4900 5300 2    50   Input ~ 0
BT_3
Wire Wire Line
	3850 5300 4100 5300
Wire Wire Line
	4100 4900 4100 4950
Wire Wire Line
	4100 5250 4100 5300
Connection ~ 4100 5300
Wire Wire Line
	4100 5300 4250 5300
$Comp
L Switch:SW_Push SW4
U 1 1 63AC2D95
P 5800 3300
F 0 "SW4" V 5850 3700 50  0000 R CNN
F 1 "SW_Push" V 5750 3750 50  0000 R CNN
F 2 "Button_Switch_THT:SW_PUSH_6mm_H5mm" H 5800 3500 50  0001 C CNN
F 3 "~" H 5800 3500 50  0001 C CNN
	1    5800 3300
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R23
U 1 1 63AC2D9B
P 6050 2900
F 0 "R23" H 5800 2950 50  0000 L CNN
F 1 "330" H 5800 2850 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 5980 2900 50  0001 C CNN
F 3 "~" H 6050 2900 50  0001 C CNN
F 4 "JLC" H 6050 2900 50  0001 C CNN "Mfg"
	1    6050 2900
	1    0    0    -1  
$EndComp
$Comp
L Device:R R24
U 1 1 63AC2DA1
P 6350 3100
F 0 "R24" V 6150 3100 50  0000 C CNN
F 1 "10k" V 6250 3100 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 6280 3100 50  0001 C CNN
F 3 "~" H 6350 3100 50  0001 C CNN
F 4 "JLC" H 6350 3100 50  0001 C CNN "Mfg"
	1    6350 3100
	0    1    1    0   
$EndComp
$Comp
L Device:C C17
U 1 1 63AC2DA7
P 6650 3350
F 0 "C17" H 6765 3396 50  0000 L CNN
F 1 "1u" H 6765 3305 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 6688 3200 50  0001 C CNN
F 3 "~" H 6650 3350 50  0001 C CNN
F 4 "JLC" H 6650 3350 50  0001 C CNN "Mfg"
	1    6650 3350
	1    0    0    -1  
$EndComp
Wire Wire Line
	6500 3100 6650 3100
Wire Wire Line
	6650 3100 6650 3200
Wire Wire Line
	5800 3500 5800 3650
Wire Wire Line
	5800 3650 6250 3650
Wire Wire Line
	6650 3650 6650 3500
$Comp
L power:+3.3V #PWR049
U 1 1 63AC2DB2
P 6050 2700
F 0 "#PWR049" H 6050 2550 50  0001 C CNN
F 1 "+3.3V" H 6065 2873 50  0000 C CNN
F 2 "" H 6050 2700 50  0001 C CNN
F 3 "" H 6050 2700 50  0001 C CNN
	1    6050 2700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR051
U 1 1 63AC2DB8
P 6250 3650
F 0 "#PWR051" H 6250 3400 50  0001 C CNN
F 1 "GND" H 6255 3477 50  0000 C CNN
F 2 "" H 6250 3650 50  0001 C CNN
F 3 "" H 6250 3650 50  0001 C CNN
	1    6250 3650
	1    0    0    -1  
$EndComp
Connection ~ 6250 3650
Wire Wire Line
	6250 3650 6650 3650
Wire Wire Line
	6650 3100 6850 3100
Connection ~ 6650 3100
Text HLabel 6850 3100 2    50   Input ~ 0
BT_4
Wire Wire Line
	5800 3100 6050 3100
Wire Wire Line
	6050 2700 6050 2750
Wire Wire Line
	6050 3050 6050 3100
Connection ~ 6050 3100
Wire Wire Line
	6050 3100 6200 3100
$Comp
L Switch:SW_Push SW5
U 1 1 63AC5B96
P 5800 4850
F 0 "SW5" V 5850 5250 50  0000 R CNN
F 1 "SW_Push" V 5750 5300 50  0000 R CNN
F 2 "Button_Switch_THT:SW_PUSH_6mm_H5mm" H 5800 5050 50  0001 C CNN
F 3 "~" H 5800 5050 50  0001 C CNN
	1    5800 4850
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R27
U 1 1 63AC5B9C
P 6050 4450
F 0 "R27" H 5800 4500 50  0000 L CNN
F 1 "330" H 5800 4400 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 5980 4450 50  0001 C CNN
F 3 "~" H 6050 4450 50  0001 C CNN
F 4 "JLC" H 6050 4450 50  0001 C CNN "Mfg"
	1    6050 4450
	1    0    0    -1  
$EndComp
$Comp
L Device:R R28
U 1 1 63AC5BA2
P 6350 4650
F 0 "R28" V 6150 4650 50  0000 C CNN
F 1 "10k" V 6250 4650 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 6280 4650 50  0001 C CNN
F 3 "~" H 6350 4650 50  0001 C CNN
F 4 "JLC" H 6350 4650 50  0001 C CNN "Mfg"
	1    6350 4650
	0    1    1    0   
$EndComp
$Comp
L Device:C C19
U 1 1 63AC5BA8
P 6650 4900
F 0 "C19" H 6765 4946 50  0000 L CNN
F 1 "1u" H 6765 4855 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 6688 4750 50  0001 C CNN
F 3 "~" H 6650 4900 50  0001 C CNN
F 4 "JLC" H 6650 4900 50  0001 C CNN "Mfg"
	1    6650 4900
	1    0    0    -1  
$EndComp
Wire Wire Line
	6500 4650 6650 4650
Wire Wire Line
	6650 4650 6650 4750
Wire Wire Line
	5800 5050 5800 5200
Wire Wire Line
	5800 5200 6250 5200
Wire Wire Line
	6650 5200 6650 5050
$Comp
L power:+3.3V #PWR052
U 1 1 63AC5BB3
P 6050 4250
F 0 "#PWR052" H 6050 4100 50  0001 C CNN
F 1 "+3.3V" H 6065 4423 50  0000 C CNN
F 2 "" H 6050 4250 50  0001 C CNN
F 3 "" H 6050 4250 50  0001 C CNN
	1    6050 4250
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR055
U 1 1 63AC5BB9
P 6250 5200
F 0 "#PWR055" H 6250 4950 50  0001 C CNN
F 1 "GND" H 6255 5027 50  0000 C CNN
F 2 "" H 6250 5200 50  0001 C CNN
F 3 "" H 6250 5200 50  0001 C CNN
	1    6250 5200
	1    0    0    -1  
$EndComp
Connection ~ 6250 5200
Wire Wire Line
	6250 5200 6650 5200
Wire Wire Line
	6650 4650 6850 4650
Connection ~ 6650 4650
Text HLabel 6850 4650 2    50   Input ~ 0
BT_5
Wire Wire Line
	5800 4650 6050 4650
Wire Wire Line
	6050 4250 6050 4300
Wire Wire Line
	6050 4600 6050 4650
Connection ~ 6050 4650
Wire Wire Line
	6050 4650 6200 4650
Text Label 4050 2400 2    50   ~ 0
SW1+
Text Label 4050 3850 2    50   ~ 0
SW2+
Text Label 4050 5300 2    50   ~ 0
SW3+
Text Label 6000 3100 2    50   ~ 0
SW4+
Text Label 6000 4650 2    50   ~ 0
SW5+
$Comp
L Connector:TestPoint_Alt TP32
U 1 1 63B16D4E
P 4700 2300
F 0 "TP32" H 4758 2418 50  0000 L CNN
F 1 "BT_1" H 4758 2327 50  0000 L CNN
F 2 "TestPoint:TestPoint_Pad_1.5x1.5mm" H 4900 2300 50  0001 C CNN
F 3 "~" H 4900 2300 50  0001 C CNN
	1    4700 2300
	1    0    0    -1  
$EndComp
Wire Wire Line
	4700 2300 4700 2400
$Comp
L Connector:TestPoint_Alt TP34
U 1 1 63B18324
P 4700 3750
F 0 "TP34" H 4758 3868 50  0000 L CNN
F 1 "BT_2" H 4758 3777 50  0000 L CNN
F 2 "TestPoint:TestPoint_Pad_1.5x1.5mm" H 4900 3750 50  0001 C CNN
F 3 "~" H 4900 3750 50  0001 C CNN
	1    4700 3750
	1    0    0    -1  
$EndComp
Wire Wire Line
	4700 3750 4700 3850
$Comp
L Connector:TestPoint_Alt TP36
U 1 1 63B198B7
P 4700 5200
F 0 "TP36" H 4758 5318 50  0000 L CNN
F 1 "BT_3" H 4758 5227 50  0000 L CNN
F 2 "TestPoint:TestPoint_Pad_1.5x1.5mm" H 4900 5200 50  0001 C CNN
F 3 "~" H 4900 5200 50  0001 C CNN
	1    4700 5200
	1    0    0    -1  
$EndComp
Wire Wire Line
	4700 5200 4700 5300
$Comp
L Connector:TestPoint_Alt TP35
U 1 1 63B1AF23
P 6650 4550
F 0 "TP35" H 6708 4668 50  0000 L CNN
F 1 "BT_5" H 6708 4577 50  0000 L CNN
F 2 "TestPoint:TestPoint_Pad_1.5x1.5mm" H 6850 4550 50  0001 C CNN
F 3 "~" H 6850 4550 50  0001 C CNN
	1    6650 4550
	1    0    0    -1  
$EndComp
Wire Wire Line
	6650 4550 6650 4650
$Comp
L Connector:TestPoint_Alt TP33
U 1 1 63B1C873
P 6650 3000
F 0 "TP33" H 6708 3118 50  0000 L CNN
F 1 "BT_4" H 6708 3027 50  0000 L CNN
F 2 "TestPoint:TestPoint_Pad_1.5x1.5mm" H 6850 3000 50  0001 C CNN
F 3 "~" H 6850 3000 50  0001 C CNN
	1    6650 3000
	1    0    0    -1  
$EndComp
Wire Wire Line
	6650 3000 6650 3100
$EndSCHEMATC
