# mmem
Based on the documentation from Marcus Comstedt's [Dreamcast Programming page](http://mc.pp.se/dc/vms/flashmem.html). 

Currently can only read the memory card's blocks and print some of the data. 

**Still a lot of work left to do.**

## Example output:
```
$ ./mmem img/mem3.BIN
[255] = Root Block
 custom color: Y
 color: rgba(31, 31, 255, 24)
 created: 2004-01-29 17:07
 FAT block: 254
 directory blocks: 253-241
 icon shape: 34
 user blocks: 200
[254] = FAT Block
 [000]=0001  [001]=0002  [002]=0003  [003]=0004  [004]=0005  [005]=0006  [006]=LAST  [007]=____
 [008]=____  [009]=____  [010]=____  [011]=____  [012]=LAST  [013]=0012  [014]=0013  [015]=0014
 [016]=0015  [017]=0016  [018]=0017  [019]=0018  [020]=0019  [021]=LAST  [022]=0021  [023]=0022
 [024]=0023  [025]=0024  [026]=0025  [027]=0026  [028]=0027  [029]=0028  [030]=0029  [031]=LAST
 [032]=0031  [033]=0032  [034]=LAST  [035]=0034  [036]=0035  [037]=LAST  [038]=0037  [039]=0038
 [040]=LAST  [041]=0040  [042]=LAST  [043]=0042  [044]=0043  [045]=LAST  [046]=0045  [047]=0046
 [048]=0047  [049]=0048  [050]=0030  [051]=LAST  [052]=0051  [053]=0052  [054]=0053  [055]=LAST
 [056]=0055  [057]=0056  [058]=0057  [059]=0058  [060]=0059  [061]=0060  [062]=0061  [063]=0062
 [064]=0063  [065]=LAST  [066]=0065  [067]=LAST  [068]=0067  [069]=0068  [070]=LAST  [071]=0070
 [072]=0050  [073]=0072  [074]=0073  [075]=0074  [076]=0075  [077]=0076  [078]=0077  [079]=0078
 [080]=LAST  [081]=0080  [082]=0081  [083]=LAST  [084]=0083  [085]=0084  [086]=0085  [087]=0086
 [088]=0087  [089]=LAST  [090]=0089  [091]=0090  [092]=0091  [093]=0092  [094]=0093  [095]=0094
 [096]=0095  [097]=0096  [098]=0097  [099]=0098  [100]=0099  [101]=0100  [102]=0101  [103]=0102
 [104]=0103  [105]=0104  [106]=0105  [107]=LAST  [108]=0107  [109]=0108  [110]=0109  [111]=0110
 [112]=0111  [113]=0112  [114]=0113  [115]=0114  [116]=0115  [117]=0116  [118]=0117  [119]=0118
 [120]=0119  [121]=0120  [122]=0121  [123]=0122  [124]=0123  [125]=LAST  [126]=0125  [127]=0126
 [128]=0127  [129]=0128  [130]=0129  [131]=0130  [132]=0131  [133]=0132  [134]=0133  [135]=0134
 [136]=0135  [137]=0136  [138]=0137  [139]=0138  [140]=0139  [141]=LAST  [142]=0141  [143]=0142
 [144]=0143  [145]=LAST  [146]=0145  [147]=0146  [148]=0147  [149]=0148  [150]=0149  [151]=0150
 [152]=0151  [153]=0152  [154]=0153  [155]=0154  [156]=0155  [157]=0156  [158]=0157  [159]=0158
 [160]=0079  [161]=0160  [162]=0161  [163]=0162  [164]=0163  [165]=0164  [166]=0165  [167]=0166
 [168]=LAST  [169]=0168  [170]=LAST  [171]=0170  [172]=0171  [173]=0172  [174]=0173  [175]=0174
 [176]=0175  [177]=0176  [178]=0177  [179]=0178  [180]=0179  [181]=0180  [182]=0181  [183]=0182
 [184]=0183  [185]=0184  [186]=0185  [187]=0186  [188]=0187  [189]=0188  [190]=LAST  [191]=0190
 [192]=LAST  [193]=0192  [194]=0193  [195]=0194  [196]=0195  [197]=0196  [198]=0197  [199]=0198
 [200]=____  [201]=____  [202]=____  [203]=____  [204]=____  [205]=____  [206]=____  [207]=____
 [208]=____  [209]=____  [210]=____  [211]=____  [212]=____  [213]=____  [214]=____  [215]=____
 [216]=____  [217]=____  [218]=____  [219]=____  [220]=____  [221]=____  [222]=____  [223]=____
 [224]=____  [225]=____  [226]=____  [227]=____  [228]=____  [229]=____  [230]=____  [231]=____
 [232]=____  [233]=____  [234]=____  [235]=____  [236]=____  [237]=____  [238]=____  [239]=____
 [240]=____  [241]=LAST  [242]=0241  [243]=0242  [244]=0243  [245]=0244  [246]=0245  [247]=0246
 [248]=0247  [249]=0248  [250]=0249  [251]=0250  [252]=0251  [253]=0252  [254]=LAST  [255]=0250
[253] = FAT Directory Block
[253]
#000 FILE:
 title: HYDRO THUNDER
 app: HYDRO THUNDER
 desc: SAVE GAME
 name: HYDRO___SYS
 crc: 0x2cb5
 icon(s): 1
 animation speed: 1
 size: 207 bytes
 first block: #199
 created: 2704-02-29 11:05
 offset: 000
#001 FILE:
 title: OOGA BOOGA GAME DATA
 app: OOGA BOOGA
 desc: OOGA BOOGA DATA
 name: OOGA___BOOGA
 crc: 0x926a
 icon(s): 1
 animation speed: 59
 size: 193 bytes
 first block: #191
 created: 2604-02-29 11:05
 offset: 000
#002 FILE:
 title: PSOV2/MAIN_DATA
 app:
 desc: PSOV2/MAIN_DATA
 name: PSO______SYS
 crc: 0xdeb8
 icon(s): 1
 animation speed: 1
 size: 174 bytes
 first block: #159
 created: 2404-02-29 11:05
 offset: 000
#003 FILE:
 title: READY 2 RUMBLE BOXING: ROUND 2
 app: READY 2 RUMBLE 2
 desc: SAVE GAME
 name: RUMBLER2.SYS
 crc: 0x4c95
 icon(s): 2
 animation speed: 25
 size: 148 bytes
 first block: #144
 created: 2304-02-29 11:05
 offset: 000
#004 FILE:
 title: SOLDIER OF FORTUNE
 app: SOLDIEROFFORTUNE
 desc: TRAIN
 name: SFORTUNE.000
 crc: 0xe65e
 icon(s): 1
 animation speed: 0
 size: 156 bytes
 first block: #140
 created: 2204-02-29 11:05
 offset: 000
#005 FILE:
 title: SNOCROSS CHAMPIONSHIP RACING
 app: SNOCROSS
 desc: GAME SETTINGS
 name: SNOCROSS_CFG
 crc: 0x0000
 icon(s): 1
 animation speed: 10
 size: 142 bytes
 first block: #124
 created: 2104-02-29 11:05
 offset: 000
#006 FILE:
 title: SONIC ADVENTURE 2 / Main File
 app:
 desc: MAIN FILE
 name: SONIC2___S01
 crc: 0x017d
 icon(s): 1
 animation speed: 0
 size: 124 bytes
 first block: #106
 created: 2004-02-29 11:05
 offset: 000
#007 FILE:
 title: TD-VRally Preferences
 app:
 desc: TDVR CONFIG
 name: TDVRALLY_GAM
 crc: 0x6377
 icon(s): 1
 animation speed: 1
 size: 94 bytes
 first block: #88
 created: 2504-02-29 11:05
 offset: 000
#008 FILE:
 title: TEST (ITALY)
 app:
 desc: VR2 TRACK
 name: TDVRALLY_T00
 crc: 0xe470
 icon(s): 1
 animation speed: 1
 size: 85 bytes
 first block: #82
 created: 2404-02-29 11:05
 offset: 000
#009 FILE:
 title: BANGAI-O
 app:
 desc: AUTO SAVE DATA
 name: BANGAIO_DAT
 crc: 0xda7a
 icon(s): 3
 animation speed: 6
 size: 54 bytes
 first block: #49
 created: 2204-02-29 11:05
 offset: 000
#010 FILE:
 title: CONFIDENTIAL MISSION
 app:
 desc: CM_MAIN_DATA
 name: C_MISSION_DC
 crc: 0x0bf0
 icon(s): 1
 animation speed: 0
 size: 47 bytes
 first block: #44
 created: 2104-02-29 11:06
 offset: 000
#011 FILE:
 title: (c)Interact - www.gameshark.com
 app:
 desc:  GameShark Code
 name: CDX_CODES_00
 crc: 0x0000
 icon(s): 1
 animation speed: 0
 size: 43 bytes
 first block: #41
 created: 2004-02-29 11:06
 offset: 000
#012 FILE:
 title: (c)Interact - www.gameshark.com
 app:
 desc:  GameShark Code
 name: CDX_SETTINGS
 crc: 0x0000
 icon(s): 1
 animation speed: 0
 size: 42 bytes
 first block: #39
 created: 1904-02-29 11:06
 offset: 000
#013 FILE:
 title: CHUCHU!/PZ EDIT
 app:
 desc: CHU_CHU__E01
 name: CHU_CHU__E01
 crc: 0x2099
 icon(s): 1
 animation speed: 1
 size: 39 bytes
 first block: #36
 created: 1804-02-29 11:06
 offset: 000
#014 FILE:
 title: CHUCHU!/PZ EDIT
 app:
 desc: CHU_CHU__E02
 name: CHU_CHU__E02
 crc: 0x6b32
 icon(s): 1
 animation speed: 1
 size: 36 bytes
 first block: #33
 created: 1704-02-29 11:06
 offset: 000
#015 FILE:
 title: CHUCHU!/MAIN
 app:
 desc: CHU_CHU__RCT
 name: CHU_CHU__RCT
 crc: 0x9ef3
 icon(s): 1
 animation speed: 1
 size: 72 bytes
 first block: #69
 created: 1998-11-27 00:01
 offset: 000
[252]
#016 FILE:
 title: HIDDEN FILE
 desc: VMU Tool icon
 name: ICONDATA_VMS
 crc: 0x63bb
 icon(s): 57277
 animation speed: 32635
 size: 73 bytes
 first block: #71
 created: 1998-11-27 02:50
 offset: 000
#017 FILE:
 title: PROJECT JUSTICE
 app:
 desc: SYSTEM FILE
 name: PJUSTICE_SYS
 crc: 0x49ee
 icon(s): 1
 animation speed: 1
 size: 68 bytes
 first block: #66
 created: 1998-11-27 00:01
 offset: 000
#018 FILE:
 title: SONIC ADVENTURE / Main Save File
 app: 0000000000000000
 desc: MAIN_SAVE_FILE
 name: SONICADV_INT
 crc: 0x0d4a
 icon(s): 2
 animation speed: 20
 size: 74 bytes
 first block: #64
 created: 1998-11-27 00:10
 offset: 000
#019 FILE:
 title: ????
 app:
 desc: ????
 name: PUYOFEVERSYS
 crc: 0xfd47
 icon(s): 1
 animation speed: 1
 size: 58 bytes
 first block: #54
 created: 1998-11-27 00:00
 offset: 000
#020 FILE:
 title: Draconus: Cult of the Wyrm
 app: DRACONUS
 desc: treetop
 name: DRACONUS.001
 crc: 0x6fff
 icon(s): 1
 animation speed: 1
 size: 209 bytes
 first block: #189
 created: 1504-02-29 11:06
 offset: 000
#021 FILE:
 title: EXPENDABLE
 app:  ;S!7L
 desc: LEVEL 03
 name: EXPENDAB.001
 crc: 0x6a3a
 icon(s): 1
 animation speed: 1
 size: 171 bytes
 first block: #169
 created: 1404-02-29 11:06
 offset: 000
#022 GAME:
 title: VMU Script
 app:
 desc: Logic
 name: LOGIC
 crc: 0x0000
 icon(s): 1
 animation speed: 10
 size: 7 bytes
 first block: #0
 created: 2004-02-12 23:14
 offset: 001
#023 FILE:
 title: Arcadia                   Soltis
 app:
 desc: Arcadia Savefile
 name: ARCADIA_E001
 crc: 0x47c7
 icon(s): 1
 animation speed: 8
 size: 194 bytes
 first block: #167
 created: 2006-06-04 11:14
 offset: 000
#024 FILE:
 title: GRANDIA2 NEW VALMAR, ROOM OF CHA
 app:
 desc: GRANDIA2
 name: GRANDIA2_003
 crc: 0xe457
 icon(s): 1
 animation speed: 1
 size: 29 bytes
 first block: #20
 created: 2003-10-24 01:23
 offset: 000
[251]
[250]
[249]
[248]
[247]
[246]
[245]
[244]
[243]
[242]
[241]
```
