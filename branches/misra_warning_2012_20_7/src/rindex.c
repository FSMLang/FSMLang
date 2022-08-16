/**
	rindex.c

		Implements rindex for those OS's which don't play well with others.

	Long Description:

	Creation: 	stanst00		Dec-02-2004
*/

char *rindex(const char *str,int c)
{
	char *retVal = 0;
	const char *cp = str;
	
	while (*cp++) {
		if (*cp == c)
			retVal = cp;
	}

	return retVal;

}
