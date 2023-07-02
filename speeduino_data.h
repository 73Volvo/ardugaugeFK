// Stat struct settings
struct stat{
    int number; // the number of the stat on the Speeduino serial connection
    int subNumber; // sub-number if the stat is a bit from a whole byte
    String name;
    int type; // bit/byte/word
    int multiplier; // 0 = temp, divide by 10 then multiply stat with resulting number. If the multiplier is "0" the stat goes through temp conversion
    int decimal; // 0 for no decimal, 1 for a single decimal
};

stat speeduino[] = {
  {0,0,"SECL",1,10,0},// 0 - Simple counter, incremented each second
  {1,0,"INJ1",0,10,0},// 1 - Status Register, see single bits below
  {1,1,"INJ2",0,10,0},// 2
  {1,2,"INJ3",0,10,0},// 3
  {1,3,"INJ4",0,10,0},// 4
  {1,4,"DFCO",0,10,0},// 5 - Deceleration Fuel Cut Off
  {1,5,"BST CUT",0,10,0},// 6
  {1,6,"TOOTH1",0,10,0},// 7 - Tooth Log 1 Ready
  {1,7,"TOOTH2",0,10,0},// 8 - Tooth Log 2 Ready
  {2,0,"RUNNING",0,10,0},// 9 - Engine Register, see single bits below
  {2,1,"CRANK",0,10,0},// 10
  {2,2,"ASE",0,10,0},// 11 - Afterstart Enrichment
  {2,3,"WARMUP",0,10,0},// 12 - Warmup Enrichment
  {2,4,"TPSAE",0,10,0},// 13 - TPS-based Acceleration Enrichment
  {2,5,"TPSDE",0,10,0},// 14 - TPS-based Deceleration Enleanment
  {2,6,"MAPAE",0,10,0},// 15 - MAP-based Acceleration Enrichment
  {2,7,"MAPDE",0,10,0},// 16 - MAP-based Deceleration Enleanment
  {3,0,"DWELL",1,1,1},// 17 - Dwell (ms, divide by 10)
  {4,0,"MAP",2,10,0},// 18 - MAP (kPa, 2 bytes)
  {6,0,"IAT",1,0,0},// 19 - Intake Air Temperature (Degrees, subtract 40)
  {7,0,"CLT",1,0,0},// 20 - Coolant Temperature (Degrees, subtract 40)
  {9,0,"BAT",1,10,1},// 21 - Battery Voltage (Volts, divide by 10)
  {10,0,"AFR",1,10,1},// 22 - Air-Fuel Ratio (divide by 10)
  {11,0,"EGOCORR",1,10,0},// 23 - EGO Correction (%)
  {12,0,"IATCORR",1,10,0},// 24 - Air Temperature Correction (%)
  {13,0,"WUECORR",1,10,0},// 25 - Warmup Enrichment Correction (%)
  {14,0,"RPM",2,10,0},// 26 - RPM (2 bytes)
  {16,0,"AECORR",1,10,0},// 27 - Acceleration Enrichment Correction (%)
  {17,0,"GAMMAE",1,10,0},// 28 - Gamma Correction (%)
  {18,0,"CURRENT VE",1,10,0},// 29 - Current VE
  {19,0,"AFR TGT",1,10,1},// 30 - AFR Target (divide by 10)
  {20,0,"PW1",2,1,2},// 31 - PulseWidth 1 (ms, divide by 10)
  {22,0,"TPSDOT",1,100,0},// 32 - TPSdot (%/s, multiply by 10)
  {23,0,"ADVANCE",1,10,0},// 33 - (Degrees BTDC)
  {24,0,"TPS",1,5,1},// 34 - Throttle Position (%, divide by 2, 0.5% resolution)
  {25,0,"LOOPSEC",2,10,0},// 35 - (Loops/s, 2 bytes)
  {27,0,"FREE RAM",2,10,0},// 36 - (Bytes, 2 bytes)
  {29,0,"BST TGT",1,20,0},// 37 - Boost Target - (kPa, multiply by 2)
  {30,0,"BST DTY",1,10,0},// 38 - Boost PWM duty (%)
  {31,0,"HRDLNCH",0,10,0},// 39 - Launch Control limiter, hard // Spark Register, see single bits below
  {31,1,"SFTLNCH",0,10,0},// 40 - Launch Control limiter, soft
  {31,2,"HRDLIMT",0,10,0},// 41 - Rev limiter, hard
  {31,3,"SFTLMIT",0,10,0},// 42 - Rev limiter, soft
  {31,4,"BST CUT",0,10,0},// 43
  {31,5,"ERROR",0,10,0},// 44
  {31,6,"IDLCTRL",0,10,0},// 45
  {31,7,"SYNC",0,10,0},// 46
  {32,0,"RPMDOT",2,10,0},// 47 - RPMdot (rpm/s, 2 bytes)
  {34,0,"ETH PCT",1,10,0},// 48 - Ethanol content (%, 0 if not used)
  {35,0,"FLXCORR",1,10,0},// 49 - Flex fuel Correction (%)
  {36,0,"FLICORR",1,10,0},// 50 - Flex fuel advance correction (Degrees)
  {37,0,"IDLE LD",1,10,0},// 51 - Idle valve load (PWM duty or Steps)
  {38,0,"OUTENBL",0,10,0},// 52 - Test Outputs Register, see single bits below
  {38,1,"OUTACTV",0,10,0},// 53
  {39,0,"AFR2",1,1,1},// 54 - Secondary AFR Sensor (divide by 10)
  {40,0,"BARO",1,10,0},// 55 - Berometric pressure (kPa)
  {41,0,"CAN0",2,10,0},// 56 - CAN Input 0 (2 bytes)
  {43,0,"CAN1",2,10,0},// 57 - CAN Input 1 (2 bytes)
  {45,0,"CAN2",2,10,0},// 58 - CAN Input 2 (2 bytes)
  {47,0,"CAN3",2,10,0},// 59 - CAN Input 3 (2 bytes)
  {49,0,"CAN4",2,10,0},// 60 - CAN Input 4 (2 bytes)
  {51,0,"CAN5",2,10,0},// 61 - CAN Input 5 (2 bytes)
  {53,0,"CAN6",2,10,0},// 62 - CAN Input 6 (2 bytes)
  {55,0,"CAN7",2,10,0},// 63 - CAN Input 7 (2 bytes)
  {57,0,"CAN8",2,10,0},// 64 - CAN Input 8 (2 bytes)
  {59,0,"CAN9",2,10,0},// 65 - CAN Input 9 (2 bytes)
  {61,0,"CAN10",2,10,0},// 66 - CAN Input 10 (2 bytes)
  {63,0,"CAN11",2,10,0},// 67 - CAN Input 11 (2 bytes)
  {65,0,"CAN12",2,10,0},// 68 - CAN Input 12 (2 bytes)
  {67,0,"CAN13",2,10,0},// 69 - CAN Input 13 (2 bytes)
  {69,0,"CAN14",2,10,0},// 70 - CAN Input 14 (2 bytes)
  {71,0,"CAN15",2,10,0},// 71 - CAN Input 15 (2 bytes)
  {73,0,"TPSADC",1,10,0},// 72 - TPS ADC Value (0...255)
  {74,0,"ERROR",1,10,0},// 73 - Error code (Error Number 0:1, Current Error 2:7)
  {75,0,"LNCHCOR",1,10,0},// 74 - Launch Correction (%)
  {76,0,"PW2",2,1,2},// 75 - PulseWidth 2 (ms, divide by 10)
  {78,0,"PW3",2,1,2},// 76 - PulseWidth 3 (ms, divide by 10)
  {80,0,"PW4",2,1,2},// 77 - PulseWidth 4 (ms, divide by 10)
  {82,0,"RST LCK",0,10,0},// 78 - Status3 Register, see single bits below
  {82,1,"NIT ON",0,10,0},// 79
  {82,2,"FL2 ACT",0,10,0},// 80 - Secondary VE Table
  {82,3,"VSS RF",0,10,0},// 81 - VSS Refresh
  {82,4,"HF SYNC",0,10,0},// 82
  {82,6,"#SQTS6",0,10,0},// 83 - Bits 6:7
  {82,7,"#SQTS7",0,10,0},// 84 - Bits 6:7
  {83,0,"PRO RPM",0,10,0},// 85 - Engine Protect Register, see single bits below
  {83,1,"PRO MAP",0,10,0},// 86
  {83,2,"PRO OIL",0,10,0},// 87
  {83,3,"PRO AFR",0,10,0},// 88
  {84,0,"FUEL LOAD",2,10,0},// 89 - Fuel Table load (kPa or TPS%, 2 bytes)
  {86,0,"IGN LOAD",2,10,0},// 90 - Spark Table load (kPa or TPS%, 2 bytes)
  {88,0,"INJ ANGLE",2,10,0},// 91 - Injection angle (2 bytes)
  {90,0,"IDLE LOAD",1,10,0},// 92 - DUPLICATE OF ADDRESS 37 - HOPEFULLY WILL BE FIXED IN THE SPEEDUINO FIRMWARE
  {91,0,"IDLE TARGET",1,100,0},// 93 - Closed-Loop Idle Target RPM (multiply by 10)
  {92,0,"MAP DOT",1,100,0},// 94 - MAPdot (kPa/s, multiply by 10)
  {93,0,"VVT1ANG",1,10,0},// 95 - VVT1 Angle (Degrees, SIGNED)
  {94,0,"VVT1TGT",1,10,0},// 96 - VVT1 Target Angle (Degrees)
  {95,0,"VVT1DUTY",1,10,0},// 97 - VVT1 PWM Duty (%)
  {96,0,"FLXBSTCORR",2,10,0},// 98 - Flex fuel Boost Correction, 2 bytes)
  {98,0,"BARO CORR",1,10,0},// 99 - Barometric Correction (%)
  {99,0,"ASE CORR",1,10,0},// 100 - After Start Enrichment Correction (%)
  {100,0,"VSS",2,10,0},// 101 - Speed reading via VSS (2 bytes)
  {102,0,"GEAR",1,10,0},// 102 - Current gear
  {103,0,"FUELPRSS",1,10,0},// 103 - (PSI or Bar)
  {104,0,"OILPRSS",1,10,0},// 104 - (PSI or Bar)
  {105,0,"WMI_PW",1,10,0},// 105 - Water-Meth Injection Pulsewidth
  {106,0,"WM_EMPT",0,10,0},// 106 - Water-Meth Tank empty // Status4 Register, see single bits below
  {106,1,"VVT1_ER",0,10,0},// 107
  {106,2,"VVT2_ER",0,10,0},// 108
  {106,3,"FAN",0,10,0},// 109 - Cooling fan
  {107,0,"VVT2ANG",1,10,0},// 110 - VVT2 Angle (Degree, SIGNED)
  {108,0,"VVT2TGT",1,10,0},// 111 - VVT2 Target Angle (Degrees)
  {109,0,"VVT2DUTY",1,10,0},// 112 - VVT2 PWM Duty (%)
  {110,0,"OUTSTAT",1,10,0},// 113 - Programmable Outputs
  {112,0,"FUELTEMP",1,0,0},// 114 - Fuel Temperature from Flex Sensor (Degrees, subtract 40)
  {113,0,"FUELTEMP CORR",1,10,0},// 115 - Fuel Temperature Correction (%)
  {114,0,"VE1",1,10,0},// 116 - VE from Primary Fuel Table
  {115,0,"VE2",1,10,0},// 117 - VE from Secondary Fuel Table
  {116,0,"ADV1",1,10,0},// 118 - Advance from Primary Spark Table (Degrees BTDC)
  {117,0,"ADV2",1,10,0},// 119 - Advance from Secondary Spark Table (Degrees BTDC)
  {117,0,"NIT_OFF",0,10,0},// 120 - N2O status, see values below
  {117,1,"NIT_ST1",0,10,0},// 121
  {117,2,"NIT_ST2",0,10,0},// 122
  {117,3,"NITBOTH",0,10,0},// 123
  {118,0,"SDPRES",0,10,0},// 124 - SD Card is inserted // SD Card Register, see single bits below
  {118,1,"SDTYPE",0,10,0},// 125 - SD Card type (0=SD, 1=SDHC)
  {118,2,"SDREADY",0,10,0},// 126
  {118,3,"SDLOGG",0,10,0},// 127
  {118,4,"SD_ERR",0,10,0},// 128
  {118,5,"SD_VER",0,10,0},// 129 - SD Card version (0=1.x, 1=2.x)
  {118,6,"SD_FS",0,10,0},// 130 - SD Card filesystem (0=no FAT16, 1=FAT32)
  {118,7,"SD_UNU",0,10,0},// 131 - SD Unused
  {119,0,"EMAP",2,10,0},// 132 - EMAP (kPa, 2 bytes)
  {121,0,"FAN DUTY",1,10,0},// 133 - Cooling Fan PWM duty (%)
  {122,0,"AC REQ",0,10,0},// 134 - Air Conditioning register, see single bits below
  {122,1,"AC COMP",0,10,0},// 135
  {122,2,"AC RPM",0,10,0},// 136 - AC RPM LOCK
  {122,3,"AC TPS",0,10,0},// 137 - AC TPS LOCK
  {122,4,"AC ON",0,10,0},// 138
  {122,5,"AC CLT",0,10,0},// 139 - AC CLT LOCK
  {122,6,"AC FAN",0,10,0}// 140
};
// Updated on 2023-01-11

#define SPEEDUINO_DATA_LEN 123

#define SPEEDUINO_BYTE_SECL               0   // Simple counter, incremented each second
#define SPEEDUINO_STATUS_BITFIELD         1   // Status Register, see single bits below
  #define SPEEDUINO_STATUS_INJ1_BIT         0
  #define SPEEDUINO_STATUS_INJ2_BIT         1
  #define SPEEDUINO_STATUS_INJ3_BIT         2
  #define SPEEDUINO_STATUS_INJ4_BIT         3
  #define SPEEDUINO_STATUS_DFCO_BIT         4 // Deceleration Fuel Cut Off
  #define SPEEDUINO_STATUS_BOOSTCUT_BIT     5
  #define SPEEDUINO_STATUS_TOOTH1_BIT       6 // Tooth Log 1 Ready
  #define SPEEDUINO_STATUS_TOOTH2_BIT       7 // Tooth Log 2 Ready
#define SPEEDUINO_ENGINE_BITFIELD         2   // Engine Register, see single bits below
  #define SPEEDUINO_ENGINE_RUNNING_BIT      0
  #define SPEEDUINO_ENGINE_CRANKING_BIT     1
  #define SPEEDUINO_ENGINE_ASE_BIT          2 // Afterstart Enrichment
  #define SPEEDUINO_ENGINE_WARMUP_BIT       3 // Warmup Enrichment
  #define SPEEDUINO_ENGINE_TPSAE_BIT        4 // TPS-based Acceleration Enrichment
  #define SPEEDUINO_ENGINE_TPSDE_BIT        5 // TPS-based Deceleration Enleanment
  #define SPEEDUINO_ENGINE_MAPAE_BIT        6 // MAP-based Acceleration Enrichment
  #define SPEEDUINO_ENGINE_MAPDE_BIT        7 // MAP-based Deceleration Enleanment
#define SPEEDUINO_DWELL_BYTE              3   // Dwell (ms, divide by 10)
#define SPEEDUINO_MAP_WORD                4   // MAP (kPa, 2 bytes)
#define SPEEDUINO_IAT_BYTE                6   // Intake Air Temperature (Degrees, subtract 40)
#define SPEEDUINO_CLT_BYTE                7   // Coolant Temperature (Degrees, subtract 40)
#define SPEEDUINO_VOLTAGE_BYTE            9   // Battery Voltage (Volts, divide by 10)
#define SPEEDUINO_AFR_BYTE                10  // Air-Fuel Ratio (divide by 10)
#define SPEEDUINO_EGOCORR_BYTE            11  // EGO Correction (%)
#define SPEEDUINO_IATCORR_BYTE            12  // Air Temperature Correction (%)
#define SPEEDUINO_WUECORR_BYTE            13  // Warmup Enrichment Correction (%)
#define SPEEDUINO_RPM_WORD                14  // RPM (2 bytes)
#define SPEEDUINO_AECORR_BYTE             16  // Acceleration Enrichment Correction (%)
#define SPEEDUINO_GAMMAE_BYTE             17  // Gamma Correction (%)
#define SPEEDUINO_CURRENTVE_BYTE          18  // Current VE
#define SPEEDUINO_AFRTARGET_BYTE          19  // AFR Target (divide by 10)
#define SPEEDUINO_PW1_WORD                20  // PulseWidth 1 (ms, divide by 10)
#define SPEEDUINO_TPSDOT_BYTE             22  // TPSdot (%/s, multiply by 10)
#define SPEEDUINO_ADVANCE                 23  // (Degrees BTDC)
#define SPEEDUINO_TPS_BYTE                24  // Throttle Position (%, divide by 2, 0.5% resolution)
#define SPEEDUINO_LOOPSPERSEC_WORD        25  // (Loops/s, 2 bytes)
#define SPEEDUINO_FREERAM_WORD            27  // (Bytes, 2 bytes)
#define SPEEDUINO_BOOSTTARGET             29  // (kPa, multiply by 2)
#define SPEEDUINO_BOOSTDUTY               30  // Boost PWM duty (%)
#define SPEEDUINO_SPARK_BITFIELD          31  // Spark Register, see single bits below
  #define SPEEDUINO_SPARK_HARDLAUNCH_BIT    0 // Launch Control limiter, hard
  #define SPEEDUINO_SPARK_SOFTLAUNCH_BIT    1 // Launch Control limiter, soft
  #define SPEEDUINO_SPARK_HARDLIMIT_BIT     2 // Rev limiter, hard
  #define SPEEDUINO_SPARK_SOFTLIMIT_BIT     3 // Rev limiter, soft
  #define SPEEDUINO_SPARK_BOOSTCUT_BIT      4 
  #define SPEEDUINO_SPARK_ERROR_BIT         5
  #define SPEEDUINO_SPARK_IDLECTRL_BIT      6
  #define SPEEDUINO_SPARK_SYNC_BIT          7 
#define SPEEDUINO_RPMDOT_WORD             32  // RPMdot (rpm/s, 2 bytes)
#define SPEEDUINO_ETHANOLPCT_BYTE         34  // Ethanol content (%, 0 if not used)
#define SPEEDUINO_FLEXCORR_BYTE           35  // Flex fuel Correction (%)
#define SPEEDUINO_FLEXIGNCORR_BYTE        36  // Flex fuel advance correction (Degrees)
#define SPEEDUINO_IDLELOAD_BYTE           37  // Idle valve load (PWM duty or Steps)
#define SPEEDUINO_TESTOUTPUTS_BITFIELD    38  // Test Outputs Register, see single bits below
  #define SPEEDUINO_TESTOUTPUTS_ENABLED_BIT 0
  #define SPEEDUINO_TESTOUTPUTS_ACTIVE_BIT  1
#define SPEEDUINO_02_2_BYTE               39  // Secondary AFR Sensor (divide by 10)
#define SPEEDUINO_BARO_BYTE               40  // Berometric pressure (kPa)
#define SPEEDUINO_CANIN0_WORD             41  // CAN Input 0 (2 bytes)
#define SPEEDUINO_CANIN1_WORD             43  // CAN Input 1 (2 bytes)
#define SPEEDUINO_CANIN2_WORD             45  // CAN Input 2 (2 bytes)
#define SPEEDUINO_CANIN3_WORD             47  // CAN Input 3 (2 bytes)
#define SPEEDUINO_CANIN4_WORD             49  // CAN Input 4 (2 bytes)
#define SPEEDUINO_CANIN5_WORD             51  // CAN Input 5 (2 bytes)
#define SPEEDUINO_CANIN6_WORD             53  // CAN Input 6 (2 bytes)
#define SPEEDUINO_CANIN7_WORD             55  // CAN Input 7 (2 bytes)
#define SPEEDUINO_CANIN8_WORD             57  // CAN Input 8 (2 bytes)
#define SPEEDUINO_CANIN9_WORD             59  // CAN Input 9 (2 bytes)
#define SPEEDUINO_CANIN10_WORD            61  // CAN Input 10 (2 bytes)
#define SPEEDUINO_CANIN11_WORD            63  // CAN Input 11 (2 bytes)
#define SPEEDUINO_CANIN12_WORD            65  // CAN Input 12 (2 bytes)
#define SPEEDUINO_CANIN13_WORD            67  // CAN Input 13 (2 bytes)
#define SPEEDUINO_CANIN14_WORD            69  // CAN Input 14 (2 bytes)
#define SPEEDUINO_CANIN15_WORD            71  // CAN Input 15 (2 bytes)
#define SPEEDUINO_TPSADC_BYTE             73  // TPS ADC Value (0...255)
#define SPEEDUINO_ERROR_BYTE              74  // Error code (Error Number 0:1, Current Error 2:7)
#define SPEEDUINO_LAUNCHCORR_BYTE         75  // Launch Correction (%)
#define SPEEDUINO_PW2_WORD                76  // PulseWidth 2 (ms, divide by 10)
#define SPEEDUINO_PW3_WORD                78  // PulseWidth 3 (ms, divide by 10)
#define SPEEDUINO_PW4_WORD                80  // PulseWidth 4 (ms, divide by 10)
#define SPEEDUINO_STATUS3_BITFIELD        82  // Status3 Register, see single bits below
  #define SPEEDUINO_STATUS3_RESETLOCKON_BIT 0
  #define SPEEDUINO_STATUS3_NITROUSON_BIT   1
  #define SPEEDUINO_STATUS3_FUEL2ACTIVE_BIT 2 // Secondary VE Table
  #define SPEEDUINO_STATUS3_VSSREFRESH_BIT  3
  #define SPEEDUINO_STATUS3_HALFSYNC_BIT    4
  #define SPEEDUINO_STATUS3_NSQUIRTS_BITS   6 // Bits 6:7
#define SPEEDUINO_ENGINEPROTECT_BITFIELD  83  // Engine Protect Register, see single bits below
  #define SPEEDUINO_ENGINEPROTECT_RPM_BIT   0
  #define SPEEDUINO_ENGINEPROTECT_MAP_BIT   1
  #define SPEEDUINO_ENGINEPROTECT_OIL_BIT   2
  #define SPEEDUINO_ENGINEPROTECT_AFR_BIT   3
#define SPEEDUINO_FUELLOAD_WORD           84  // Fuel Table load (kPa or TPS%, 2 bytes)
#define SPEEDUINO_IGNLOAD_WORD            86  // Spark Table load (kPa or TPS%, 2 bytes)
#define SPEEDUINO_INJANGLE_WORD           88  // Injection angle (2 bytes)
#define SPEEDUINO_IDLELOAD_BYTE           90  // DUPLICATE OF ADDRESS 37 - HOPEFULLY WILL BE FIXED IN THE SPEEDUINO FIRMWARE
#define SPEEDUINO_CLIDLETARGET_BYTE       91  // Closed-Loop Idle Target RPM (multiply by 10)
#define SPEEDUINO_MAPDOT_BYTE             92  // MAPdot (kPa/s, multiply by 10)
#define SPEEDUINO_VVT1ANGLE_BYTE          93  // VVT1 Angle (Degrees, SIGNED)
#define SPEEDUINO_VVT1TARGET_BYTE         94  // VVT1 Target Angle (Degrees)
#define SPEEDUINO_VVT1DUTY_BYTE           95  // VVT1 PWM Duty (%)
#define SPEEDUINO_FLEXBOOSTCORR_WORD      96  // Flex fuel Boost Correction, 2 bytes)
#define SPEEDUINO_BAROCORR_BYTE           98  // Barometric Correction (%)
#define SPEEDUINO_ASECORR_BYTE            99  // After Start Enrichment Correction (%)
#define SPEEDUINO_VSS_WORD                100 // Speed reading via VSS (2 bytes)
#define SPEEDUINO_GEAR_BYTE               102 // Current gear
#define SPEEDUINO_FUELPRESSURE_BYTE       103 // (PSI or Bar)
#define SPEEDUINO_OILPRESSURE_BYTE        104 // (PSI or Bar)
#define SPEEDUINO_WMIPW_BYTE              105 // Water-Meth Injection Pulsewidth
#define SPEEDUINO_STATUS4_BITFIELD        106 // Status4 Register, see single bits below
  #define SPEEDUINO_STATUS4_WMIEMPTY_BIT    0 // Water-Meth Tank empty
  #define SPEEDUINO_STATUS4_VVT1ERROR_BIT   1
  #define SPEEDUINO_STATUS4_VVT2ERROR_BIT   2
  #define SPEEDUINO_STATUS4_FAN_BIT         3 // Cooling fan
#define SPEEDUINO_VVT2ANGLE_BYTE          107 // VVT2 Angle (Degree, SIGNED)
#define SPEEDUINO_VVT2TARGET_BYTE         108 // VVT2 Target Angle (Degrees)
#define SPEEDUINO_VVT2DUTY_BYTE           109 // VVT2 PWM Duty (%)
#define SPEEDUINO_OUTPUTSSTATUS_BYTE      110 // Programmable Outputs
#define SPEEDUINO_FUELTEMP_BYTE           111 // Fuel Temperature from Flex Sensor (Degrees, subtract 40)
#define SPEEDUINO_FUELTEMPCORR_BYTE       112 // Fuel Temperature Correction (%)
#define SPEEDUINO_VE1_BYTE                113 // VE from Primary Fuel Table
#define SPEEDUINO_VE2_BYTE                114 // VE from Secondary Fuel Table
#define SPEEDUINO_ADVANCE1_BYTE           115 // Advance from Primary Spark Table (Degrees BTDC)
#define SPEEDUINO_ADVANCE2_BYTE           116 // Advance from Secondary Spark Table (Degrees BTDC)
#define SPEEDUINO_NITROUSSTATUS_BYTE      117 // N2O status, see values below
  #define SPEEDUINO_NITROUSSTATUS_OFF       0
  #define SPEEDUINO_NITROUSSTATUS_STAGE1    1
  #define SPEEDUINO_NITROUSSTATUS_STAGE2    2
  #define SPEEDUINO_NITROUSSTATUS_BOTH      3
#define SPEEDUINO_SDCCARD_BITFIELD        118 // SD Card Register, see single bits below
  #define SPEEDUINO_SDCARD_PRESENT_BIT      0 // SD Card is inserted
  #define SPEEDUINO_SDCARD_TYPE_BIT         1 // SD Card type (0=SD, 1=SDHC)
  #define SPEEDUINO_SDCARD_READY_BIT        2
  #define SPEEDUINO_SDCARD_LOGGING_BIT      3
  #define SPEEDUINO_SDCARD_ERROR            4
  #define SPEEDUINO_SDCARD_VERSION_BIT      5 // SD Card version (0=1.x, 1=2.x)
  #define SPEEDUINO_SDCARD_FS_BIT           6 // SD Card filesystem (0=no FAT16, 1=FAT32)
  #define SPEEDUINO_SDCARD_UNUSED           7
#define SPEEDUINO_EMAP_WORD               119 // EMAP (kPa, 2 bytes)
#define SPEEDUINO_FANDUTY_BYTE            121 // Cooling Fan PWM duty (%)
#define SPEEDUINO_AIRCON_BITFIELD         122 // Air Conditioning register, see single bits below
  #define SPEEDUINO_AIRCON_REQUEST_BIT      0
  #define SPEEDUINO_AIRCON_COMPRESSOR_BIT   1
  #define SPEEDUINO_AIRCON_RPMLOCKOUT_BIT   2
  #define SPEEDUINO_AIRCON_TPSLOCKOUT_BIT   3
  #define SPEEDUINO_AIRCON_TURNINGON_BIT    4
  #define SPEEDUINO_AIRCON_CLTLOCKOUT_BIT   5
  #define SPEEDUINO_AIRCON_FAN_BIT          6
