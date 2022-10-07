EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Connector_Generic:Conn_01x04 J1
U 1 1 632DE4A5
P 9500 1550
F 0 "J1" H 9580 1542 50  0000 L CNN
F 1 "Comm_Connector" H 9580 1451 50  0000 L CNN
F 2 "Stepper_Motor_Controller_Footprint_Lib:1005-003" H 9500 1550 50  0001 C CNN
F 3 "~" H 9500 1550 50  0001 C CNN
	1    9500 1550
	1    0    0    -1  
$EndComp
$Comp
L power:+12V #PWR01
U 1 1 632E10F7
P 9200 1350
F 0 "#PWR01" H 9200 1200 50  0001 C CNN
F 1 "+12V" H 9215 1523 50  0000 C CNN
F 2 "" H 9200 1350 50  0001 C CNN
F 3 "" H 9200 1350 50  0001 C CNN
	1    9200 1350
	1    0    0    -1  
$EndComp
Wire Wire Line
	9200 1350 9200 1450
Wire Wire Line
	9200 1450 9300 1450
$Comp
L power:GND #PWR02
U 1 1 632E167F
P 9200 1850
F 0 "#PWR02" H 9200 1600 50  0001 C CNN
F 1 "GND" H 9205 1677 50  0000 C CNN
F 2 "" H 9200 1850 50  0001 C CNN
F 3 "" H 9200 1850 50  0001 C CNN
	1    9200 1850
	1    0    0    -1  
$EndComp
Wire Wire Line
	9200 1850 9200 1750
Wire Wire Line
	9200 1750 9300 1750
Text GLabel 9300 1550 0    50   Input ~ 0
SDA
Text GLabel 9300 1650 0    50   Input ~ 0
SCL
$Comp
L Connector_Generic:Conn_01x04 J2
U 1 1 632E3322
P 9500 2550
F 0 "J2" H 9580 2542 50  0000 L CNN
F 1 "Comm_Connector" H 9580 2451 50  0000 L CNN
F 2 "Stepper_Motor_Controller_Footprint_Lib:1005-003" H 9500 2550 50  0001 C CNN
F 3 "~" H 9500 2550 50  0001 C CNN
	1    9500 2550
	1    0    0    -1  
$EndComp
$Comp
L power:+12V #PWR03
U 1 1 632E3328
P 9200 2350
F 0 "#PWR03" H 9200 2200 50  0001 C CNN
F 1 "+12V" H 9215 2523 50  0000 C CNN
F 2 "" H 9200 2350 50  0001 C CNN
F 3 "" H 9200 2350 50  0001 C CNN
	1    9200 2350
	1    0    0    -1  
$EndComp
Wire Wire Line
	9200 2350 9200 2450
Wire Wire Line
	9200 2450 9300 2450
$Comp
L power:GND #PWR04
U 1 1 632E3330
P 9200 2850
F 0 "#PWR04" H 9200 2600 50  0001 C CNN
F 1 "GND" H 9205 2677 50  0000 C CNN
F 2 "" H 9200 2850 50  0001 C CNN
F 3 "" H 9200 2850 50  0001 C CNN
	1    9200 2850
	1    0    0    -1  
$EndComp
Wire Wire Line
	9200 2850 9200 2750
Wire Wire Line
	9200 2750 9300 2750
Text GLabel 9300 2550 0    50   Input ~ 0
SDA
Text GLabel 9300 2650 0    50   Input ~ 0
SCL
$Comp
L Connector_Generic:Conn_01x04 J3
U 1 1 632E4ADE
P 9500 3550
F 0 "J3" H 9580 3542 50  0000 L CNN
F 1 "Comm_Connector" H 9580 3451 50  0000 L CNN
F 2 "Stepper_Motor_Controller_Footprint_Lib:1005-003" H 9500 3550 50  0001 C CNN
F 3 "~" H 9500 3550 50  0001 C CNN
	1    9500 3550
	1    0    0    -1  
$EndComp
$Comp
L power:+12V #PWR05
U 1 1 632E4AE4
P 9200 3350
F 0 "#PWR05" H 9200 3200 50  0001 C CNN
F 1 "+12V" H 9215 3523 50  0000 C CNN
F 2 "" H 9200 3350 50  0001 C CNN
F 3 "" H 9200 3350 50  0001 C CNN
	1    9200 3350
	1    0    0    -1  
$EndComp
Wire Wire Line
	9200 3350 9200 3450
Wire Wire Line
	9200 3450 9300 3450
$Comp
L power:GND #PWR06
U 1 1 632E4AEC
P 9200 3850
F 0 "#PWR06" H 9200 3600 50  0001 C CNN
F 1 "GND" H 9205 3677 50  0000 C CNN
F 2 "" H 9200 3850 50  0001 C CNN
F 3 "" H 9200 3850 50  0001 C CNN
	1    9200 3850
	1    0    0    -1  
$EndComp
Wire Wire Line
	9200 3850 9200 3750
Wire Wire Line
	9200 3750 9300 3750
Text GLabel 9300 3550 0    50   Input ~ 0
SDA
Text GLabel 9300 3650 0    50   Input ~ 0
SCL
$Comp
L Connector_Generic:Conn_01x04 J4
U 1 1 632E538B
P 9500 4550
F 0 "J4" H 9580 4542 50  0000 L CNN
F 1 "Comm_Connector" H 9580 4451 50  0000 L CNN
F 2 "Stepper_Motor_Controller_Footprint_Lib:1005-003" H 9500 4550 50  0001 C CNN
F 3 "~" H 9500 4550 50  0001 C CNN
	1    9500 4550
	1    0    0    -1  
$EndComp
$Comp
L power:+12V #PWR07
U 1 1 632E5391
P 9200 4350
F 0 "#PWR07" H 9200 4200 50  0001 C CNN
F 1 "+12V" H 9215 4523 50  0000 C CNN
F 2 "" H 9200 4350 50  0001 C CNN
F 3 "" H 9200 4350 50  0001 C CNN
	1    9200 4350
	1    0    0    -1  
$EndComp
Wire Wire Line
	9200 4350 9200 4450
Wire Wire Line
	9200 4450 9300 4450
$Comp
L power:GND #PWR08
U 1 1 632E5399
P 9200 4850
F 0 "#PWR08" H 9200 4600 50  0001 C CNN
F 1 "GND" H 9205 4677 50  0000 C CNN
F 2 "" H 9200 4850 50  0001 C CNN
F 3 "" H 9200 4850 50  0001 C CNN
	1    9200 4850
	1    0    0    -1  
$EndComp
Wire Wire Line
	9200 4850 9200 4750
Wire Wire Line
	9200 4750 9300 4750
Text GLabel 9300 4550 0    50   Input ~ 0
SDA
Text GLabel 9300 4650 0    50   Input ~ 0
SCL
$Comp
L Device:C C1
U 1 1 632EA05E
P 8850 1600
F 0 "C1" H 8735 1646 50  0000 R CNN
F 1 ".1u" H 8735 1555 50  0000 R CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.18x1.45mm_HandSolder" H 8888 1450 50  0001 C CNN
F 3 "~" H 8850 1600 50  0001 C CNN
	1    8850 1600
	-1   0    0    -1  
$EndComp
Wire Wire Line
	8850 1450 9200 1450
Connection ~ 9200 1450
Wire Wire Line
	8850 1750 9200 1750
Connection ~ 9200 1750
$Comp
L Device:C C2
U 1 1 632EAB4A
P 8850 2600
F 0 "C2" H 8735 2646 50  0000 R CNN
F 1 ".1u" H 8735 2555 50  0000 R CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.18x1.45mm_HandSolder" H 8888 2450 50  0001 C CNN
F 3 "~" H 8850 2600 50  0001 C CNN
	1    8850 2600
	-1   0    0    -1  
$EndComp
Wire Wire Line
	8850 2450 9200 2450
Connection ~ 9200 2450
Wire Wire Line
	8850 2750 9200 2750
Connection ~ 9200 2750
$Comp
L Device:C C3
U 1 1 632EB4F3
P 8850 3600
F 0 "C3" H 8735 3646 50  0000 R CNN
F 1 ".1u" H 8735 3555 50  0000 R CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.18x1.45mm_HandSolder" H 8888 3450 50  0001 C CNN
F 3 "~" H 8850 3600 50  0001 C CNN
	1    8850 3600
	-1   0    0    -1  
$EndComp
Wire Wire Line
	8850 3450 9200 3450
Connection ~ 9200 3450
Wire Wire Line
	8850 3750 9200 3750
Connection ~ 9200 3750
$Comp
L Device:C C4
U 1 1 632EBF11
P 8850 4600
F 0 "C4" H 8735 4646 50  0000 R CNN
F 1 ".1u" H 8735 4555 50  0000 R CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.18x1.45mm_HandSolder" H 8888 4450 50  0001 C CNN
F 3 "~" H 8850 4600 50  0001 C CNN
	1    8850 4600
	-1   0    0    -1  
$EndComp
Wire Wire Line
	8850 4450 9200 4450
Connection ~ 9200 4450
Wire Wire Line
	8850 4750 9200 4750
Connection ~ 9200 4750
$Comp
L Mechanical:MountingHole_Pad H4
U 1 1 632F0BFC
P 6750 3550
F 0 "H4" V 6704 3700 50  0000 L CNN
F 1 "MountingHole_Pad" V 6795 3700 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_DIN965_Pad" H 6750 3550 50  0001 C CNN
F 3 "~" H 6750 3550 50  0001 C CNN
	1    6750 3550
	0    1    1    0   
$EndComp
$Comp
L Mechanical:MountingHole_Pad H3
U 1 1 632F0DFC
P 6750 3350
F 0 "H3" V 6704 3500 50  0000 L CNN
F 1 "MountingHole_Pad" V 6795 3500 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_DIN965_Pad" H 6750 3350 50  0001 C CNN
F 3 "~" H 6750 3350 50  0001 C CNN
	1    6750 3350
	0    1    1    0   
$EndComp
$Comp
L Mechanical:MountingHole_Pad H2
U 1 1 632F1B0B
P 6750 3150
F 0 "H2" V 6704 3300 50  0000 L CNN
F 1 "MountingHole_Pad" V 6795 3300 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_DIN965_Pad" H 6750 3150 50  0001 C CNN
F 3 "~" H 6750 3150 50  0001 C CNN
	1    6750 3150
	0    1    1    0   
$EndComp
$Comp
L Mechanical:MountingHole_Pad H1
U 1 1 632F1C9E
P 6750 2950
F 0 "H1" V 6704 3100 50  0000 L CNN
F 1 "MountingHole_Pad" V 6795 3100 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_DIN965_Pad" H 6750 2950 50  0001 C CNN
F 3 "~" H 6750 2950 50  0001 C CNN
	1    6750 2950
	0    1    1    0   
$EndComp
$Comp
L Mechanical:MountingHole_Pad H6
U 1 1 632F1F82
P 6750 4300
F 0 "H6" V 6704 4450 50  0000 L CNN
F 1 "MountingHole_Pad" V 6795 4450 50  0000 L CNN
F 2 "MountingHole:MountingHole_2.2mm_M2_DIN965_Pad" H 6750 4300 50  0001 C CNN
F 3 "~" H 6750 4300 50  0001 C CNN
	1    6750 4300
	0    1    1    0   
$EndComp
$Comp
L Mechanical:MountingHole_Pad H5
U 1 1 632F22B5
P 6750 4100
F 0 "H5" V 6704 4250 50  0000 L CNN
F 1 "MountingHole_Pad" V 6795 4250 50  0000 L CNN
F 2 "MountingHole:MountingHole_2.2mm_M2_DIN965_Pad" H 6750 4100 50  0001 C CNN
F 3 "~" H 6750 4100 50  0001 C CNN
	1    6750 4100
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 632F33EB
P 6550 3650
F 0 "#PWR?" H 6550 3400 50  0001 C CNN
F 1 "GND" H 6555 3477 50  0000 C CNN
F 2 "" H 6550 3650 50  0001 C CNN
F 3 "" H 6550 3650 50  0001 C CNN
	1    6550 3650
	1    0    0    -1  
$EndComp
Wire Wire Line
	6550 3650 6550 3550
Wire Wire Line
	6550 3550 6650 3550
Wire Wire Line
	6650 3350 6550 3350
Wire Wire Line
	6550 3350 6550 3550
Connection ~ 6550 3550
Wire Wire Line
	6650 3150 6550 3150
Wire Wire Line
	6550 3150 6550 3350
Connection ~ 6550 3350
Wire Wire Line
	6650 2950 6550 2950
Wire Wire Line
	6550 2950 6550 3150
Connection ~ 6550 3150
$Comp
L power:GND #PWR?
U 1 1 632F4F10
P 6550 4400
F 0 "#PWR?" H 6550 4150 50  0001 C CNN
F 1 "GND" H 6555 4227 50  0000 C CNN
F 2 "" H 6550 4400 50  0001 C CNN
F 3 "" H 6550 4400 50  0001 C CNN
	1    6550 4400
	1    0    0    -1  
$EndComp
Wire Wire Line
	6650 4100 6550 4100
Wire Wire Line
	6550 4100 6550 4300
Wire Wire Line
	6650 4300 6550 4300
Connection ~ 6550 4300
Wire Wire Line
	6550 4300 6550 4400
$EndSCHEMATC