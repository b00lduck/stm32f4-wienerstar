#include "hamming.h"

float hamming1024[1024] = { 0.0800005834, 0.080013727, 0.0800441712, 0.080091915, 0.0801569565, 0.0802392933,
		0.0803389222, 0.0804558396, 0.0805900411, 0.0807415215, 0.0809102752, 0.0810962959, 0.0812995765, 0.0815201095,
		0.0817578864, 0.0820128984, 0.0822851359, 0.0825745886, 0.0828812457, 0.0832050956, 0.0835461262, 0.0839043245,
		0.0842796772, 0.0846721701, 0.0850817885, 0.0855085169, 0.0859523393, 0.086413239, 0.0868911987, 0.0873862004,
		0.0878982255, 0.0884272547, 0.0889732681, 0.0895362452, 0.0901161649, 0.0907130052, 0.0913267438, 0.0919573575,
		0.0926048228, 0.0932691151, 0.0939502095, 0.0946480804, 0.0953627016, 0.0960940462, 0.0968420866, 0.0976067948,
		0.0983881419, 0.0991860986, 0.1000006349, 0.1008317201, 0.101679323, 0.1025434117, 0.1034239537, 0.1043209159,
		0.1052342645, 0.1061639652, 0.107109983, 0.1080722824, 0.1090508271, 0.1100455804, 0.1110565048, 0.1120835623,
		0.1131267143, 0.1141859215, 0.1152611442, 0.1163523418, 0.1174594734, 0.1185824972, 0.1197213711, 0.1208760522,
		0.1220464971, 0.1232326617, 0.1244345015, 0.1256519712, 0.1268850251, 0.1281336168, 0.1293976993, 0.1306772251,
		0.131972146, 0.1332824133, 0.1346079779, 0.1359487897, 0.1373047984, 0.138675953, 0.1400622019, 0.141463493,
		0.1428797735, 0.1443109903, 0.1457570894, 0.1472180165, 0.1486937166, 0.1501841342, 0.1516892133, 0.1532088973,
		0.1547431289, 0.1562918506, 0.157855004, 0.1594325304, 0.1610243704, 0.1626304642, 0.1642507513, 0.1658851708,
		0.1675336613, 0.1691961607, 0.1708726065, 0.1725629356, 0.1742670845, 0.1759849891, 0.1777165848, 0.1794618064,
		0.1812205883, 0.1829928643, 0.1847785679, 0.1865776318, 0.1883899883, 0.1902155693, 0.1920543062, 0.1939061298,
		0.1957709704, 0.1976487578, 0.1995394216, 0.2014428905, 0.2033590929, 0.2052879569, 0.2072294098, 0.2091833786,
		0.2111497899, 0.2131285696, 0.2151196434, 0.2171229363, 0.2191383731, 0.2211658779, 0.2232053744, 0.225256786,
		0.2273200355, 0.2293950452, 0.2314817372, 0.233580033, 0.2356898536, 0.2378111197, 0.2399437516, 0.2420876689,
		0.2442427911, 0.246409037, 0.2485863254, 0.2507745741, 0.252973701, 0.2551836233, 0.2574042579, 0.2596355213,
		0.2618773296, 0.2641295984, 0.2663922431, 0.2686651784, 0.2709483191, 0.2732415791, 0.2755448722, 0.2778581118,
		0.280181211, 0.2825140822, 0.2848566378, 0.2872087897, 0.2895704493, 0.291941528, 0.2943219364, 0.2967115851,
		0.2991103842, 0.3015182435, 0.3039350723, 0.3063607799, 0.3087952749, 0.3112384658, 0.3136902608, 0.3161505675,
		0.3186192935, 0.3210963459, 0.3235816316, 0.326075057, 0.3285765284, 0.3310859517, 0.3336032326, 0.3361282762,
		0.3386609878, 0.341201272, 0.3437490332, 0.3463041757, 0.3488666034, 0.3514362198, 0.3540129283, 0.3565966321,
		0.3591872339, 0.3617846362, 0.3643887415, 0.3669994518, 0.3696166688, 0.3722402941, 0.3748702291, 0.3775063749,
		0.3801486322, 0.3827969018, 0.385451084, 0.3881110789, 0.3907767866, 0.3934481067, 0.3961249389, 0.3988071824,
		0.4014947363, 0.4041874996, 0.4068853709, 0.4095882489, 0.4122960318, 0.4150086179, 0.417725905, 0.420447791,
		0.4231741735, 0.42590495, 0.4286400177, 0.4313792739, 0.4341226154, 0.436869939, 0.4396211416, 0.4423761195,
		0.4451347691, 0.4478969868, 0.4506626686, 0.4534317104, 0.4562040083, 0.4589794577, 0.4617579545, 0.4645393941,
		0.4673236718, 0.470110683, 0.4729003228, 0.4756924863, 0.4784870685, 0.4812839642, 0.4840830684, 0.4868842756,
		0.4896874807, 0.492492578, 0.4952994621, 0.4981080275, 0.5009181685, 0.5037297794, 0.5065427545, 0.509356988,
		0.512172374, 0.5149888066, 0.5178061799, 0.5206243879, 0.5234433247, 0.5262628842, 0.5290829604, 0.5319034472,
		0.5347242385, 0.5375452282, 0.5403663103, 0.5431873786, 0.546008327, 0.5488290494, 0.5516494397, 0.5544693919,
		0.5572887999, 0.5601075577, 0.5629255591, 0.5657426983, 0.5685588693, 0.5713739662, 0.574187883, 0.577000514,
		0.5798117533, 0.5826214953, 0.5854296342, 0.5882360645, 0.5910406806, 0.5938433769, 0.5966440481, 0.5994425889,
		0.6022388939, 0.6050328581, 0.6078243763, 0.6106133436, 0.6133996549, 0.6161832057, 0.6189638911, 0.6217416065,
		0.6245162476, 0.6272877098, 0.6300558891, 0.6328206813, 0.6355819823, 0.6383396884, 0.6410936959, 0.643843901,
		0.6465902005, 0.6493324909, 0.6520706693, 0.6548046325, 0.6575342777, 0.6602595024, 0.6629802039, 0.66569628,
		0.6684076285, 0.6711141474, 0.673815735, 0.6765122896, 0.6792037097, 0.6818898943, 0.6845707422, 0.6872461525,
		0.6899160248, 0.6925802586, 0.6952387536, 0.6978914099, 0.7005381276, 0.7031788074, 0.7058133498, 0.7084416557,
		0.7110636263, 0.713679163, 0.7162881674, 0.7188905414, 0.7214861871, 0.7240750069, 0.7266569033, 0.7292317794,
		0.7317995382, 0.7343600832, 0.7369133181, 0.7394591468, 0.7419974736, 0.744528203, 0.7470512398, 0.7495664892,
		0.7520738565, 0.7545732474, 0.757064568, 0.7595477245, 0.7620226236, 0.764489172, 0.7669472772, 0.7693968466,
		0.7718377881, 0.7742700098, 0.7766934204, 0.7791079286, 0.7815134437, 0.7839098751, 0.7862971328, 0.788675127,
		0.7910437681, 0.7934029672, 0.7957526355, 0.7980926846, 0.8004230264, 0.8027435735, 0.8050542384, 0.8073549343,
		0.8096455747, 0.8119260733, 0.8141963445, 0.8164563027, 0.8187058631, 0.8209449411, 0.8231734523, 0.825391313,
		0.8275984398, 0.8297947496, 0.8319801599, 0.8341545884, 0.8363179535, 0.8384701736, 0.8406111679, 0.8427408558,
		0.8448591572, 0.8469659925, 0.8490612824, 0.8511449482, 0.8532169113, 0.855277094, 0.8573254187, 0.8593618084,
		0.8613861865, 0.8633984769, 0.8653986038, 0.8673864921, 0.8693620669, 0.8713252541, 0.8732759796, 0.8752141703,
		0.8771397531, 0.8790526556, 0.8809528059, 0.8828401326, 0.8847145646, 0.8865760315, 0.8884244632, 0.8902597902,
		0.8920819435, 0.8938908546, 0.8956864553, 0.8974686783, 0.8992374564, 0.9009927231, 0.9027344124, 0.9044624588,
		0.9061767973, 0.9078773635, 0.9095640933, 0.9112369233, 0.9128957907, 0.9145406329, 0.9161713882, 0.9177879953,
		0.9193903932, 0.9209785218, 0.9225523213, 0.9241117325, 0.9256566968, 0.927187156, 0.9287030527, 0.9302043297,
		0.9316909307, 0.9331627996, 0.9346198812, 0.9360621207, 0.9374894637, 0.9389018567, 0.9402992464, 0.9416815804,
		0.9430488067, 0.9444008737, 0.9457377308, 0.9470593275, 0.9483656142, 0.9496565417, 0.9509320616, 0.9521921257,
		0.9534366868, 0.9546656981, 0.9558791132, 0.9570768866, 0.9582589731, 0.9594253285, 0.9605759087, 0.9617106705,
		0.9628295712, 0.9639325688, 0.9650196218, 0.9660906892, 0.9671457308, 0.9681847069, 0.9692075784, 0.9702143069,
		0.9712048545, 0.9721791839, 0.9731372585, 0.9740790423, 0.9750044998, 0.9759135962, 0.9768062974, 0.9776825697,
		0.9785423803, 0.9793856967, 0.9802124872, 0.9810227208, 0.981816367, 0.9825933958, 0.9833537782, 0.9840974855,
		0.9848244897, 0.9855347636, 0.9862282802, 0.9869050137, 0.9875649385, 0.9882080298, 0.9888342634, 0.9894436158,
		0.9900360641, 0.9906115859, 0.9911701597, 0.9917117644, 0.9922363796, 0.9927439856, 0.9932345634, 0.9937080944,
		0.9941645609, 0.9946039457, 0.9950262323, 0.9954314047, 0.9958194478, 0.9961903469, 0.9965440882, 0.9968806582,
		0.9972000444, 0.9975022347, 0.9977872177, 0.9980549828, 0.9983055199, 0.9985388195, 0.9987548729, 0.9989536719,
		0.9991352092, 0.9992994777, 0.9994464714, 0.9995761847, 0.9996886128, 0.9997837515, 0.999861597, 0.9999221466,
		0.999965398, 0.9999913494, 1, 0.9999913494, 0.999965398, 0.9999221466, 0.999861597, 0.9997837515, 0.9996886128,
		0.9995761847, 0.9994464714, 0.9992994777, 0.9991352092, 0.9989536719, 0.9987548729, 0.9985388195, 0.9983055199,
		0.9980549828, 0.9977872177, 0.9975022347, 0.9972000444, 0.9968806582, 0.9965440882, 0.9961903469, 0.9958194478,
		0.9954314047, 0.9950262323, 0.9946039457, 0.9941645609, 0.9937080944, 0.9932345634, 0.9927439856, 0.9922363796,
		0.9917117644, 0.9911701597, 0.9906115859, 0.9900360641, 0.9894436158, 0.9888342634, 0.9882080298, 0.9875649385,
		0.9869050137, 0.9862282802, 0.9855347636, 0.9848244897, 0.9840974855, 0.9833537782, 0.9825933958, 0.981816367,
		0.9810227208, 0.9802124872, 0.9793856967, 0.9785423803, 0.9776825697, 0.9768062974, 0.9759135962, 0.9750044998,
		0.9740790423, 0.9731372585, 0.9721791839, 0.9712048545, 0.9702143069, 0.9692075784, 0.9681847069, 0.9671457308,
		0.9660906892, 0.9650196218, 0.9639325688, 0.9628295712, 0.9617106705, 0.9605759087, 0.9594253285, 0.9582589731,
		0.9570768866, 0.9558791132, 0.9546656981, 0.9534366868, 0.9521921257, 0.9509320616, 0.9496565417, 0.9483656142,
		0.9470593275, 0.9457377308, 0.9444008737, 0.9430488067, 0.9416815804, 0.9402992464, 0.9389018567, 0.9374894637,
		0.9360621207, 0.9346198812, 0.9331627996, 0.9316909307, 0.9302043297, 0.9287030527, 0.927187156, 0.9256566968,
		0.9241117325, 0.9225523213, 0.9209785218, 0.9193903932, 0.9177879953, 0.9161713882, 0.9145406329, 0.9128957907,
		0.9112369233, 0.9095640933, 0.9078773635, 0.9061767973, 0.9044624588, 0.9027344124, 0.9009927231, 0.8992374564,
		0.8974686783, 0.8956864553, 0.8938908546, 0.8920819435, 0.8902597902, 0.8884244632, 0.8865760315, 0.8847145646,
		0.8828401326, 0.8809528059, 0.8790526556, 0.8771397531, 0.8752141703, 0.8732759796, 0.8713252541, 0.8693620669,
		0.8673864921, 0.8653986038, 0.8633984769, 0.8613861865, 0.8593618084, 0.8573254187, 0.855277094, 0.8532169113,
		0.8511449482, 0.8490612824, 0.8469659925, 0.8448591572, 0.8427408558, 0.8406111679, 0.8384701736, 0.8363179535,
		0.8341545884, 0.8319801599, 0.8297947496, 0.8275984398, 0.825391313, 0.8231734523, 0.8209449411, 0.8187058631,
		0.8164563027, 0.8141963445, 0.8119260733, 0.8096455747, 0.8073549343, 0.8050542384, 0.8027435735, 0.8004230264,
		0.7980926846, 0.7957526355, 0.7934029672, 0.7910437681, 0.788675127, 0.7862971328, 0.7839098751, 0.7815134437,
		0.7791079286, 0.7766934204, 0.7742700098, 0.7718377881, 0.7693968466, 0.7669472772, 0.764489172, 0.7620226236,
		0.7595477245, 0.757064568, 0.7545732474, 0.7520738565, 0.7495664892, 0.7470512398, 0.744528203, 0.7419974736,
		0.7394591468, 0.7369133181, 0.7343600832, 0.7317995382, 0.7292317794, 0.7266569033, 0.7240750069, 0.7214861871,
		0.7188905414, 0.7162881674, 0.713679163, 0.7110636263, 0.7084416557, 0.7058133498, 0.7031788074, 0.7005381276,
		0.6978914099, 0.6952387536, 0.6925802586, 0.6899160248, 0.6872461525, 0.6845707422, 0.6818898943, 0.6792037097,
		0.6765122896, 0.673815735, 0.6711141474, 0.6684076285, 0.66569628, 0.6629802039, 0.6602595024, 0.6575342777,
		0.6548046325, 0.6520706693, 0.6493324909, 0.6465902005, 0.643843901, 0.6410936959, 0.6383396884, 0.6355819823,
		0.6328206813, 0.6300558891, 0.6272877098, 0.6245162476, 0.6217416065, 0.6189638911, 0.6161832057, 0.6133996549,
		0.6106133436, 0.6078243763, 0.6050328581, 0.6022388939, 0.5994425889, 0.5966440481, 0.5938433769, 0.5910406806,
		0.5882360645, 0.5854296342, 0.5826214953, 0.5798117533, 0.577000514, 0.574187883, 0.5713739662, 0.5685588693,
		0.5657426983, 0.5629255591, 0.5601075577, 0.5572887999, 0.5544693919, 0.5516494397, 0.5488290494, 0.546008327,
		0.5431873786, 0.5403663103, 0.5375452282, 0.5347242385, 0.5319034472, 0.5290829604, 0.5262628842, 0.5234433247,
		0.5206243879, 0.5178061799, 0.5149888066, 0.512172374, 0.509356988, 0.5065427545, 0.5037297794, 0.5009181685,
		0.4981080275, 0.4952994621, 0.492492578, 0.4896874807, 0.4868842756, 0.4840830684, 0.4812839642, 0.4784870685,
		0.4756924863, 0.4729003228, 0.470110683, 0.4673236718, 0.4645393941, 0.4617579545, 0.4589794577, 0.4562040083,
		0.4534317104, 0.4506626686, 0.4478969868, 0.4451347691, 0.4423761195, 0.4396211416, 0.436869939, 0.4341226154,
		0.4313792739, 0.4286400177, 0.42590495, 0.4231741735, 0.420447791, 0.417725905, 0.4150086179, 0.4122960318,
		0.4095882489, 0.4068853709, 0.4041874996, 0.4014947363, 0.3988071824, 0.3961249389, 0.3934481067, 0.3907767866,
		0.3881110789, 0.385451084, 0.3827969018, 0.3801486322, 0.3775063749, 0.3748702291, 0.3722402941, 0.3696166688,
		0.3669994518, 0.3643887415, 0.3617846362, 0.3591872339, 0.3565966321, 0.3540129283, 0.3514362198, 0.3488666034,
		0.3463041757, 0.3437490332, 0.341201272, 0.3386609878, 0.3361282762, 0.3336032326, 0.3310859517, 0.3285765284,
		0.326075057, 0.3235816316, 0.3210963459, 0.3186192935, 0.3161505675, 0.3136902608, 0.3112384658, 0.3087952749,
		0.3063607799, 0.3039350723, 0.3015182435, 0.2991103842, 0.2967115851, 0.2943219364, 0.291941528, 0.2895704493,
		0.2872087897, 0.2848566378, 0.2825140822, 0.280181211, 0.2778581118, 0.2755448722, 0.2732415791, 0.2709483191,
		0.2686651784, 0.2663922431, 0.2641295984, 0.2618773296, 0.2596355213, 0.2574042579, 0.2551836233, 0.252973701,
		0.2507745741, 0.2485863254, 0.246409037, 0.2442427911, 0.2420876689, 0.2399437516, 0.2378111197, 0.2356898536,
		0.233580033, 0.2314817372, 0.2293950452, 0.2273200355, 0.225256786, 0.2232053744, 0.2211658779, 0.2191383731,
		0.2171229363, 0.2151196434, 0.2131285696, 0.2111497899, 0.2091833786, 0.2072294098, 0.2052879569, 0.2033590929,
		0.2014428905, 0.1995394216, 0.1976487578, 0.1957709704, 0.1939061298, 0.1920543062, 0.1902155693, 0.1883899883,
		0.1865776318, 0.1847785679, 0.1829928643, 0.1812205883, 0.1794618064, 0.1777165848, 0.1759849891, 0.1742670845,
		0.1725629356, 0.1708726065, 0.1691961607, 0.1675336613, 0.1658851708, 0.1642507513, 0.1626304642, 0.1610243704,
		0.1594325304, 0.157855004, 0.1562918506, 0.1547431289, 0.1532088973, 0.1516892133, 0.1501841342, 0.1486937166,
		0.1472180165, 0.1457570894, 0.1443109903, 0.1428797735, 0.141463493, 0.1400622019, 0.138675953, 0.1373047984,
		0.1359487897, 0.1346079779, 0.1332824133, 0.131972146, 0.1306772251, 0.1293976993, 0.1281336168, 0.1268850251,
		0.1256519712, 0.1244345015, 0.1232326617, 0.1220464971, 0.1208760522, 0.1197213711, 0.1185824972, 0.1174594734,
		0.1163523418, 0.1152611442, 0.1141859215, 0.1131267143, 0.1120835623, 0.1110565048, 0.1100455804, 0.1090508271,
		0.1080722824, 0.107109983, 0.1061639652, 0.1052342645, 0.1043209159, 0.1034239537, 0.1025434117, 0.101679323,
		0.1008317201, 0.1000006349, 0.0991860986, 0.0983881419, 0.0976067948, 0.0968420866, 0.0960940462, 0.0953627016,
		0.0946480804, 0.0939502095, 0.0932691151, 0.0926048228, 0.0919573575, 0.0913267438, 0.0907130052, 0.0901161649,
		0.0895362452, 0.0889732681, 0.0884272547, 0.0878982255, 0.0873862004, 0.0868911987, 0.086413239, 0.0859523393,
		0.0855085169, 0.0850817885, 0.0846721701, 0.0842796772, 0.0839043245, 0.0835461262, 0.0832050956, 0.0828812457,
		0.0825745886, 0.0822851359, 0.0820128984, 0.0817578864, 0.0815201095, 0.0812995765, 0.0810962959, 0.0809102752,
		0.0807415215, 0.0805900411, 0.0804558396, 0.0803389222, 0.0802392933, 0.0801569565, 0.080091915, 0.0800441712,
		0.080013727 };

float hamming512[512] = { 0.0800005834, 0.0800441712, 0.0801569565, 0.0803389222, 0.0805900411, 0.0809102752,
		0.0812995765, 0.0817578864, 0.0822851359, 0.0828812457, 0.0835461262, 0.0842796772, 0.0850817885, 0.0859523393,
		0.0868911987, 0.0878982255, 0.0889732681, 0.0901161649, 0.0913267438, 0.0926048228, 0.0939502095, 0.0953627016,
		0.0968420866, 0.0983881419, 0.1000006349, 0.101679323, 0.1034239537, 0.1052342645, 0.107109983, 0.1090508271,
		0.1110565048, 0.1131267143, 0.1152611442, 0.1174594734, 0.1197213711, 0.1220464971, 0.1244345015, 0.1268850251,
		0.1293976993, 0.131972146, 0.1346079779, 0.1373047984, 0.1400622019, 0.1428797735, 0.1457570894, 0.1486937166,
		0.1516892133, 0.1547431289, 0.157855004, 0.1610243704, 0.1642507513, 0.1675336613, 0.1708726065, 0.1742670845,
		0.1777165848, 0.1812205883, 0.1847785679, 0.1883899883, 0.1920543062, 0.1957709704, 0.1995394216, 0.2033590929,
		0.2072294098, 0.2111497899, 0.2151196434, 0.2191383731, 0.2232053744, 0.2273200355, 0.2314817372, 0.2356898536,
		0.2399437516, 0.2442427911, 0.2485863254, 0.252973701, 0.2574042579, 0.2618773296, 0.2663922431, 0.2709483191,
		0.2755448722, 0.280181211, 0.2848566378, 0.2895704493, 0.2943219364, 0.2991103842, 0.3039350723, 0.3087952749,
		0.3136902608, 0.3186192935, 0.3235816316, 0.3285765284, 0.3336032326, 0.3386609878, 0.3437490332, 0.3488666034,
		0.3540129283, 0.3591872339, 0.3643887415, 0.3696166688, 0.3748702291, 0.3801486322, 0.385451084, 0.3907767866,
		0.3961249389, 0.4014947363, 0.4068853709, 0.4122960318, 0.417725905, 0.4231741735, 0.4286400177, 0.4341226154,
		0.4396211416, 0.4451347691, 0.4506626686, 0.4562040083, 0.4617579545, 0.4673236718, 0.4729003228, 0.4784870685,
		0.4840830684, 0.4896874807, 0.4952994621, 0.5009181685, 0.5065427545, 0.512172374, 0.5178061799, 0.5234433247,
		0.5290829604, 0.5347242385, 0.5403663103, 0.546008327, 0.5516494397, 0.5572887999, 0.5629255591, 0.5685588693,
		0.574187883, 0.5798117533, 0.5854296342, 0.5910406806, 0.5966440481, 0.6022388939, 0.6078243763, 0.6133996549,
		0.6189638911, 0.6245162476, 0.6300558891, 0.6355819823, 0.6410936959, 0.6465902005, 0.6520706693, 0.6575342777,
		0.6629802039, 0.6684076285, 0.673815735, 0.6792037097, 0.6845707422, 0.6899160248, 0.6952387536, 0.7005381276,
		0.7058133498, 0.7110636263, 0.7162881674, 0.7214861871, 0.7266569033, 0.7317995382, 0.7369133181, 0.7419974736,
		0.7470512398, 0.7520738565, 0.757064568, 0.7620226236, 0.7669472772, 0.7718377881, 0.7766934204, 0.7815134437,
		0.7862971328, 0.7910437681, 0.7957526355, 0.8004230264, 0.8050542384, 0.8096455747, 0.8141963445, 0.8187058631,
		0.8231734523, 0.8275984398, 0.8319801599, 0.8363179535, 0.8406111679, 0.8448591572, 0.8490612824, 0.8532169113,
		0.8573254187, 0.8613861865, 0.8653986038, 0.8693620669, 0.8732759796, 0.8771397531, 0.8809528059, 0.8847145646,
		0.8884244632, 0.8920819435, 0.8956864553, 0.8992374564, 0.9027344124, 0.9061767973, 0.9095640933, 0.9128957907,
		0.9161713882, 0.9193903932, 0.9225523213, 0.9256566968, 0.9287030527, 0.9316909307, 0.9346198812, 0.9374894637,
		0.9402992464, 0.9430488067, 0.9457377308, 0.9483656142, 0.9509320616, 0.9534366868, 0.9558791132, 0.9582589731,
		0.9605759087, 0.9628295712, 0.9650196218, 0.9671457308, 0.9692075784, 0.9712048545, 0.9731372585, 0.9750044998,
		0.9768062974, 0.9785423803, 0.9802124872, 0.981816367, 0.9833537782, 0.9848244897, 0.9862282802, 0.9875649385,
		0.9888342634, 0.9900360641, 0.9911701597, 0.9922363796, 0.9932345634, 0.9941645609, 0.9950262323, 0.9958194478,
		0.9965440882, 0.9972000444, 0.9977872177, 0.9983055199, 0.9987548729, 0.9991352092, 0.9994464714, 0.9996886128,
		0.999861597, 0.999965398, 1, 0.999965398, 0.999861597, 0.9996886128, 0.9994464714, 0.9991352092, 0.9987548729,
		0.9983055199, 0.9977872177, 0.9972000444, 0.9965440882, 0.9958194478, 0.9950262323, 0.9941645609, 0.9932345634,
		0.9922363796, 0.9911701597, 0.9900360641, 0.9888342634, 0.9875649385, 0.9862282802, 0.9848244897, 0.9833537782,
		0.981816367, 0.9802124872, 0.9785423803, 0.9768062974, 0.9750044998, 0.9731372585, 0.9712048545, 0.9692075784,
		0.9671457308, 0.9650196218, 0.9628295712, 0.9605759087, 0.9582589731, 0.9558791132, 0.9534366868, 0.9509320616,
		0.9483656142, 0.9457377308, 0.9430488067, 0.9402992464, 0.9374894637, 0.9346198812, 0.9316909307, 0.9287030527,
		0.9256566968, 0.9225523213, 0.9193903932, 0.9161713882, 0.9128957907, 0.9095640933, 0.9061767973, 0.9027344124,
		0.8992374564, 0.8956864553, 0.8920819435, 0.8884244632, 0.8847145646, 0.8809528059, 0.8771397531, 0.8732759796,
		0.8693620669, 0.8653986038, 0.8613861865, 0.8573254187, 0.8532169113, 0.8490612824, 0.8448591572, 0.8406111679,
		0.8363179535, 0.8319801599, 0.8275984398, 0.8231734523, 0.8187058631, 0.8141963445, 0.8096455747, 0.8050542384,
		0.8004230264, 0.7957526355, 0.7910437681, 0.7862971328, 0.7815134437, 0.7766934204, 0.7718377881, 0.7669472772,
		0.7620226236, 0.757064568, 0.7520738565, 0.7470512398, 0.7419974736, 0.7369133181, 0.7317995382, 0.7266569033,
		0.7214861871, 0.7162881674, 0.7110636263, 0.7058133498, 0.7005381276, 0.6952387536, 0.6899160248, 0.6845707422,
		0.6792037097, 0.673815735, 0.6684076285, 0.6629802039, 0.6575342777, 0.6520706693, 0.6465902005, 0.6410936959,
		0.6355819823, 0.6300558891, 0.6245162476, 0.6189638911, 0.6133996549, 0.6078243763, 0.6022388939, 0.5966440481,
		0.5910406806, 0.5854296342, 0.5798117533, 0.574187883, 0.5685588693, 0.5629255591, 0.5572887999, 0.5516494397,
		0.546008327, 0.5403663103, 0.5347242385, 0.5290829604, 0.5234433247, 0.5178061799, 0.512172374, 0.5065427545,
		0.5009181685, 0.4952994621, 0.4896874807, 0.4840830684, 0.4784870685, 0.4729003228, 0.4673236718, 0.4617579545,
		0.4562040083, 0.4506626686, 0.4451347691, 0.4396211416, 0.4341226154, 0.4286400177, 0.4231741735, 0.417725905,
		0.4122960318, 0.4068853709, 0.4014947363, 0.3961249389, 0.3907767866, 0.385451084, 0.3801486322, 0.3748702291,
		0.3696166688, 0.3643887415, 0.3591872339, 0.3540129283, 0.3488666034, 0.3437490332, 0.3386609878, 0.3336032326,
		0.3285765284, 0.3235816316, 0.3186192935, 0.3136902608, 0.3087952749, 0.3039350723, 0.2991103842, 0.2943219364,
		0.2895704493, 0.2848566378, 0.280181211, 0.2755448722, 0.2709483191, 0.2663922431, 0.2618773296, 0.2574042579,
		0.252973701, 0.2485863254, 0.2442427911, 0.2399437516, 0.2356898536, 0.2314817372, 0.2273200355, 0.2232053744,
		0.2191383731, 0.2151196434, 0.2111497899, 0.2072294098, 0.2033590929, 0.1995394216, 0.1957709704, 0.1920543062,
		0.1883899883, 0.1847785679, 0.1812205883, 0.1777165848, 0.1742670845, 0.1708726065, 0.1675336613, 0.1642507513,
		0.1610243704, 0.157855004, 0.1547431289, 0.1516892133, 0.1486937166, 0.1457570894, 0.1428797735, 0.1400622019,
		0.1373047984, 0.1346079779, 0.131972146, 0.1293976993, 0.1268850251, 0.1244345015, 0.1220464971, 0.1197213711,
		0.1174594734, 0.1152611442, 0.1131267143, 0.1110565048, 0.1090508271, 0.107109983, 0.1052342645, 0.1034239537,
		0.101679323, 0.1000006349, 0.0983881419, 0.0968420866, 0.0953627016, 0.0939502095, 0.0926048228, 0.0913267438,
		0.0901161649, 0.0889732681, 0.0878982255, 0.0868911987, 0.0859523393, 0.0850817885, 0.0842796772, 0.0835461262,
		0.0828812457, 0.0822851359, 0.0817578864, 0.0812995765, 0.0809102752, 0.0805900411, 0.0803389222, 0.0801569565,
		0.0800441712 };
