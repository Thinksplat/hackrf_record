#ifndef AB1D2D32_1634_4820_B88E_EFB5DC492AF4
#define AB1D2D32_1634_4820_B88E_EFB5DC492AF4

#include "IDataSink.h"

void WriteWaveFileHeader(IDataWriter &writer, int sample_rate, int num_samples, int num_channels, int bitdepth)
{
    // RIFF header
    writer.PutData("RIFF", 4);
    int file_size = 36 + num_samples * num_channels * (bitdepth / 8);
    writer.PutData(reinterpret_cast<char*>(&file_size), 4);
    writer.PutData("WAVE", 4);

    // fmt 
    writer.PutData("fmt ", 4);
    int subchunk1_size = 16;
    writer.PutData(reinterpret_cast<char*>(&subchunk1_size), 4);
    short audio_format = 1;
    writer.PutData(reinterpret_cast<char*>(&audio_format), 2);
    writer.PutData(reinterpret_cast<char*>(&num_channels), 2);
    writer.PutData(reinterpret_cast<char*>(&sample_rate), 4);
    int byte_rate = sample_rate * num_channels * (bitdepth / 8);
    writer.PutData(reinterpret_cast<char*>(&byte_rate), 4);
    short block_align = num_channels * (bitdepth / 8);
    writer.PutData(reinterpret_cast<char*>(&block_align), 2);
    writer.PutData(reinterpret_cast<char*>(&bitdepth), 2);

    // data
    writer.PutData("data", 4);
    writer.PutData(reinterpret_cast<char*>(&num_samples), 4);
}


#endif /* AB1D2D32_1634_4820_B88E_EFB5DC492AF4 */
