/* Byte image I/O routines from files */

#define CHECK_FOR_OLD_IMAGE_FORMAT
#include "imgdec.h"
#include "imghdr.h"

//#include "bimgio.proto.h"
void _read_bimgstr(FILE *stream, BIMAGE image);
void write_bimgstr(FILE *stream, BIMAGE image);

/******************************************************************************/

read_bimage(char *filename, BIMAGE *image)
{
    FILE *stream;

    errno_t err = fopen_s(&stream, filename, "r");
    if (err)
        return(0);
    read_bimgstr(stream, image);
    fclose(stream);
    return(1);
}

/*****************************************************************************/

void write_bimage(char *filename, BIMAGE image)
{
    FILE *stream;
    int filetype();

    if ((stream = fopen(filename, "w")) == NULL)
        die("write_bimage: Couldn't open %s\n", filename);
    switch (filetype(filename)) {
        case UBYTE:
        case DUNNO:
            write_bimgstr(stream, image);
            break;
        case FLOAT:
        {  FIMAGE scr;
        scr.w = image.w;  scr.h = image.h;  alloc_fimage(&scr);
        btof(scr, image);
        write_fimgstr(stream, scr);
        free_fimage(scr);
        }
        break;
        case SHORT:
        {  SIMAGE scr;
        scr.w = image.w;  scr.h = image.h;  alloc_simage(&scr);
        btos(scr, image);
        write_simgstr(stream, scr);
        free_simage(scr);
        }
        break;
        case LONG:
        {  LIMAGE scr;
        scr.w = image.w;  scr.h = image.h;  alloc_limage(&scr);
        btol(scr, image);
        write_limgstr(stream, scr);
        free_limage(scr);
        }
        break;
        default:
            die("write_bimage: file %s is of unknown type\n", filename);
    }
    fclose(stream);
}


/******************************************************************************/

void read_bimgstr(FILE *stream, BIMAGE *image)
{
    IMAGE_HEADER hdr;
    char *typestr();

    if (fread(&hdr, sizeof(hdr), 1, stream) != 1)
        die("read_bimgstr: end of file on stream\n");
#ifdef CHECK_FOR_OLD_IMAGE_FORMAT
    if (!hdr.width)
        die("read_bimgstr(): Input file seems to be in the old image format\n     Use oip2nip to update file to new image format\n");
#endif
    image->w = hdr.width;
    image->h = hdr.height;
    alloc_bimage(image);
    switch (hdr.type) {
        case UBYTE:
        case DUNNO:
            _read_bimgstr(stream, *image);
            break;
        case FLOAT:
        {  FIMAGE scr;
        scr.w = image->w;  scr.h = image->h;  alloc_fimage(&scr);
        _read_fimgstr(stream, scr);
        ftob(*image, scr);
        free_fimage(scr);
        }
        break;
        case SHORT:
        {  SIMAGE scr;
        scr.w = image->w;  scr.h = image->h;  alloc_simage(&scr);
        _read_simgstr(stream, scr);
        stob(*image, scr);
        free_simage(scr);
        }
        break;
        case LONG:
        {  LIMAGE scr;
        scr.w = image->w;  scr.h = image->h;  alloc_limage(&scr);
        _read_limgstr(stream, scr);
        ltob(*image, scr);
        free_limage(scr);
        }
        break;
        case BYTE:
        case USHORT:
            die("read_bimgstr: Unsupported image type %s\n", typestr(hdr.type));
        default:
            die("read_bimgstr: Unknown image type %d\n", hdr.type);
    }
}

/******************************************************************************/

void write_bimgstr(FILE *stream, BIMAGE image)
{
    register byte **p = image.ptr, **fp;
    register int w = image.w, h = image.h;
    IMAGE_HEADER hdr;

    bzero((char *)&hdr, sizeof(hdr));
    hdr.width = w;  hdr.height = h;  hdr.type = UBYTE;  hdr.depth = 1;
    fwrite(&hdr, sizeof(hdr), 1, stream);
    for (fp = p + h; p < fp; p++)
        if (fwrite(*p, sizeof(**p), w, stream) == 0)
            die("write_bimgstr: Error while writing to stream\n");
}

/******************************************************************************/

void _read_bimgstr(FILE *stream, BIMAGE image)
{
    register byte **p = image.ptr, **fp;
    register int w = image.w, h = image.h;

    for (fp = p + h; p < fp; p++)
        if (fread(*p, sizeof(**p), w, stream) != w)
            die("_read_bimgstr: End of file on stream\n");
}

