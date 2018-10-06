#include    "test1.h"


// Main entry point for this example
//
int ANTLR3_CDECL
main	(int argc, char *argv[])
{
    // Name of the input file. Note that we always use the abstract type pANTLR3_UINT8
    // for ASCII/8 bit strings - the runtime library guarantees that this will be
    // good on all platforms. This is a general rule - always use the ANTLR3 supplied
    // typedefs for pointers/types/etc.
    //
    pANTLR3_UINT8	    fName;

    // The ANTLR3 character input stream, which abstracts the input source such that
    // it is easy to provide input from different sources such as files, or
    // memory strings.
    //
    // For input from a file, see code below
    //
    // Note that this is essentially a pointer to a structure containing pointers to functions.
    // You can create your own input stream type (copy one of the existing ones) and override any
    // individual function by installing your own pointer after you have created the standard
    // version.
    //
    pANTLR3_INPUT_STREAM    input;

    // The lexer is of course generated by ANTLR, and so the lexer type is not upper case.
    // The lexer is supplied with a pANTLR3_INPUT_STREAM from whence it consumes its
    // input and generates a token stream as output.
    //
    ptest1		    lxr;

    // The token stream is produced by the ANTLR3 generated lexer. Again it is a structure based
    // API/Object, which you can customise and override methods of as you wish. a Token stream is
    // supplied to the generated parser, and you can write your own token stream and pass this in
    // if you wish.
    //
    pANTLR3_COMMON_TOKEN_STREAM	    tstream;

    pANTLR3_VECTOR the_all_tokens;

    int i;

    if (argc == 2 && argv[1] != NULL) {
    	fName = (pANTLR3_UINT8) argv[1];
    } else {
    	exit(1);
    }


    // Create the input stream using the supplied file name.
    input	= antlr3FileStreamNew(fName, ANTLR3_ENC_8BIT);

    // The input will be created successfully, providing that there is enough
    // memory and the file exists etc
    //
    if (input == 0) {
	fprintf(stderr, "Failed to open file %s\n", (char *)fName);
	exit(1);
    }

    // Our input stream is now open and all set to go, so we can create a new instance of our
    // lexer and set the lexer input to our input stream:
    lxr	    = test1New(input);	    // CLexerNew is generated by ANTLR

    // Need to check for errors
    //
    if (lxr == NULL) {

	fprintf(stderr, "Unable to create the lexer due to malloc() failure1\n");
	exit(1);
    }

    // Our lexer is in place, so we can create the token stream from it
    // NB: Nothing happens yet other than the file has been read. We are just
    // connecting all these things together and they will be invoked when we
    // call the parser rule. ANTLR3_SIZE_HINT can be left at the default usually
    // unless you have a very large token stream/input. Each generated lexer
    // provides a token source interface, which is the second argument to the
    // token stream creator.
    // Note that even if you implement your own token structure, it will always
    // contain a standard common token within it and this is the pointer that
    // you pass around to everything else. A common token as a pointer within
    // it that should point to your own outer token structure.
    //

    tstream = antlr3CommonTokenStreamSourceNew(ANTLR3_SIZE_HINT, TOKENSOURCE(lxr));

    if (tstream == NULL) {
	fprintf(stderr, "Out of memory trying to allocate token stream\n");
	exit(1);
    }

    the_all_tokens = tstream->getTokens(tstream);
    for (i=0; i<the_all_tokens->elementsSize; i++) {
       pANTLR3_COMMON_TOKEN the_token = (pANTLR3_COMMON_TOKEN) the_all_tokens->get(the_all_tokens, i);
       printf("Token:%d  %s\n", the_token->getType(the_token), the_token->getText(the_token)->chars);
    }


    // We did not return anything from this parser rule, so we can finish. It only remains
    // to close down our open objects, in the reverse order we created them
    //
    tstream ->free  (tstream);	    tstream = NULL;
    lxr	    ->free  (lxr);	    lxr = NULL;
    input   ->close (input);	    input = NULL;

    return 0;
}