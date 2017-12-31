/* complete.c, Atto Emacs, Hugh Barney, Public Domain, 2016 */

#include "atto.h"
#if ATTO_COMPLETE_READDIR
#include <dirent.h>
#endif

/* basic filename completion, based on code in uemacs/PK */
int getfilename(char *prompt, char *buf, int nbuf)
{
#if ATTO_COMPLETE_READDIR
	DIR *dir = NULL;
	struct dirent *entry;
	int dpos, mlen, any = FALSE;
#else
	static char temp_file[] = TEMPFILE;
	int c, fd, iswild = 0;
	char sys_command[255];
	FILE *fp = NULL;
#endif
	int cpos = 0;	/* current character position in string */
	int k = 0, didtry;
	buf[0] ='\0';

	for (;;) {
		didtry = (k == 0x09);	/* Was last command tab-completion? */
		display_prompt_and_response(prompt, buf);
		k = getch(); /* get a character from the user */

		switch(k) {
		case 0x07: /* ctrl-g, abort */
		case 0x0a: /* cr, lf */
		case 0x0d:
#if ATTO_COMPLETE_READDIR
			if (dir) closedir(dir);
#else
			if (fp != NULL) fclose(fp);
#endif
			return (k != 0x07 && cpos > 0);

		case 0x7f: /* del, erase */
		case 0x08: /* backspace */
			if (cpos == 0) continue;
			buf[--cpos] = '\0';
			break;

		case  0x15: /* C-u kill */
			cpos = 0;
			buf[0] = '\0';
			break;

		case 0x09: /* TAB, complete file name */
#if ATTO_COMPLETE_READDIR
			if (! didtry) {
				if (dir)
					closedir(dir);
				dir = NULL;
				// Find directory name to open
				dpos = cpos;
				while (dpos > 0 && buf[dpos-1] != '/')
					-- dpos;
				// Now either at start of buf or after last '/'
				mlen = cpos - dpos; /* Length to match */
			}
			while (1) {
				if (dir == NULL) {
					// Directory to open, given buffer:
					// "a/b"  -> "a"
					// "/a/b" -> "/a"
					// "/b"   -> "/"
					// ""     -> "."
					// "b"    -> "."
					if (dpos == 0) {
						dir = opendir(".");
					} else if (dpos == 1) {
						dir = opendir("/");
					} else {
						buf[dpos-1] = 0; // Was '/'
						dir = opendir(buf);
						buf[dpos-1] = '/';
					}
					any = FALSE;
					if (! dir) break;
				}
				// Get next matching entry
				entry = readdir(dir);
				if (! entry) {
#if ATTO_NO_REWINDDIR
					closedir(dir);
					dir = NULL;
#else
					rewinddir(dir);
#endif
					if (! any)
						break;
				} else if (! strncmp(entry->d_name, buf+dpos, mlen)) {
					any = TRUE;
					strncpy(buf+dpos, entry->d_name, nbuf-dpos-1);
					buf[nbuf-1] = 0;
					cpos = dpos + strlen(buf+dpos);
					break;
				}
			}
#else
			/* scan backwards for a wild card and set */
			iswild=0;
			while (cpos > 0) {
				cpos--;
				if (buf[cpos] == '*' || buf[cpos] == '?')
					iswild = 1;
			}

			/* first time retrieval */
			if (! didtry) {
				if (fp != NULL) fclose(fp);
				strcpy(temp_file, TEMPFILE);
				if (-1 == (fd = mkstemp(temp_file)))
					fatal("%s: Failed to create temp file\n");
				strcpy(sys_command, "echo ");
				strcat(sys_command, buf);
				if (!iswild) strcat(sys_command, "*");
				strcat(sys_command, " >");
				strcat(sys_command, temp_file);
				strcat(sys_command, " 2>&1");
				(void) ! system(sys_command); /* stop compiler unused result warning */
				fp = fdopen(fd, "r");
				unlink(temp_file);
			}

			/* copy next filename into buf */
			while ((c = getc(fp)) != EOF && c != '\n' && c != ' ')
				if (cpos < nbuf - 1 && c != '*')
					buf[cpos++] = c;

			buf[cpos] = '\0';
			if (c != ' ') rewind(fp);
#endif
			break;

		default:
			if (cpos < nbuf - 1) {
				  buf[cpos++] = k;
				  buf[cpos] = '\0';
			}
			break;
		}
	}
}
