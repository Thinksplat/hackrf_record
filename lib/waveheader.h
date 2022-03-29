#ifndef AB1D2D32_1634_4820_B88E_EFB5DC492AF4
#define AB1D2D32_1634_4820_B88E_EFB5DC492AF4

void WriteWaveFileHeader(FILE *fp, int sample_rate, int num_samples, int num_channels, int bitdepth)
{
    // RIFF header
    fwrite("RIFF", 1, 4, fp);
    int file_size = 36 + num_samples * num_channels * (bitdepth / 8);
    fwrite(&file_size, 4, 1, fp);
    fwrite("WAVE", 1, 4, fp);

    // fmt 
    fwrite("fmt ", 1, 4, fp);
    int subchunk1_size = 16;
    fwrite(&subchunk1_size, 4, 1, fp);
    short audio_format = 1;
    fwrite(&audio_format, 2, 1, fp);
    fwrite(&num_channels, 2, 1, fp);
    fwrite(&sample_rate, 4, 1, fp);
    int byte_rate = sample_rate * num_channels * (bitdepth / 8);
    fwrite(&byte_rate, 4, 1, fp);
    short block_align = num_channels * (bitdepth / 8);
    fwrite(&block_align, 2, 1, fp);
    fwrite(&bitdepth, 2, 1, fp);

    // data
    fwrite("data", 1, 4, fp);
    fwrite(&num_samples, 4, 1, fp);
}


#endif /* AB1D2D32_1634_4820_B88E_EFB5DC492AF4 */
