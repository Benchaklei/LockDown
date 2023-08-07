#include "key_generator.h"
#define cimg_use_jpeg
#include "CImg.h"
using namespace cimg_library;
using namespace cv;

int m = 192; // const value for equation, for now its just a number i chose but later we will change it to zimbabwe temperature or something
const int MAX_RGB = 255;
bool isPrime[MAX_RGB]; // array to store if number is prime or not 
int random_res = 0;
int prev_result = 0;
bool took_screenshot = false;

/*
Takes a screenshot of user's current screen. Saving the screenshot in the project's dir.
input: path to save the file at
output: none
*/
void take_screenshot(std::string path) 
{
    try 
    {
        //setting to the screen shot
        keybd_event(VK_LWIN, 0, KEYEVENTF_EXTENDEDKEY, 0);

        keybd_event(VK_SNAPSHOT, 0x45, KEYEVENTF_EXTENDEDKEY, 0);
        keybd_event(VK_SNAPSHOT, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);

        keybd_event(VK_LWIN, 0, KEYEVENTF_KEYUP, 0);

        //handler of the bitmap that save the screen shot
        HBITMAP hBitmap;

        //I have to give for it time to make it work in pratica guarda
        Sleep(1000); //Modified

        //take the screen shot
        OpenClipboard(NULL);

        //save the screen shot in the bitmap handler
        hBitmap = (HBITMAP)GetClipboardData(CF_BITMAP);

        //release the screen shot
        CloseClipboard();

        std::vector<BYTE> buf;
        IStream* stream = NULL;
        HRESULT hr = CreateStreamOnHGlobal(0, TRUE, &stream);
        CImage image;
        ULARGE_INTEGER liSize;

        // screenshot to jpg and save to stream
        image.Attach(hBitmap);
        image.Save(stream, Gdiplus::ImageFormatJPEG);
        IStream_Size(stream, &liSize);
        DWORD len = liSize.LowPart;
        IStream_Reset(stream);
        buf.resize(len);
        IStream_Read(stream, &buf[0], len);
        stream->Release();

        // put the imapge in the file
        std::fstream fi;
        fi.open(path, std::fstream::binary | std::fstream::out);
        fi.write(reinterpret_cast<const char*>(&buf[0]), buf.size() * sizeof(BYTE));
        fi.close();
    }
    catch (...) 
    {
        _exit(ERORR_SCREENSHOT); // if screenshot failed - exit program
    }
}

/*
Returns the image dimensions(width and height)
input: path of the image to check
return: int vector with the dimensions
*/
std::vector<int> get_image_dimensions(std::string path) 
{
    std::vector<int> img_dimensions = {};
    CImg<unsigned char> image(path.c_str());

    img_dimensions.push_back(image.width());
    img_dimensions.push_back(image.height());

    return img_dimensions;
}

/*
Returns the RBG value of the image in given position
input: path of image, x and y positions
output: vector with RBG values
*/
int get_pixel_value(std::string path, int posX, int posY)
{
    CImg<unsigned char> src(path.c_str());

    int rgb_index = (posX * posY) % 3; // since we need only 1 value we will choose what value we take like this

    return (int)src(posX, posY, 0, rgb_index);
}

/*
calculates a random value for the seed
our random value will be a pixel
input: none
output: none
*/
int key_generator::calc_random_value(int round)
{
    const std::string screen_shot_name = "ss.jpg";
    int posX = 0, posY = 0;

    if (took_screenshot == false) 
    {
        take_screenshot(screen_shot_name);
        Sleep(500); // pausing just to make sure the screenshot is saved before we proceed
        took_screenshot = true;
    }
    else 
    {
        Mat img_in = imread("ss.jpg");

        if (img_in.empty()) 
        {
            printf("error opening image file!\n");
            _exit(-1);
        }

        Mat img_color;
        applyColorMap(img_in, img_color, (std::time(0) + round) % 12); // 12 is the max code for a color map in opencv
        imwrite("ss.jpg", img_color);
    }

    std::vector<int> img_dimensions = get_image_dimensions(screen_shot_name); //index1=width, index2= height
    std::time_t current_time = std::time(0) * 1000; // get the current time

    posX = current_time % img_dimensions[WIDTH_POS]; // getting pixel x position
    posY = current_time % img_dimensions[HEIGHT_POS]; // getting pixel y position
    
    int rgb_val = get_pixel_value(screen_shot_name, posX, posY);

    return rgb_val + 3;
}

int key_generator::create_seed() 
{
    generate_all_primes(); // generating all the primes needed
    auto primes1 = getTwoPrimes(random_res); 
    int seed = (primes1[0] * primes1[1]) % m;
    
    return seed;
}

std::string key_generator::create_key() 
{
    random_res = key_generator::calc_random_value(0);
    prev_result = key_generator::create_seed();
    std::string final_key_str = "";

    for (int i = 0; i < KEY_SIZE_BYTE; i++) 
    {
        int f = calc_random_value(i);

        auto primes2 = getTwoPrimes(f);
        auto prev_primes = getTwoPrimes(prev_result);

        int b = f * prev_primes[0] * prev_primes[1];
        int a = primes2[0] * primes2[1];

        int x = (prev_result * b + a) % m;

        prev_result = x;

        final_key_str += ('0' + x); // adding the random number generated(X) to the final key string
        if (final_key_str[i] < 0) final_key_str[i] *= -1; 
    }

    return final_key_str;
}

/*
this function will generate all the prime numbers so 
we can use it for the seed calculation and stores it in the primes vector
*/
void key_generator::generate_all_primes()
{
    // assigh value false
    // to the boolean array isprime
    memset(isPrime, true, sizeof(isPrime));

    for (int i = 2; i * i <= MAX_RGB; i++) /// we are using the following algorithm https://www.geeksforgeeks.org/sieve-of-eratosthenes/
    {
        // If isPrime[i] is not changed,
        // then it is a prime
        if (isPrime[i]) 
        {
            // Update all multiples of i greater than or
            // equal to the square of it numbers which are
            // multiple of i and are less than i^2 are
            // already been marked.
            for (int j = i * i; j < MAX_RGB; j += i)
                isPrime[j] = false;
        }
    }
}

/*
function returns the nearest prime less than the value and nearest prime bigger than the value
input: rgb value (int)
output: vector with the 2 prime numbers
*/
std::vector<int> key_generator::getTwoPrimes(int rgb_val)
{
    std::vector<int> primes;

    // Traverse for finding prime just less than the value
    for (int i = rgb_val - 1;; i--) 
    {
        // Prime just less than value found
        if (isPrime[i]) 
        {
            primes.push_back(i);
            break;
        }
    }

    // Traverse for finding prime just greater than the value
    for (int j = rgb_val + 1;; j++) 
    {
        // Prime just greater than value found
        if (isPrime[j]) 
        {
            primes.push_back(j);
            break;
        }
    }
    return primes;
}

/*
The function will cast a key in string format into key in 2d array format
input: string - password(key), 2d int arr - arr to store the casted password in
output: none
*/
void key_generator::str_to_2d(std::string password, unsigned int arr[BLOCK_SIZE_ROW][BLOCK_SIZE_COLUMN]) 
{
    for (int i = 0; i < BLOCK_SIZE_ROW; i++) 
    {
        for (int j = 0; j < BLOCK_SIZE_COLUMN; j++) 
        {
            arr[i][j] = password[i * BLOCK_SIZE_ROW + j];
        }
    }
}