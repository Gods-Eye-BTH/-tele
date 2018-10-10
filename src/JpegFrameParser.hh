#ifndef _JPEG_FRAME_PARSER_HH_INCLUDED
#define _JPEG_FRAME_PARSER_HH_INCLUDED
 
 
class JpegFrameParser
{
public:
    JpegFrameParser();
    virtual ~JpegFrameParser();
 
    unsigned char width()     { return _width; }
    unsigned char height()    { return _height; }
    unsigned char type()      { return _type; }
    unsigned char precision() { return _precision; }
    unsigned char qFactor()   { return _qFactor; }
 
    unsigned short restartInterval() { return _restartInterval; }
 
    unsigned char const* quantizationTables(unsigned short& length)
    {
        length = _qTablesLength;
        return _qTables;
    }
 
    int parse(unsigned char* data, unsigned int size);
 
    unsigned char const* scandata(unsigned int& length)
    {
        length = _scandataLength;
 
        return _scandata;
    }
 
private:
    unsigned int scanJpegMarker(const unsigned char* data,
                                unsigned int size,
                                unsigned int* offset);
    int readSOF(const unsigned char* data,
                unsigned int size, unsigned int* offset);
    unsigned int readDQT(const unsigned char* data,
                         unsigned int size, unsigned int offset);
    int readDRI(const unsigned char* data,
                unsigned int size, unsigned int* offset);
 
private:
    unsigned char _width;
    unsigned char _height;
    unsigned char _type;
    unsigned char _precision;
    unsigned char _qFactor;
 
    unsigned char* _qTables;
    unsigned short _qTablesLength;
 
    unsigned short _restartInterval;
 
    unsigned char* _scandata;
    unsigned int   _scandataLength;
};
 
 
#endif /* _JPEG_FRAME_PARSER_HH_INCLUDED */
