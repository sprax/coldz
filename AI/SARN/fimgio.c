/* Float image I/O routines from files */

#define CHECK_FOR_OLD_IMAGE_FORMAT
#include "imgdec.h"
#include "imghdr.h"

/******************************************************************************/

int read_fimage(char *filename, FIMAGE *image)
{
    FILE *stream;

    if ((stream = fopen(filename, "r")) == NULL) {
        fprintf(stderr, "read_fimage: Couldn't open filter file %s\n", filename);
        return(0);
    }

    read_fimgstr(stream, image);
    fclose(stream);
    return(1);
}

/*****************************************************************************/

void write_fimage(char *filename, FIMAGE image)
{
    FILE *stream;
    int filetype();

    if ((stream = fopen(filename, "w")) == NULL)
        die("write_fimage: Couldn't open %s\n", filename);
    switch (filetype(filename)) {
        case UBYTE:
        {  BIMAGE scr;
        scr.w = image.w;  scr.h = image.h;  alloc_bimage(&scr);
        ftob(scr, image);
        write_bimgstr(stream, scr);
        free_bimage(scr);
        }
        break;
        case FLOAT:
        case DUNNO:
            write_fimgstr(stream, image);
            break;
        case SHORT:
        {  SIMAGE scr;
        scr.w = image.w;  scr.h = image.h;  alloc_simage(&scr);
        ftos(scr, image);
        write_simgstr(stream, scr);
        free_simage(scr);
        }
        break;
        case LONG:
        {  LIMAGE scr;
        scr.w = image.w;  scr.h = image.h;  alloc_limage(&scr);
        ftol(scr, image);
        write_limgstr(stream, scr);
        free_limage(scr);
        }
        break;
        default:
            die("write_fimage: file %s is of unknown type\n", filename);
    }
    fclose(stream);
}

/******************************************************************************/

read_fimgstr(stream, image)
FILE *stream;
FIMAGE *image;
{
    IMAGE_HEADER hdr;
    char *typestr();

    if (fread(&hdr, sizeof(hdr), 1, stream) != 1)
        die("read_fimgstr: end of file on stream\n");
#ifdef CHECK_FOR_OLD_IMAGE_FORMAT
    if (!hdr.width)
        die("read_bimgstr(): Input file seems to be in the old image format\n     Use oip2nip to update file to new image format\n");
#endif
    image->w = hdr.width;
    image->h = hdr.height;
    alloc_fimage(image);
    switch (hdr.type) {
        case UBYTE:
        {  BIMAGE scr;
        scr.w = image->w;  scr.h = image->h;  alloc_bimage(&scr);
        _read_bimgstr(stream, scr);
        btof(*image, scr);
        free_bimage(scr);
        }
        break;
        case FLOAT:
        case DUNNO:
            _read_fimgstr(stream, *image);
            break;
        case SHORT:
        {  SIMAGE scr;
        scr.w = image->w;  scr.h = image->h;  alloc_simage(&scr);
        _read_simgstr(stream, scr);
        stof(*image, scr);
        free_simage(scr);
        }
        break;
        case LONG:
        {  LIMAGE scr;
        scr.w = image->w;  scr.h = image->h;  alloc_limage(&scr);
        _read_limgstr(stream, scr);
        ltof(*image, scr);
        free_limage(scr);
        }
        break;
        case BYTE:
        case USHORT:
            die("read_fimgstr: Unsupported image type %s\n", typestr(hdr.type));
        default:
            die("read_fimgstr: Unknown image type %d\n", hdr.type);
    }
}

/******************************************************************************/

write_fimgstr(stream, image)
FILE *stream;
FIMAGE image;
{
    register float **p = image.ptr, **fp;
    register int w = image.w, h = image.h;
    IMAGE_HEADER hdr;

    bzero((char *)&hdr, sizeof(hdr));
    hdr.width = w;  hdr.height = h;  hdr.type = FLOAT;  hdr.depth = 1;
    fwrite(&hdr, sizeof(hdr), 1, stream);
    for (fp = p + h; p < fp; p++)
        if (fwrite(*p, sizeof(**p), w, stream) == 0)
            die("write_fimgstr: Error while writing to stream\n");
}

/******************************************************************************/

_read_fimgstr(stream, image)
FILE *stream;
FIMAGE image;
{
    register float **p = image.ptr, **fp;
    register int w = image.w, h = image.h;

    for (fp = p + h; p < fp; p++)
        if (fread(*p, sizeof(**p), w, stream) != w)
            die("_read_fimgstr: End of file on stream\n");
}

