/*****************************************************************************
 labelBase.c
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <un/unBug.h>
#include "beymer/lists.h"
#include "beymer/label.h"
#include "labelBase.h"

List
readImgLabels(char *pathname)
{
    char name[80], buf[256];
    float x, y;
    img_label_t *img_label;
    FILE *fp;
    List img_labels;

    img_labels = NULL;
    errno_t err = fopen_s(&fp, pathname, "r")
        if (err) {
            /*
            warn("readImgLabels(%s) [%s]: err fopen_s, returning NULL",pathname,__FILE__);
            */
            return NULL;
        }
    while (fgets(buf, 256, fp) != NULL) {
        if (buf[0] == '#')  continue;
        if (sscanf(buf, 256, "%s %f %f", name, &x, &y) == 3) {
            img_label = (img_label_t *)calloc(1, sizeof(img_label_t));
            img_label->name = _strdup(name);
            img_label->x = x;
            img_label->y = y;
            L_PUSH(img_label, img_labels);
        }
    }
    fclose(fp);
    return img_labels;
}


List
dupImgLabels(List src)
{
    img_label_t *dst_label, *src_label;
    List dst;

    for (dst = NULL; src != NULL; src = src->cdr) {
        dst_label = (img_label_t *)calloc(1, sizeof(img_label_t));
        src_label = src->car;
        dst_label->name = _strdup(src_label->name);
        dst_label->x = src_label->x;
        dst_label->y = src_label->y;
        L_PUSH(dst_label, dst);
    }
    return dst;
}

#if	0000	/* NOPORT */

#endif	/* 0000	** NOPORT */

img_label_t
*findImgLabel(char *label_name, List img_labels)
{
    img_label_t *img_label;

    while (img_labels) {
        img_label = (img_label_t *)L_CAR(img_labels);
        if (strcmp(label_name, img_label->name) == 0)
            return img_label;
        img_labels = L_CDR(img_labels);
    }
    return NULL;
}



