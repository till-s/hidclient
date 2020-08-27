#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MOD_L_CTRL (1<<0)
#define MOD_L_SHFT (1<<1)
#define MOD_L_ALT  (1<<2)
#define MOD_L_CMD  (1<<3)
#define MOD_R_CTRL (1<<4)
#define MOD_R_SHFT (1<<5)
#define MOD_R_ALT  (1<<6)
#define MOD_R_CMD  (1<<7)

static void mod2txt(FILE *f, int mod)
{
const char *p = "";
	if ( (mod & MOD_R_CMD) )
	{
		fprintf(f, "%sMOD_R_CMD", p);
		p = " | ";
	}
	if ( (mod & MOD_R_ALT) )
	{
		fprintf(f, "%sMOD_R_ALT", p);
		p = " | ";
	}
	if ( (mod & MOD_R_SHFT) )
	{
		fprintf(f, "%sMOD_R_SHFT", p);
		p = " | ";
	}
	if ( (mod & MOD_R_CTRL) )
	{
		fprintf(f, "%sMOD_R_CTRL", p);
		p = " | ";
	}
	if ( (mod & MOD_L_CMD) )
	{
		fprintf(f, "%sMOD_L_CMD", p);
		p = " | ";
	}
	if ( (mod & MOD_L_ALT) )
	{
		fprintf(f, "%sMOD_L_ALT", p);
		p = " | ";
	}
	if ( (mod & MOD_L_SHFT) )
	{
		fprintf(f, "%sMOD_L_SHFT", p);
		p = " | ";
	}
	if ( (mod & MOD_L_CTRL) )
	{
		fprintf(f, "%sMOD_L_CTRL", p);
		p = " | ";
	}
	if ( ! *p )
	{
		fprintf(f, "0");
	}
}

struct Asciimap {
	int ascii;
	char ps1;
	char mod;
};

static struct Asciimap
asciimap[] = {
	{ '!',  0x1e, MOD_L_SHFT },
	{ '@',  0x1f, MOD_L_SHFT },
	{ '#',  0x20, MOD_L_SHFT },
	{ '$',  0x21, MOD_L_SHFT },
	{ '%',  0x22, MOD_L_SHFT },
	{ '^',  0x23, MOD_L_SHFT },
	{ '&',  0x24, MOD_L_SHFT },
	{ '*',  0x25, MOD_L_SHFT },
	{ '(',  0x26, MOD_L_SHFT },
	{ ')',  0x27, MOD_L_SHFT },
	{ '\n', 0x28, 0          },
	{ '\r', 0x28, 0          },
	{ '\b', 0x2a, 0          },
	{ '\t', 0x2b, 0          },
	{ ' ',  0x2c, 0          },
	{ '-',  0x2d, 0          },
	{ '_',  0x2d, MOD_L_SHFT },
	{ '=',  0x2e, 0          },
	{ '+',  0x2e, MOD_L_SHFT },
	{ '[',  0x2f, 0          },
	{ '{',  0x2f, MOD_L_SHFT },
	{ ']',  0x30, 0          },
	{ '}',  0x30, MOD_L_SHFT },
	{ '\\', 0x31, 0          },
	{ '|',  0x31, MOD_L_SHFT },
	{ ';',  0x33, 0          },
	{ ':',  0x33, MOD_L_SHFT },
	{ '\'', 0x34, 0          },
	{ '"',  0x34, MOD_L_SHFT },
	{ '`',  0x35, 0          },
	{ '~',  0x35, MOD_L_SHFT },
	{ ',',  0x36, 0          },
	{ '<',  0x36, MOD_L_SHFT },
	{ '.',  0x37, 0          },
	{ '>',  0x37, MOD_L_SHFT },
	{ '/',  0x38, 0          },
	{ '?',  0x38, MOD_L_SHFT },
	{ 127,  0x4c, MOD_L_SHFT },
};

static int cmp(const void *a, const void *b)
{
const struct Asciimap **ma = (const struct Asciimap **)a;
const struct Asciimap **mb = (const struct Asciimap **)b;
	return (*ma)->ascii - (*mb)->ascii;
}

int
main(int argc, char **argv)
{
int               nelms = sizeof(asciimap)/sizeof(asciimap[0]);
struct Asciimap * p[nelms];
int               i;
FILE             *f = stdout;
	for ( i = 0; i < nelms; i++ )
	{
		p[i] = asciimap + i;
	}

	qsort( p, nelms, sizeof(struct Asciimap *), cmp );

	fprintf(f, "#ifndef HIDCLIENT_ASCIIMAP_H\n");
	fprintf(f, "#define HIDCLIENT_ASCIIMAP_H\n");
	fprintf(f, "/* THIS IS AN AUTOMATICALLY GENERATED FILE -- DO NOT EDIT */\n");

	fprintf(f, "#define MOD_L_CTRL (1<<0)\n");
	fprintf(f, "#define MOD_L_SHFT (1<<1)\n");
	fprintf(f, "#define MOD_L_ALT  (1<<2)\n");
	fprintf(f, "#define MOD_L_CMD  (1<<3)\n");
	fprintf(f, "#define MOD_R_CTRL (1<<4)\n");
	fprintf(f, "#define MOD_R_SHFT (1<<5)\n");
	fprintf(f, "#define MOD_R_ALT  (1<<6)\n");
	fprintf(f, "#define MOD_R_CMD  (1<<7)\n");

	fprintf(f, "struct AsciiMap {\n");
	fprintf(f, "	int  ascii;\n");
	fprintf(f, "	char ps1;\n");
	fprintf(f, "	char mod;\n");
	fprintf(f, "};\n");
	fprintf(f, "static struct AsciiMap asciimap[] = {\n");

	for ( i = 0; i < nelms; i++ )
	{
		int ch = p[i]->ascii;
		const char *s = 0;
		/* Handle escape-sequences first; some of
		 * these are also printable but result in
		 * non-escaped output below.
		 */
		switch ( ch )
		{
			case '\'': s = "'\\''" ; break;
			case '\\': s = "'\\\\'"; break;
			case '\a': s = "'\\a'" ; break;
			case '\b': s = "'\\b'" ; break;
			case '\f': s = "'\\f'" ; break;
			case '\n': s = "'\\n'" ; break;
			case '\r': s = "'\\r'" ; break;
			case '\t': s = "'\\t'" ; break;
			case '\v': s = "'\\v'" ; break;
			default:
				   break;
		}
		fprintf(f, "	{ ");
		if ( s )
		{
			fprintf(f, "%6s", s);
		}
		else if ( isprint( ch ) )
		{
			fprintf(f, "   '%c'", ch);
		}
		else
		{
			fprintf(f, "'\\x%02x'", ch);
		}
		fprintf(f, ", 0x%02x, ", p[i]->ps1);
		mod2txt(f, p[i]->mod);
		fprintf(f, " },\n");
	}
	fprintf(f, "};\n");

	fprintf(f, "#endif  /* HIDCLIENT_ASCIIMAP_H */\n");

}

