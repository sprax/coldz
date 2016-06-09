/*****************************
 beymer/math.h 
 *****************************/
float *make_vector(float x, float y, float z);
int copy_matrix(float **dest, float **source, int l1, int l2);
int scale_matrix(float **dest, float **source, float factor, int l1, int l2);
int multiply_matrices(float **dest, float **source1, float **source2, int l1, int l2, int l3);
int multiply_matrix_vector(float *dest, float **matrix, float *vector, int l1, int l2);
int transpose_matrix(float **dest, float **source, int l1, int l2);
int invert_matrix(float **dest, float **source, int n);
int rotation_matrix_x(float **dest, double rad);
int rotation_matrix_y(float **dest, double rad);
int rotation_matrix_z(float **dest, double rad);


