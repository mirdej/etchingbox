EESchema Schematic File Version 4
LIBS:Etchingbox_Controller-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "Etching Box Controller"
Date "2018-11-13"
Rev "v2.0"
Comp "[ a n y m a ]  / SGMK"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Connector:Conn_01x04_Male Display1
U 1 1 5BE61456
P 1300 2250
F 0 "Display1" H 1406 2528 50  0000 C CNN
F 1 "Conn_01x04_Male" H 1406 2437 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 1300 2250 50  0001 C CNN
F 3 "~" H 1300 2250 50  0001 C CNN
	1    1300 2250
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR013
U 1 1 5BE6186A
P 1550 2250
F 0 "#PWR013" H 1550 2000 50  0001 C CNN
F 1 "GND" H 1550 2100 50  0000 C CNN
F 2 "" H 1550 2250 50  0000 C CNN
F 3 "" H 1550 2250 50  0000 C CNN
	1    1550 2250
	0    -1   -1   0   
$EndComp
$Comp
L power:+3V3 #PWR012
U 1 1 5BE619D5
P 1550 2150
F 0 "#PWR012" H 1550 2000 50  0001 C CNN
F 1 "+3.3V" H 1550 2290 28  0000 C CNN
F 2 "" H 1550 2150 50  0000 C CNN
F 3 "" H 1550 2150 50  0000 C CNN
	1    1550 2150
	0    1    1    0   
$EndComp
$Comp
L Connector:Conn_01x02_Male Switch1
U 1 1 5BE68532
P 1300 2850
F 0 "Switch1" H 1406 3028 50  0000 C CNN
F 1 "Conn_01x02_Male" H 1406 2937 50  0000 C CNN
F 2 "Connector_Molex:Molex_Mini-Fit_Jr_5566-02A_2x01_P4.20mm_Vertical" H 1300 2850 50  0001 C CNN
F 3 "~" H 1300 2850 50  0001 C CNN
	1    1300 2850
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR03
U 1 1 5BE6893D
P 1500 2850
F 0 "#PWR03" H 1500 2600 50  0001 C CNN
F 1 "GND" H 1500 2700 50  0000 C CNN
F 2 "" H 1500 2850 50  0000 C CNN
F 3 "" H 1500 2850 50  0000 C CNN
	1    1500 2850
	0    -1   -1   0   
$EndComp
Wire Wire Line
	1500 2950 1750 2950
$Comp
L Device:R R4
U 1 1 5BE80D30
P 4450 3350
F 0 "R4" H 4520 3396 50  0000 L CNN
F 1 "10k" H 4520 3305 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 4380 3350 50  0001 C CNN
F 3 "~" H 4450 3350 50  0001 C CNN
	1    4450 3350
	1    0    0    -1  
$EndComp
$Comp
L Device:R R7
U 1 1 5BE80D37
P 4800 3200
F 0 "R7" V 4593 3200 50  0000 C CNN
F 1 "100" V 4684 3200 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 4730 3200 50  0001 C CNN
F 3 "~" H 4800 3200 50  0001 C CNN
	1    4800 3200
	0    1    1    0   
$EndComp
Wire Wire Line
	4650 3200 4450 3200
Wire Wire Line
	4450 3200 4000 3200
$Comp
L Transistor_FET:QM6015D Q2
U 1 1 5BE80D40
P 5150 3200
F 0 "Q2" H 5356 3246 50  0000 L CNN
F 1 "IRFR120NTRPBF" H 5356 3155 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:TO-252-2" H 5350 3125 50  0001 L CIN
F 3 "http://www.jaolen.com/images/pdf/QM6015D.pdf" V 5150 3200 50  0001 L CNN
	1    5150 3200
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR017
U 1 1 5BE80D47
P 4450 3500
F 0 "#PWR017" H 4450 3250 50  0001 C CNN
F 1 "GND" H 4450 3350 50  0000 C CNN
F 2 "" H 4450 3500 50  0000 C CNN
F 3 "" H 4450 3500 50  0000 C CNN
	1    4450 3500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR024
U 1 1 5BE80D4D
P 5250 3500
F 0 "#PWR024" H 5250 3250 50  0001 C CNN
F 1 "GND" H 5250 3350 50  0000 C CNN
F 2 "" H 5250 3500 50  0000 C CNN
F 3 "" H 5250 3500 50  0000 C CNN
	1    5250 3500
	1    0    0    -1  
$EndComp
Wire Wire Line
	5250 3400 5250 3500
$Comp
L Connector:Conn_01x02_Male Lightpad1
U 1 1 5BE80D54
P 5050 2900
F 0 "Lightpad1" H 4600 3050 50  0000 C CNN
F 1 "Conn_01x02_Male" H 4800 2950 50  0000 C CNN
F 2 "Connector_Molex:Molex_Mini-Fit_Jr_5566-02A_2x01_P4.20mm_Vertical" H 5050 2900 50  0001 C CNN
F 3 "~" H 5050 2900 50  0001 C CNN
	1    5050 2900
	1    0    0    -1  
$EndComp
$Comp
L power:+12V #PWR023
U 1 1 5BE80D5B
P 5250 2600
F 0 "#PWR023" H 5250 2450 50  0001 C CNN
F 1 "+12V" H 5265 2773 50  0000 C CNN
F 2 "" H 5250 2600 50  0001 C CNN
F 3 "" H 5250 2600 50  0001 C CNN
	1    5250 2600
	1    0    0    -1  
$EndComp
Connection ~ 4450 3200
$Comp
L Device:R R5
U 1 1 5BE83312
P 4450 4750
F 0 "R5" H 4520 4796 50  0000 L CNN
F 1 "10k" H 4520 4705 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 4380 4750 50  0001 C CNN
F 3 "~" H 4450 4750 50  0001 C CNN
	1    4450 4750
	1    0    0    -1  
$EndComp
$Comp
L Device:R R8
U 1 1 5BE83319
P 4800 4600
F 0 "R8" V 4593 4600 50  0000 C CNN
F 1 "100" V 4684 4600 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 4730 4600 50  0001 C CNN
F 3 "~" H 4800 4600 50  0001 C CNN
	1    4800 4600
	0    1    1    0   
$EndComp
Wire Wire Line
	4650 4600 4450 4600
Wire Wire Line
	4450 4600 4000 4600
$Comp
L Transistor_FET:QM6015D Q3
U 1 1 5BE83322
P 5150 4600
F 0 "Q3" H 5356 4646 50  0000 L CNN
F 1 "IRFR120NTRPBF" H 5356 4555 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:TO-252-2" H 5350 4525 50  0001 L CIN
F 3 "http://www.jaolen.com/images/pdf/QM6015D.pdf" V 5150 4600 50  0001 L CNN
	1    5150 4600
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR018
U 1 1 5BE83329
P 4450 4900
F 0 "#PWR018" H 4450 4650 50  0001 C CNN
F 1 "GND" H 4450 4750 50  0000 C CNN
F 2 "" H 4450 4900 50  0000 C CNN
F 3 "" H 4450 4900 50  0000 C CNN
	1    4450 4900
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR026
U 1 1 5BE8332F
P 5250 4900
F 0 "#PWR026" H 5250 4650 50  0001 C CNN
F 1 "GND" H 5250 4750 50  0000 C CNN
F 2 "" H 5250 4900 50  0000 C CNN
F 3 "" H 5250 4900 50  0000 C CNN
	1    5250 4900
	1    0    0    -1  
$EndComp
Wire Wire Line
	5250 4800 5250 4900
$Comp
L Connector:Conn_01x02_Male Bubbles1
U 1 1 5BE83336
P 5050 4300
F 0 "Bubbles1" H 5156 4478 50  0000 C CNN
F 1 "Conn_01x02_Male" H 5156 4387 50  0000 C CNN
F 2 "Connector_Molex:Molex_Mini-Fit_Jr_5566-02A_2x01_P4.20mm_Vertical" H 5050 4300 50  0001 C CNN
F 3 "~" H 5050 4300 50  0001 C CNN
	1    5050 4300
	1    0    0    -1  
$EndComp
$Comp
L power:+12V #PWR025
U 1 1 5BE8333D
P 5250 4000
F 0 "#PWR025" H 5250 3850 50  0001 C CNN
F 1 "+12V" H 5265 4173 50  0000 C CNN
F 2 "" H 5250 4000 50  0001 C CNN
F 3 "" H 5250 4000 50  0001 C CNN
	1    5250 4000
	1    0    0    -1  
$EndComp
$Comp
L Device:D_Schottky D1
U 1 1 5BE83345
P 5500 4250
F 0 "D1" V 5454 4329 50  0000 L CNN
F 1 "B160-13-F" V 5545 4329 50  0000 L CNN
F 2 "Diode_SMD:D_SMA-SMB_Universal_Handsoldering" H 5500 4250 50  0001 C CNN
F 3 "~" H 5500 4250 50  0001 C CNN
	1    5500 4250
	0    1    1    0   
$EndComp
Wire Wire Line
	5250 4400 5500 4400
Connection ~ 5250 4400
$Comp
L power:+12V #PWR029
U 1 1 5BE8334E
P 5500 4000
F 0 "#PWR029" H 5500 3850 50  0001 C CNN
F 1 "+12V" H 5515 4173 50  0000 C CNN
F 2 "" H 5500 4000 50  0001 C CNN
F 3 "" H 5500 4000 50  0001 C CNN
	1    5500 4000
	1    0    0    -1  
$EndComp
Wire Wire Line
	5500 4000 5500 4100
Connection ~ 4450 4600
$Comp
L Device:R R6
U 1 1 5BE83356
P 4450 6250
F 0 "R6" H 4520 6296 50  0000 L CNN
F 1 "10k" H 4520 6205 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 4380 6250 50  0001 C CNN
F 3 "~" H 4450 6250 50  0001 C CNN
	1    4450 6250
	1    0    0    -1  
$EndComp
$Comp
L Device:R R9
U 1 1 5BE8335D
P 4800 6100
F 0 "R9" V 4593 6100 50  0000 C CNN
F 1 "100" V 4684 6100 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 4730 6100 50  0001 C CNN
F 3 "~" H 4800 6100 50  0001 C CNN
	1    4800 6100
	0    1    1    0   
$EndComp
Wire Wire Line
	4650 6100 4450 6100
Wire Wire Line
	4450 6100 4000 6100
$Comp
L Transistor_FET:QM6015D Q4
U 1 1 5BE83366
P 5150 6100
F 0 "Q4" H 5356 6146 50  0000 L CNN
F 1 "IRFR120NTRPBF" H 5356 6055 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:TO-252-2" H 5350 6025 50  0001 L CIN
F 3 "http://www.jaolen.com/images/pdf/QM6015D.pdf" V 5150 6100 50  0001 L CNN
	1    5150 6100
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR019
U 1 1 5BE8336D
P 4450 6400
F 0 "#PWR019" H 4450 6150 50  0001 C CNN
F 1 "GND" H 4450 6250 50  0000 C CNN
F 2 "" H 4450 6400 50  0000 C CNN
F 3 "" H 4450 6400 50  0000 C CNN
	1    4450 6400
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR028
U 1 1 5BE83373
P 5250 6400
F 0 "#PWR028" H 5250 6150 50  0001 C CNN
F 1 "GND" H 5250 6250 50  0000 C CNN
F 2 "" H 5250 6400 50  0000 C CNN
F 3 "" H 5250 6400 50  0000 C CNN
	1    5250 6400
	1    0    0    -1  
$EndComp
Wire Wire Line
	5250 6300 5250 6400
$Comp
L Connector:Conn_01x02_Male UV1
U 1 1 5BE8337A
P 5050 5800
F 0 "UV1" H 5156 5978 50  0000 C CNN
F 1 "Conn_01x02_Male" H 5156 5887 50  0000 C CNN
F 2 "Connector_Molex:Molex_Mini-Fit_Jr_5566-02A_2x01_P4.20mm_Vertical" H 5050 5800 50  0001 C CNN
F 3 "~" H 5050 5800 50  0001 C CNN
	1    5050 5800
	1    0    0    -1  
$EndComp
$Comp
L power:+12V #PWR027
U 1 1 5BE83381
P 5250 5500
F 0 "#PWR027" H 5250 5350 50  0001 C CNN
F 1 "+12V" H 5265 5673 50  0000 C CNN
F 2 "" H 5250 5500 50  0001 C CNN
F 3 "" H 5250 5500 50  0001 C CNN
	1    5250 5500
	1    0    0    -1  
$EndComp
Wire Wire Line
	5250 5500 5250 5800
Connection ~ 4450 6100
Wire Wire Line
	5250 4000 5250 4300
$Comp
L Connector:Conn_01x02_Male Piezo1
U 1 1 5BE902EA
P 1350 5550
F 0 "Piezo1" H 1456 5728 50  0000 C CNN
F 1 "Conn_01x02_Male" H 1456 5637 50  0000 C CNN
F 2 "Connector_Molex:Molex_Mini-Fit_Jr_5566-02A_2x01_P4.20mm_Vertical" H 1350 5550 50  0001 C CNN
F 3 "~" H 1350 5550 50  0001 C CNN
	1    1350 5550
	1    0    0    -1  
$EndComp
$Comp
L Device:R R2
U 1 1 5BE9146C
P 1900 5600
F 0 "R2" V 1693 5600 50  0000 C CNN
F 1 "1k" V 1784 5600 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 1830 5600 50  0001 C CNN
F 3 "~" H 1900 5600 50  0001 C CNN
	1    1900 5600
	-1   0    0    1   
$EndComp
$Comp
L Connector:Conn_01x03_Male Temperature1
U 1 1 5BEADC23
P 1300 3500
F 0 "Temperature1" H 1406 3778 50  0000 C CNN
F 1 "Conn_01x03_Male" H 1406 3687 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x03_P2.54mm_Vertical" H 1300 3500 50  0001 C CNN
F 3 "~" H 1300 3500 50  0001 C CNN
	1    1300 3500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR04
U 1 1 5BEAF206
P 1500 3400
F 0 "#PWR04" H 1500 3150 50  0001 C CNN
F 1 "GND" H 1500 3250 50  0000 C CNN
F 2 "" H 1500 3400 50  0000 C CNN
F 3 "" H 1500 3400 50  0000 C CNN
	1    1500 3400
	0    -1   -1   0   
$EndComp
$Comp
L power:+3V3 #PWR05
U 1 1 5BEAFB45
P 1500 3600
F 0 "#PWR05" H 1500 3450 50  0001 C CNN
F 1 "+3.3V" H 1500 3740 28  0000 C CNN
F 2 "" H 1500 3600 50  0000 C CNN
F 3 "" H 1500 3600 50  0000 C CNN
	1    1500 3600
	0    1    1    0   
$EndComp
$Comp
L Connector:Conn_01x02_Male Supply1
U 1 1 5BEC4C4C
P 1300 1000
F 0 "Supply1" H 1406 1178 50  0000 C CNN
F 1 "Conn_01x02_Male" H 1406 1087 50  0000 C CNN
F 2 "Connector_Molex:Molex_Mini-Fit_Jr_5566-02A_2x01_P4.20mm_Vertical" H 1300 1000 50  0001 C CNN
F 3 "~" H 1300 1000 50  0001 C CNN
	1    1300 1000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR01
U 1 1 5BEC595A
P 1500 1000
F 0 "#PWR01" H 1500 750 50  0001 C CNN
F 1 "GND" H 1500 850 50  0000 C CNN
F 2 "" H 1500 1000 50  0000 C CNN
F 3 "" H 1500 1000 50  0000 C CNN
	1    1500 1000
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5250 2600 5250 2900
$Comp
L power:+12V #PWR02
U 1 1 5BEE1427
P 1500 1100
F 0 "#PWR02" H 1500 950 50  0001 C CNN
F 1 "+12V" V 1515 1228 50  0000 L CNN
F 2 "" H 1500 1100 50  0001 C CNN
F 3 "" H 1500 1100 50  0001 C CNN
	1    1500 1100
	0    1    1    0   
$EndComp
Wire Wire Line
	1500 2150 1550 2150
Wire Wire Line
	1500 2250 1550 2250
Wire Wire Line
	1500 2350 1750 2350
Wire Wire Line
	1500 2450 1750 2450
$Comp
L Anyma_Library:ArduinoMKR_Wifi_1010 U3
U 1 1 5BEBC04F
P 10050 1700
F 0 "U3" H 10050 2615 50  0000 C CNN
F 1 "ArduinoMKR_Wifi_1010" H 10050 2524 50  0000 C CNN
F 2 "footprints:Arduino_MKR_Wifi_1010" H 9900 2050 50  0001 C CNN
F 3 "" H 9900 2050 50  0001 C CNN
	1    10050 1700
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x04_Male I2C1
U 1 1 5BEC9DAE
P 1300 1600
F 0 "I2C1" H 1406 1878 50  0000 C CNN
F 1 "Conn_01x04_Male" H 1406 1787 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 1300 1600 50  0001 C CNN
F 3 "~" H 1300 1600 50  0001 C CNN
	1    1300 1600
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR011
U 1 1 5BEC9DB4
P 1550 1600
F 0 "#PWR011" H 1550 1350 50  0001 C CNN
F 1 "GND" H 1550 1450 50  0000 C CNN
F 2 "" H 1550 1600 50  0000 C CNN
F 3 "" H 1550 1600 50  0000 C CNN
	1    1550 1600
	0    -1   -1   0   
$EndComp
$Comp
L power:+3V3 #PWR010
U 1 1 5BEC9DBA
P 1550 1500
F 0 "#PWR010" H 1550 1350 50  0001 C CNN
F 1 "+3.3V" H 1550 1640 28  0000 C CNN
F 2 "" H 1550 1500 50  0000 C CNN
F 3 "" H 1550 1500 50  0000 C CNN
	1    1550 1500
	0    1    1    0   
$EndComp
Wire Wire Line
	1500 1500 1550 1500
Wire Wire Line
	1500 1600 1550 1600
Wire Wire Line
	1500 1700 1750 1700
Wire Wire Line
	1500 1800 1750 1800
$Comp
L Device:R R10
U 1 1 5BECC0F6
P 7000 3550
F 0 "R10" V 6793 3550 50  0000 C CNN
F 1 "300" V 6884 3550 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 6930 3550 50  0001 C CNN
F 3 "~" H 7000 3550 50  0001 C CNN
	1    7000 3550
	0    1    1    0   
$EndComp
$Comp
L Transistor_BJT:BC847 Q5
U 1 1 5BECCB77
P 7350 3550
F 0 "Q5" H 7541 3596 50  0000 L CNN
F 1 "BC847" H 7541 3505 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 7550 3475 50  0001 L CIN
F 3 "http://www.infineon.com/dgdl/Infineon-BC847SERIES_BC848SERIES_BC849SERIES_BC850SERIES-DS-v01_01-en.pdf?fileId=db3a304314dca389011541d4630a1657" H 7350 3550 50  0001 L CNN
	1    7350 3550
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR037
U 1 1 5BECEBD7
P 7450 3750
F 0 "#PWR037" H 7450 3500 50  0001 C CNN
F 1 "GND" H 7450 3600 50  0000 C CNN
F 2 "" H 7450 3750 50  0000 C CNN
F 3 "" H 7450 3750 50  0000 C CNN
	1    7450 3750
	1    0    0    -1  
$EndComp
$Comp
L power:+12V #PWR036
U 1 1 5BECF8DF
P 7450 2450
F 0 "#PWR036" H 7450 2300 50  0001 C CNN
F 1 "+12V" H 7465 2623 50  0000 C CNN
F 2 "" H 7450 2450 50  0001 C CNN
F 3 "" H 7450 2450 50  0001 C CNN
	1    7450 2450
	1    0    0    -1  
$EndComp
$Comp
L Device:R R12
U 1 1 5BED21C3
P 7450 3200
F 0 "R12" V 7243 3200 50  0000 C CNN
F 1 "150" V 7334 3200 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 7380 3200 50  0001 C CNN
F 3 "~" H 7450 3200 50  0001 C CNN
	1    7450 3200
	-1   0    0    1   
$EndComp
$Comp
L power:+12V #PWR044
U 1 1 5BED3159
P 7950 2450
F 0 "#PWR044" H 7950 2300 50  0001 C CNN
F 1 "+12V" H 7965 2623 50  0000 C CNN
F 2 "" H 7950 2450 50  0001 C CNN
F 3 "" H 7950 2450 50  0001 C CNN
	1    7950 2450
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR042
U 1 1 5BED3AEE
P 7850 3750
F 0 "#PWR042" H 7850 3500 50  0001 C CNN
F 1 "GND" H 7850 3600 50  0000 C CNN
F 2 "" H 7850 3750 50  0000 C CNN
F 3 "" H 7850 3750 50  0000 C CNN
	1    7850 3750
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x02_Male Heater1
U 1 1 5BED3E1F
P 8050 3450
F 0 "Heater1" H 7600 3600 50  0000 C CNN
F 1 "Conn_01x02_Male" H 7800 3500 50  0000 C CNN
F 2 "Connector_Molex:Molex_Mini-Fit_Jr_5566-02A_2x01_P4.20mm_Vertical" H 8050 3450 50  0001 C CNN
F 3 "~" H 8050 3450 50  0001 C CNN
	1    8050 3450
	-1   0    0    1   
$EndComp
$Comp
L Device:D_Schottky D2
U 1 1 5BED6731
P 7100 2750
F 0 "D2" V 7150 2550 50  0000 L CNN
F 1 "D_Schottky" V 7050 2250 50  0000 L CNN
F 2 "Diode_SMD:D_SMA-SMB_Universal_Handsoldering" H 7100 2750 50  0001 C CNN
F 3 "~" H 7100 2750 50  0001 C CNN
	1    7100 2750
	0    1    1    0   
$EndComp
Wire Wire Line
	7100 2600 7100 2450
Wire Wire Line
	7100 2450 7450 2450
Wire Wire Line
	7450 3050 7100 3050
Wire Wire Line
	7100 3050 7100 2900
Wire Wire Line
	7850 3350 7850 3050
Wire Wire Line
	7850 3450 7850 3750
Wire Wire Line
	6850 3550 6650 3550
$Comp
L Device:R R11
U 1 1 5BEE42E9
P 7000 5500
F 0 "R11" V 6793 5500 50  0000 C CNN
F 1 "300" V 6884 5500 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 6930 5500 50  0001 C CNN
F 3 "~" H 7000 5500 50  0001 C CNN
	1    7000 5500
	0    1    1    0   
$EndComp
$Comp
L Transistor_BJT:BC847 Q6
U 1 1 5BEE42EF
P 7350 5500
F 0 "Q6" H 7541 5546 50  0000 L CNN
F 1 "BC847" H 7541 5455 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 7550 5425 50  0001 L CIN
F 3 "http://www.infineon.com/dgdl/Infineon-BC847SERIES_BC848SERIES_BC849SERIES_BC850SERIES-DS-v01_01-en.pdf?fileId=db3a304314dca389011541d4630a1657" H 7350 5500 50  0001 L CNN
	1    7350 5500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR039
U 1 1 5BEE42FB
P 7450 5700
F 0 "#PWR039" H 7450 5450 50  0001 C CNN
F 1 "GND" H 7450 5550 50  0000 C CNN
F 2 "" H 7450 5700 50  0000 C CNN
F 3 "" H 7450 5700 50  0000 C CNN
	1    7450 5700
	1    0    0    -1  
$EndComp
$Comp
L power:+12V #PWR038
U 1 1 5BEE4301
P 7450 4400
F 0 "#PWR038" H 7450 4250 50  0001 C CNN
F 1 "+12V" H 7465 4573 50  0000 C CNN
F 2 "" H 7450 4400 50  0001 C CNN
F 3 "" H 7450 4400 50  0001 C CNN
	1    7450 4400
	1    0    0    -1  
$EndComp
$Comp
L Device:R R13
U 1 1 5BEE4307
P 7450 5150
F 0 "R13" V 7243 5150 50  0000 C CNN
F 1 "150" V 7334 5150 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 7380 5150 50  0001 C CNN
F 3 "~" H 7450 5150 50  0001 C CNN
	1    7450 5150
	-1   0    0    1   
$EndComp
$Comp
L power:+12V #PWR045
U 1 1 5BEE430D
P 7950 4400
F 0 "#PWR045" H 7950 4250 50  0001 C CNN
F 1 "+12V" H 7965 4573 50  0000 C CNN
F 2 "" H 7950 4400 50  0001 C CNN
F 3 "" H 7950 4400 50  0001 C CNN
	1    7950 4400
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR043
U 1 1 5BEE4313
P 7850 5700
F 0 "#PWR043" H 7850 5450 50  0001 C CNN
F 1 "GND" H 7850 5550 50  0000 C CNN
F 2 "" H 7850 5700 50  0000 C CNN
F 3 "" H 7850 5700 50  0000 C CNN
	1    7850 5700
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x02_Male Heater2
U 1 1 5BEE4319
P 8050 5400
F 0 "Heater2" H 7600 5550 50  0000 C CNN
F 1 "Conn_01x02_Male" H 7800 5450 50  0000 C CNN
F 2 "Connector_Molex:Molex_Mini-Fit_Jr_5566-02A_2x01_P4.20mm_Vertical" H 8050 5400 50  0001 C CNN
F 3 "~" H 8050 5400 50  0001 C CNN
	1    8050 5400
	-1   0    0    1   
$EndComp
$Comp
L Device:D_Schottky D3
U 1 1 5BEE431F
P 7100 4700
F 0 "D3" V 7150 4500 50  0000 L CNN
F 1 "D_Schottky" V 7050 4200 50  0000 L CNN
F 2 "Diode_SMD:D_SMA-SMB_Universal_Handsoldering" H 7100 4700 50  0001 C CNN
F 3 "~" H 7100 4700 50  0001 C CNN
	1    7100 4700
	0    1    1    0   
$EndComp
Wire Wire Line
	7100 4550 7100 4400
Wire Wire Line
	7100 4400 7450 4400
Wire Wire Line
	7450 5000 7100 5000
Wire Wire Line
	7100 5000 7100 4850
Wire Wire Line
	7850 5300 7850 5000
Wire Wire Line
	7850 5400 7850 5700
Wire Wire Line
	6850 5500 6650 5500
$Comp
L Connector:Conn_01x02_Male UV2
U 1 1 5BEF9793
P 5950 5800
F 0 "UV2" H 6056 5978 50  0000 C CNN
F 1 "Conn_01x02_Male" H 6056 5887 50  0000 C CNN
F 2 "Connector_Molex:Molex_Mini-Fit_Jr_5566-02A_2x01_P4.20mm_Vertical" H 5950 5800 50  0001 C CNN
F 3 "~" H 5950 5800 50  0001 C CNN
	1    5950 5800
	-1   0    0    -1  
$EndComp
Wire Wire Line
	5750 5800 5250 5800
Connection ~ 5250 5800
Wire Wire Line
	5250 5900 5750 5900
Connection ~ 5250 5900
$Comp
L Connector:Conn_01x03_Male Temperature2
U 1 1 5BF02AE6
P 1300 4100
F 0 "Temperature2" H 1406 4378 50  0000 C CNN
F 1 "Conn_01x03_Male" H 1406 4287 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x03_P2.54mm_Vertical" H 1300 4100 50  0001 C CNN
F 3 "~" H 1300 4100 50  0001 C CNN
	1    1300 4100
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR06
U 1 1 5BF02AEC
P 1500 4000
F 0 "#PWR06" H 1500 3750 50  0001 C CNN
F 1 "GND" H 1500 3850 50  0000 C CNN
F 2 "" H 1500 4000 50  0000 C CNN
F 3 "" H 1500 4000 50  0000 C CNN
	1    1500 4000
	0    -1   -1   0   
$EndComp
$Comp
L power:+3V3 #PWR07
U 1 1 5BF02AF2
P 1500 4200
F 0 "#PWR07" H 1500 4050 50  0001 C CNN
F 1 "+3.3V" H 1500 4340 28  0000 C CNN
F 2 "" H 1500 4200 50  0000 C CNN
F 3 "" H 1500 4200 50  0000 C CNN
	1    1500 4200
	0    1    1    0   
$EndComp
$Comp
L Connector:Conn_01x03_Male Temperature3
U 1 1 5BF0380F
P 1300 4700
F 0 "Temperature3" H 1406 4978 50  0000 C CNN
F 1 "Conn_01x03_Male" H 1406 4887 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x03_P2.54mm_Vertical" H 1300 4700 50  0001 C CNN
F 3 "~" H 1300 4700 50  0001 C CNN
	1    1300 4700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR08
U 1 1 5BF03815
P 1500 4600
F 0 "#PWR08" H 1500 4350 50  0001 C CNN
F 1 "GND" H 1500 4450 50  0000 C CNN
F 2 "" H 1500 4600 50  0000 C CNN
F 3 "" H 1500 4600 50  0000 C CNN
	1    1500 4600
	0    -1   -1   0   
$EndComp
$Comp
L power:+3V3 #PWR09
U 1 1 5BF0381B
P 1500 4800
F 0 "#PWR09" H 1500 4650 50  0001 C CNN
F 1 "+3.3V" H 1500 4940 28  0000 C CNN
F 2 "" H 1500 4800 50  0000 C CNN
F 3 "" H 1500 4800 50  0000 C CNN
	1    1500 4800
	0    1    1    0   
$EndComp
$Comp
L Device:R R3
U 1 1 5BF0897C
P 2000 3350
F 0 "R3" V 1793 3350 50  0000 C CNN
F 1 "22" V 1884 3350 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 1930 3350 50  0001 C CNN
F 3 "~" H 2000 3350 50  0001 C CNN
	1    2000 3350
	-1   0    0    1   
$EndComp
$Comp
L power:+3.3V #PWR016
U 1 1 5BF0964E
P 2000 3200
F 0 "#PWR016" H 2000 3050 50  0001 C CNN
F 1 "+3.3V" H 2015 3373 50  0000 C CNN
F 2 "" H 2000 3200 50  0001 C CNN
F 3 "" H 2000 3200 50  0001 C CNN
	1    2000 3200
	1    0    0    -1  
$EndComp
Wire Wire Line
	1500 3500 2000 3500
Wire Wire Line
	2000 3500 2400 3500
Connection ~ 2000 3500
Wire Wire Line
	2000 4100 2000 3500
Wire Wire Line
	1500 4100 2000 4100
Wire Wire Line
	2000 4700 2000 4100
Wire Wire Line
	1500 4700 2000 4700
Connection ~ 2000 4100
$Comp
L power:+12V #PWR014
U 1 1 5BF1343E
P 1900 5350
F 0 "#PWR014" H 1900 5200 50  0001 C CNN
F 1 "+12V" H 1915 5523 50  0000 C CNN
F 2 "" H 1900 5350 50  0001 C CNN
F 3 "" H 1900 5350 50  0001 C CNN
	1    1900 5350
	1    0    0    -1  
$EndComp
Wire Wire Line
	1550 5550 1800 5550
Wire Wire Line
	1800 5550 1800 5450
Wire Wire Line
	1800 5450 1900 5450
Wire Wire Line
	1900 5450 1900 5350
Connection ~ 1900 5450
Wire Wire Line
	1550 5650 1800 5650
Wire Wire Line
	1800 5650 1800 5750
Wire Wire Line
	1800 5750 1900 5750
$Comp
L Device:R R1
U 1 1 5BF1D11B
P 1450 6000
F 0 "R1" V 1243 6000 50  0000 C CNN
F 1 "1k" V 1334 6000 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 1380 6000 50  0001 C CNN
F 3 "~" H 1450 6000 50  0001 C CNN
	1    1450 6000
	0    1    1    0   
$EndComp
$Comp
L Transistor_BJT:BC847 Q1
U 1 1 5BF1D121
P 1800 6000
F 0 "Q1" H 1991 6046 50  0000 L CNN
F 1 "BC847" H 1991 5955 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 2000 5925 50  0001 L CIN
F 3 "http://www.infineon.com/dgdl/Infineon-BC847SERIES_BC848SERIES_BC849SERIES_BC850SERIES-DS-v01_01-en.pdf?fileId=db3a304314dca389011541d4630a1657" H 1800 6000 50  0001 L CNN
	1    1800 6000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR015
U 1 1 5BF1D127
P 1900 6200
F 0 "#PWR015" H 1900 5950 50  0001 C CNN
F 1 "GND" H 1900 6050 50  0000 C CNN
F 2 "" H 1900 6200 50  0000 C CNN
F 3 "" H 1900 6200 50  0000 C CNN
	1    1900 6200
	1    0    0    -1  
$EndComp
Wire Wire Line
	1300 6000 1100 6000
Wire Wire Line
	1900 5800 1900 5750
Connection ~ 1900 5750
$Comp
L Regulator_Linear:L7805 U1
U 1 1 5BF2A4A9
P 5550 1150
F 0 "U1" H 5550 1392 50  0000 C CNN
F 1 "L7805" H 5550 1301 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:TO-252-2" H 5575 1000 50  0001 L CIN
F 3 "http://www.st.com/content/ccc/resource/technical/document/datasheet/41/4f/b3/b0/12/d4/47/88/CD00000444.pdf/files/CD00000444.pdf/jcr:content/translations/en.CD00000444.pdf" H 5550 1100 50  0001 C CNN
	1    5550 1150
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR030
U 1 1 5BF2B9B6
P 5550 1450
F 0 "#PWR030" H 5550 1200 50  0001 C CNN
F 1 "GND" H 5550 1300 50  0000 C CNN
F 2 "" H 5550 1450 50  0000 C CNN
F 3 "" H 5550 1450 50  0000 C CNN
	1    5550 1450
	1    0    0    -1  
$EndComp
$Comp
L Device:C C2
U 1 1 5BF2CAE0
P 4950 1350
F 0 "C2" H 5065 1396 50  0000 L CNN
F 1 "C" H 5065 1305 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 4988 1200 50  0001 C CNN
F 3 "~" H 4950 1350 50  0001 C CNN
	1    4950 1350
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR022
U 1 1 5BF2D52E
P 4950 1500
F 0 "#PWR022" H 4950 1250 50  0001 C CNN
F 1 "GND" H 4950 1350 50  0000 C CNN
F 2 "" H 4950 1500 50  0000 C CNN
F 3 "" H 4950 1500 50  0000 C CNN
	1    4950 1500
	1    0    0    -1  
$EndComp
$Comp
L Device:CP C1
U 1 1 5BF2E187
P 4650 1350
F 0 "C1" H 4768 1396 50  0000 L CNN
F 1 "CP" H 4768 1305 50  0000 L CNN
F 2 "Capacitor_Tantalum_SMD:CP_EIA-7343-20_Kemet-V_Pad2.25x2.55mm_HandSolder" H 4688 1200 50  0001 C CNN
F 3 "~" H 4650 1350 50  0001 C CNN
	1    4650 1350
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR021
U 1 1 5BF2EAC8
P 4650 1500
F 0 "#PWR021" H 4650 1250 50  0001 C CNN
F 1 "GND" H 4650 1350 50  0000 C CNN
F 2 "" H 4650 1500 50  0000 C CNN
F 3 "" H 4650 1500 50  0000 C CNN
	1    4650 1500
	1    0    0    -1  
$EndComp
Wire Wire Line
	5250 1150 4950 1150
Wire Wire Line
	4950 1150 4950 1200
Wire Wire Line
	4650 1150 4650 1200
Wire Wire Line
	4650 1150 4950 1150
Connection ~ 4950 1150
$Comp
L power:+12V #PWR020
U 1 1 5BF35404
P 4650 1150
F 0 "#PWR020" H 4650 1000 50  0001 C CNN
F 1 "+12V" V 4665 1278 50  0000 L CNN
F 2 "" H 4650 1150 50  0001 C CNN
F 3 "" H 4650 1150 50  0001 C CNN
	1    4650 1150
	1    0    0    -1  
$EndComp
Connection ~ 4650 1150
$Comp
L Device:C C3
U 1 1 5BF35F4C
P 6000 1350
F 0 "C3" H 6115 1396 50  0000 L CNN
F 1 "C" H 6115 1305 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 6038 1200 50  0001 C CNN
F 3 "~" H 6000 1350 50  0001 C CNN
	1    6000 1350
	1    0    0    -1  
$EndComp
$Comp
L Device:CP C4
U 1 1 5BF3640D
P 6300 1350
F 0 "C4" H 6418 1396 50  0000 L CNN
F 1 "CP" H 6418 1305 50  0000 L CNN
F 2 "Capacitor_Tantalum_SMD:CP_EIA-7343-20_Kemet-V_Pad2.25x2.55mm_HandSolder" H 6338 1200 50  0001 C CNN
F 3 "~" H 6300 1350 50  0001 C CNN
	1    6300 1350
	1    0    0    -1  
$EndComp
Wire Wire Line
	5850 1150 6000 1150
Wire Wire Line
	6300 1150 6300 1200
Wire Wire Line
	6000 1200 6000 1150
Connection ~ 6000 1150
Wire Wire Line
	6000 1150 6150 1150
$Comp
L power:GND #PWR031
U 1 1 5BF38E34
P 6000 1500
F 0 "#PWR031" H 6000 1250 50  0001 C CNN
F 1 "GND" H 6000 1350 50  0000 C CNN
F 2 "" H 6000 1500 50  0000 C CNN
F 3 "" H 6000 1500 50  0000 C CNN
	1    6000 1500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR033
U 1 1 5BF392D1
P 6300 1500
F 0 "#PWR033" H 6300 1250 50  0001 C CNN
F 1 "GND" H 6300 1350 50  0000 C CNN
F 2 "" H 6300 1500 50  0000 C CNN
F 3 "" H 6300 1500 50  0000 C CNN
	1    6300 1500
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR032
U 1 1 5BF3C28F
P 6150 1150
F 0 "#PWR032" H 6150 1000 50  0001 C CNN
F 1 "+5V" H 6165 1323 50  0000 C CNN
F 2 "" H 6150 1150 50  0001 C CNN
F 3 "" H 6150 1150 50  0001 C CNN
	1    6150 1150
	1    0    0    -1  
$EndComp
Connection ~ 6150 1150
Wire Wire Line
	6150 1150 6300 1150
$Comp
L Regulator_Linear:LD1117S33TR_SOT223 U2
U 1 1 5BF3E130
P 6850 1150
F 0 "U2" H 6850 1392 50  0000 C CNN
F 1 "LD1117S33TR_SOT223" H 6850 1301 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-223-3_TabPin2" H 6850 1350 50  0001 C CNN
F 3 "http://www.st.com/st-web-ui/static/active/en/resource/technical/document/datasheet/CD00000544.pdf" H 6950 900 50  0001 C CNN
	1    6850 1150
	1    0    0    -1  
$EndComp
Wire Wire Line
	6300 1150 6550 1150
Connection ~ 6300 1150
$Comp
L power:GND #PWR034
U 1 1 5BF40614
P 6850 1450
F 0 "#PWR034" H 6850 1200 50  0001 C CNN
F 1 "GND" H 6850 1300 50  0000 C CNN
F 2 "" H 6850 1450 50  0000 C CNN
F 3 "" H 6850 1450 50  0000 C CNN
	1    6850 1450
	1    0    0    -1  
$EndComp
$Comp
L Device:C C5
U 1 1 5BF40C0D
P 7250 1350
F 0 "C5" H 7365 1396 50  0000 L CNN
F 1 "C" H 7365 1305 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 7288 1200 50  0001 C CNN
F 3 "~" H 7250 1350 50  0001 C CNN
	1    7250 1350
	1    0    0    -1  
$EndComp
$Comp
L Device:CP C6
U 1 1 5BF40F12
P 7600 1350
F 0 "C6" H 7718 1396 50  0000 L CNN
F 1 "CP" H 7718 1305 50  0000 L CNN
F 2 "Capacitor_Tantalum_SMD:CP_EIA-7343-20_Kemet-V_Pad2.25x2.55mm_HandSolder" H 7638 1200 50  0001 C CNN
F 3 "~" H 7600 1350 50  0001 C CNN
	1    7600 1350
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR035
U 1 1 5BF41289
P 7250 1500
F 0 "#PWR035" H 7250 1250 50  0001 C CNN
F 1 "GND" H 7250 1350 50  0000 C CNN
F 2 "" H 7250 1500 50  0000 C CNN
F 3 "" H 7250 1500 50  0000 C CNN
	1    7250 1500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR041
U 1 1 5BF41474
P 7600 1500
F 0 "#PWR041" H 7600 1250 50  0001 C CNN
F 1 "GND" H 7600 1350 50  0000 C CNN
F 2 "" H 7600 1500 50  0000 C CNN
F 3 "" H 7600 1500 50  0000 C CNN
	1    7600 1500
	1    0    0    -1  
$EndComp
Wire Wire Line
	7150 1150 7250 1150
Wire Wire Line
	7250 1150 7250 1200
Wire Wire Line
	7250 1150 7600 1150
Wire Wire Line
	7600 1150 7600 1200
Connection ~ 7250 1150
$Comp
L power:+3.3V #PWR040
U 1 1 5BF461F5
P 7600 1150
F 0 "#PWR040" H 7600 1000 50  0001 C CNN
F 1 "+3.3V" H 7615 1323 50  0000 C CNN
F 2 "" H 7600 1150 50  0001 C CNN
F 3 "" H 7600 1150 50  0001 C CNN
	1    7600 1150
	1    0    0    -1  
$EndComp
Connection ~ 7600 1150
$Comp
L power:+5V #PWR046
U 1 1 5BF47E90
P 10550 1150
F 0 "#PWR046" H 10550 1000 50  0001 C CNN
F 1 "+5V" V 10565 1278 50  0000 L CNN
F 2 "" H 10550 1150 50  0001 C CNN
F 3 "" H 10550 1150 50  0001 C CNN
	1    10550 1150
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR047
U 1 1 5BF48E8B
P 10550 1350
F 0 "#PWR047" H 10550 1100 50  0001 C CNN
F 1 "GND" H 10550 1200 50  0000 C CNN
F 2 "" H 10550 1350 50  0000 C CNN
F 3 "" H 10550 1350 50  0000 C CNN
	1    10550 1350
	0    -1   -1   0   
$EndComp
Wire Wire Line
	10550 1750 10750 1750
Wire Wire Line
	10550 1850 10750 1850
Text Label 10750 1850 0    50   ~ 0
SDA
Text Label 10750 1750 0    50   ~ 0
SCL
Text Label 1750 1700 0    50   ~ 0
SCL
Text Label 1750 1800 0    50   ~ 0
SDA
Text Label 1750 2350 0    50   ~ 0
SCL
Text Label 1750 2450 0    50   ~ 0
SDA
Wire Wire Line
	9550 2350 9350 2350
Wire Wire Line
	9550 2250 9350 2250
Wire Wire Line
	9550 2150 9350 2150
Wire Wire Line
	9550 2050 9350 2050
Wire Wire Line
	9550 1950 9350 1950
Wire Wire Line
	9550 1850 9350 1850
Text Label 9350 2350 0    50   ~ 0
D5
Text Label 6650 5500 0    50   ~ 0
D5
Text Label 9350 2250 0    50   ~ 0
D4
Text Label 6650 3550 0    50   ~ 0
D4
Text Label 9350 2150 0    50   ~ 0
D3
Text Label 4000 6100 0    50   ~ 0
D3
Text Label 9350 2050 0    50   ~ 0
D2
Text Label 4000 4600 0    50   ~ 0
D2
Text Label 9350 1950 0    50   ~ 0
D1
Text Label 4000 3200 0    50   ~ 0
D1
Text Label 1100 6000 0    50   ~ 0
D0
Text Label 9350 1850 0    50   ~ 0
D0
Text Label 2350 3500 0    50   ~ 0
D6
Wire Wire Line
	10550 2350 10750 2350
Wire Wire Line
	10550 2250 10750 2250
Text Label 1750 2950 0    50   ~ 0
D7
Text Label 10750 2350 0    50   ~ 0
D6
Text Label 10750 2250 0    50   ~ 0
D7
$Comp
L Anyma_Library:LZ_9H K2
U 1 1 5BFB5772
P 7650 4700
F 0 "K2" H 8080 4746 50  0000 L CNN
F 1 "LZ_9H" H 8080 4655 50  0000 L CNN
F 2 "Relay_THT:LZ-9H" H 8100 4650 50  0001 L CNN
F 3 "https://www.fujitsu.com/downloads/MICRO/fcai/relays/lz.pdf" H 7650 4700 50  0001 C CNN
	1    7650 4700
	1    0    0    -1  
$EndComp
Connection ~ 7450 5000
Connection ~ 7450 4400
$Comp
L Anyma_Library:LZ_9H K1
U 1 1 5BFB9E7C
P 7650 2750
F 0 "K1" H 8080 2796 50  0000 L CNN
F 1 "LZ_9H" H 8080 2705 50  0000 L CNN
F 2 "Relay_THT:LZ-9H" H 8100 2700 50  0001 L CNN
F 3 "https://www.fujitsu.com/downloads/MICRO/fcai/relays/lz.pdf" H 7650 2750 50  0001 C CNN
	1    7650 2750
	1    0    0    -1  
$EndComp
Connection ~ 7450 3050
Connection ~ 7450 2450
$EndSCHEMATC
