/*
 * This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */

#include <config.h>
#include <stdlib.h>
#include <string.h>
#define YYBYACC 1
#define YYMAJOR 1
#define YYMINOR 9
#define YYLEX yylex()
#define YYEMPTY -1
#define yyclearin (yychar=(YYEMPTY))
#define yyerrok (yyerrflag=0)
#define YYRECOVERING() (yyerrflag!=0)
#define yyparse sudoersparse
#define yylex sudoerslex
#define yyerror sudoerserror
#define yychar sudoerschar
#define yyval sudoersval
#define yylval sudoerslval
#define yydebug sudoersdebug
#define yynerrs sudoersnerrs
#define yyerrflag sudoerserrflag
#define yyss sudoersss
#define yysslim sudoerssslim
#define yyssp sudoersssp
#define yyvs sudoersvs
#define yyvsp sudoersvsp
#define yystacksize sudoersstacksize
#define yylhs sudoerslhs
#define yylen sudoerslen
#define yydefred sudoersdefred
#define yydgoto sudoersdgoto
#define yysindex sudoerssindex
#define yyrindex sudoersrindex
#define yygindex sudoersgindex
#define yytable sudoerstable
#define yycheck sudoerscheck
#define yyname sudoersname
#define yyrule sudoersrule
#define YYPREFIX "sudoers"
#line 2 "gram.y"
/*
 * SPDX-License-Identifier: ISC
 *
 * Copyright (c) 1996, 1998-2005, 2007-2013, 2014-2020
 *	Todd C. Miller <Todd.Miller@sudo.ws>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Sponsored in part by the Defense Advanced Research Projects
 * Agency (DARPA) and Air Force Research Laboratory, Air Force
 * Materiel Command, USAF, under agreement number F39502-99-1-0512.
 */

#include <config.h>

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>
#if defined(YYBISON) && defined(HAVE_ALLOCA_H) && !defined(__GNUC__)
# include <alloca.h>
#endif /* YYBISON && HAVE_ALLOCA_H && !__GNUC__ */
#include <errno.h>

#include "sudoers.h"
#include "sudo_digest.h"
#include "toke.h"

#ifdef YYBISON
# define YYERROR_VERBOSE
#endif

/* If we last saw a newline the entry is on the preceding line. */
#define this_lineno	(last_token == '\n' ? sudolineno - 1 : sudolineno)

/*
 * Globals
 */
bool sudoers_warnings = true;
bool sudoers_strict = false;
bool parse_error = false;
int errorlineno = -1;
char *errorfile = NULL;

struct sudoers_parse_tree parsed_policy = {
    TAILQ_HEAD_INITIALIZER(parsed_policy.userspecs),
    TAILQ_HEAD_INITIALIZER(parsed_policy.defaults),
    NULL, /* aliases */
    NULL, /* lhost */
    NULL /* shost */
};

/*
 * Local prototypes
 */
static void init_options(struct command_options *opts);
static bool add_defaults(int, struct member *, struct defaults *);
static bool add_userspec(struct member *, struct privilege *);
static struct defaults *new_default(char *, char *, short);
static struct member *new_member(char *, int);
static struct sudo_command *new_command(char *, char *);
static struct command_digest *new_digest(int, char *);
#line 77 "gram.y"
#ifndef YYSTYPE_DEFINED
#define YYSTYPE_DEFINED
typedef union {
    struct cmndspec *cmndspec;
    struct defaults *defaults;
    struct member *member;
    struct runascontainer *runas;
    struct privilege *privilege;
    struct command_digest *digest;
    struct sudo_command command;
    struct command_options options;
    struct cmndtag tag;
    char *string;
    int tok;
} YYSTYPE;
#endif /* YYSTYPE_DEFINED */
#line 130 "gram.c"
#define END 0
#define COMMAND 257
#define ALIAS 258
#define DEFVAR 259
#define NTWKADDR 260
#define NETGROUP 261
#define USERGROUP 262
#define WORD 263
#define DIGEST 264
#define INCLUDE 265
#define INCLUDEDIR 266
#define DEFAULTS 267
#define DEFAULTS_HOST 268
#define DEFAULTS_USER 269
#define DEFAULTS_RUNAS 270
#define DEFAULTS_CMND 271
#define NOPASSWD 272
#define PASSWD 273
#define NOEXEC 274
#define EXEC 275
#define SETENV 276
#define NOSETENV 277
#define LOG_INPUT 278
#define NOLOG_INPUT 279
#define LOG_OUTPUT 280
#define NOLOG_OUTPUT 281
#define MAIL 282
#define NOMAIL 283
#define FOLLOWLNK 284
#define NOFOLLOWLNK 285
#define ALL 286
#define HOSTALIAS 287
#define CMNDALIAS 288
#define USERALIAS 289
#define RUNASALIAS 290
#define ERROR 291
#define TYPE 292
#define ROLE 293
#define PRIVS 294
#define LIMITPRIVS 295
#define CMND_TIMEOUT 296
#define NOTBEFORE 297
#define NOTAFTER 298
#define MYSELF 299
#define SHA224_TOK 300
#define SHA256_TOK 301
#define SHA384_TOK 302
#define SHA512_TOK 303
#define YYERRCODE 256
const short sudoerslhs[] =
	{                                        -1,
    0,    0,   35,   35,   36,   36,   36,   36,   36,   36,
   36,   36,   36,   36,   36,   36,   36,   36,   31,   31,
   31,   31,   32,   32,   32,   32,    4,    4,    3,    3,
    3,    3,    3,   21,   21,   20,   11,   11,    9,    9,
    9,    9,    9,    2,    2,    1,   33,   33,   33,   33,
   34,   34,    7,    7,    6,    6,   28,   29,   30,   24,
   25,   26,   27,   18,   18,   19,   19,   19,   19,   19,
   23,   23,   23,   23,   23,   23,   23,   23,   22,   22,
   22,   22,   22,   22,   22,   22,   22,   22,   22,   22,
   22,   22,   22,    5,    5,    5,   38,   38,   41,   10,
   10,   39,   39,   42,    8,    8,   40,   40,   43,   37,
   37,   44,   14,   14,   12,   12,   13,   13,   13,   13,
   13,   17,   17,   15,   15,   16,   16,   16,
};
const short sudoerslen[] =
	{                                         2,
    0,    1,    1,    2,    1,    2,    1,    1,    2,    2,
    2,    2,    2,    2,    3,    3,    3,    3,    3,    4,
    3,    4,    3,    4,    3,    4,    1,    3,    1,    2,
    3,    3,    3,    1,    3,    3,    1,    2,    1,    1,
    1,    1,    1,    1,    3,    4,    3,    3,    3,    3,
    1,    3,    1,    2,    1,    2,    3,    3,    3,    3,
    3,    3,    3,    0,    3,    0,    1,    3,    2,    1,
    0,    2,    2,    2,    2,    2,    2,    2,    0,    2,
    2,    2,    2,    2,    2,    2,    2,    2,    2,    2,
    2,    2,    2,    1,    1,    1,    1,    3,    3,    1,
    3,    1,    3,    3,    1,    3,    1,    3,    3,    1,
    3,    3,    1,    3,    1,    2,    1,    1,    1,    1,
    1,    1,    3,    1,    2,    1,    1,    1,
};
const short sudoersdefred[] =
	{                                      0,
    0,  117,  119,  120,  121,    0,    0,    0,    0,    0,
    0,    0,  118,    0,    0,    0,    0,    0,    5,    0,
  113,  115,    0,    7,    8,    0,    3,    6,    0,    0,
    0,    0,   27,    0,   39,   42,   41,   43,   40,    0,
   37,    0,  100,    0,    0,   96,   95,   94,    0,    0,
    0,    0,    0,   55,   53,  105,    0,   51,    0,    0,
    0,   97,    0,    0,  102,    0,    0,  110,    0,    0,
  107,  116,    0,    0,   34,    0,    4,    0,   21,   19,
    0,   25,   23,    0,    0,    0,   30,    0,   38,    0,
    0,    0,    0,   56,    0,    0,    0,    0,    0,    0,
    0,   54,    0,    0,    0,    0,    0,    0,    0,    0,
  114,    0,    0,   22,   20,   26,   24,   31,   32,   33,
   28,  101,   47,   48,   49,   50,  106,   52,    0,   98,
    0,  103,    0,  111,    0,  108,    0,   44,    0,   71,
   35,    0,    0,    0,    0,    0,  126,  128,  127,    0,
  122,  124,    0,    0,   65,   45,    0,    0,    0,    0,
    0,    0,    0,    0,   75,   76,   77,   78,   74,   72,
   73,  125,    0,    0,    0,    0,    0,    0,    0,    0,
    0,   80,   81,   82,   83,   84,   85,   86,   87,   88,
   89,   92,   93,   90,   91,   46,  123,   61,   60,   62,
   63,   57,   58,   59,
};
const short sudoersdgoto[] =
	{                                      20,
  138,  139,   33,   34,   54,   55,   56,   57,   41,   74,
   43,   21,   22,   23,  151,  152,  153,  140,  144,   75,
   76,  164,  146,  165,  166,  167,  168,  169,  170,  171,
   24,   25,   58,   59,   26,   27,   67,   61,   64,   70,
   62,   65,   71,   68,
};
const short sudoerssindex[] =
	{                                    565,
    4,    0,    0,    0,    0, -241, -237,  -24,  514,   23,
   23,  -26,    0, -205, -203, -201, -198, -221,    0,    0,
    0,    0,  -13,    0,    0,  565,    0,    0,    2,    5,
    9, -195,    0,   17,    0,    0,    0,    0,    0, -228,
    0,  -23,    0,  -20,  -20,    0,    0,    0, -239,    8,
   10,   24,   29,    0,    0,    0,  -16,    0,   -6,   27,
   31,    0,   30,   34,    0,   32,   37,    0,   35,   39,
    0,    0,   23,  -38,    0,   40,    0,   49,    0,    0,
   62,    0,    0, -164, -162, -160,    0,  -24,    0,  514,
   17,   17,   17,    0, -159, -157, -156, -155,  -26,   17,
 -217,    0,  514, -205,  -26, -203,   23, -201,   23, -198,
    0,   72,  514,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,   67,    0,
   69,    0,   70,    0,   70,    0,  -33,    0,   73,    0,
    0,   57,  -15,   74,   72, -218,    0,    0,    0, -219,
    0,    0,   75,   57,    0,    0,   59,   60,   61,   63,
   64,   65,   66,  631,    0,    0,    0,    0,    0,    0,
    0,    0,   57,   75, -145, -140, -135, -132, -131, -130,
 -129,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,};
const short sudoersrindex[] =
	{                                    135,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  136,    0,    0,    0,    0,
    1,    0,    0,  211,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  246,    0,    0,  283,    0,    0,  318,    0,    0,  353,
    0,    0,    0,    0,    0,  388,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  423,  458,  493,    0,    0,    0,    0,    0,    0,  528,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  584,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,   36,    0,
   71,    0,  106,    0,  141,    0,   96,    0,  176,    0,
    0,   97,   99,    0,  584,  663,    0,    0,    0,    0,
    0,    0,  101,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  102,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,};
const short sudoersgindex[] =
	{                                      0,
   -1,    0,   58,    6,   98,   86,  -91,   43,  109,    7,
   68,   77,  134,   -7,  -19,    3,   11,    0,    0,   42,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,   55,    0,    0,  131,    0,    0,    0,    0,
   56,   53,   51,   54,
};
#define YYTABLESIZE 966
const short sudoerstable[] =
	{                                      18,
   29,   79,   44,   45,   82,   90,   49,  127,   32,   32,
   29,   80,   32,   28,   83,   42,   32,   46,   47,   40,
   90,   29,  112,   73,  142,   30,   49,   99,   73,   35,
   73,   36,   37,   29,   38,   99,    2,  101,  147,    3,
    4,    5,  154,  148,   29,   99,   48,   91,  114,   92,
   93,   85,   60,   86,   63,   18,   66,   39,  115,   69,
   88,  116,  100,   87,   13,   95,  149,   96,   99,   84,
  104,  117,  196,  157,  158,  159,  160,  161,  162,  163,
  104,   97,   50,   51,   52,   53,   98,  103,  104,  150,
  105,  106,  107,   99,  108,  109,  110,  113,  118,  133,
  119,  135,  120,  104,  123,  112,  124,  125,  126,  129,
   90,  137,   99,   73,  155,  112,  145,  198,  173,  175,
  176,  177,  199,  178,  179,  180,  181,  200,  104,  143,
  201,  202,  203,  204,    1,    2,   66,   70,  112,   67,
  109,   69,   68,  156,  102,  121,   94,  131,   89,  111,
  109,   72,  172,  197,  141,  128,   77,  122,  132,  130,
  136,  134,    0,  112,  174,    0,    0,    0,    0,    0,
    0,    0,    0,  109,    0,   36,    0,    0,    0,    0,
    0,    0,    0,    0,    0,   36,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  109,    0,
    0,    0,    0,    0,    0,    0,    0,    0,   36,    0,
   14,    0,    0,    0,    0,    0,    0,    0,    0,    0,
   14,    0,    0,    0,    2,    0,    0,    3,    4,    5,
   46,   47,    0,   36,   31,   31,    0,    0,   31,    0,
    0,    0,   31,   14,   35,   11,   36,   37,    0,   38,
   46,   47,   13,    0,    0,   11,   29,   78,   29,   48,
   81,   29,   29,   29,    0,   29,   29,   29,   29,   29,
   29,   29,   39,   50,   51,   52,   53,    0,   11,   48,
    2,    0,   12,    3,    4,    5,   29,   29,   29,   29,
   29,   99,   12,   99,    0,    0,   99,   99,   99,    0,
   99,   99,   99,   99,   99,   99,   99,    0,   13,    0,
    0,    0,    0,    0,  147,   12,    0,   10,    0,  148,
    0,   99,   99,   99,   99,   99,  104,   10,  104,    0,
    0,  104,  104,  104,    0,  104,  104,  104,  104,  104,
  104,  104,  149,    0,    0,    0,    0,    0,    0,    0,
   10,    0,   13,    0,    0,    0,  104,  104,  104,  104,
  104,  112,   13,  112,    0,    0,  112,  112,  112,    0,
  112,  112,  112,  112,  112,  112,  112,    0,    0,    0,
    0,    0,    0,    0,    0,   13,    0,    9,    0,    0,
    0,  112,  112,  112,  112,  112,  109,    9,  109,    0,
    0,  109,  109,  109,    0,  109,  109,  109,  109,  109,
  109,  109,    0,    0,    0,    0,    0,    0,    0,    0,
    9,    0,   17,    0,    0,    0,  109,  109,  109,  109,
  109,   36,   17,   36,    0,    0,   36,   36,   36,    0,
   36,   36,   36,   36,   36,   36,   36,    0,    0,    0,
    0,    0,    0,    0,    0,   17,    0,   15,    0,    0,
    0,   36,   36,   36,   36,   36,   14,   15,   14,    0,
    0,   14,   14,   14,    0,   14,   14,   14,   14,   14,
   14,   14,    0,    0,    0,    0,    0,    0,    0,    0,
   15,    0,   16,    0,    0,    0,   14,   14,   14,   14,
   14,   11,   16,   11,    0,    0,   11,   11,   11,    0,
   11,   11,   11,   11,   11,   11,   11,    0,    0,    0,
    0,    0,    0,    0,    0,   16,    0,   18,    0,    0,
    0,   11,   11,   11,   11,   11,    0,   18,   12,    0,
   12,    0,    0,   12,   12,   12,   40,   12,   12,   12,
   12,   12,   12,   12,    0,    0,    0,    0,    0,    0,
   18,    0,    0,    0,    0,    0,    0,    0,   12,   12,
   12,   12,   12,   10,   19,   10,    0,    0,   10,   10,
   10,    0,   10,   10,   10,   10,   10,   10,   10,    0,
    0,    0,    0,    0,    0,    0,    0,   18,    0,    0,
    0,    0,    0,   10,   10,   10,   10,   10,   13,    0,
   13,    0,    0,   13,   13,   13,   64,   13,   13,   13,
   13,   13,   13,   13,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,   13,   13,
   13,   13,   13,    9,    0,    9,    0,    0,    9,    9,
    9,    0,    9,    9,    9,    9,    9,    9,    9,    0,
    0,    0,    0,   49,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    9,    9,    9,    9,    9,   17,    0,
   17,    0,    0,   17,   17,   17,    0,   17,   17,   17,
   17,   17,   17,   17,    0,   79,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,   17,   17,
   17,   17,   17,   15,    0,   15,    0,    0,   15,   15,
   15,    0,   15,   15,   15,   15,   15,   15,   15,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,   15,   15,   15,   15,   15,   16,    0,
   16,    0,    0,   16,   16,   16,    0,   16,   16,   16,
   16,   16,   16,   16,    0,    0,    0,    0,    0,    0,
    0,   35,    0,   36,   37,    0,   38,    0,   16,   16,
   16,   16,   16,   18,    0,   18,    0,    0,   18,   18,
   18,    0,   18,   18,   18,   18,   18,   18,   18,   39,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,   18,   18,   18,   18,   18,    0,    0,
    1,    0,    2,    0,    0,    3,    4,    5,    0,    6,
    7,    8,    9,   10,   11,   12,    0,    0,    0,    0,
   64,   64,    0,    0,    0,    0,    0,    0,    0,    0,
   13,   14,   15,   16,   17,   64,   64,   64,   64,   64,
   64,   64,   64,   64,   64,   64,   64,   64,   64,   64,
    0,    0,    0,    0,    0,   64,   64,   64,   64,   64,
   64,   64,    0,   64,   64,   64,   64,   46,   47,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  182,  183,  184,  185,  186,  187,  188,  189,
  190,  191,  192,  193,  194,  195,   48,    0,    0,   79,
   79,    0,    0,    0,    0,    0,    0,    0,    0,    0,
   50,   51,   52,   53,   79,   79,   79,   79,   79,   79,
   79,   79,   79,   79,   79,   79,   79,   79,   79,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,   79,   79,   79,   79,
};
const short sudoerscheck[] =
	{                                      33,
    0,    0,   10,   11,    0,   44,   33,   99,   33,   33,
   10,   10,   33,   10,   10,    9,   33,  257,  258,   33,
   44,  263,   61,   44,   58,  263,   33,   44,   44,  258,
   44,  260,  261,   33,  263,    0,  258,   44,  258,  261,
  262,  263,   58,  263,   44,   10,  286,   42,    0,   44,
   45,   43,  258,   45,  258,   33,  258,  286,   10,  258,
   44,    0,   57,  259,  286,   58,  286,   58,   33,   61,
    0,   10,  164,  292,  293,  294,  295,  296,  297,  298,
   10,   58,  300,  301,  302,  303,   58,   61,   58,   33,
   61,   58,   61,   58,   58,   61,   58,   58,  263,  107,
  263,  109,  263,   33,  264,    0,  264,  264,  264,  103,
   44,   40,   44,   44,   41,   10,   44,  263,   44,   61,
   61,   61,  263,   61,   61,   61,   61,  263,   58,  137,
  263,  263,  263,  263,    0,    0,   41,   41,   33,   41,
    0,   41,   41,  145,   59,   88,   49,  105,   40,   73,
   10,   18,  150,  173,  113,  101,   26,   90,  106,  104,
  110,  108,   -1,   58,  154,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   33,   -1,    0,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   10,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   33,   -1,
    0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   10,   -1,   -1,   -1,  258,   -1,   -1,  261,  262,  263,
  257,  258,   -1,   58,  259,  259,   -1,   -1,  259,   -1,
   -1,   -1,  259,   33,  258,    0,  260,  261,   -1,  263,
  257,  258,  286,   -1,   -1,   10,  256,  256,  258,  286,
  256,  261,  262,  263,   -1,  265,  266,  267,  268,  269,
  270,  271,  286,  300,  301,  302,  303,   -1,   33,  286,
  258,   -1,    0,  261,  262,  263,  286,  287,  288,  289,
  290,  256,   10,  258,   -1,   -1,  261,  262,  263,   -1,
  265,  266,  267,  268,  269,  270,  271,   -1,  286,   -1,
   -1,   -1,   -1,   -1,  258,   33,   -1,    0,   -1,  263,
   -1,  286,  287,  288,  289,  290,  256,   10,  258,   -1,
   -1,  261,  262,  263,   -1,  265,  266,  267,  268,  269,
  270,  271,  286,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   33,   -1,    0,   -1,   -1,   -1,  286,  287,  288,  289,
  290,  256,   10,  258,   -1,   -1,  261,  262,  263,   -1,
  265,  266,  267,  268,  269,  270,  271,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   33,   -1,    0,   -1,   -1,
   -1,  286,  287,  288,  289,  290,  256,   10,  258,   -1,
   -1,  261,  262,  263,   -1,  265,  266,  267,  268,  269,
  270,  271,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   33,   -1,    0,   -1,   -1,   -1,  286,  287,  288,  289,
  290,  256,   10,  258,   -1,   -1,  261,  262,  263,   -1,
  265,  266,  267,  268,  269,  270,  271,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   33,   -1,    0,   -1,   -1,
   -1,  286,  287,  288,  289,  290,  256,   10,  258,   -1,
   -1,  261,  262,  263,   -1,  265,  266,  267,  268,  269,
  270,  271,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   33,   -1,    0,   -1,   -1,   -1,  286,  287,  288,  289,
  290,  256,   10,  258,   -1,   -1,  261,  262,  263,   -1,
  265,  266,  267,  268,  269,  270,  271,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   33,   -1,    0,   -1,   -1,
   -1,  286,  287,  288,  289,  290,   -1,   10,  256,   -1,
  258,   -1,   -1,  261,  262,  263,   33,  265,  266,  267,
  268,  269,  270,  271,   -1,   -1,   -1,   -1,   -1,   -1,
   33,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  286,  287,
  288,  289,  290,  256,   10,  258,   -1,   -1,  261,  262,
  263,   -1,  265,  266,  267,  268,  269,  270,  271,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   33,   -1,   -1,
   -1,   -1,   -1,  286,  287,  288,  289,  290,  256,   -1,
  258,   -1,   -1,  261,  262,  263,   33,  265,  266,  267,
  268,  269,  270,  271,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  286,  287,
  288,  289,  290,  256,   -1,  258,   -1,   -1,  261,  262,
  263,   -1,  265,  266,  267,  268,  269,  270,  271,   -1,
   -1,   -1,   -1,   33,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  286,  287,  288,  289,  290,  256,   -1,
  258,   -1,   -1,  261,  262,  263,   -1,  265,  266,  267,
  268,  269,  270,  271,   -1,   33,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  286,  287,
  288,  289,  290,  256,   -1,  258,   -1,   -1,  261,  262,
  263,   -1,  265,  266,  267,  268,  269,  270,  271,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  286,  287,  288,  289,  290,  256,   -1,
  258,   -1,   -1,  261,  262,  263,   -1,  265,  266,  267,
  268,  269,  270,  271,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  258,   -1,  260,  261,   -1,  263,   -1,  286,  287,
  288,  289,  290,  256,   -1,  258,   -1,   -1,  261,  262,
  263,   -1,  265,  266,  267,  268,  269,  270,  271,  286,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  286,  287,  288,  289,  290,   -1,   -1,
  256,   -1,  258,   -1,   -1,  261,  262,  263,   -1,  265,
  266,  267,  268,  269,  270,  271,   -1,   -1,   -1,   -1,
  257,  258,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  286,  287,  288,  289,  290,  272,  273,  274,  275,  276,
  277,  278,  279,  280,  281,  282,  283,  284,  285,  286,
   -1,   -1,   -1,   -1,   -1,  292,  293,  294,  295,  296,
  297,  298,   -1,  300,  301,  302,  303,  257,  258,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  272,  273,  274,  275,  276,  277,  278,  279,
  280,  281,  282,  283,  284,  285,  286,   -1,   -1,  257,
  258,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  300,  301,  302,  303,  272,  273,  274,  275,  276,  277,
  278,  279,  280,  281,  282,  283,  284,  285,  286,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  300,  301,  302,  303,
};
#define YYFINAL 20
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 303
#if YYDEBUG
const char * const sudoersname[] =
	{
"end-of-file",0,0,0,0,0,0,0,0,0,"'\\n'",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,"'!'",0,0,0,0,0,0,"'('","')'",0,"'+'","','","'-'",0,0,0,0,0,0,0,0,0,0,0,0,
"':'",0,0,"'='",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,"COMMAND","ALIAS","DEFVAR","NTWKADDR","NETGROUP","USERGROUP","WORD",
"DIGEST","INCLUDE","INCLUDEDIR","DEFAULTS","DEFAULTS_HOST","DEFAULTS_USER",
"DEFAULTS_RUNAS","DEFAULTS_CMND","NOPASSWD","PASSWD","NOEXEC","EXEC","SETENV",
"NOSETENV","LOG_INPUT","NOLOG_INPUT","LOG_OUTPUT","NOLOG_OUTPUT","MAIL",
"NOMAIL","FOLLOWLNK","NOFOLLOWLNK","ALL","HOSTALIAS","CMNDALIAS","USERALIAS",
"RUNASALIAS","ERROR","TYPE","ROLE","PRIVS","LIMITPRIVS","CMND_TIMEOUT",
"NOTBEFORE","NOTAFTER","MYSELF","SHA224_TOK","SHA256_TOK","SHA384_TOK",
"SHA512_TOK",
};
const char * const sudoersrule[] =
	{"$accept : file",
"file :",
"file : line",
"line : entry",
"line : line entry",
"entry : '\\n'",
"entry : error '\\n'",
"entry : include",
"entry : includedir",
"entry : userlist privileges",
"entry : USERALIAS useraliases",
"entry : HOSTALIAS hostaliases",
"entry : CMNDALIAS cmndaliases",
"entry : RUNASALIAS runasaliases",
"entry : DEFAULTS defaults_list",
"entry : DEFAULTS_USER userlist defaults_list",
"entry : DEFAULTS_RUNAS userlist defaults_list",
"entry : DEFAULTS_HOST hostlist defaults_list",
"entry : DEFAULTS_CMND cmndlist defaults_list",
"include : INCLUDE WORD '\\n'",
"include : INCLUDE WORD error '\\n'",
"include : INCLUDE WORD END",
"include : INCLUDE WORD error END",
"includedir : INCLUDEDIR WORD '\\n'",
"includedir : INCLUDEDIR WORD error '\\n'",
"includedir : INCLUDEDIR WORD END",
"includedir : INCLUDEDIR WORD error END",
"defaults_list : defaults_entry",
"defaults_list : defaults_list ',' defaults_entry",
"defaults_entry : DEFVAR",
"defaults_entry : '!' DEFVAR",
"defaults_entry : DEFVAR '=' WORD",
"defaults_entry : DEFVAR '+' WORD",
"defaults_entry : DEFVAR '-' WORD",
"privileges : privilege",
"privileges : privileges ':' privilege",
"privilege : hostlist '=' cmndspeclist",
"ophost : host",
"ophost : '!' host",
"host : ALIAS",
"host : ALL",
"host : NETGROUP",
"host : NTWKADDR",
"host : WORD",
"cmndspeclist : cmndspec",
"cmndspeclist : cmndspeclist ',' cmndspec",
"cmndspec : runasspec options cmndtag digcmnd",
"digestspec : SHA224_TOK ':' DIGEST",
"digestspec : SHA256_TOK ':' DIGEST",
"digestspec : SHA384_TOK ':' DIGEST",
"digestspec : SHA512_TOK ':' DIGEST",
"digestlist : digestspec",
"digestlist : digestlist ',' digestspec",
"digcmnd : opcmnd",
"digcmnd : digestlist opcmnd",
"opcmnd : cmnd",
"opcmnd : '!' cmnd",
"timeoutspec : CMND_TIMEOUT '=' WORD",
"notbeforespec : NOTBEFORE '=' WORD",
"notafterspec : NOTAFTER '=' WORD",
"rolespec : ROLE '=' WORD",
"typespec : TYPE '=' WORD",
"privsspec : PRIVS '=' WORD",
"limitprivsspec : LIMITPRIVS '=' WORD",
"runasspec :",
"runasspec : '(' runaslist ')'",
"runaslist :",
"runaslist : userlist",
"runaslist : userlist ':' grouplist",
"runaslist : ':' grouplist",
"runaslist : ':'",
"options :",
"options : options notbeforespec",
"options : options notafterspec",
"options : options timeoutspec",
"options : options rolespec",
"options : options typespec",
"options : options privsspec",
"options : options limitprivsspec",
"cmndtag :",
"cmndtag : cmndtag NOPASSWD",
"cmndtag : cmndtag PASSWD",
"cmndtag : cmndtag NOEXEC",
"cmndtag : cmndtag EXEC",
"cmndtag : cmndtag SETENV",
"cmndtag : cmndtag NOSETENV",
"cmndtag : cmndtag LOG_INPUT",
"cmndtag : cmndtag NOLOG_INPUT",
"cmndtag : cmndtag LOG_OUTPUT",
"cmndtag : cmndtag NOLOG_OUTPUT",
"cmndtag : cmndtag FOLLOWLNK",
"cmndtag : cmndtag NOFOLLOWLNK",
"cmndtag : cmndtag MAIL",
"cmndtag : cmndtag NOMAIL",
"cmnd : ALL",
"cmnd : ALIAS",
"cmnd : COMMAND",
"hostaliases : hostalias",
"hostaliases : hostaliases ':' hostalias",
"hostalias : ALIAS '=' hostlist",
"hostlist : ophost",
"hostlist : hostlist ',' ophost",
"cmndaliases : cmndalias",
"cmndaliases : cmndaliases ':' cmndalias",
"cmndalias : ALIAS '=' cmndlist",
"cmndlist : digcmnd",
"cmndlist : cmndlist ',' digcmnd",
"runasaliases : runasalias",
"runasaliases : runasaliases ':' runasalias",
"runasalias : ALIAS '=' userlist",
"useraliases : useralias",
"useraliases : useraliases ':' useralias",
"useralias : ALIAS '=' userlist",
"userlist : opuser",
"userlist : userlist ',' opuser",
"opuser : user",
"opuser : '!' user",
"user : ALIAS",
"user : ALL",
"user : NETGROUP",
"user : USERGROUP",
"user : WORD",
"grouplist : opgroup",
"grouplist : grouplist ',' opgroup",
"opgroup : group",
"opgroup : '!' group",
"group : ALIAS",
"group : ALL",
"group : WORD",
};
#endif
#ifdef YYSTACKSIZE
#undef YYMAXDEPTH
#define YYMAXDEPTH YYSTACKSIZE
#else
#ifdef YYMAXDEPTH
#define YYSTACKSIZE YYMAXDEPTH
#else
#define YYSTACKSIZE 10000
#define YYMAXDEPTH 10000
#endif
#endif
#define YYINITSTACKSIZE 200
/* LINTUSED */
int yydebug;
int yynerrs;
int yyerrflag;
int yychar;
short *yyssp;
YYSTYPE *yyvsp;
YYSTYPE yyval;
YYSTYPE yylval;
short *yyss;
short *yysslim;
YYSTYPE *yyvs;
unsigned int yystacksize;
int yyparse(void);
#line 973 "gram.y"
void
sudoerserror(const char *s)
{
    static int last_error_line = -1;
    static char *last_error_file = NULL;
    debug_decl(sudoerserror, SUDOERS_DEBUG_PARSER);

    /* Avoid displaying a generic error after a more specific one. */
    if (last_error_file == sudoers && last_error_line == this_lineno)
	debug_return;
    last_error_file = sudoers;
    last_error_line = this_lineno;

    /* Save the line the first error occurred on. */
    if (errorlineno == -1) {
	errorlineno = this_lineno;
	rcstr_delref(errorfile);
	errorfile = rcstr_addref(sudoers);
    }
    if (sudoers_warnings && s != NULL) {
	LEXTRACE("<*> ");
#ifndef TRACELEXER
	if (trace_print == NULL || trace_print == sudoers_trace_print) {
	    int oldlocale;

	    /* Warnings are displayed in the user's locale. */
	    sudoers_setlocale(SUDOERS_LOCALE_USER, &oldlocale);
	    sudo_printf(SUDO_CONV_ERROR_MSG, _("%s:%d: %s\n"), sudoers,
		this_lineno, _(s));
	    sudoers_setlocale(oldlocale, NULL);

	    /* Display the offending line and token if possible. */
	    if (sudolinebuf.len != 0) {
		char tildes[128];
		size_t tlen = 0;

		sudo_printf(SUDO_CONV_ERROR_MSG, "%s%s", sudolinebuf.buf,
		    sudolinebuf.buf[sudolinebuf.len - 1] == '\n' ? "" : "\n");
		if (sudolinebuf.toke_end > sudolinebuf.toke_start) {
		    tlen = sudolinebuf.toke_end - sudolinebuf.toke_start - 1;
		    if (tlen >= sizeof(tildes))
			tlen = sizeof(tildes) - 1;
		    memset(tildes, '~', tlen);
		}
		tildes[tlen] = '\0';
		sudo_printf(SUDO_CONV_ERROR_MSG, "%*s^%s\n",
		    (int)sudolinebuf.toke_start, "", tildes);
	    }
	}
#endif
    }
    parse_error = true;
    debug_return;
}

static struct defaults *
new_default(char *var, char *val, short op)
{
    struct defaults *d;
    debug_decl(new_default, SUDOERS_DEBUG_PARSER);

    if ((d = calloc(1, sizeof(struct defaults))) == NULL) {
	sudo_debug_printf(SUDO_DEBUG_ERROR|SUDO_DEBUG_LINENO,
	    "unable to allocate memory");
	debug_return_ptr(NULL);
    }

    d->var = var;
    d->val = val;
    /* d->type = 0; */
    d->op = op;
    /* d->binding = NULL */
    d->lineno = this_lineno;
    d->file = rcstr_addref(sudoers);
    HLTQ_INIT(d, entries);

    debug_return_ptr(d);
}

static struct member *
new_member(char *name, int type)
{
    struct member *m;
    debug_decl(new_member, SUDOERS_DEBUG_PARSER);

    if ((m = calloc(1, sizeof(struct member))) == NULL) {
	sudo_debug_printf(SUDO_DEBUG_ERROR|SUDO_DEBUG_LINENO,
	    "unable to allocate memory");
	debug_return_ptr(NULL);
    }

    m->name = name;
    m->type = type;
    HLTQ_INIT(m, entries);

    debug_return_ptr(m);
}

static struct sudo_command *
new_command(char *cmnd, char *args)
{
    struct sudo_command *c;
    debug_decl(new_command, SUDOERS_DEBUG_PARSER);

    if ((c = calloc(1, sizeof(*c))) == NULL) {
	sudo_debug_printf(SUDO_DEBUG_ERROR|SUDO_DEBUG_LINENO,
	    "unable to allocate memory");
	debug_return_ptr(NULL);
    }

    c->cmnd = cmnd;
    c->args = args;
    TAILQ_INIT(&c->digests);

    debug_return_ptr(c);
}

static struct command_digest *
new_digest(int digest_type, char *digest_str)
{
    struct command_digest *digest;
    debug_decl(new_digest, SUDOERS_DEBUG_PARSER);

    if ((digest = malloc(sizeof(*digest))) == NULL) {
	sudo_debug_printf(SUDO_DEBUG_ERROR|SUDO_DEBUG_LINENO,
	    "unable to allocate memory");
	debug_return_ptr(NULL);
    }

    HLTQ_INIT(digest, entries);
    digest->digest_type = digest_type;
    digest->digest_str = digest_str;
    if (digest->digest_str == NULL) {
	sudo_debug_printf(SUDO_DEBUG_ERROR|SUDO_DEBUG_LINENO,
	    "unable to allocate memory");
	free(digest);
	digest = NULL;
    }

    debug_return_ptr(digest);
}

/*
 * Add a list of defaults structures to the defaults list.
 * The binding, if non-NULL, specifies a list of hosts, users, or
 * runas users the entries apply to (specified by the type).
 */
static bool
add_defaults(int type, struct member *bmem, struct defaults *defs)
{
    struct defaults *d, *next;
    struct member_list *binding;
    bool ret = true;
    debug_decl(add_defaults, SUDOERS_DEBUG_PARSER);

    if (defs != NULL) {
	/*
	 * We use a single binding for each entry in defs.
	 */
	if ((binding = malloc(sizeof(*binding))) == NULL) {
	    sudo_debug_printf(SUDO_DEBUG_ERROR|SUDO_DEBUG_LINENO,
		"unable to allocate memory");
	    sudoerserror(N_("unable to allocate memory"));
	    debug_return_bool(false);
	}
	if (bmem != NULL)
	    HLTQ_TO_TAILQ(binding, bmem, entries);
	else
	    TAILQ_INIT(binding);

	/*
	 * Set type and binding (who it applies to) for new entries.
	 * Then add to the global defaults list.
	 */
	HLTQ_FOREACH_SAFE(d, defs, entries, next) {
	    d->type = type;
	    d->binding = binding;
	    TAILQ_INSERT_TAIL(&parsed_policy.defaults, d, entries);
	}
    }

    debug_return_bool(ret);
}

/*
 * Allocate a new struct userspec, populate it, and insert it at the
 * end of the userspecs list.
 */
static bool
add_userspec(struct member *members, struct privilege *privs)
{
    struct userspec *u;
    debug_decl(add_userspec, SUDOERS_DEBUG_PARSER);

    if ((u = calloc(1, sizeof(*u))) == NULL) {
	sudo_debug_printf(SUDO_DEBUG_ERROR|SUDO_DEBUG_LINENO,
	    "unable to allocate memory");
	debug_return_bool(false);
    }
    u->lineno = this_lineno;
    u->file = rcstr_addref(sudoers);
    HLTQ_TO_TAILQ(&u->users, members, entries);
    HLTQ_TO_TAILQ(&u->privileges, privs, entries);
    STAILQ_INIT(&u->comments);
    TAILQ_INSERT_TAIL(&parsed_policy.userspecs, u, entries);

    debug_return_bool(true);
}

/*
 * Free a member struct and its contents.
 */
void
free_member(struct member *m)
{
    debug_decl(free_member, SUDOERS_DEBUG_PARSER);

    if (m->type == COMMAND || (m->type == ALL && m->name != NULL)) {
	struct command_digest *digest;
	struct sudo_command *c = (struct sudo_command *)m->name;
	free(c->cmnd);
	free(c->args);
	while ((digest = TAILQ_FIRST(&c->digests)) != NULL) {
	    TAILQ_REMOVE(&c->digests, digest, entries);
	    free(digest->digest_str);
	    free(digest);
	}
    }
    free(m->name);
    free(m);

    debug_return;
}

/*
 * Free a tailq of members but not the struct member_list container itself.
 */
void
free_members(struct member_list *members)
{
    struct member *m;
    debug_decl(free_members, SUDOERS_DEBUG_PARSER);

    while ((m = TAILQ_FIRST(members)) != NULL) {
	TAILQ_REMOVE(members, m, entries);
	free_member(m);
    }

    debug_return;
}

void
free_defaults(struct defaults_list *defs)
{
    struct member_list *prev_binding = NULL;
    struct defaults *def;
    debug_decl(free_defaults, SUDOERS_DEBUG_PARSER);

    while ((def = TAILQ_FIRST(defs)) != NULL) {
	TAILQ_REMOVE(defs, def, entries);
	free_default(def, &prev_binding);
    }

    debug_return;
}

void
free_default(struct defaults *def, struct member_list **binding)
{
    debug_decl(free_default, SUDOERS_DEBUG_PARSER);

    if (def->binding != *binding) {
	*binding = def->binding;
	if (def->binding != NULL) {
	    free_members(def->binding);
	    free(def->binding);
	}
    }
    rcstr_delref(def->file);
    free(def->var);
    free(def->val);
    free(def);

    debug_return;
}

void
free_privilege(struct privilege *priv)
{
    struct member_list *runasuserlist = NULL, *runasgrouplist = NULL;
    struct member_list *prev_binding = NULL;
    struct cmndspec *cs;
    struct defaults *def;
#ifdef HAVE_SELINUX
    char *role = NULL, *type = NULL;
#endif /* HAVE_SELINUX */
#ifdef HAVE_PRIV_SET
    char *privs = NULL, *limitprivs = NULL;
#endif /* HAVE_PRIV_SET */
    debug_decl(free_privilege, SUDOERS_DEBUG_PARSER);

    free(priv->ldap_role);
    free_members(&priv->hostlist);
    while ((cs = TAILQ_FIRST(&priv->cmndlist)) != NULL) {
	TAILQ_REMOVE(&priv->cmndlist, cs, entries);
#ifdef HAVE_SELINUX
	/* Only free the first instance of a role/type. */
	if (cs->role != role) {
	    role = cs->role;
	    free(cs->role);
	}
	if (cs->type != type) {
	    type = cs->type;
	    free(cs->type);
	}
#endif /* HAVE_SELINUX */
#ifdef HAVE_PRIV_SET
	/* Only free the first instance of privs/limitprivs. */
	if (cs->privs != privs) {
	    privs = cs->privs;
	    free(cs->privs);
	}
	if (cs->limitprivs != limitprivs) {
	    limitprivs = cs->limitprivs;
	    free(cs->limitprivs);
	}
#endif /* HAVE_PRIV_SET */
	/* Only free the first instance of runas user/group lists. */
	if (cs->runasuserlist && cs->runasuserlist != runasuserlist) {
	    runasuserlist = cs->runasuserlist;
	    free_members(runasuserlist);
	    free(runasuserlist);
	}
	if (cs->runasgrouplist && cs->runasgrouplist != runasgrouplist) {
	    runasgrouplist = cs->runasgrouplist;
	    free_members(runasgrouplist);
	    free(runasgrouplist);
	}
	free_member(cs->cmnd);
	free(cs);
    }
    while ((def = TAILQ_FIRST(&priv->defaults)) != NULL) {
	TAILQ_REMOVE(&priv->defaults, def, entries);
	free_default(def, &prev_binding);
    }
    free(priv);

    debug_return;
}

void
free_userspecs(struct userspec_list *usl)
{
    struct userspec *us;
    debug_decl(free_userspecs, SUDOERS_DEBUG_PARSER);

    while ((us = TAILQ_FIRST(usl)) != NULL) {
	TAILQ_REMOVE(usl, us, entries);
	free_userspec(us);
    }

    debug_return;
}

void
free_userspec(struct userspec *us)
{
    struct privilege *priv;
    struct sudoers_comment *comment;
    debug_decl(free_userspec, SUDOERS_DEBUG_PARSER);

    free_members(&us->users);
    while ((priv = TAILQ_FIRST(&us->privileges)) != NULL) {
	TAILQ_REMOVE(&us->privileges, priv, entries);
	free_privilege(priv);
    }
    while ((comment = STAILQ_FIRST(&us->comments)) != NULL) {
	STAILQ_REMOVE_HEAD(&us->comments, entries);
	free(comment->str);
	free(comment);
    }
    rcstr_delref(us->file);
    free(us);

    debug_return;
}

/*
 * Initialized a sudoers parse tree.
 */
void
init_parse_tree(struct sudoers_parse_tree *parse_tree, const char *lhost,
    const char *shost)
{
    TAILQ_INIT(&parse_tree->userspecs);
    TAILQ_INIT(&parse_tree->defaults);
    parse_tree->aliases = NULL;
    parse_tree->shost = shost;
    parse_tree->lhost = lhost;
}

/*
 * Move the contents of parsed_policy to new_tree.
 */
void
reparent_parse_tree(struct sudoers_parse_tree *new_tree)
{
    TAILQ_CONCAT(&new_tree->userspecs, &parsed_policy.userspecs, entries);
    TAILQ_CONCAT(&new_tree->defaults, &parsed_policy.defaults, entries);
    new_tree->aliases = parsed_policy.aliases;
    parsed_policy.aliases = NULL;
}

/*
 * Free the contents of a sudoers parse tree and initialize it.
 */
void
free_parse_tree(struct sudoers_parse_tree *parse_tree)
{
    free_userspecs(&parse_tree->userspecs);
    free_defaults(&parse_tree->defaults);
    free_aliases(parse_tree->aliases);
    parse_tree->aliases = NULL;
}

/*
 * Free up space used by data structures from a previous parser run and sets
 * the current sudoers file to path.
 */
bool
init_parser(const char *path, bool quiet, bool strict)
{
    bool ret = true;
    debug_decl(init_parser, SUDOERS_DEBUG_PARSER);

    free_parse_tree(&parsed_policy);
    init_lexer();

    rcstr_delref(sudoers);
    if (path != NULL) {
	if ((sudoers = rcstr_dup(path)) == NULL) {
	    sudo_warnx(U_("%s: %s"), __func__, U_("unable to allocate memory"));
	    ret = false;
	}
    } else {
	sudoers = NULL;
    }

    parse_error = false;
    errorlineno = -1;
    rcstr_delref(errorfile);
    errorfile = NULL;
    sudoers_warnings = !quiet;
    sudoers_strict = strict;

    debug_return_bool(ret);
}

/*
 * Initialize all options in a cmndspec.
 */
static void
init_options(struct command_options *opts)
{
    opts->notbefore = UNSPEC;
    opts->notafter = UNSPEC;
    opts->timeout = UNSPEC;
#ifdef HAVE_SELINUX
    opts->role = NULL;
    opts->type = NULL;
#endif
#ifdef HAVE_PRIV_SET
    opts->privs = NULL;
    opts->limitprivs = NULL;
#endif
}
#line 1150 "gram.c"
/* allocate initial stack or double stack size, up to YYMAXDEPTH */
static int yygrowstack(void)
{
    unsigned int newsize;
    long sslen;
    short *newss;
    YYSTYPE *newvs;

    if ((newsize = yystacksize) == 0)
        newsize = YYINITSTACKSIZE;
    else if (newsize >= YYMAXDEPTH)
        return -1;
    else if ((newsize *= 2) > YYMAXDEPTH)
        newsize = YYMAXDEPTH;
#ifdef SIZE_MAX
#define YY_SIZE_MAX SIZE_MAX
#else
#define YY_SIZE_MAX 0xffffffffU
#endif
    if (YY_SIZE_MAX / newsize < sizeof *newss)
        goto bail;
    sslen = yyssp - yyss;
    newss = yyss ? realloc(yyss, newsize * sizeof *newss) :
      malloc(newsize * sizeof *newss); /* overflow check above */
    if (newss == NULL)
        goto bail;
    yyss = newss;
    yyssp = newss + sslen;
    newvs = yyvs ? realloc(yyvs, newsize * sizeof *newvs) :
      malloc(newsize * sizeof *newvs); /* overflow check above */
    if (newvs == NULL)
        goto bail;
    yyvs = newvs;
    yyvsp = newvs + sslen;
    yystacksize = newsize;
    yysslim = yyss + newsize - 1;
    return 0;
bail:
    free(yyss);
    free(yyvs);
    yyss = yyssp = NULL;
    yyvs = yyvsp = NULL;
    yystacksize = 0;
    return -1;
}

#define YYABORT goto yyabort
#define YYREJECT goto yyabort
#define YYACCEPT goto yyaccept
#define YYERROR goto yyerrlab
int
yyparse(void)
{
    int yym, yyn, yystate;
#if YYDEBUG
    const char *yys;

    if ((yys = getenv("YYDEBUG")))
    {
        yyn = *yys;
        if (yyn >= '0' && yyn <= '9')
            yydebug = yyn - '0';
    }
#endif /* YYDEBUG */

    yynerrs = 0;
    yyerrflag = 0;
    yychar = (-1);

    if (yyss == NULL && yygrowstack()) goto yyoverflow;
    yyssp = yyss;
    yyvsp = yyvs;
    *yyssp = yystate = 0;

yyloop:
    if ((yyn = yydefred[yystate]) != 0) goto yyreduce;
    if (yychar < 0)
    {
        if ((yychar = yylex()) < 0) yychar = 0;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, reading %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
    }
    if ((yyn = yysindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: state %d, shifting to state %d\n",
                    YYPREFIX, yystate, yytable[yyn]);
#endif
        if (yyssp >= yysslim && yygrowstack())
        {
            goto yyoverflow;
        }
        *++yyssp = yystate = yytable[yyn];
        *++yyvsp = yylval;
        yychar = (-1);
        if (yyerrflag > 0)  --yyerrflag;
        goto yyloop;
    }
    if ((yyn = yyrindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
        yyn = yytable[yyn];
        goto yyreduce;
    }
    if (yyerrflag) goto yyinrecovery;
#if defined(__GNUC__)
    goto yynewerror;
#endif
yynewerror:
    yyerror("syntax error");
#if defined(__GNUC__)
    goto yyerrlab;
#endif
yyerrlab:
    ++yynerrs;
yyinrecovery:
    if (yyerrflag < 3)
    {
        yyerrflag = 3;
        for (;;)
        {
            if ((yyn = yysindex[*yyssp]) && (yyn += YYERRCODE) >= 0 &&
                    yyn <= YYTABLESIZE && yycheck[yyn] == YYERRCODE)
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: state %d, error recovery shifting\
 to state %d\n", YYPREFIX, *yyssp, yytable[yyn]);
#endif
                if (yyssp >= yysslim && yygrowstack())
                {
                    goto yyoverflow;
                }
                *++yyssp = yystate = yytable[yyn];
                *++yyvsp = yylval;
                goto yyloop;
            }
            else
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: error recovery discarding state %d\n",
                            YYPREFIX, *yyssp);
#endif
                if (yyssp <= yyss) goto yyabort;
                --yyssp;
                --yyvsp;
            }
        }
    }
    else
    {
        if (yychar == 0) goto yyabort;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, error recovery discards token %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
        yychar = (-1);
        goto yyloop;
    }
yyreduce:
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: state %d, reducing by rule %d (%s)\n",
                YYPREFIX, yystate, yyn, yyrule[yyn]);
#endif
    yym = yylen[yyn];
    if (yym)
        yyval = yyvsp[1-yym];
    else
        memset(&yyval, 0, sizeof yyval);
    switch (yyn)
    {
case 1:
#line 181 "gram.y"
{ ; }
break;
case 5:
#line 189 "gram.y"
{
			    ;
			}
break;
case 6:
#line 192 "gram.y"
{
			    yyerrok;
			}
break;
case 7:
#line 195 "gram.y"
{
			    if (!push_include(yyvsp[0].string, false)) {
				free(yyvsp[0].string);
				YYERROR;
			    }
			    free(yyvsp[0].string);
			}
break;
case 8:
#line 202 "gram.y"
{
			    if (!push_include(yyvsp[0].string, true)) {
				free(yyvsp[0].string);
				YYERROR;
			    }
			    free(yyvsp[0].string);
			}
break;
case 9:
#line 209 "gram.y"
{
			    if (!add_userspec(yyvsp[-1].member, yyvsp[0].privilege)) {
				sudoerserror(N_("unable to allocate memory"));
				YYERROR;
			    }
			}
break;
case 10:
#line 215 "gram.y"
{
			    ;
			}
break;
case 11:
#line 218 "gram.y"
{
			    ;
			}
break;
case 12:
#line 221 "gram.y"
{
			    ;
			}
break;
case 13:
#line 224 "gram.y"
{
			    ;
			}
break;
case 14:
#line 227 "gram.y"
{
			    if (!add_defaults(DEFAULTS, NULL, yyvsp[0].defaults))
				YYERROR;
			}
break;
case 15:
#line 231 "gram.y"
{
			    if (!add_defaults(DEFAULTS_USER, yyvsp[-1].member, yyvsp[0].defaults))
				YYERROR;
			}
break;
case 16:
#line 235 "gram.y"
{
			    if (!add_defaults(DEFAULTS_RUNAS, yyvsp[-1].member, yyvsp[0].defaults))
				YYERROR;
			}
break;
case 17:
#line 239 "gram.y"
{
			    if (!add_defaults(DEFAULTS_HOST, yyvsp[-1].member, yyvsp[0].defaults))
				YYERROR;
			}
break;
case 18:
#line 243 "gram.y"
{
			    if (!add_defaults(DEFAULTS_CMND, yyvsp[-1].member, yyvsp[0].defaults))
				YYERROR;
			}
break;
case 19:
#line 249 "gram.y"
{
			    yyval.string = yyvsp[-1].string;
			}
break;
case 20:
#line 252 "gram.y"
{
			    yyerrok;
			    yyval.string = yyvsp[-2].string;
			}
break;
case 21:
#line 256 "gram.y"
{
			    yyval.string = yyvsp[-1].string;
			}
break;
case 22:
#line 259 "gram.y"
{
			    yyerrok;
			    yyval.string = yyvsp[-2].string;
			}
break;
case 23:
#line 265 "gram.y"
{
			    yyval.string = yyvsp[-1].string;
			}
break;
case 24:
#line 268 "gram.y"
{
			    yyerrok;
			    yyval.string = yyvsp[-2].string;
			}
break;
case 25:
#line 272 "gram.y"
{
			    yyval.string = yyvsp[-1].string;
			}
break;
case 26:
#line 275 "gram.y"
{
			    yyerrok;
			    yyval.string = yyvsp[-2].string;
			}
break;
case 28:
#line 282 "gram.y"
{
			    HLTQ_CONCAT(yyvsp[-2].defaults, yyvsp[0].defaults, entries);
			    yyval.defaults = yyvsp[-2].defaults;
			}
break;
case 29:
#line 288 "gram.y"
{
			    yyval.defaults = new_default(yyvsp[0].string, NULL, true);
			    if (yyval.defaults == NULL) {
				sudoerserror(N_("unable to allocate memory"));
				YYERROR;
			    }
			}
break;
case 30:
#line 295 "gram.y"
{
			    yyval.defaults = new_default(yyvsp[0].string, NULL, false);
			    if (yyval.defaults == NULL) {
				sudoerserror(N_("unable to allocate memory"));
				YYERROR;
			    }
			}
break;
case 31:
#line 302 "gram.y"
{
			    yyval.defaults = new_default(yyvsp[-2].string, yyvsp[0].string, true);
			    if (yyval.defaults == NULL) {
				sudoerserror(N_("unable to allocate memory"));
				YYERROR;
			    }
			}
break;
case 32:
#line 309 "gram.y"
{
			    yyval.defaults = new_default(yyvsp[-2].string, yyvsp[0].string, '+');
			    if (yyval.defaults == NULL) {
				sudoerserror(N_("unable to allocate memory"));
				YYERROR;
			    }
			}
break;
case 33:
#line 316 "gram.y"
{
			    yyval.defaults = new_default(yyvsp[-2].string, yyvsp[0].string, '-');
			    if (yyval.defaults == NULL) {
				sudoerserror(N_("unable to allocate memory"));
				YYERROR;
			    }
			}
break;
case 35:
#line 326 "gram.y"
{
			    HLTQ_CONCAT(yyvsp[-2].privilege, yyvsp[0].privilege, entries);
			    yyval.privilege = yyvsp[-2].privilege;
			}
break;
case 36:
#line 332 "gram.y"
{
			    struct privilege *p = calloc(1, sizeof(*p));
			    if (p == NULL) {
				sudoerserror(N_("unable to allocate memory"));
				YYERROR;
			    }
			    TAILQ_INIT(&p->defaults);
			    HLTQ_TO_TAILQ(&p->hostlist, yyvsp[-2].member, entries);
			    HLTQ_TO_TAILQ(&p->cmndlist, yyvsp[0].cmndspec, entries);
			    HLTQ_INIT(p, entries);
			    yyval.privilege = p;
			}
break;
case 37:
#line 346 "gram.y"
{
			    yyval.member = yyvsp[0].member;
			    yyval.member->negated = false;
			}
break;
case 38:
#line 350 "gram.y"
{
			    yyval.member = yyvsp[0].member;
			    yyval.member->negated = true;
			}
break;
case 39:
#line 356 "gram.y"
{
			    yyval.member = new_member(yyvsp[0].string, ALIAS);
			    if (yyval.member == NULL) {
				sudoerserror(N_("unable to allocate memory"));
				YYERROR;
			    }
			}
break;
case 40:
#line 363 "gram.y"
{
			    yyval.member = new_member(NULL, ALL);
			    if (yyval.member == NULL) {
				sudoerserror(N_("unable to allocate memory"));
				YYERROR;
			    }
			}
break;
case 41:
#line 370 "gram.y"
{
			    yyval.member = new_member(yyvsp[0].string, NETGROUP);
			    if (yyval.member == NULL) {
				sudoerserror(N_("unable to allocate memory"));
				YYERROR;
			    }
			}
break;
case 42:
#line 377 "gram.y"
{
			    yyval.member = new_member(yyvsp[0].string, NTWKADDR);
			    if (yyval.member == NULL) {
				sudoerserror(N_("unable to allocate memory"));
				YYERROR;
			    }
			}
break;
case 43:
#line 384 "gram.y"
{
			    yyval.member = new_member(yyvsp[0].string, WORD);
			    if (yyval.member == NULL) {
				sudoerserror(N_("unable to allocate memory"));
				YYERROR;
			    }
			}
break;
case 45:
#line 394 "gram.y"
{
			    struct cmndspec *prev;
			    prev = HLTQ_LAST(yyvsp[-2].cmndspec, cmndspec, entries);
			    HLTQ_CONCAT(yyvsp[-2].cmndspec, yyvsp[0].cmndspec, entries);
#ifdef HAVE_SELINUX
			    /* propagate role and type */
			    if (yyvsp[0].cmndspec->role == NULL && yyvsp[0].cmndspec->type == NULL) {
				yyvsp[0].cmndspec->role = prev->role;
				yyvsp[0].cmndspec->type = prev->type;
			    }
#endif /* HAVE_SELINUX */
#ifdef HAVE_PRIV_SET
			    /* propagate privs & limitprivs */
			    if (yyvsp[0].cmndspec->privs == NULL && yyvsp[0].cmndspec->limitprivs == NULL) {
			        yyvsp[0].cmndspec->privs = prev->privs;
			        yyvsp[0].cmndspec->limitprivs = prev->limitprivs;
			    }
#endif /* HAVE_PRIV_SET */
			    /* propagate command time restrictions */
			    if (yyvsp[0].cmndspec->notbefore == UNSPEC)
				yyvsp[0].cmndspec->notbefore = prev->notbefore;
			    if (yyvsp[0].cmndspec->notafter == UNSPEC)
				yyvsp[0].cmndspec->notafter = prev->notafter;
			    /* propagate command timeout */
			    if (yyvsp[0].cmndspec->timeout == UNSPEC)
				yyvsp[0].cmndspec->timeout = prev->timeout;
			    /* propagate tags and runas list */
			    if (yyvsp[0].cmndspec->tags.nopasswd == UNSPEC)
				yyvsp[0].cmndspec->tags.nopasswd = prev->tags.nopasswd;
			    if (yyvsp[0].cmndspec->tags.noexec == UNSPEC)
				yyvsp[0].cmndspec->tags.noexec = prev->tags.noexec;
			    if (yyvsp[0].cmndspec->tags.setenv == UNSPEC &&
				prev->tags.setenv != IMPLIED)
				yyvsp[0].cmndspec->tags.setenv = prev->tags.setenv;
			    if (yyvsp[0].cmndspec->tags.log_input == UNSPEC)
				yyvsp[0].cmndspec->tags.log_input = prev->tags.log_input;
			    if (yyvsp[0].cmndspec->tags.log_output == UNSPEC)
				yyvsp[0].cmndspec->tags.log_output = prev->tags.log_output;
			    if (yyvsp[0].cmndspec->tags.send_mail == UNSPEC)
				yyvsp[0].cmndspec->tags.send_mail = prev->tags.send_mail;
			    if (yyvsp[0].cmndspec->tags.follow == UNSPEC)
				yyvsp[0].cmndspec->tags.follow = prev->tags.follow;
			    if ((yyvsp[0].cmndspec->runasuserlist == NULL &&
				 yyvsp[0].cmndspec->runasgrouplist == NULL) &&
				(prev->runasuserlist != NULL ||
				 prev->runasgrouplist != NULL)) {
				yyvsp[0].cmndspec->runasuserlist = prev->runasuserlist;
				yyvsp[0].cmndspec->runasgrouplist = prev->runasgrouplist;
			    }
			    yyval.cmndspec = yyvsp[-2].cmndspec;
			}
break;
case 46:
#line 447 "gram.y"
{
			    struct cmndspec *cs = calloc(1, sizeof(*cs));
			    if (cs == NULL) {
				sudoerserror(N_("unable to allocate memory"));
				YYERROR;
			    }
			    if (yyvsp[-3].runas != NULL) {
				if (yyvsp[-3].runas->runasusers != NULL) {
				    cs->runasuserlist =
					malloc(sizeof(*cs->runasuserlist));
				    if (cs->runasuserlist == NULL) {
					free(cs);
					sudoerserror(N_("unable to allocate memory"));
					YYERROR;
				    }
				    HLTQ_TO_TAILQ(cs->runasuserlist,
					yyvsp[-3].runas->runasusers, entries);
				}
				if (yyvsp[-3].runas->runasgroups != NULL) {
				    cs->runasgrouplist =
					malloc(sizeof(*cs->runasgrouplist));
				    if (cs->runasgrouplist == NULL) {
					free(cs);
					sudoerserror(N_("unable to allocate memory"));
					YYERROR;
				    }
				    HLTQ_TO_TAILQ(cs->runasgrouplist,
					yyvsp[-3].runas->runasgroups, entries);
				}
				free(yyvsp[-3].runas);
			    }
#ifdef HAVE_SELINUX
			    cs->role = yyvsp[-2].options.role;
			    cs->type = yyvsp[-2].options.type;
#endif
#ifdef HAVE_PRIV_SET
			    cs->privs = yyvsp[-2].options.privs;
			    cs->limitprivs = yyvsp[-2].options.limitprivs;
#endif
			    cs->notbefore = yyvsp[-2].options.notbefore;
			    cs->notafter = yyvsp[-2].options.notafter;
			    cs->timeout = yyvsp[-2].options.timeout;
			    cs->tags = yyvsp[-1].tag;
			    cs->cmnd = yyvsp[0].member;
			    HLTQ_INIT(cs, entries);
			    /* sudo "ALL" implies the SETENV tag */
			    if (cs->cmnd->type == ALL && !cs->cmnd->negated &&
				cs->tags.setenv == UNSPEC)
				cs->tags.setenv = IMPLIED;
			    yyval.cmndspec = cs;
			}
break;
case 47:
#line 500 "gram.y"
{
			    yyval.digest = new_digest(SUDO_DIGEST_SHA224, yyvsp[0].string);
			    if (yyval.digest == NULL) {
				sudoerserror(N_("unable to allocate memory"));
				YYERROR;
			    }
			}
break;
case 48:
#line 507 "gram.y"
{
			    yyval.digest = new_digest(SUDO_DIGEST_SHA256, yyvsp[0].string);
			    if (yyval.digest == NULL) {
				sudoerserror(N_("unable to allocate memory"));
				YYERROR;
			    }
			}
break;
case 49:
#line 514 "gram.y"
{
			    yyval.digest = new_digest(SUDO_DIGEST_SHA384, yyvsp[0].string);
			    if (yyval.digest == NULL) {
				sudoerserror(N_("unable to allocate memory"));
				YYERROR;
			    }
			}
break;
case 50:
#line 521 "gram.y"
{
			    yyval.digest = new_digest(SUDO_DIGEST_SHA512, yyvsp[0].string);
			    if (yyval.digest == NULL) {
				sudoerserror(N_("unable to allocate memory"));
				YYERROR;
			    }
			}
break;
case 52:
#line 531 "gram.y"
{
			    HLTQ_CONCAT(yyvsp[-2].digest, yyvsp[0].digest, entries);
			    yyval.digest = yyvsp[-2].digest;
			}
break;
case 53:
#line 537 "gram.y"
{
			    yyval.member = yyvsp[0].member;
			}
break;
case 54:
#line 540 "gram.y"
{
			    struct sudo_command *c =
				(struct sudo_command *) yyvsp[0].member->name;

			    if (yyvsp[0].member->type != COMMAND && yyvsp[0].member->type != ALL) {
				sudoerserror(N_("a digest requires a path name"));
				YYERROR;
			    }
			    if (c == NULL) {
				/* lazy-allocate sudo_command for ALL */
				if ((c = new_command(NULL, NULL)) == NULL) {
				    sudoerserror(N_("unable to allocate memory"));
				    YYERROR;
				}
				yyvsp[0].member->name = (char *)c;
			    }
			    HLTQ_TO_TAILQ(&c->digests, yyvsp[-1].digest, entries);
			    yyval.member = yyvsp[0].member;
			}
break;
case 55:
#line 561 "gram.y"
{
			    yyval.member = yyvsp[0].member;
			    yyval.member->negated = false;
			}
break;
case 56:
#line 565 "gram.y"
{
			    yyval.member = yyvsp[0].member;
			    yyval.member->negated = true;
			}
break;
case 57:
#line 571 "gram.y"
{
			    yyval.string = yyvsp[0].string;
			}
break;
case 58:
#line 576 "gram.y"
{
			    yyval.string = yyvsp[0].string;
			}
break;
case 59:
#line 580 "gram.y"
{
			    yyval.string = yyvsp[0].string;
			}
break;
case 60:
#line 585 "gram.y"
{
			    yyval.string = yyvsp[0].string;
			}
break;
case 61:
#line 590 "gram.y"
{
			    yyval.string = yyvsp[0].string;
			}
break;
case 62:
#line 595 "gram.y"
{
			    yyval.string = yyvsp[0].string;
			}
break;
case 63:
#line 599 "gram.y"
{
			    yyval.string = yyvsp[0].string;
			}
break;
case 64:
#line 604 "gram.y"
{
			    yyval.runas = NULL;
			}
break;
case 65:
#line 607 "gram.y"
{
			    yyval.runas = yyvsp[-1].runas;
			}
break;
case 66:
#line 612 "gram.y"
{
			    yyval.runas = calloc(1, sizeof(struct runascontainer));
			    if (yyval.runas != NULL) {
				yyval.runas->runasusers = new_member(NULL, MYSELF);
				/* $$->runasgroups = NULL; */
				if (yyval.runas->runasusers == NULL) {
				    free(yyval.runas);
				    yyval.runas = NULL;
				}
			    }
			    if (yyval.runas == NULL) {
				sudoerserror(N_("unable to allocate memory"));
				YYERROR;
			    }
			}
break;
case 67:
#line 627 "gram.y"
{
			    yyval.runas = calloc(1, sizeof(struct runascontainer));
			    if (yyval.runas == NULL) {
				sudoerserror(N_("unable to allocate memory"));
				YYERROR;
			    }
			    yyval.runas->runasusers = yyvsp[0].member;
			    /* $$->runasgroups = NULL; */
			}
break;
case 68:
#line 636 "gram.y"
{
			    yyval.runas = calloc(1, sizeof(struct runascontainer));
			    if (yyval.runas == NULL) {
				sudoerserror(N_("unable to allocate memory"));
				YYERROR;
			    }
			    yyval.runas->runasusers = yyvsp[-2].member;
			    yyval.runas->runasgroups = yyvsp[0].member;
			}
break;
case 69:
#line 645 "gram.y"
{
			    yyval.runas = calloc(1, sizeof(struct runascontainer));
			    if (yyval.runas == NULL) {
				sudoerserror(N_("unable to allocate memory"));
				YYERROR;
			    }
			    /* $$->runasusers = NULL; */
			    yyval.runas->runasgroups = yyvsp[0].member;
			}
break;
case 70:
#line 654 "gram.y"
{
			    yyval.runas = calloc(1, sizeof(struct runascontainer));
			    if (yyval.runas != NULL) {
				yyval.runas->runasusers = new_member(NULL, MYSELF);
				/* $$->runasgroups = NULL; */
				if (yyval.runas->runasusers == NULL) {
				    free(yyval.runas);
				    yyval.runas = NULL;
				}
			    }
			    if (yyval.runas == NULL) {
				sudoerserror(N_("unable to allocate memory"));
				YYERROR;
			    }
			}
break;
case 71:
#line 671 "gram.y"
{
			    init_options(&yyval.options);
			}
break;
case 72:
#line 674 "gram.y"
{
			    yyval.options.notbefore = parse_gentime(yyvsp[0].string);
			    free(yyvsp[0].string);
			    if (yyval.options.notbefore == -1) {
				sudoerserror(N_("invalid notbefore value"));
				YYERROR;
			    }
			}
break;
case 73:
#line 682 "gram.y"
{
			    yyval.options.notafter = parse_gentime(yyvsp[0].string);
			    free(yyvsp[0].string);
			    if (yyval.options.notafter == -1) {
				sudoerserror(N_("invalid notafter value"));
				YYERROR;
			    }
			}
break;
case 74:
#line 690 "gram.y"
{
			    yyval.options.timeout = parse_timeout(yyvsp[0].string);
			    free(yyvsp[0].string);
			    if (yyval.options.timeout == -1) {
				if (errno == ERANGE)
				    sudoerserror(N_("timeout value too large"));
				else
				    sudoerserror(N_("invalid timeout value"));
				YYERROR;
			    }
			}
break;
case 75:
#line 701 "gram.y"
{
#ifdef HAVE_SELINUX
			    free(yyval.options.role);
			    yyval.options.role = yyvsp[0].string;
#endif
			}
break;
case 76:
#line 707 "gram.y"
{
#ifdef HAVE_SELINUX
			    free(yyval.options.type);
			    yyval.options.type = yyvsp[0].string;
#endif
			}
break;
case 77:
#line 713 "gram.y"
{
#ifdef HAVE_PRIV_SET
			    free(yyval.options.privs);
			    yyval.options.privs = yyvsp[0].string;
#endif
			}
break;
case 78:
#line 719 "gram.y"
{
#ifdef HAVE_PRIV_SET
			    free(yyval.options.limitprivs);
			    yyval.options.limitprivs = yyvsp[0].string;
#endif
			}
break;
case 79:
#line 727 "gram.y"
{
			    TAGS_INIT(yyval.tag);
			}
break;
case 80:
#line 730 "gram.y"
{
			    yyval.tag.nopasswd = true;
			}
break;
case 81:
#line 733 "gram.y"
{
			    yyval.tag.nopasswd = false;
			}
break;
case 82:
#line 736 "gram.y"
{
			    yyval.tag.noexec = true;
			}
break;
case 83:
#line 739 "gram.y"
{
			    yyval.tag.noexec = false;
			}
break;
case 84:
#line 742 "gram.y"
{
			    yyval.tag.setenv = true;
			}
break;
case 85:
#line 745 "gram.y"
{
			    yyval.tag.setenv = false;
			}
break;
case 86:
#line 748 "gram.y"
{
			    yyval.tag.log_input = true;
			}
break;
case 87:
#line 751 "gram.y"
{
			    yyval.tag.log_input = false;
			}
break;
case 88:
#line 754 "gram.y"
{
			    yyval.tag.log_output = true;
			}
break;
case 89:
#line 757 "gram.y"
{
			    yyval.tag.log_output = false;
			}
break;
case 90:
#line 760 "gram.y"
{
			    yyval.tag.follow = true;
			}
break;
case 91:
#line 763 "gram.y"
{
			    yyval.tag.follow = false;
			}
break;
case 92:
#line 766 "gram.y"
{
			    yyval.tag.send_mail = true;
			}
break;
case 93:
#line 769 "gram.y"
{
			    yyval.tag.send_mail = false;
			}
break;
case 94:
#line 774 "gram.y"
{
			    yyval.member = new_member(NULL, ALL);
			    if (yyval.member == NULL) {
				sudoerserror(N_("unable to allocate memory"));
				YYERROR;
			    }
			}
break;
case 95:
#line 781 "gram.y"
{
			    yyval.member = new_member(yyvsp[0].string, ALIAS);
			    if (yyval.member == NULL) {
				sudoerserror(N_("unable to allocate memory"));
				YYERROR;
			    }
			}
break;
case 96:
#line 788 "gram.y"
{
			    struct sudo_command *c;

			    if ((c = new_command(yyvsp[0].command.cmnd, yyvsp[0].command.args)) == NULL) {
				sudoerserror(N_("unable to allocate memory"));
				YYERROR;
			    }
			    yyval.member = new_member((char *)c, COMMAND);
			    if (yyval.member == NULL) {
				free(c);
				sudoerserror(N_("unable to allocate memory"));
				YYERROR;
			    }
			}
break;
case 99:
#line 808 "gram.y"
{
			    const char *s;
			    s = alias_add(&parsed_policy, yyvsp[-2].string, HOSTALIAS,
				sudoers, this_lineno, yyvsp[0].member);
			    if (s != NULL) {
				sudoerserror(s);
				YYERROR;
			    }
			}
break;
case 101:
#line 820 "gram.y"
{
			    HLTQ_CONCAT(yyvsp[-2].member, yyvsp[0].member, entries);
			    yyval.member = yyvsp[-2].member;
			}
break;
case 104:
#line 830 "gram.y"
{
			    const char *s;
			    s = alias_add(&parsed_policy, yyvsp[-2].string, CMNDALIAS,
				sudoers, this_lineno, yyvsp[0].member);
			    if (s != NULL) {
				sudoerserror(s);
				YYERROR;
			    }
			}
break;
case 106:
#line 842 "gram.y"
{
			    HLTQ_CONCAT(yyvsp[-2].member, yyvsp[0].member, entries);
			    yyval.member = yyvsp[-2].member;
			}
break;
case 109:
#line 852 "gram.y"
{
			    const char *s;
			    s = alias_add(&parsed_policy, yyvsp[-2].string, RUNASALIAS,
				sudoers, this_lineno, yyvsp[0].member);
			    if (s != NULL) {
				sudoerserror(s);
				YYERROR;
			    }
			}
break;
case 112:
#line 867 "gram.y"
{
			    const char *s;
			    s = alias_add(&parsed_policy, yyvsp[-2].string, USERALIAS,
				sudoers, this_lineno, yyvsp[0].member);
			    if (s != NULL) {
				sudoerserror(s);
				YYERROR;
			    }
			}
break;
case 114:
#line 879 "gram.y"
{
			    HLTQ_CONCAT(yyvsp[-2].member, yyvsp[0].member, entries);
			    yyval.member = yyvsp[-2].member;
			}
break;
case 115:
#line 885 "gram.y"
{
			    yyval.member = yyvsp[0].member;
			    yyval.member->negated = false;
			}
break;
case 116:
#line 889 "gram.y"
{
			    yyval.member = yyvsp[0].member;
			    yyval.member->negated = true;
			}
break;
case 117:
#line 895 "gram.y"
{
			    yyval.member = new_member(yyvsp[0].string, ALIAS);
			    if (yyval.member == NULL) {
				sudoerserror(N_("unable to allocate memory"));
				YYERROR;
			    }
			}
break;
case 118:
#line 902 "gram.y"
{
			    yyval.member = new_member(NULL, ALL);
			    if (yyval.member == NULL) {
				sudoerserror(N_("unable to allocate memory"));
				YYERROR;
			    }
			}
break;
case 119:
#line 909 "gram.y"
{
			    yyval.member = new_member(yyvsp[0].string, NETGROUP);
			    if (yyval.member == NULL) {
				sudoerserror(N_("unable to allocate memory"));
				YYERROR;
			    }
			}
break;
case 120:
#line 916 "gram.y"
{
			    yyval.member = new_member(yyvsp[0].string, USERGROUP);
			    if (yyval.member == NULL) {
				sudoerserror(N_("unable to allocate memory"));
				YYERROR;
			    }
			}
break;
case 121:
#line 923 "gram.y"
{
			    yyval.member = new_member(yyvsp[0].string, WORD);
			    if (yyval.member == NULL) {
				sudoerserror(N_("unable to allocate memory"));
				YYERROR;
			    }
			}
break;
case 123:
#line 933 "gram.y"
{
			    HLTQ_CONCAT(yyvsp[-2].member, yyvsp[0].member, entries);
			    yyval.member = yyvsp[-2].member;
			}
break;
case 124:
#line 939 "gram.y"
{
			    yyval.member = yyvsp[0].member;
			    yyval.member->negated = false;
			}
break;
case 125:
#line 943 "gram.y"
{
			    yyval.member = yyvsp[0].member;
			    yyval.member->negated = true;
			}
break;
case 126:
#line 949 "gram.y"
{
			    yyval.member = new_member(yyvsp[0].string, ALIAS);
			    if (yyval.member == NULL) {
				sudoerserror(N_("unable to allocate memory"));
				YYERROR;
			    }
			}
break;
case 127:
#line 956 "gram.y"
{
			    yyval.member = new_member(NULL, ALL);
			    if (yyval.member == NULL) {
				sudoerserror(N_("unable to allocate memory"));
				YYERROR;
			    }
			}
break;
case 128:
#line 963 "gram.y"
{
			    yyval.member = new_member(yyvsp[0].string, WORD);
			    if (yyval.member == NULL) {
				sudoerserror(N_("unable to allocate memory"));
				YYERROR;
			    }
			}
break;
#line 2351 "gram.c"
    }
    yyssp -= yym;
    yystate = *yyssp;
    yyvsp -= yym;
    yym = yylhs[yyn];
    if (yystate == 0 && yym == 0)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: after reduction, shifting from state 0 to\
 state %d\n", YYPREFIX, YYFINAL);
#endif
        yystate = YYFINAL;
        *++yyssp = YYFINAL;
        *++yyvsp = yyval;
        if (yychar < 0)
        {
            if ((yychar = yylex()) < 0) yychar = 0;
#if YYDEBUG
            if (yydebug)
            {
                yys = 0;
                if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
                if (!yys) yys = "illegal-symbol";
                printf("%sdebug: state %d, reading %d (%s)\n",
                        YYPREFIX, YYFINAL, yychar, yys);
            }
#endif
        }
        if (yychar == 0) goto yyaccept;
        goto yyloop;
    }
    if ((yyn = yygindex[yym]) && (yyn += yystate) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yystate)
        yystate = yytable[yyn];
    else
        yystate = yydgoto[yym];
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: after reduction, shifting from state %d \
to state %d\n", YYPREFIX, *yyssp, yystate);
#endif
    if (yyssp >= yysslim && yygrowstack())
    {
        goto yyoverflow;
    }
    *++yyssp = yystate;
    *++yyvsp = yyval;
    goto yyloop;
yyoverflow:
    yyerror("yacc stack overflow");
yyabort:
    free(yyss);
    free(yyvs);
    yyss = yyssp = NULL;
    yyvs = yyvsp = NULL;
    yystacksize = 0;
    return (1);
yyaccept:
    free(yyss);
    free(yyvs);
    yyss = yyssp = NULL;
    yyvs = yyvsp = NULL;
    yystacksize = 0;
    return (0);
}
