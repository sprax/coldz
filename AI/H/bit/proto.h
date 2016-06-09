/*
 * $Id: proto.h,v 0.87 1993/10/11 00:00:00 zhao Pre-Release $
 *
 *. This file is part of BIT package.
 *
 *
 *  Copyright(c) 1993 by T.C. Zhao.  All rights reserved.
 *
 *  Permission to do anything with this software non-commercial and
 *  without fee is hereby granted, provided that the copyright notice
 *  and disclaimer appear in all copies and their documentation.
 *
 *  This software is provided "as is" without expressed or implied
 *  warranty of any kind.
 *.
 *
 * Function prototypes. All char * is const while char [] is not
 *
 */
#ifndef BIT_PROTO_H
#define BIT_PROTO_H

/* Just something to make compiler happy in case some of the definations
 * are not included
 */
#if  !defined(BIT_H) && !defined(BIT_UTYPE_H)
typedef void (*IPTR);
typedef void (*FLIST);
typedef void (*CMPTR);
typedef void (*FL_OBJECT);
typedef void (*FL_FORM);
#endif

/**********************************************************
 * General stuff
 *********************************************************/
extern void version_cb(FL_OBJECT *, long);
extern const char *is_image_file(const char *);
extern IPTR open_image(const char *);
extern IPTR open_image_fp(FILE *, const char *);
extern IPTR load_image(const char *);
extern IPTR load_image_fp(FILE *, const char *);
extern void close_image(IPTR);
extern int write_image(const IMG_IO *, IPTR, const char *);
extern void Generic_display(IPTR, int, int);
extern void display_image(IPTR, int, int);
extern const char *gds_string(void);
extern FILE *msg_fopen(const char *, const char *);
extern FILE *get_BITfile_fp(const char *, const char *);
extern FILE *get_HELPfile_fp(const char *, const char *);
extern char *get_TMPfile(const char *);
extern FILE *get_TMPfile_fp(const char *, const char *);
extern const char *get_HELPFile(const char *);


extern void bit_iconify(void);
extern void bit_de_iconify(void);
extern void bit_draw_icon(int);

/***********************************************************
 * Supported image formats
 *********************************************************{*/

/*---------------- IRIS RGB format -------------------------- */
extern int RGB_desc(IPTR), RGB_load(IPTR), RGB_dump(IPTR);

/*---------------- PNM format --------------------------------*/
extern int PNM_desc(IPTR);
extern int PPM_load(IPTR), PPM_dump(IPTR);
extern int PPMdump_init(IPTR);
extern const char *PPM_wdefault(const IPTR);
extern int PBM_load(IPTR), PBM_dump(IPTR);
extern void set_pnm_raw(int);

/*---------------- JFIF  format --------------------------------*/
extern int JPEG1_desc(IPTR), JPEG1_load(IPTR), JPEG1_dump(IPTR);
extern int JPEG1dump_init(IPTR);
extern const char *JPEG1_wdefault(const IPTR im);

/*---------------- X bitmaps -----------------------------------*/
extern int XBM_desc(IPTR), XBM_load(IPTR), XBM_dump(IPTR);
extern void bw_special(IPTR);

/*---------------- CompuServ GIF format ------------------------*/
extern int GIF_desc(IPTR), GIF_load(IPTR), GIF_dump(IPTR);
extern int GIFdump_init(IPTR);
extern const char *GIF_wdefault(const IPTR);

/*--------------- PostScript: writing only. --------------------*/
extern int PS_desc(IPTR), PS_dump(IPTR), PSdump_init(IPTR);
extern const char *PS_wdefault(const IPTR);

/*--------------- Sun Rasterfile format   ----------------------*/
extern int RAS_desc(IPTR), RAS_load(IPTR), RAS_dump(IPTR);

/*---------- Formats not directly supported, but cheats -------- */

/* Compressed image: reading only.  */
extern int Compress_desc(IPTR), Compress_load(IPTR);

/* TIFF support thru fromtiff/totiff or tifftopnm/pnmtotiff     */
extern int TIFF_desc(IPTR), TIFF_dump(IPTR), TIFF_load(IPTR);
extern int TIFF_dumpinit(IPTR);
extern const char *TIFF_wdefault(const IPTR);

/*----------- X-ray photo-emission data hologram ---------------*/
extern int HOLO_desc(IPTR), HOLO_load(IPTR), HOLO_dump(IPTR);
extern const char *HOLO_wdefault(const IPTR);
extern int HOLOdump_init(IPTR);
extern void HOLO_special(IPTR);

/* my own stuff */
extern int TC_desc(IPTR), TC_load(IPTR);

/*---------- swap file ------------------------------------------*/
extern int TMP_desc(IPTR), TMP_load(IPTR), TMP_dump(IPTR);
extern int TMPdump_init(IPTR);
extern const char *TMP_wdefault(const IPTR);

/************** End of formats *******************************}*/

/***************************************************************
 * General 2D raster transformations
 **************************************************************/
extern int img_tran(IPTR, int, int, float[][2], float[], rgba_t, int);

/*
 * image panning and zooming
 */
extern void center_image(IPTR, int);
extern int get_pan_speed(void);
extern int image_pan(IPTR, int, int);
extern int image_zoom(IPTR);
extern void auto_pan(IPTR);

extern long show_info_window(int, int, int, int, int);
extern int save_win_position(int);
extern long show_control_window(int, int, int, int, int);
extern void set_control_window_position(int, int);
extern void set_info_window_position(int, int);
extern void write_control_window_position(FILE * fp);
extern void write_info_window_position(FILE * fp);
extern void show_mouse_position(int, int);
extern void hide_mouse_position(void);
extern long get_info_wid(void);
extern long get_control_wid(void);
extern void hide_info_window(void);
extern void hide_control_window(void);
extern void put_up_clock_mail(int, int, int, int);
extern void remove_clock_mail(void);
extern void update_filename(char *);
extern void update_format_info(IPTR);
extern void update_image_info(IPTR);
extern void update_color_info(IPTR);
extern void update_size_info(int, int);
extern char *rm_rcs_kw(const char *);
extern void set_iformat_info(IPTR, const char *);
extern void fb_to_ras(IPTR, int, int, int, int);
extern void slideshow_off(void);

/*
 * active list
 */
extern int add_to_list(const char *, const char *);
extern int delete_from_list(const char *, const char *);
extern int delete_from_list1(const char *);
extern int is_on_list(const char *, const char *);
extern const char *prev_file(void);
extern const char *next_file(void);
extern void load_files(void);
extern int make_list(int, char *[]);

/****************************************************************
 * Image manipulation or info reporting
 ***************************************************************/
extern int do_crop(IPTR);
extern int img_diff(IPTR);
extern int do_scp(IPTR);	/* simple cut & paste */
extern int do_text(IPTR);	/* add text to image */
extern int do_cut_paste(IPTR);
extern int do_coledit(IPTR);
extern int do_editmap(IPTR);
extern int do_repaint(IPTR);
extern int do_scale(IPTR);
extern int do_rotate(IPTR);
extern int do_ext_convolv(IPTR);
extern int do_ext_filter(IPTR);
extern int do_dump(IPTR);
extern int do_merge(IPTR);
extern int do_hspec(IPTR);

extern int img_smooth(IPTR);
extern int img_fft(IPTR);
extern int img_paint(IPTR);
extern int img_enhance(IPTR);
extern int img_sharpen(IPTR);
extern int img_norm(IPTR);
extern int img_measure(IPTR);
extern int img_edge(IPTR);
extern int pixel_edit(IPTR);

extern void pewin_iconify(void);
extern void pewin_de_iconify(void);

extern int no_of_cut_paste_buffers(void);
extern const char *cut_paste_buffer_name(int);
extern void cleanup_cut_paste(void);

extern int init_text(void);
extern int number_of_text(void);
extern const char *get_text_fontname(int);
extern const char *get_text_string(int);
extern float get_text_fontsize(int);
extern void get_text_location(int, int *, int *);
extern int get_text_placement(int);
extern void get_text_color(int, int *, int *, int *);
extern float get_text_rotation(int);
extern void text_to_gray(void);
extern void text_to_cmap(void);
extern void dump_text(FILE *);
extern void get_text_bounds(IPTR, int *, int *, int *, int *);
extern void img_add_text(IPTR, const char *, const char *, float,
			 float, rgba_t, int, int, int, rgba_t,
			 int, rgba_t, int, int);
extern int get_fm_cache(void);
extern int get_fm_cacheused(void);

extern void load_text(IPTR);
extern void set_marking_origin(int, int);
extern void set_marking_units(float, float);
extern int img_marking(IPTR);

extern int img_show_histogram(IPTR);
extern int ibrowse_dir(const char *);
extern void free_all_ibrowsers(void);

extern int img_scale(IPTR, int, int, int, int, rgba_t);
extern int img_convolv(IPTR, int **, int, int, int *, const char *);

/* on matrices */
extern int rgb_convolve(pc_t **, pc_t **, pc_t **, int, int,
			int **, int, int, int *, const char *);
extern int gray_convolve(pc_t **, pc_t **, pc_t **, int, int,
			 int **, int, int, int *, const char *);

extern void update_dumpinfo(const char *);
extern void free_all_ext_filter(void);
extern void free_all_ext_convolv(void);

extern void display_text(IPTR);
extern void del_text(void);
extern void modify_rgb(rgba_t *, long, pc_t *, pc_t *, pc_t *);
extern void modify_cmap(CMPTR, pc_t *, pc_t *, pc_t *);
extern void modify_all_ci(ci_t *, long, ci_t *);
extern void squeeze_image(IPTR);
extern void do_ci_mask(ci_t *, unsigned int, unsigned int);
extern void img_preserve_wm_colors(IPTR);

extern void display_sgf(IPTR);
extern void del_marking(void);
extern int number_of_sgf(void);
extern const char *sgf_name(int);
extern const char *sgf_psdraw(int);
extern void get_sgf_color(int, int *, int *, int *);
extern void get_sgf_info(int, int *, int *, int *, int *, int *, int *, int *);
extern void get_sgf_bounds(IPTR, int *, int *, int *, int *);
extern void sgf_to_gray(void);
extern void sgf_to_cmap(void);
extern void dump_sgf(FILE *);
extern void load_sgf(IPTR);
/*
 * Progress report and misc. info reporting
 */
extern long progress_report(const char *, long);
extern void update_progress_report(long);
extern void remove_progress_report(void);
extern void showit(const char *);

extern void show_rect_ori(int, int);
extern void show_rect_size(int, int);
extern void hide_rect_size(void);
extern void show_rect_speed(int);
extern void show_rect_all(int, int, int, int, int);
extern void hide_rect_all(void);
extern void show_misc_info(const char *);
extern void hide_misc_info(void);
extern void show_misc_info2(const char *);
extern void hide_misc_info2(void);

/******************************************************
 * Window manager resize, win position change handler
 ******************************************************/
extern void install_wm_handler(WMhandler);
extern void remove_wm_handler(WMhandler);
extern int total_wm_handler(void);

/*******************************************************
 * Multiwindow redraw handlers
 *****************************************************/
extern void install_multiw_handler(MWhandler);
extern void remove_multiw_handler(MWhandler);

/*****************************************************
 * GL events handler
 *****************************************************/
extern void install_GLQ_handler(GLQhandler);
extern void remove_GLQ_handler(GLQhandler);
extern int total_GLQ_handler(void);


extern void bit_init(void);
extern int get_scr_dpi(void);
extern void repaint(IPTR, int);
extern void clean_up(void);
extern void cleanup_files(void);
extern void process_clean_up(void);

/*
 * configuration
 */
extern void config_sys(void);
extern void init_configsys(void);
extern int load_optfile(const char *, int);

/* input/output  */
extern int getint(const char *, int *, int, int, int);
extern int getfloat(const char *, float *, float, float, int, int);
extern const char *getstring(const char *, const char *, int);
extern int yes_no(const char *, const char *, const char *, int where);
extern void TC_continue(const char *, const char *, const char *, int);
extern void set_message_color(int, int, int);
extern void get_message_color(int *, int *, int *);

extern void get_theta(int, int, float *);

extern void rect_redraw(IPTR, int, int, int, int);
extern void dbl_rect_redraw(IPTR, int, int, int, int);
extern void draw_subimage(IPTR, int, int, int, int);
extern int check_signature(FILE *, int[], const char *);
extern void *get_subimage(IPTR, int, int, int, int);
extern void *no_fail_get_subimage(IPTR, int, int, int, int);
extern int put_subimage(IPTR, void *, int, int, int, int);
extern int enlarge_image(IPTR, int, int, int, int, rgba_t);


/*--------------------------------------------------------------
 * Color selection
 *-------------------------------------------------------------*/

typedef void (*Getcolor_cb) (int[]);

extern void set_getcolor_cb(Getcolor_cb);
extern void set_getcolor_glcb(GLQhandler);

extern int get_color(IPTR, int[], int);
extern void set_getcolor(int[]);
extern void current_color(int[]);
extern int get_RGBcolor(IPTR, int[], int);
extern int get_cmapcolor(IPTR, int[], int);
extern void set_getcolor_nohide(int);
extern void hide_getcolor(void);

/*
 * prototypes from iutil.c
 */
extern const char *image_vital_info(IPTR);
extern void img_free_rastermem(IPTR);
extern void img_free_rgbmem(IPTR);
extern int img_get_rastermem(IPTR);
extern void img_replace_rgb(IPTR, pc_t **, pc_t **, pc_t **);
extern int img_get_rgbmem(IPTR);
extern int img_get_rgb(IPTR);
extern int image_ready(IPTR, const char *);
extern int img_get_histogram(IPTR);
extern IPTR img_dup(IPTR);
extern int img_dup_raster(IPTR, IPTR);
extern int img_dup_cmap(IPTR, IPTR);
extern void check_emergency(void);
extern void pausing(IPTR);
extern void free_image(IPTR);
extern void free_image_mem(IPTR);
extern void img_free_cmap(IPTR);
extern IPTR get_mem_imgptr(void);
extern void free_mem_imgptr(IPTR);
extern int img_get_border(IPTR, int *, int *, int *, int *, int);
extern CMPTR get_mem_cmap(void);
extern void free_mem_cmap(CMPTR);
extern int best_format(IPTR, const char *[]);
extern void enumerate_formats(void (*)(const char *));
extern void scale_color(int[], float, int[], int, int);

extern void sim_dbl_swap(IPTR, int, int, void *, int, int, int, int, int);
extern void sim_op_dbl_swap(IPTR, int, int, void *, int, int, int, int);

extern void normal_pc_vec(pc_t *, int, int, int);
extern void interpolate_pc_vec(pc_t *, int, pc_t *, int);
extern void get_bit_defmap(int, pc_t *, pc_t *, pc_t *, int);
extern int get_number_of_defmaps(void);
/*
 * debug mem
 */
extern void set_mem_warn(int);
extern int get_mem_warn(void);
extern void mem_stat(void);

/*
 * colormap routines
 */
extern void set_colmap(int, pc_t *, pc_t *, pc_t *);
extern void set_cmap(CMPTR);
extern void set_cmap_entry(CMPTR, int p[], int);
extern void get_cmap_entry(CMPTR, int p[], int);
extern void get_colmap(int, pc_t *, pc_t *, pc_t *);
extern void pack_cmap(CMPTR);
extern void pack_cmap_g(CMPTR);
extern void get_sysmap(void);
extern void set_sysmap(void);
extern int cmap_closematch(CMPTR, int, int, int);

extern int fill_image_struct(IPTR, void *, int, int, IMG_TYPE);
extern long myqread(short *, long, long);
extern void set_bk_color(rgba_t);

extern void clear_screen(long, int);
extern void clear_over_pup(void);
extern void rect_clear(long, int, int, int, int);
extern void Rectwrite(int, int, int, int, void *);
extern int read_screen(IPTR, int, int, int, int);
extern void set_display_mode(IPTR);


/*
 * type conversion
 */
extern int img_convert_type(IPTR, IMG_TYPE);
extern int img_rgb_to_cpack(IPTR);
extern int rgb_to_cmap(IPTR);
/*  quantization */
extern int pre_quant(void);
extern void set_quant_max_color(int);
extern void set_quant_parameters(int, int, int);
extern const char *quant_opt_string(void);
extern const char *qdither_opt_string(void);
extern int get_quant_p(const char *, int *, int *, int *,
		       const char *, int *, const char *, int *);
/* b&w dither */
extern const char *bw_dither_string(void);
extern int pre_bw(void);
extern int img_half_toning(IPTR);

extern int rgb2gray(int, int, int);
extern int cpack2gray(rgba_t);
extern void pick_pixel(IPTR, void *, int, int, int[]);
extern void pack_pixel(IPTR, void *, int, int, int[]);

extern void img_modify_pixel(IPTR, int[], int[], int);
extern void modify_cpack_pixel(rgba_t *, long, rgba_t, rgba_t, int);
extern void modify_ci_pixel(ci_t *, long, ci_t, ci_t, int);

/**************** Dirty Rectangle routines ******/

extern void add_d_rect(int, int, int, int);
extern const Rect_t *rect_union(Rect_t *, Rect_t *);


#ifdef TIMING
extern void start_timing(void);
extern void end_timing(void);
extern void show_timing(void);
#endif
/*
 * application independent stuff
 */
#include "ulib.h"
#include "uproto.h"
#include "gluproto.h"
#include "gui.h"


#endif

