/* Start defining the basic language using the lex
 * way of doing this. We will then hook this into
 * a yacc specification that builds an AST and compiles
 * it
 */
%{
/* We need this to call atof() below */
%}

DIGIT	[0-9]
ID	[a-z][a-z0-9]*

%%

^[ \t]*{DIGIT}+	{
	printf("A line label: %s (%d)\n", yytext,
		atoi(yytext));
}

{DIGIT}+ {
	printf("A number: %s (%d)\n", yytext,
		atoi(yytext));
}

[ \t]*rem.*\n	{
	printf("A comment: %s", yytext);
}

input|sinput|print|sprint|inc|dec|let|goto|if|call|ret|end {
	printf("A keyword: %s\n", yytext);
}

{ID}   {
	printf("A variable name: %s\n", yytext);
}

"=="|"!="|">"|">="|"<"|"<="    {
	printf("A Comparison: %s\n", yytext);
}

"="|"+"|"-"|"*"|"/"|"%"|"("|")"    {
	printf("An operator: %s\n", yytext);
}

[ \t]+		/* eat whitespace */

.	printf("Unrecognized character: %s\n", yytext);

%%

main(int argc, char *argv[])
{
   ++argv; --argc;  /* skip over program name */
   if (argc > 0)
	   yyin = fopen(argv[0], "r");
   else
	   yyin = stdin;

   yylex();
}

