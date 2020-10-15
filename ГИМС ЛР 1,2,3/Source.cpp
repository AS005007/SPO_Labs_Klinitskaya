
#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;
#pragma pack(2)
//Заголовок файла BMP 
typedef struct tBITMAPFILEHEADER
{
	WORD bfType;
	DWORD bfSize;
	WORD bfReserved1;
	WORD bfReserved2;
	DWORD bfOffBits;
}sFileHead;

//Заголовок BitMap's
typedef struct tBITMAPINFOHEADER
{
	DWORD biSize;
	LONG biWidth;
	LONG biHeight;
	WORD biPlanes;
	WORD biBitCount;
	DWORD biCompression;
	DWORD biSizeImage;
	LONG biXPelsPerMeter;
	LONG biYPelsPerMeter;
	DWORD biClrUsed;
	DWORD biClrImportant;
}sInfoHead;

sFileHead FileHead;
sInfoHead InfoHead;
//Пиксель
struct Color
{
	BYTE red;
	BYTE green;
	BYTE blue;
};

//Размер 1-го пикселя
int pixel_size = sizeof(Color);

//1 - BMP, 2 - CMP
int img_type = 0;

//Исходное изображение
Color *src_image = 0;
//Результативное изображение
Color *dst_image = 0;

//Размер изображения
int width = 0;
int height = 0;

//Вывести заголовок BMP файла
void ShowBMPHeaders(tBITMAPFILEHEADER fh, tBITMAPINFOHEADER ih)
{
	cout << "Type: " << (CHAR)fh.bfType << endl;
	cout << "Size: " << fh.bfSize << endl;
	cout << "Shift of bits: " << fh.bfOffBits << endl;
	cout << "Width: " << ih.biWidth << endl;
	cout << "Height: " << ih.biHeight << endl;
	cout << "Planes: " << ih.biPlanes << endl;
	cout << "BitCount: " << ih.biBitCount << endl;
	cout << "Compression: " << ih.biCompression << endl;
}

//Функция для загрузки изображения
bool OpenImage(string path)
{
	ifstream img_file;
	Color temp;
	char buf[3];

	//Открыть файл на чтение
	img_file.open(path.c_str(), ios::in | ios::binary);
	if (!img_file)
	{
		cout << "File isn`t open!" << endl;
		return false;
	}

	//Считать заголовки BMP
	img_file.read((char*)&FileHead, sizeof(FileHead));
	img_file.read((char*)&InfoHead, sizeof(InfoHead));

	img_type = 1;
	ShowBMPHeaders(FileHead, InfoHead);
	//Присвоить длину и ширину изображения
	width = InfoHead.biWidth;
	height = InfoHead.biHeight;


	//Выделить место под изображение
	src_image = new Color[width*height];

	int i, j;
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			img_file.read((char*)&temp, pixel_size);
			src_image[i*width + j] = temp;
		}
		//Дочитать биты используемые для выравнивания до двойного слова
		img_file.read((char*)buf, j % 4);
	}
	img_file.close();

	return true;
}

//Функция для сохранения изображение
bool SaveImage(string path)
{
	ofstream img_file;
	char buf[3];

	//Открыть файл на запись
	img_file.open(path.c_str(), ios::out | ios::binary);
	if (!img_file)
	{
		return false;
	}

	img_file.write((char*)&FileHead, sizeof(FileHead));
	img_file.write((char*)&InfoHead, sizeof(InfoHead));

	//Скопировать из исходного в результирующее изображение
	//if (dst_image == 0)
	//{
		dst_image = new Color[width*height];
		memcpy(dst_image, src_image, width*height * sizeof(Color));
	//}

	//Записать файл
	int i, j;
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			img_file.write((char*)&dst_image[i*width + j], pixel_size);
		}
		img_file.write((char*)buf, j % 4);
	}
	img_file.close();

	return true;
}

//Скопировать содержимое результируещего изображения в начальное
void CopyDstToSrc()
{
	if (dst_image != 0)
	{
		memcpy(src_image, dst_image, width*height * sizeof(Color));
	}
}

//Зашумление изображения с заданной долей вероятности
void AddNoise(double probability)
{
	int size = width * height;
	int count = (int)(size*probability) / 100;
	int x, y;
	long pos;
	for (int i = 0; i < count; i++)
	{
		x = rand() % width;
		y = rand() % height;
		pos = y * width + x;
		src_image[pos].blue = rand() % 256;
		src_image[pos].green = rand() % 256;
		src_image[pos].red = rand() % 256;
	}
	cout << "Point was added: " << count << endl;
}
//task1


//Отобразить текущее изображение с помощью вызова стандартного просмотрщика
void ShowImage(string path)
{
	ShowBMPHeaders(FileHead, InfoHead);
	system(path.c_str());
}

//Считать путь к изображению
void ReadPath(string &str)
{
	str.clear();
	cout << "Enter path to image" << endl;
	cin >> str;
}

void Filter();

void Wallis_Operator();

void Lab3Task();

int main(int argc, char* argv[])
{
	srand((unsigned)time(NULL));

	//Путь к текущему изображению
	string path_to_image, temp;
	//First lab "Threshold Filter" compiling
	bool flag1 = true;
	if (flag1) {
		ReadPath(path_to_image);
		OpenImage(path_to_image);
		ShowImage(path_to_image);
		AddNoise(15);
		ReadPath(temp);
		SaveImage(temp);
		ShowImage(temp);
		Filter();
		ReadPath(temp);
		SaveImage(temp);
		ShowImage(temp);
	}
	bool flag2 = true;
	//Reinitializing of "path_t_im" and "temp" variables for the Lab2 "Wallis Operator"
	if (flag2) {
		ReadPath(path_to_image);
		OpenImage(path_to_image);
		ShowImage(path_to_image);
		Wallis_Operator();
		ReadPath(temp);
		SaveImage(temp);
		ShowImage(temp);
	}
	//Lab3 Attenuate the color of any channel that user choosed for some number
	bool flag3 = true;
	if (flag3) {
		ReadPath(path_to_image);
		OpenImage(path_to_image);
		ShowImage(path_to_image);
		Lab3Task();
		ReadPath(temp);
		SaveImage(temp);
		ShowImage(temp);
	}
	//Освободить память исходного изображения
	if (src_image != 0)
	{
		delete[] src_image;
	}
	//Освободить память результрующего изображения
	if (dst_image != 0)
	{
		delete[] dst_image;
	}

	return 0;
}

//Lab1
//Threshold Filter code realization
void Filter()
{
	//Initializing Threshold value (e)
	cout << "Enter Threshold value" << endl;
	int Threshold; cin >> Threshold;
	long pos, pos1, pos2;
	int Y1, Y2, Y3;
	//start of the algorithm
	for (int i = 1; i < height - 1; i++) {
		for (int j = 0; j < width - 1; j++) {
			/*find the poses of our 1x3 window:
			  [pos1]
			  [pos]
			  [pos2]
			 */
			pos = i * width + j;
			pos1 = (i - 1) * width + j;
			pos2 = (i + 1) * width + j;
			//calculate the average values for each channel, subtract them from the 
			//center pixel and compare the res with Threshold 
			Y1 = (src_image[pos1].red + src_image[pos2].red) / 2;
			//if the difference between channel values is greater than threshold, then we
			//change the center pixel channel value for average of two neighbors
			if (src_image[pos].red - Y1 > Threshold) {
				src_image[pos].red = Y1;
			}
			//the same operation do for every of 2 others channels
			Y2 = (src_image[pos1].green + src_image[pos2].green) / 2;
			if (src_image[pos].green - Y2 > Threshold) {
				src_image[pos].green = Y2;
			}
			Y3 = (src_image[pos1].blue + src_image[pos2].blue) / 2;
			if (src_image[pos].blue - Y3 > Threshold) {
				src_image[pos].blue = Y3;
			}
		}
	}
}

//Lab2
//Wallis Operator code realization
void Wallis_Operator() {

	//Choose the Threshold (0 the best cuz of ln.)
	double Threshold;
	cout << "Choose the Threshold" << endl;
	cin >> Threshold;

	//dynamic init of array F[i] and F*[i]
	int size = width * height;
	double* F_calc = new double[size];
	double* F_result = new double[size];

	//Calculate the F[i] and F*[i] arrays
	for (int i = 0; i < size; i++) {
		F_calc[i] = 0.299 * src_image[i].red + 0.587 * src_image[i].green + 0.114 * src_image[i].blue;
		F_result[i] = F_calc[i];
	}

	//start of the algorythm
	int posC, posT, posB, posL, posR;
	for (int i = 1; i < height - 1; i++) 
	{
		for (int j = 0; j < width - 1; j++) 
		{
			/*Find the poses of all 5 pixels
				  [posT]
			[posL][posC][posR]
				  [posB]
			*/
			posC = i * width + j;
			posT = (i - 1) * width + j;
			posB = (i + 1) * width + j;
			posL = i * width + j - 1;
			posR = i * width + j + 1;

			//Calculate new F*[i] value by the Wallis operator formula
			float numerator = pow(F_calc[posC], 4);
			float denominator = F_calc[posT] * F_calc[posB] * F_calc[posL] * F_calc[posR];
			F_result[posC] = log((numerator / denominator));
			
			//And now we check if our F*[i] higher or less than the threshold we choose
			if (Threshold >= F_result[posC]) {
				src_image[posC].red = 255;
				src_image[posC].green = 255;
				src_image[posC].blue = 255;
			}
			else {
				src_image[posC].red = 0;
				src_image[posC].green = 0;
				src_image[posC].blue = 0;
			}
		}
	}
	delete[] F_calc;
	delete[] F_result;
}

void Lab3Task() {
	int size = width * height;
	//Channel choosing
	cout << "Choose the channel u want to attenuate:\n1-Red\n2-Green\n3-Blue" << endl;
	int channel;
	cin >> channel;


	//Enter the number of attenuation
	cout << "Enter the number of attenuation" << endl;
	int number;
	cin >> number;
	//Algorythm
	for (int i = 0; i < size; i++) {
		switch (channel)
		{
		case 1: src_image[i].red -= number;
		case 2: src_image[i].green -= number;
		case 3: src_image[i].blue -= number;
		/*default: cout << "No channel were choosed" << endl;
			break;*/
		}
	}
}