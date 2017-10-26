/* header.h, Atto Emacs, Public Domain, Hugh Barney, 2016, Derived from: Anthony's Editor January 93 */

#define _XOPEN_SOURCE
#include <locale.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include <curses.h>
#include <stdio.h>
#include <sys/types.h>
#include <ctype.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>
#include <wchar.h>
int mkstemp(char *);

/* Define external names to avoid clashes when linked as a library */
#define curbp atto_curbp
#define bheadp atto_bheadp
#define curwp atto_curwp
#define wheadp atto_wheadp
#define done atto_done
#define msgflag atto_msgflag
#define nscrap atto_nscrap
#define scrap atto_scrap
#define input atto_input
#define msgline atto_msgline
#define temp atto_temp
#define searchtext atto_searchtext
#define replace atto_replace
#define key_map atto_key_map
#define key_return atto_key_return
#define fatal atto_fatal
#define msg atto_msg
#define display atto_display
#define dispmsg atto_dispmsg
#define modeline atto_modeline
#define utf8_size atto_utf8_size
#define prev_utf8_char_size atto_prev_utf8_char_size
#define display_utf8 atto_display_utf8
#define lnstart atto_lnstart
#define lncolumn atto_lncolumn
#define segstart atto_segstart
#define segnext atto_segnext
#define upup atto_upup
#define dndn atto_dndn
#define getinput atto_getinput
#define getfilename atto_getfilename
#define display_prompt_and_response atto_display_prompt_and_response
#define growgap atto_growgap
#define movegap atto_movegap
#define pos atto_pos
#define ptr atto_ptr
#define posix_file atto_posix_file
#define save atto_save
#define load_file atto_load_file
#define insert_file atto_insert_file
#define backsp atto_backsp
#define block atto_block
#define iblock atto_iblock
#define bottom atto_bottom
#define cut atto_cut
#define copy atto_copy
#define copy_cut atto_copy_cut
#define delete atto_delete
#define toggle_overwrite_mode atto_toggle_overwrite_mode
#define down atto_down
#define insert atto_insert
#define left atto_left
#define lnbegin atto_lnbegin
#define lnend atto_lnend
#define paste atto_paste
#define pgdown atto_pgdown
#define pgup atto_pgup
#define quit atto_quit
#define yesno atto_yesno
#define quit_ask atto_quit_ask
#define redraw atto_redraw
#define readfile atto_readfile
#define insertfile atto_insertfile
#define right atto_right
#define top atto_top
#define up atto_up
#define version atto_version
#define wleft atto_wleft
#define wright atto_wright
#define writefile atto_writefile
#define savebuffer atto_savebuffer
#define showpos atto_showpos
#define killtoeol atto_killtoeol
#define gotoline atto_gotoline
#define search atto_search
#define query_replace atto_query_replace
#define line_to_point atto_line_to_point
#define search_forward atto_search_forward
#define search_backwards atto_search_backwards
#define update_search_prompt atto_update_search_prompt
#define display_search_result atto_display_search_result
#define get_temp_file atto_get_temp_file
#define find_buffer atto_find_buffer
#define buffer_init atto_buffer_init
#define delete_buffer atto_delete_buffer
#define next_buffer atto_next_buffer
#define count_buffers atto_count_buffers
#define modified_buffers atto_modified_buffers
#define killbuffer atto_killbuffer
#define get_buffer_name atto_get_buffer_name
#define get_line_stats atto_get_line_stats
#define query_replace atto_query_replace
#define new_window atto_new_window
#define onw_window atto_onw_window
#define split_window atto_split_window
#define next_window atto_next_window
#define delete_other_window atto_delete_other_window
#define free_other_windows atto_free_other_windows
#define update_display atto_update_display
#define w2b atto_w2b
#define b2w atto_b2w
#define associate_b2w atto_associate_b2w
#define disassociate_b atto_disassociate_b
#define set_parse_state atto_set_parse_state
#define set_parse_state2 atto_set_parse_state2
#define parse_text atto_parse_text
#define resize_terminal atto_resize_terminal

#define VERSION	 "Atto 1.16, Public Domain, Nov 2017, by Hugh Barney,  No warranty."
#define PROG_NAME "atto"
#define B_MODIFIED	0x01		/* modified buffer */
#define B_OVERWRITE	0x02		/* overwite mode */
#define MSGLINE         (LINES-1)
#define NOMARK          -1
#define CHUNK           8096L
#define K_BUFFER_LENGTH 256
#define TEMPBUF         512
#define STRBUF_L        256
#define STRBUF_M        64
#define STRBUF_S        16
#define MIN_GAP_EXPAND  512
#define TEMPFILE        "/tmp/feXXXXXX"
#define F_NONE          0
#define F_CLEAR         1
#define ID_DEFAULT         1
#define ID_SYMBOL          2
#define ID_MODELINE        3
#define ID_DIGITS          4
#define ID_LINE_COMMENT    5
#define ID_BLOCK_COMMENT   6
#define ID_DOUBLE_STRING   7
#define ID_SINGLE_STRING   8

typedef unsigned char char_t;
typedef long point_t;

typedef struct keymap_t {
	char *key_desc;                 /* name of bound function */
	char *key_bytes;		/* the string of bytes when this key is pressed */
	void (*func)(void);
} keymap_t;

typedef struct buffer_t
{
	struct buffer_t *b_next;  /* Link to next buffer_t */
	point_t b_mark;	     	  /* the mark */
	point_t b_point;          /* the point */
	point_t b_cpoint;         /* the original current point, used for mutliple window displaying */
	point_t b_page;           /* start of page */
	point_t b_epage;          /* end of page */
	int b_cnt;                /* count of windows referencing this buffer */
	int b_size;               /* current size of text being edited (not including gap) */
	int b_psize;              /* previous size */
	char_t *b_buf;            /* start of buffer */
	char_t *b_ebuf;           /* end of buffer */
	char_t *b_gap;            /* start of gap */
	char_t *b_egap;           /* end of gap */
	int b_row;                /* cursor row */
	int b_col;                /* cursor col */
	char b_fname[NAME_MAX + 1]; /* filename */
	char b_bname[STRBUF_S];   /* buffer name */
	char b_flags;             /* buffer flags */
} buffer_t;

typedef struct window_t
{
	struct window_t *w_next;   /* Next window */
	struct buffer_t *w_bufp;   /* Buffer displayed in window */
	point_t w_point;
	point_t w_mark;
	point_t w_page;
	point_t w_epage;
	char w_top;	    /* Origin 0 top row of window */
	char w_rows;        /* no. of rows of text in window */
	int w_row;          /* cursor row */
	int w_col;          /* cursor col */
	int w_update;
	char w_name[STRBUF_S];
} window_t;

extern buffer_t *curbp;			/* current buffer */
extern buffer_t *bheadp;		/* head of list of buffers */
extern window_t *curwp;
extern window_t *wheadp;

/*
 * Some compilers define size_t as a unsigned 16 bit number while
 * point_t and off_t might be defined as a signed 32 bit number.  
 * malloc(), realloc(), fread(), and fwrite() take size_t parameters,
 * which means there will be some size limits because size_t is too
 * small of a type.
 */
#define MAX_SIZE_T      ((unsigned long) (size_t) ~0)

extern int done;                /* Quit flag. */
extern int msgflag;             /* True if msgline should be displayed. */
extern point_t nscrap;          /* Length of scrap buffer. */
extern char_t *scrap;           /* Allocated scrap buffer. */
extern char_t *input;
extern char msgline[];          /* Message line input/output buffer. */
extern char temp[];             /* Temporary buffer. */
extern char searchtext[];
extern char replace[];
extern keymap_t *key_map;       /* Command key mappings. */
extern keymap_t keymap[];
extern keymap_t *key_return;    /* Command key return */

extern void fatal(char *);
extern void msg(char *, ...);
extern void display(window_t *, int);
extern void dispmsg(void);
extern void modeline(window_t *);
extern int utf8_size(char_t);
extern int prev_utf8_char_size(void);
extern void display_utf8(buffer_t *, char_t, int);
extern point_t lnstart(buffer_t *, point_t);
extern point_t lncolumn(buffer_t *, point_t, int);
extern point_t segstart(buffer_t *, point_t, point_t);
extern point_t segnext(buffer_t *, point_t, point_t);
extern point_t upup(buffer_t *, point_t);
extern point_t dndn(buffer_t *, point_t);
extern char_t *get_key(keymap_t *, keymap_t **);
extern int getinput(char *, char *, int, int);
extern int getfilename(char *, char *, int);
extern void display_prompt_and_response(char *, char *);
extern int growgap(buffer_t *, point_t);
extern point_t movegap(buffer_t *, point_t);
extern point_t pos(buffer_t *, char_t *);
extern char_t *ptr(buffer_t *, point_t);
extern int posix_file(char *);
extern int save(char *);
extern int load_file(char *);
extern int insert_file(char *, int);
extern void backsp(void);
extern void block(void);
extern void iblock(void);
extern void bottom(void);
extern void cut(void);
extern void copy(void);
extern void copy_cut(int);
extern void delete(void);
extern void toggle_overwrite_mode(void);
extern void down(void);
extern void insert(void);
extern void left(void);
extern void lnbegin(void);
extern void lnend(void);
extern void paste(void);
extern void pgdown(void);
extern void pgup(void);
extern void quit(void);
extern int yesno(int);
extern void quit_ask(void);
extern void redraw(void);
extern void readfile(void);
extern void insertfile(void);
extern void right(void);
extern void top(void);
extern void up(void);
extern void version(void);
extern void wleft(void);
extern void wright(void);
extern void writefile(void);
extern void savebuffer(void);
extern void showpos(void);
extern void killtoeol(void);
extern void gotoline(void);
extern void search(void);
extern void query_replace(void);
extern point_t line_to_point(int);
extern point_t search_forward(buffer_t *, point_t, char *);
extern point_t search_backwards(buffer_t *, point_t, char *);
extern void update_search_prompt(char *, char *);
extern void display_search_result(point_t, int, char *, char *);
extern buffer_t* find_buffer(char *, int);
extern void buffer_init(buffer_t *);
extern int delete_buffer(buffer_t *);
extern void next_buffer(void);
extern int count_buffers(void);
extern int modified_buffers(void);
extern void killbuffer(void);
extern char* get_buffer_name(buffer_t *);
extern void get_line_stats(int *, int *);
extern void query_replace(void);
extern window_t *new_window();
extern void one_window(window_t *);
extern void split_window();
extern void next_window();
extern void delete_other_windows();
extern void free_other_windows();
extern void update_display();
extern void w2b(window_t *);
extern void b2w(window_t *);
extern void associate_b2w(buffer_t *, window_t *);
extern void disassociate_b(window_t *);
extern void set_parse_state(buffer_t *, point_t);
extern void set_parse_state2(buffer_t *, point_t);
extern int parse_text(buffer_t *, point_t);
extern void resize_terminal();
