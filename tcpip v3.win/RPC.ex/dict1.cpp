/* dict1.cpp - main, nextin */

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#define	MAXWORDLEN	50	/* maximum length of a command or word	*/

int	nextin(char *, char *), initw(void), insertw(char *),
	deletew(char *), lookupw(char *);

/*------------------------------------------------------------------------
 * main - insert, delete, or lookup words in a dictionary as specified
 *------------------------------------------------------------------------
 */
int
main(int argc, char *argv[])
{
	char	word[MAXWORDLEN+1]; /*space to hold word from input line*/
	char	cmd;
	int	wrdlen;		/* length of input word			*/

	while (1) {
		wrdlen = nextin(&cmd, word);
		if (wrdlen < 0)
			exit(0);
		switch (cmd) {
		case 'I':	/* "initialize" */
			initw();
			printf("Dictionary initialized to empty.\n");
			break;
		case 'i':	/* "insert" */
			insertw(word);
			printf("%s inserted.\n",word);
			break;
		case 'd':	/* "delete" */
			if (deletew(word))
				printf("%s deleted.\n",word);
			else
				printf("%s not found.\n",word);
			break;
		case 'l':	/* "lookup" */
			if (lookupw(word))
				printf("%s was found.\n",word);
			else
				printf("%s was not found.\n",word);
			break;
		case 'q':	/* quit */
			printf("program quits.\n");
			exit(0);
		default:	/* illegal input */
			printf("command %c invalid.\n", cmd);
			break;
		}
	}
}

/*------------------------------------------------------------------------
 * nextin - read a command and (possibly) a word from the next input line
 *------------------------------------------------------------------------
 */
int
nextin(char *cmd, char *word)
{
	int	i, ch;

	ch = getc(stdin);
	while (isspace(ch))
		ch = getc(stdin);
	if (ch == EOF)
		return -1;
	*cmd = (char) ch;
	ch = getc(stdin);
	while (isspace(ch))
		ch = getc(stdin);
	if (ch == EOF)
		return -1;
	if (ch == '\n')
		return 0;
	i = 0;
	while (!isspace(ch)) {
		if (++i > MAXWORDLEN) {
			printf("error: word too long.\n");
			exit(1);
		}
		*word++ = ch;
		ch = getc(stdin);
	}
	return i;
}
