/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "stm32f4xx.h"
#include "lcd_stm32f4.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
// TODO [Task 2]: Add values for below variables
#define NS        128  // Number of samples in LUT
#define TIM2CLK   16000000  // STM Clock frequency: Hint You might want to check the ioc file
#define F_SIGNAL  440      // Frequency of output analog signal

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
DMA_HandleTypeDef hdma_tim2_ch1;

/* USER CODE BEGIN PV */
// TODO [Task 1]: Add code for global variables, including LUTs
uint32_t Sine_LUT[128] = {2048, 2148, 2248, 2348, 2447, 2545, 2642, 2737, 2831, 2923, 3013, 3100, 3185, 3267, 3346, 3423, 3495, 3565, 3630, 3692, 3750, 3804, 3853, 3898, 3939, 3975, 4007, 4034, 4056, 4073, 4085, 4093, 4095, 4093, 4085, 4073, 4056, 4034, 4007, 3975, 3939, 3898, 3853, 3804, 3750, 3692, 3630, 3565, 3495, 3423, 3346, 3267, 3185, 3100, 3013, 2923, 2831, 2737, 2642, 2545, 2447, 2348, 2248, 2148, 2048, 1947, 1847, 1747, 1648, 1550, 1453, 1358, 1264, 1172, 1082, 995, 910, 828, 749, 672, 600, 530, 465, 403, 345, 291, 242, 197, 156, 120, 88, 61, 39, 22, 10, 2, 0, 2, 10, 22, 39, 61, 88, 120, 156, 197, 242, 291, 345, 403, 465, 530, 600, 672, 749, 828, 910, 995, 1082, 1172, 1264, 1358, 1453, 1550, 1648, 1747, 1847, 1947};
uint32_t Saw_LUT[128] = {0, 32, 64, 97, 129, 161, 193, 226, 258, 290, 322, 355, 387, 419, 451, 484, 516, 548, 580, 613, 645, 677, 709, 742, 774, 806, 838, 871, 903, 935, 967, 1000, 1032, 1064, 1096, 1129, 1161, 1193, 1225, 1258, 1290, 1322, 1354, 1386, 1419, 1451, 1483, 1515, 1548, 1580, 1612, 1644, 1677, 1709, 1741, 1773, 1806, 1838, 1870, 1902, 1935, 1967, 1999, 2031, 2064, 2096, 2128, 2160, 2193, 2225, 2257, 2289, 2322, 2354, 2386, 2418, 2451, 2483, 2515, 2547, 2580, 2612, 2644, 2676, 2709, 2741, 2773, 2805, 2837, 2870, 2902, 2934, 2966, 2999, 3031, 3063, 3095, 3128, 3160, 3192, 3224, 3257, 3289, 3321, 3353, 3386, 3418, 3450, 3482, 3515, 3547, 3579, 3611, 3644, 3676, 3708, 3740, 3773, 3805, 3837, 3869, 3902, 3934, 3966, 3998, 4031, 4063, 4095};
uint32_t Triangle_LUT[128] = {0, 65, 130, 195, 260, 325, 390, 455, 520, 585, 650, 715, 780, 845, 910, 975, 1040, 1105, 1170, 1235, 1300, 1365, 1430, 1495, 1560, 1625, 1690, 1755, 1820, 1885, 1950, 2015, 2080, 2145, 2210, 2275, 2340, 2405, 2470, 2535, 2600, 2665, 2730, 2795, 2860, 2925, 2990, 3055, 3120, 3185, 3250, 3315, 3380, 3445, 3510, 3575, 3640, 3705, 3770, 3835, 3900, 3965, 4030, 4095, 4030, 3965, 3900, 3835, 3770, 3705, 3640, 3575, 3510, 3445, 3380, 3315, 3250, 3185, 3120, 3055, 2990, 2925, 2860, 2795, 2730, 2665, 2600, 2535, 2470, 2405, 2340, 2275, 2210, 2145, 2080, 2015, 1950, 1885, 1820, 1755, 1690, 1625, 1560, 1495, 1430, 1365, 1300, 1235, 1170, 1105, 1040, 975, 910, 845, 780, 715, 650, 585, 520, 455, 390, 325, 260, 195, 130, 65, 0};


uint32_t Piano_LUT[20000] = { 2637, 2644, 2652, 2661, 2670, 2679, 2686, 2692, 2698, 2703, 2708, 2714, 2720, 2726, 2733, 2739, 2744, 2747, 2749, 2750, 2748, 2744, 2739, 2732, 2723, 2714, 2705, 2696, 2688, 2682, 2678, 2675, 2674, 2673, 2672, 2671, 2669, 2666, 2663, 2659, 2656, 2654, 2652, 2650, 2650, 2650, 2650, 2651, 2653, 2655, 2657, 2660, 2664, 2669, 2674, 2679, 2684, 2690, 2696, 2703, 2709, 2717, 2725, 2733, 2741, 2749, 2756, 2762, 2766, 2768, 2769, 2768, 2767, 2766, 2764, 2763, 2762, 2761, 2760, 2759, 2758, 2757, 2756, 2755, 2755, 2754, 2753, 2752, 2749, 2745, 2740, 2733, 2724, 2714, 2703, 2690, 2676, 2660, 2642, 2623, 2602, 2581, 2560, 2539, 2520, 2502, 2487, 2473, 2460, 2447, 2434, 2419, 2404, 2387, 2370, 2351, 2333, 2314, 2295, 2277, 2260, 2243, 2226, 2211, 2197, 2184, 2172, 2162, 2153, 2144, 2135, 2126, 2117, 2108, 2099, 2090, 2082, 2075, 2068, 2063, 2057, 2052, 2046, 2040, 2034, 2027, 2021, 2015, 2010, 2005, 2000, 1996, 1990, 1984, 1977, 1970, 1962, 1953, 1945, 1937, 1930, 1922, 1914, 1906, 1898, 1889, 1880, 1870, 1861, 1853, 1845, 1838, 1831, 1825, 1818, 1812, 1805, 1799, 1792, 1785, 1777, 1770, 1762, 1753, 1744, 1735, 1725, 1714, 1704, 1694, 1685, 1675, 1665, 1655, 1644, 1631, 1618, 1604, 1589, 1575, 1562, 1549, 1538, 1529, 1520, 1511, 1502, 1493, 1484, 1474, 1464, 1454, 1445, 1436, 1428, 1422, 1416, 1412, 1409, 1407, 1407, 1408, 1411, 1414, 1419, 1424, 1429, 1434, 1439, 1444, 1449, 1454, 1459, 1463, 1468, 1473, 1478, 1484, 1490, 1496, 1504, 1512, 1521, 1531, 1541, 1551, 1561, 1570, 1579, 1587, 1594, 1601, 1608, 1615, 1622, 1630, 1638, 1646, 1655, 1664, 1674, 1684, 1695, 1706, 1717, 1729, 1740, 1751, 1761, 1770, 1778, 1785, 1791, 1798, 1804, 1810, 1817, 1823, 1829, 1834, 1839, 1844, 1848, 1852, 1855, 1859, 1863, 1867, 1870, 1873, 1876, 1880, 1883, 1886, 1891, 1896, 1901, 1907, 1913, 1920, 1926, 1932, 1938, 1945, 1952, 1960, 1969, 1978, 1989, 2000, 2011, 2022, 2033, 2045, 2057, 2071, 2085, 2101, 2119, 2137, 2157, 2177, 2197, 2217, 2237, 2258, 2278, 2300, 2322, 2345, 2369, 2393, 2418, 2444, 2471, 2499, 2527, 2556, 2585, 2614, 2642, 2668, 2692, 2715, 2737, 2758, 2778, 2799, 2821, 2843, 2865, 2888, 2911, 2933, 2953, 2973, 2992, 3009, 3026, 3042, 3056, 3070, 3082, 3093, 3101, 3108, 3114, 3119, 3123, 3127, 3131, 3135, 3139, 3141, 3142, 3142, 3140, 3136, 3131, 3124, 3117, 3109, 3101, 3093, 3085, 3077, 3069, 3061, 3052, 3045, 3037, 3030, 3023, 3017, 3010, 3003, 2997, 2989, 2982, 2975, 2968, 2962, 2956, 2951, 2945, 2940, 2935, 2929, 2924, 2920, 2917, 2914, 2914, 2914, 2915, 2918, 2920, 2922, 2924, 2926, 2928, 2930, 2932, 2935, 2938, 2942, 2946, 2951, 2957, 2963, 2970, 2977, 2984, 2992, 3001, 3009, 3017, 3025, 3031, 3037, 3043, 3047, 3052, 3057, 3062, 3067, 3072, 3077, 3081, 3084, 3086, 3088, 3089, 3090, 3090, 3090, 3091, 3091, 3090, 3089, 3087, 3083, 3079, 3073, 3066, 3057, 3048, 3038, 3027, 3015, 3003, 2992, 2980, 2969, 2958, 2948, 2938, 2927, 2917, 2906, 2895, 2884, 2871, 2859, 2846, 2832, 2818, 2803, 2788, 2772, 2757, 2741, 2726, 2711, 2697, 2685, 2673, 2662, 2650, 2638, 2625, 2611, 2595, 2579, 2561, 2544, 2527, 2511, 2496, 2482, 2469, 2457, 2446, 2436, 2426, 2416, 2406, 2396, 2386, 2376, 2365, 2354, 2342, 2330, 2318, 2305, 2293, 2281, 2270, 2259, 2248, 2237, 2226, 2215, 2204, 2193, 2183, 2172, 2162, 2152, 2142, 2131, 2119, 2108, 2095, 2083, 2070, 2057, 2044, 2031, 2018, 2005, 1991, 1976, 1960, 1944, 1928, 1913, 1898, 1884, 1871, 1859, 1848, 1836, 1825, 1813, 1801, 1789, 1777, 1766, 1755, 1744, 1734, 1724, 1714, 1704, 1694, 1683, 1673, 1664, 1655, 1647, 1640, 1634, 1628, 1622, 1617, 1612, 1608, 1605, 1603, 1602, 1602, 1603, 1604, 1605, 1607, 1608, 1610, 1612, 1615, 1618, 1622, 1626, 1631, 1636, 1641, 1646, 1651, 1656, 1661, 1667, 1674, 1682, 1691, 1700, 1710, 1719, 1728, 1737, 1745, 1754, 1761, 1769, 1777, 1785, 1793, 1800, 1807, 1814, 1821, 1828, 1834, 1841, 1848, 1855, 1862, 1868, 1873, 1878, 1883, 1887, 1892, 1897, 1903, 1910, 1919, 1928, 1937, 1947, 1957, 1967, 1977, 1987, 1996, 2006, 2015, 2024, 2032, 2040, 2048, 2057, 2065, 2074, 2085, 2096, 2108, 2121, 2135, 2149, 2162, 2175, 2188, 2199, 2211, 2223, 2235, 2249, 2263, 2279, 2295, 2313, 2331, 2349, 2367, 2386, 2404, 2423, 2442, 2462, 2482, 2503, 2524, 2546, 2569, 2593, 2616, 2641, 2665, 2689, 2714, 2738, 2763, 2788, 2814, 2839, 2865, 2891, 2917, 2942, 2966, 2988, 3009, 3029, 3047, 3064, 3081, 3098, 3114, 3131, 3148, 3166, 3184, 3202, 3220, 3238, 3256, 3274, 3293, 3311, 3329, 3347, 3366, 3384, 3402, 3419, 3436, 3452, 3468, 3483, 3497, 3511, 3523, 3535, 3546, 3557, 3567, 3577, 3586, 3594, 3602, 3609, 3616, 3622, 3627, 3632, 3638, 3644, 3650, 3657, 3665, 3673, 3682, 3691, 3700, 3709, 3719, 3728, 3737, 3747, 3758, 3769, 3780, 3793, 3805, 3819, 3833, 3847, 3862, 3877, 3893, 3908, 3923, 3938, 3951, 3964, 3975, 3985, 3994, 4002, 4009, 4016, 4023, 4029, 4036, 4041, 4047, 4053, 4058, 4064, 4069, 4075, 4081, 4086, 4090, 4094, 4095, 4095, 4094, 4090, 4086, 4081, 4076, 4071, 4066, 4061, 4056, 4051, 4045, 4039, 4032, 4025, 4018, 4010, 4002, 3994, 3985, 3975, 3965, 3954, 3942, 3929, 3916, 3902, 3887, 3871, 3855, 3838, 3821, 3802, 3781, 3760, 3738, 3715, 3692, 3668, 3643, 3617, 3591, 3564, 3537, 3509, 3483, 3456, 3431, 3406, 3383, 3359, 3336, 3313, 3289, 3264, 3238, 3212, 3185, 3158, 3131, 3104, 3077, 3050, 3022, 2994, 2965, 2937, 2908, 2880, 2853, 2826, 2801, 2776, 2753, 2729, 2706, 2683, 2660, 2638, 2615, 2593, 2572, 2551, 2530, 2510, 2489, 2468, 2447, 2426, 2404, 2383, 2363, 2344, 2325, 2307, 2290, 2273, 2256, 2239, 2223, 2205, 2188, 2170, 2153, 2135, 2117, 2100, 2082, 2065, 2047, 2030, 2013, 1996, 1979, 1961, 1942, 1922, 1901, 1879, 1856, 1833, 1809, 1786, 1763, 1741, 1719, 1698, 1677, 1656, 1635, 1614, 1592, 1570, 1549, 1528, 1507, 1486, 1466, 1445, 1424, 1403, 1381, 1359, 1337, 1314, 1292, 1270, 1248, 1226, 1205, 1185, 1164, 1144, 1124, 1105, 1086, 1066, 1047, 1028, 1009, 990, 971, 952, 933, 914, 896, 878, 861, 844, 828, 811, 795, 778, 762, 745, 729, 713, 698, 684, 671, 658, 647, 636, 626, 616, 606, 597, 587, 577, 567, 557, 546, 535, 525, 514, 504, 494, 485, 476, 467, 459, 451, 443, 436, 429, 422, 416, 410, 404, 397, 391, 385, 379, 373, 367, 362, 357, 352, 348, 345, 342, 339, 336, 333, 330, 328, 325, 322, 320, 318, 316, 314, 313, 311, 310, 309, 308, 309, 309, 311, 313, 316, 319, 322, 326, 329, 333, 336, 339, 341, 344, 346, 348, 349, 352, 354, 358, 362, 368, 375, 383, 393, 403, 414, 426, 438, 452, 465, 480, 495, 511, 527, 544, 561, 577, 592, 607, 621, 635, 649, 662, 676, 689, 703, 716, 729, 742, 754, 767, 779, 791, 803, 816, 829, 842, 855, 868, 880, 893, 905, 917, 930, 942, 956, 970, 984, 998, 1013, 1028, 1043, 1058, 1073, 1088, 1103, 1119, 1135, 1151, 1167, 1183, 1200, 1216, 1232, 1248, 1264, 1279, 1295, 1310, 1324, 1338, 1352, 1365, 1378, 1392, 1405, 1419, 1433, 1448, 1464, 1480, 1497, 1513, 1530, 1548, 1565, 1583, 1601, 1619, 1637, 1654, 1671, 1688, 1705, 1722, 1738, 1755, 1773, 1790, 1808, 1826, 1843, 1860, 1875, 1890, 1904, 1916, 1929, 1940, 1952, 1964, 1977, 1990, 2003, 2016, 2029, 2042, 2055, 2067, 2079, 2090, 2101, 2110, 2119, 2127, 2134, 2141, 2148, 2154, 2161, 2168, 2176, 2184, 2193, 2201, 2210, 2218, 2226, 2233, 2239, 2244, 2249, 2253, 2256, 2259, 2262, 2265, 2268, 2270, 2273, 2275, 2277, 2280, 2282, 2284, 2287, 2289, 2290, 2292, 2293, 2294, 2295, 2295, 2295, 2295, 2294, 2294, 2294, 2294, 2295, 2296, 2297, 2300, 2302, 2305, 2308, 2311, 2314, 2317, 2318, 2319, 2320, 2320, 2319, 2318, 2317, 2315, 2314, 2312, 2310, 2309, 2307, 2306, 2304, 2302, 2301, 2299, 2298, 2296, 2295, 2293, 2291, 2288, 2285, 2282, 2279, 2275, 2271, 2266, 2261, 2256, 2251, 2245, 2238, 2231, 2223, 2215, 2207, 2199, 2190, 2182, 2174, 2166, 2157, 2149, 2140, 2130, 2121, 2110, 2099, 2088, 2076, 2064, 2051, 2038, 2024, 2010, 1995, 1979, 1963, 1947, 1931, 1916, 1900, 1885, 1870, 1855, 1841, 1827, 1813, 1800, 1786, 1772, 1759, 1745, 1732, 1719, 1706, 1693, 1680, 1667, 1655, 1642, 1629, 1617, 1604, 1592, 1580, 1569, 1557, 1546, 1535, 1525, 1515, 1506, 1497, 1488, 1479, 1471, 1463, 1454, 1446, 1437, 1428, 1420, 1412, 1403, 1395, 1388, 1380, 1373, 1367, 1361, 1356, 1352, 1348, 1346, 1344, 1342, 1341, 1340, 1339, 1337, 1336, 1334, 1332, 1331, 1329, 1328, 1327, 1327, 1326, 1326, 1326, 1325, 1325, 1324, 1324, 1323, 1322, 1321, 1320, 1320, 1319, 1319, 1318, 1318, 1319, 1320, 1322, 1325, 1328, 1332, 1336, 1340, 1345, 1350, 1355, 1360, 1364, 1369, 1373, 1377, 1382, 1386, 1391, 1396, 1402, 1408, 1414, 1420, 1426, 1433, 1439, 1446, 1452, 1458, 1463, 1469, 1474, 1479, 1484, 1489, 1494, 1499, 1504, 1509, 1514, 1520, 1525, 1531, 1537, 1543, 1550, 1556, 1564, 1571, 1580, 1588, 1597, 1607, 1616, 1626, 1635, 1645, 1655, 1664, 1674, 1684, 1694, 1704, 1713, 1723, 1733, 1742, 1751, 1760, 1769, 1778, 1786, 1795, 1803, 1811, 1819, 1828, 1836, 1844, 1852, 1861, 1869, 1877, 1884, 1892, 1899, 1905, 1912, 1918, 1924, 1930, 1937, 1944, 1951, 1959, 1966, 1974, 1982, 1990, 1997, 2005, 2013, 2022, 2030, 2039, 2048, 2057, 2066, 2076, 2086, 2095, 2105, 2116, 2126, 2137, 2148, 2159, 2170, 2180, 2190, 2200, 2209, 2218, 2227, 2236, 2244, 2252, 2260, 2268, 2275, 2282, 2288, 2294, 2300, 2306, 2311, 2317, 2322, 2327, 2332, 2338, 2343, 2348, 2353, 2359, 2365, 2371, 2377, 2384, 2391, 2397, 2404, 2410, 2417, 2423, 2430, 2436, 2443, 2450, 2456, 2462, 2468, 2473, 2478, 2483, 2487, 2490, 2493, 2496, 2499, 2501, 2502, 2503, 2504, 2505, 2505, 2506, 2507, 2508, 2509, 2510, 2512, 2513, 2514, 2515, 2516, 2517, 2518, 2519, 2519, 2519, 2519, 2518, 2517, 2516, 2514, 2513, 2511, 2509, 2507, 2506, 2504, 2502, 2500, 2498, 2495, 2492, 2489, 2485, 2481, 2476, 2472, 2467, 2462, 2456, 2451, 2446, 2441, 2436, 2432, 2427, 2423, 2418, 2413, 2408, 2402, 2396, 2390, 2384, 2378, 2372, 2367, 2361, 2356, 2351, 2346, 2341, 2335, 2330, 2324, 2318, 2312, 2305, 2298, 2291, 2283, 2275, 2266, 2258, 2249, 2240, 2232, 2223, 2215, 2206, 2198, 2190, 2182, 2173, 2165, 2157, 2149, 2141, 2132, 2124, 2116, 2108, 2100, 2091, 2083, 2075, 2068, 2060, 2052, 2044, 2036, 2028, 2020, 2011, 2002, 1993, 1984, 1975, 1966, 1958, 1949, 1940, 1932, 1923, 1914, 1904, 1894, 1885, 1875, 1865, 1855, 1845, 1835, 1825, 1815, 1805, 1796, 1787, 1778, 1770, 1762, 1754, 1747, 1740, 1733, 1727, 1720, 1714, 1708, 1701, 1695, 1689, 1683, 1677, 1671, 1665, 1659, 1653, 1647, 1642, 1636, 1631, 1626, 1622, 1617, 1612, 1608, 1603, 1598, 1593, 1588, 1583, 1577, 1572, 1568, 1563, 1559, 1555, 1552, 1549, 1546, 1544, 1543, 1541, 1540, 1538, 1536, 1534, 1532, 1529, 1526, 1523, 1520, 1516, 1513, 1509, 1506, 1502, 1499, 1495, 1492, 1489, 1486, 1484, 1482, 1481, 1480, 1479, 1479, 1479, 1479, 1479, 1479, 1478, 1478, 1477, 1476, 1475, 1473, 1472, 1470, 1468, 1466, 1464, 1463, 1462, 1461, 1461, 1461, 1462, 1463, 1465, 1467, 1470, 1472, 1475, 1479, 1482, 1485, 1489, 1492, 1496, 1499, 1502, 1506, 1509, 1512, 1515, 1518, 1521, 1525, 1528, 1532, 1536, 1540, 1544, 1549, 1553, 1559, 1564, 1570, 1576, 1583, 1590, 1597, 1605, 1613, 1622, 1631, 1640, 1649, 1659, 1669, 1679, 1690, 1702, 1713, 1725, 1737, 1750, 1763, 1776, 1789, 1803, 1817, 1831, 1845, 1859, 1874, 1888, 1902, 1916, 1930, 1943, 1957, 1970, 1983, 1996, 2008, 2021, 2033, 2046, 2058, 2071, 2084, 2096, 2109, 2122, 2134, 2147, 2160, 2173, 2186, 2199, 2212, 2225, 2238, 2252, 2265, 2278, 2292, 2305, 2318, 2331, 2344, 2357, 2370, 2383, 2396, 2409, 2421, 2434, 2446, 2459, 2471, 2482, 2494, 2506, 2517, 2528, 2539, 2549, 2560, 2570, 2580, 2590, 2599, 2608, 2617, 2626, 2635, 2643, 2652, 2661, 2669, 2677, 2685, 2693, 2701, 2709, 2716, 2724, 2731, 2739, 2746, 2753, 2760, 2767, 2774, 2781, 2788, 2794, 2801, 2807, 2813, 2818, 2823, 2828, 2832, 2835, 2839, 2842, 2844, 2847, 2849, 2851, 2853, 2855, 2857, 2858, 2859, 2860, 2861, 2862, 2862, 2862, 2862, 2861, 2860, 2859, 2858, 2857, 2855, 2853, 2851, 2849, 2846, 2844, 2841, 2838, 2835, 2832, 2828, 2825, 2821, 2817, 2813, 2809, 2804, 2800, 2795, 2790, 2785, 2780, 2775, 2770, 2765, 2760, 2754, 2748, 2742, 2736, 2729, 2722, 2715, 2707, 2699, 2691, 2683, 2675, 2666, 2658, 2650, 2641, 2633, 2624, 2615, 2606, 2597, 2588, 2579, 2570, 2560, 2551, 2542, 2532, 2523, 2514, 2505, 2496, 2488, 2479, 2471, 2463, 2454, 2446, 2437, 2428, 2419, 2410, 2400, 2389, 2379, 2368, 2357, 2346, 2334, 2322, 2310, 2298, 2286, 2274, 2262, 2250, 2238, 2226, 2215, 2203, 2191, 2179, 2168, 2156, 2144, 2133, 2121, 2110, 2098, 2086, 2075, 2063, 2052, 2040, 2029, 2017, 2006, 1994, 1983, 1972, 1961, 1950, 1940, 1929, 1919, 1908, 1898, 1887, 1877, 1867, 1856, 1846, 1836, 1826, 1815, 1805, 1795, 1785, 1776, 1767, 1758, 1749, 1741, 1733, 1725, 1717, 1709, 1702, 1694, 1686, 1679, 1671, 1663, 1655, 1647, 1639, 1631, 1624, 1616, 1608, 1601, 1594, 1587, 1581, 1575, 1569, 1564, 1559, 1554, 1549, 1545, 1540, 1536, 1532, 1528, 1525, 1521, 1518, 1515, 1513, 1510, 1509, 1507, 1506, 1505, 1505, 1505, 1505, 1506, 1507, 1508, 1509, 1510, 1512, 1514, 1516, 1518, 1520, 1522, 1525, 1527, 1529, 1532, 1534, 1537, 1540, 1542, 1545, 1548, 1551, 1555, 1559, 1563, 1567, 1571, 1576, 1581, 1587, 1592, 1598, 1604, 1611, 1617, 1624, 1630, 1637, 1644, 1651, 1658, 1666, 1673, 1680, 1688, 1696, 1704, 1712, 1721, 1729, 1738, 1746, 1755, 1764, 1773, 1782, 1791, 1800, 1809, 1818, 1826, 1835, 1844, 1853, 1861, 1870, 1879, 1887, 1896, 1905, 1914, 1923, 1932, 1942, 1952, 1962, 1972, 1982, 1993, 2004, 2015, 2027, 2038, 2049, 2061, 2072, 2084, 2095, 2107, 2118, 2129, 2140, 2150, 2161, 2172, 2182, 2193, 2203, 2213, 2224, 2234, 2244, 2254, 2263, 2273, 2282, 2291, 2300, 2309, 2317, 2326, 2334, 2342, 2351, 2359, 2367, 2375, 2384, 2392, 2400, 2409, 2418, 2426, 2435, 2444, 2453, 2462, 2472, 2481, 2490, 2500, 2510, 2519, 2529, 2538, 2548, 2558, 2567, 2577, 2586, 2595, 2604, 2613, 2622, 2630, 2639, 2647, 2655, 2662, 2670, 2677, 2685, 2692, 2700, 2707, 2715, 2722, 2730, 2737, 2745, 2752, 2759, 2767, 2774, 2780, 2787, 2794, 2800, 2806, 2812, 2819, 2825, 2831, 2837, 2843, 2849, 2855, 2861, 2867, 2872, 2878, 2884, 2889, 2895, 2900, 2905, 2910, 2914, 2918, 2922, 2926, 2929, 2932, 2934, 2937, 2939, 2941, 2942, 2944, 2945, 2946, 2947, 2948, 2949, 2950, 2951, 2952, 2953, 2953, 2954, 2955, 2955, 2956, 2957, 2957, 2957, 2957, 2957, 2957, 2956, 2955, 2953, 2951, 2949, 2946, 2943, 2940, 2936, 2931, 2927, 2922, 2917, 2912, 2906, 2901, 2895, 2890, 2884, 2878, 2872, 2866, 2860, 2853, 2846, 2839, 2831, 2823, 2815, 2806, 2797, 2788, 2779, 2769, 2760, 2751, 2741, 2732, 2723, 2713, 2704, 2696, 2687, 2678, 2669, 2661, 2652, 2644, 2635, 2627, 2618, 2609, 2600, 2590, 2581, 2571, 2561, 2551, 2540, 2529, 2519, 2508, 2497, 2485, 2474, 2463, 2451, 2439, 2428, 2416, 2404, 2392, 2380, 2368, 2356, 2345, 2333, 2321, 2309, 2297, 2285, 2273, 2261, 2249, 2237, 2225, 2213, 2201, 2189, 2177, 2165, 2153, 2141, 2129, 2116, 2104, 2092, 2080, 2067, 2055, 2044, 2032, 2021, 2010, 2000, 1990, 1980, 1970, 1961, 1952, 1943, 1935, 1926, 1918, 1910, 1902, 1893, 1885, 1877, 1869, 1861, 1853, 1845, 1838, 1830, 1823, 1816, 1809, 1802, 1796, 1789, 1782, 1776, 1769, 1763, 1756, 1749, 1743, 1736, 1729, 1723, 1717, 1710, 1705, 169 };



uint32_t Drum_LUT[128] = {2203, 2034, 2054, 2049, 2043, 2049, 2043, 2052, 2047, 2045, 2042, 2049, 2038, 2055, 2039, 2047, 2046, 2046, 2048, 2056, 2049, 2051, 2039, 2049, 2033, 2053, 2044, 2056, 2042, 2047, 2038, 2043, 2031, 2053, 2041, 2060, 2040, 2056, 2046, 2056, 2039, 2065, 2042, 2057, 2027, 1855, 1979, 2287, 2198, 1453, 1964, 3213, 4095, 2366, 0, 123, 0, 2870, 4095, 4095, 3130, 34, 108, 0, 2109, 4071, 4067, 4080, 3107, 1037, 321, 702, 2026, 2761, 2086, 821, 217, 538, 1191, 1883, 1771, 1618, 1363, 1814, 1616, 1769, 2348, 2758, 3145, 3482, 3544, 3664, 3996, 4078, 3990, 3546, 2771, 2371, 2152, 2144, 2036, 1841, 916, 0, 23, 0, 25, 0, 469, 1139, 1710, 2581, 2814, 2710, 2646, 2519, 2400, 3032, 3261, 3301, 3357, 3172, 2887, 3344, 3595, 3742, 3779, 3114};
uint32_t Guitar_LUT[128] = {2738, 2882, 2691, 2874, 1717, 1794, 1380, 2081, 2602, 1307, 1847, 1686, 1842, 2047, 2823, 2660, 2117, 2756, 1631, 1479, 1569, 2023, 2020, 1652, 2277, 1937, 2381, 2504, 2413, 2116, 1819, 1809, 1491, 1856, 1576, 2453, 2282, 1818, 2643, 2412, 2189, 1913, 2095, 1532, 1805, 2284, 1623, 1790, 2129, 2804, 2282, 2223, 2163, 1530, 1921, 1937, 2082, 1856, 1997, 2022, 2142, 2303, 2227, 2704, 1648, 1670, 2026, 1740, 1844, 1992, 2336, 2011, 2554, 2379, 1760, 1764, 2124, 2271, 1555, 2012, 1759, 1808, 2344, 2343, 2416, 2122, 2205, 1918, 1843, 1692, 1990, 2132, 1596, 2309, 2360, 2216, 2155, 2206, 2121, 1833, 2237, 1765, 1468, 1849, 2468, 2232, 1999, 2375, 1889, 2144, 2194, 2055, 1886, 1747, 1996, 1922, 2009, 2137, 2507, 2055, 1884, 2283, 1967, 1888, 1854, 2044, 1991, 2056};

uint32_t TIM2_Ticks = 16000000 / (128 * 440); // How often to write new LUT value
uint32_t DestAddress = (uint32_t) &(TIM3->CCR3); // Write LUT TO TIM3->CCR3 to modify PWM duty cycle

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM3_Init(void);
/* USER CODE BEGIN PFP */
void EXTI0_IRQHandler(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
  lcd_init();
  lcd_send_string("Sine");

  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3); // Start PWM generation on TIM3 Channel 3
  HAL_TIM_OC_Start(&htim2, TIM_CHANNEL_1);  // Start Output Compare on TIM2 Channel 1
  HAL_DMA_Start_IT(&hdma_tim2_ch1, (uint32_t)Sine_LUT, DestAddress, NS); // Start DMA in interrupt mode
  __HAL_TIM_ENABLE_DMA(&htim2, TIM_DMA_CC1); // Enable DMA requests for TIM2 Channel 1

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{
  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = TIM2_Ticks - 1;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_OC_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_TIMING;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_OC_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */
  /* TIM2_CH1 DMA Init */
  __HAL_RCC_DMA1_CLK_ENABLE();

  hdma_tim2_ch1.Instance = DMA1_Stream5;
  hdma_tim2_ch1.Init.Channel = DMA_CHANNEL_3;         // TIM2_CH1 is on channel 3
  hdma_tim2_ch1.Init.Direction = DMA_MEMORY_TO_PERIPH; // Memory -> TIM3->CCR3
  hdma_tim2_ch1.Init.PeriphInc = DMA_PINC_DISABLE;    // Peripheral address fixed
  hdma_tim2_ch1.Init.MemInc = DMA_MINC_ENABLE;        // Memory address increments
  hdma_tim2_ch1.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  hdma_tim2_ch1.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
  hdma_tim2_ch1.Init.Mode = DMA_CIRCULAR;            // Repeat LUT automatically
  hdma_tim2_ch1.Init.Priority = DMA_PRIORITY_HIGH;
  hdma_tim2_ch1.Init.FIFOMode = DMA_FIFOMODE_DISABLE;

  if (HAL_DMA_Init(&hdma_tim2_ch1) != HAL_OK)
  {
      Error_Handler();
  }

  /* Link DMA handle to TIM2 handle */
  __HAL_LINKDMA(&htim2, hdma[TIM_DMA_ID_CC1], hdma_tim2_ch1);
  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{
  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 65535;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{
  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  // -------------------------------
  // LCD pins configuration
  // -------------------------------
  // Configure PC14 (RS) and PC15 (E) as output push-pull
  GPIO_InitStruct.Pin = GPIO_PIN_14 | GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  // Configure PB8 (D4) and PB9 (D5) as output push-pull
  GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  // Configure PA12 (D6) and PA15 (D7) as output push-pull
  GPIO_InitStruct.Pin = GPIO_PIN_12 | GPIO_PIN_15;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  // Set all LCD pins LOW initially
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14 | GPIO_PIN_15, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8 | GPIO_PIN_9, GPIO_PIN_RESET);
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  // Set all LCD pins LOW initially
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14 | GPIO_PIN_15, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8 | GPIO_PIN_9, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12 | GPIO_PIN_15, GPIO_PIN_RESET);


  // -------------------------------
  // Button0 configuration (PA0)
  // -------------------------------
  GPIO_InitStruct.Pin = Button0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING; // Interrupt on rising edge
  GPIO_InitStruct.Pull = GPIO_PULLUP;         // Use pull-up resistor
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  // Enable and set EXTI line 0 interrupt priority
  HAL_NVIC_SetPriority(EXTI0_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void EXTI0_IRQHandler(void){
	// TODO [Task 4]: Debounce using HAL_GetTick()
	static uint32_t lastTick = 0;
	if (HAL_GetTick() - lastTick < 50) { // 50ms debounce
		HAL_GPIO_EXTI_IRQHandler(Button0_Pin); // Clear interrupt flags
		return;
	}
	lastTick = HAL_GetTick();

	// TODO [Task 4]: Disable DMA transfer and abort IT, then start DMA in IT mode with new LUT and re-enable transfer
	// HINT: Consider using C's "switch" function to handle LUT changes
	HAL_DMA_Abort_IT(&hdma_tim2_ch1);

	static uint32_t lutIndex = 0;
	lutIndex = (lutIndex + 1) % 6; // Cycle through 0-5
	switch (lutIndex) {
		case 0:
			HAL_DMA_Start_IT(&hdma_tim2_ch1, (uint32_t)Sine_LUT, DestAddress, NS);
			lcd_send_string("Waveform: Sine");
			break;
		case 1:
			HAL_DMA_Start_IT(&hdma_tim2_ch1, (uint32_t)Saw_LUT, DestAddress, NS);
			lcd_send_string("Waveform: Saw");
			break;
		case 2:
			HAL_DMA_Start_IT(&hdma_tim2_ch1, (uint32_t)Triangle_LUT, DestAddress, NS);
			lcd_send_string("Waveform: Triangle");
			break;
		case 3:
			HAL_DMA_Start_IT(&hdma_tim2_ch1, (uint32_t)Piano_LUT, DestAddress, NS);
			lcd_send_string("Waveform: Piano");
			break;
		case 4:
			HAL_DMA_Start_IT(&hdma_tim2_ch1, (uint32_t)Drum_LUT, DestAddress, NS);
			lcd_send_string("Waveform: Drum");
			break;
		case 5:
			HAL_DMA_Start_IT(&hdma_tim2_ch1, (uint32_t)Guitar_LUT, DestAddress, NS);
			lcd_send_string("Waveform: Guitar");
			break;
	}

	HAL_GPIO_EXTI_IRQHandler(Button0_Pin); // Clear interrupt flags
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */