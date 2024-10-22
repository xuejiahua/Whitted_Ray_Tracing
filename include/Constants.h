#pragma once

const double      PI                                    = 3.1415926535897932385;
const double      INF                                   = 1.79769e+308;
const double      TOLERANCE                             = 1e-8;
const int         BYTES_PER_PIXEL                       = 3;

/*
* PRESET IMAGE LIBRARY
*�����������������������������������������������������Щ����������������������������Щ�������������������������������
*��          SCENE          | SCENE OPTION | CAMERA OPTION |
*�� four spheres            |       1      |       1       |
*| 484 spheres             |       2      |       2       |
*�����������������������������������������������������ة����������������������������ة�������������������������������
* 
* RECOMMENDED RESOLUTION
*���������������������������Щ����������������������������Щ��������������������������Щ�����������������������������
*�� Resolution | ASPECT_RATIO | IMAGE_WIDTH | IMAGE_HEIGHT |
*��    240p    |     22:15    |      352    |      240     |
*|    360p    |      4:3     |      480    |      360     |
*|    480p    |     16:9     |      852    |      480     |
*|    720p    |     16:9     |     1280    |      720     |
*|  1080p(HD) |     16:9     |     1920    |     1080     |
*|  1440p(2k) |     16:9     |     2560    |     1440     |
*|  2160p(4k) |     16:9     |     3840    |     2160     |
*|  4320p(8k) |     16:9     |     7680    |     4320     |
*���������������������������ة����������������������������ة��������������������������ة�����������������������������
*/

/*
* ACCELERATOR OPTION
*�����������������������Щ���������������������������������������
*��  Option  |  Accelerator      |
*��    0     |  Brute force      |
*|    1     |  Equal count BVH  |
*|    2     |  Middle BVH       |
*|    3     |  SAH BVH          |
*�����������������������ة���������������������������������������
*/