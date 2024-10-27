#include "BmpLoader.hpp"

// Структура для хранения заголовка BMP файла
#pragma pack(push, 1)
struct BMPHeader
{
	uint16_t fileType;	   // Тип файла, должен быть 'BM'
	uint32_t fileSize;	   // Размер файла
	uint16_t reserved1;	   // Зарезервировано, должно быть 0
	uint16_t reserved2;	   // Зарезервировано, должно быть 0
	uint32_t offsetData;   // Смещение до начала данных изображения
	uint32_t size;		   // Размер этого заголовка (обычно 40)
	int32_t width;		   // Ширина изображения
	int32_t height;		   // Высота изображения
	uint16_t planes;	   // Число цветовых плоскостей, должно быть 1
	uint16_t bitCount;	   // Число бит на пиксель (1, 4, 8, или 24)
	uint32_t compression;  // Тип сжатия (0 = без сжатия)
	uint32_t sizeImage;	   // Размер изображения
	int32_t xPelsPerMeter; // Горизонтальное разрешение
	int32_t yPelsPerMeter; // Вертикальное разрешение
	uint32_t clrUsed;	   // Количество используемых цветов
	uint32_t clrImportant; // Количество важных цветов
};
#pragma pack(pop)

bool BmpLoader::loadBMPTexture(const char *filename, std::vector<unsigned char>& data, int32_t& height, int32_t& width)
{
	BMPHeader header;

	std::ifstream file(filename, std::ios::binary);
	if (!file)
	{
		std::cerr << "Failed to open BMP file: " << filename << std::endl;
		return false;
	}

	file.read(reinterpret_cast<char *>(&header), sizeof(header));
	if (header.fileType != 0x4D42)
	{
		std::cerr << "Not a valid BMP file: " << filename << std::endl;
		return false;
	}
	height = header.height;
	width = header.width;

	file.seekg(header.offsetData, std::ios::beg);

	int imageSize = header.width * header.height * (header.bitCount / 8);
	data.clear();
	data.resize(imageSize);
	file.read(reinterpret_cast<char *>(data.data()), imageSize);
	file.close();

	return true;
}
