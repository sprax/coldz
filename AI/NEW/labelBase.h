/* label_base.c */

List
readImgLabels(char *pathname);

List
dupImgLabels (List src);

img_label_t
*findImgLabel (char *label_name, List img_labels);


