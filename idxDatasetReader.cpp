#include <ifstream>
#include "idxDatasetReader.h"

using namespace std;

idxDatasetReader::idxDatasetReader()
{	
	
}

~idxDatasetReader::idxDatasetReader()
{
	
}

// I suspect this is not portable :(
// http://stackoverflow.com/questions/280162/is-there-a-way-to-do-a-c-style-compile-time-assertion-to-determine-machines-e
bool idxDatasetReader::isLittleEndian()
{
	union {
	char c[sizeof(int)];
	int n;
	} isLittle = 1;
	
	if (isLittle[0] == 0x00)
		return true;
	else
		return false;		
}

unsigned int idxDatasetReadr::getMagicNumber()
{
	unsigned int tempMagicNumber;	
	return (file >> tempMagicNumber);
}

// returns extracted datatype id from the magic number
char idxDatasetReader::getDatatype()
{
	return (magicNumber >> 8) & 0xFF; // the second MSB
}

// returns the extracted number of dimensions from the magic number
char idxDatasetReader::getNumberOfDimensions()
{
	return (magicNumber & 0xFF);
}

unsigned idxDatasetReader::getNumberOfDatasets()
{
	return file >> nDatasets;
}

// returns total dataset size
unsigned idxDatasetReader::getTotalDatasetSize(unsigned nDimensions)
{
	if (nDimensions > 1)
	return (file.size() - sizeof(unsigned int) * (nDimensions + 2)); // 1: magic number + 1: number of datasets	
	else
	return (file.size() - sizeof(unsigned int) * (nDimensions + 1)); // 1:magic number
}


void idxDatasetReader::getDataset(string inputFileName, unsigned nInputDatasets)
{
	filename = inputFileName;
		
	if(this->file.open(fileName) == NULL)
		{
			cout << "Invalid filename. Please ensure correct filename is specified";
			exit(0);
		}
	
	// extract magic number
	magicNumber = getMagicNumber();
	
	bool isLittleEndian;
		
	if (isLittleEndian() == true)
		{
			isLittleEndian = true;
			magicNumber = __builtin_bswap32(magicNumber);
		}
	
	// get datatype
	char datatypeId; 
	datatypeId = getDatatype();
	
	// get number of dimensions
	char nDimensions;
	nDimensions = getNumberOfDimension();
		
	// get size of dimensions
	sizeOfDimension = getSizeOfDimension();	
	
	// size of dataset as implied by file header:
	unsigned predictedDatasetSize = 1;
	
	for (unsigned i = 0; i < nDimensions; i++)
		predictedDatasetSize *= sizeOfDimension[i];
		
	unsigned actualDatasetSize = getTotalDatasetSize();
	
	bool hasValidFileSize = (predictedDatasetSize == actualDatasetSize) ? true : false;
			
	// check if file contains number of bytes as implied by file header
	if (hasValidFileSize == false)
		{
			cout << "Actual data < data size implied by the header :(";
			exit(0);			
		}
		
	// read dataset
	switch(datatypeId)
	{
		case 0x08:
		data = new unsigned char[nDatasets][totalDatasetSize];
		for (unsigned byteId = 0; byteId < totalDatasetSize; byteId++)
			file.get(data[byteId]); 
		break;
		
		case 0x09: 
		data = new char[nDatasets][totalDatasetSize];
		for (unsigned byteId = 0; byteId < totalDatasetSize; byteId++)
			file.get(data[byteId]); 
		break;
		
		case 0x0B: 
		data = new short[nDatasets][totalDatasetSize]; 
		for (unsigned byteId = 0; byteId < totalDatasetSize; byteId++)
			{
				file.get(data[byteId]); 
				data[byteId] = __builtin_bswap16(magicNumber);
			}
		break;
		
		case 0x0C: 
		data = new int[nDatasets][totalDatasetSize];
		for (unsigned byteId = 0; byteId < totalDatasetSize; byteId++)
			{
				file.get(data[byteId]); 
				data[byteId] = __builtin_bswap32(magicNumber);
			}
		break;
		
		case 0x0D: 
		data = new float[nDatasets][totalDatasetSize];
		for (unsigned byteId = 0; byteId < totalDatasetSize; byteId++)
			{
				file.get(data[byteId]); 
				data[byteId] = __builtin_bswap32(magicNumber);
			}
		break;
		
		case 0x0E: 
		data = new double[nDatasets][totalDatasetSize];
		for (unsigned byteId = 0; byteId < totalDatasetSize; byteId++)
			{
				file.get(data[byteId]); 
				data[byteId] = __builtin_bswap64(magicNumber);
			}
		break;
		
		defualt:
		cout << "Invalid dataset type specified";
		exit(0);
	}	
	
	file.close();
}


// saves dataset in JPEG form
void idxDatsetReader::saveJPEG()
{
	// save training images
	char fileName[MAX_FILENAME_CHAR];
	
	Iplimage opencvImage = cvCreateImage(cvSize(nRows, nColumns), IPL_DEPTH_8U, 1);
	
	for (unsigned imageId = 0; imageId < nImages; imageId++)
	{
		sprintf(fileName, "MNISTDataset/trainImages/%i.jpg", imageId);
			for (unsigned i = 0; i < nPixels; i++)
				opencvImage->ImageData[i] = trainImageData[imageId][i];
	}
}
*/

