#pragma once

#include <iostream>
#include <chrono>
#include <ctime>
#include <atlsafe.h>
#include <windows.h>
#include <vector>
#include <fstream>
#include <atlimage.h>
#include <vector>
#include <winsock.h>

// open cv includes:
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>


#define ERORR_SCREENSHOT 401

#define WIDTH_POS 0
#define HEIGHT_POS 1

#define KEY_SIZE_BYTE 16
#define BLOCK_SIZE_ROW 4
#define BLOCK_SIZE_COLUMN 4

class key_generator 
{
public:
	static int create_seed();
	static std::string create_key();
	static int calc_random_value(int round);
	static void generate_all_primes();
	static std::vector<int> getTwoPrimes(int rgb_val);
	static void str_to_2d(std::string password, unsigned int arr[BLOCK_SIZE_ROW][BLOCK_SIZE_COLUMN]);
};
