#ifdef USE_MPFR
#include <mpfr.h>
#define MY_PRECISION 512
#else
#include <math.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>

#define INVALID_OFFSET (-1)
#define MYDEBUG 0

#ifndef USE_MPFR
    #ifndef USE_FLOAT
        #ifndef USE_LDBL
            typedef double DBL_T;
            #define STR_FUNC strtod
            #define EXP_FUNC exp
            #define POW_FUNC pow
			#define SIN_FUNC sin
            #define TYPE_MOD "%lf"
            #ifndef NAN
            #define NAN nan("");
            #endif
        #else
            typedef long double DBL_T;
            #define STR_FUNC strtold
            #define EXP_FUNC expl
            #define POW_FUNC powl
			#define SIN_FUNC sinl
            #define TYPE_MOD "%Lf"
            #ifndef NAN
            #define NAN nanl("");
            #endif
        #endif
    #else
        typedef float DBL_T;
        #define STR_FUNC strtof
        #define EXP_FUNC expf
        #define POW_FUNC powf
		#define SIN_FUNC sinf
        #define TYPE_MOD "%f"
        #ifndef NAN
        #define NAN nanf("");
        #endif
    #endif
#endif
#ifdef USE_MPFR
#define TYPE_MOD "%.128Rf"
void parse_level1(const char* buffer, int *offset, mpfr_t res, mpfr_t* values);
void parse_level2(const char* buffer, int *offset, mpfr_t res, mpfr_t* values);
void parse_level3(const char* buffer, int *offset, mpfr_t res, mpfr_t* values);
void parse_level4(const char* buffer, int *offset, mpfr_t res, mpfr_t* values);
#else
#define MPFR_DECL_INIT(X,PERC) DBL_T X=NAN;
#define mpfr_init2(X,Y) {do{} while(0);}
#define mpfr_set(X,Y,METHOD) {do{X=Y;} while(0);}
#define mpfr_set_si(X,Y, METHOD)  {do{X=Y;} while(0);}
#define mpfr_clear(X) do{} while(0);
#define mpfr_add(RES,A,B,METHOD) {do{RES=A+B;} while(0);}
#define mpfr_mul(RES,A,B,METHOD) {do{RES=A*B;} while(0);}
#define mpfr_div(RES,A,B,METHOD) {do{RES=A/B;} while(0);}
#define mpfr_pow(RES,A,B,METHOD) {do{RES=POW_FUNC(A,B);} while(0);}
#define mpfr_sin(RES,X,METHOD) {do{RES=SIN_FUNC(X);} while(0);}
#define mpfr_exp(RES,POWER,METHOD) {do{RES=EXP_FUNC(POWER);} while(0);}
#define mpfr_set_nan(X) {do{X=NAN;} while(0);}
#define mpfr_strtofr(X, STR, TAIL, BASE, METHOD) X=STR_FUNC(STR, TAIL);
#define mpfr_const_pi(X,METHOD) {do{X=M_PI;} while(0);}
#define mpfr_printf(...) {do{printf(__VA_ARGS__);} while(0);}

#define MPFR_RNDN 0
typedef DBL_T mpfr_t;
void parse_level1(const char* buffer, int *offset, DBL_T& res, mpfr_t* values);
void parse_level2(const char* buffer, int *offset, DBL_T& res, mpfr_t* values);
void parse_level3(const char* buffer, int *offset, DBL_T& res, mpfr_t* values);
void parse_level4(const char* buffer, int *offset, DBL_T& res, mpfr_t* values);
#endif
#define STRLEN(X) (sizeof(X)/sizeof(X[0])-1) //minus one to remove the null termination
#define CHECK_INITIALS(STR,NEEDLE)  (strncmp(STR,NEEDLE,STRLEN(NEEDLE))==0 && isalnum(*(STR+STRLEN(NEEDLE)))==0)

bool is_whitespace(char c)
{
   return (' '  == c) || ('\n' == c) ||
          ('\r' == c) || ('\t' == c) ||
          ('\b' == c) || ('\v' == c) ||
          ('\f' == c) ;
}
bool is_functional(char c)
{
   return ('('  == c) || (')' == c) ||
          ('+' == c) || ('-' == c) ||
          ('*' == c) || ('/' == c)|| ('.' == c);
}
bool is_Digit(char c)
{
   return (c>='0' && c<='9');
}
bool is_Alpha(char c)
{
   return (c>='a' && c<='z')||(c>='A' && c<='Z');
}
void skip_spaces(const char* buffer, int *offset)
{
    while (is_whitespace(buffer[*offset]))
    {
        (*offset)++;
    }
}
void next_token(const char* buffer, int *offset)
{
	while (is_whitespace(buffer[*offset]))
    {
        (*offset)++;
    }
}
int token_length(const char* buffer, int offset)
{
	while (is_whitespace(buffer[offset]))
    {
        (offset)++;
    }
	return 0;
}

#ifdef USE_MPFR
void parse_level1(const char* buffer, int *offset, mpfr_t res, mpfr_t* values) //addition and subtraction
#else
void parse_level1(const char* buffer, int *offset, DBL_T& res, mpfr_t* values) //addition and subtraction
#endif
{
#if MYDEBUG
	printf("parse_level1 %d\n",*offset);
#endif

	mpfr_t left;
	int my_offset;
	skip_spaces(buffer, offset);
	my_offset=*offset;

    parse_level2(buffer,&my_offset,left,values);
	#if MYDEBUG
	printf("parse_level1 a %d\n",*offset);
    #ifdef USE_MPFR
        mpfr_printf ("parse_level1 n = " TYPE_MOD "\n",left);
	#endif
    #endif
    if (*offset==my_offset || *offset==INVALID_OFFSET)
	{
        *offset=INVALID_OFFSET;
		return;
	}

#if MYDEBUG
	printf("parse_level1 b %d\n",*offset);
#endif
	skip_spaces(buffer, &my_offset);
	*offset=my_offset;
	while (buffer[my_offset]=='+' || buffer[my_offset]=='-')
	{
#if MYDEBUG
		printf("parse_level1 loop %d\n",*offset);
#endif
		char op=buffer[my_offset];
		if (op=='-')
		{
			op='+';
		}
		else
			++my_offset;
        mpfr_t right;
		skip_spaces(buffer, &my_offset);
		*offset=my_offset;
        parse_level2(buffer,&my_offset,right,values);
#if MYDEBUG
		printf("parse_level1 c %d %d\n",*offset,my_offset);
#endif
        if (*offset==my_offset || *offset==INVALID_OFFSET)
        {
            *offset=INVALID_OFFSET;
			return;
		}
#if MYDEBUG
		printf("parse_level1 d %d\n",*offset);
		#endif
		skip_spaces(buffer, &my_offset);

		*offset=my_offset;
		mpfr_add(left,left,right,MPFR_RNDN);
		mpfr_clear (right);


	}
    if (buffer[my_offset]!='\n' && buffer[my_offset]!=')' && buffer[my_offset]!='\0' && buffer[my_offset]!=',')
	{
        *offset=INVALID_OFFSET;
		return;
	}
#if MYDEBUG
    else
    {
        printf("parse_level1 character %c\n",buffer[my_offset]);
    }
#endif

	mpfr_init2(res,MY_PRECISION);
	mpfr_set(res,left,MPFR_RNDN);
	mpfr_clear (left);

	return;
}
#ifdef USE_MPFR
void parse_level2(const char* buffer, int *offset, mpfr_t res, mpfr_t* values) //multiplication and division
#else
void parse_level2(const char* buffer, int *offset, DBL_T& res, mpfr_t* values)
#endif
{
    mpfr_t left;

	
	int my_offset;
	
	my_offset=*offset;
    parse_level3(buffer,&my_offset,left,values);
	
	
    if (*offset==my_offset || *offset==INVALID_OFFSET)
	{
        *offset=INVALID_OFFSET;
        return;
	}
	*offset=my_offset;
	while (buffer[my_offset]=='*' || buffer[my_offset]=='/')
	{
		
		char op=buffer[my_offset];
		++my_offset;
		mpfr_t right;
		skip_spaces(buffer, &my_offset);
		*offset=my_offset;
        parse_level3(buffer,&my_offset,right,values);
        if (*offset==my_offset || *offset==INVALID_OFFSET)
		{
            *offset=INVALID_OFFSET;
			return;
		}
		skip_spaces(buffer, &my_offset);
		*offset=my_offset;
		if (op=='*')
        {
			mpfr_mul(left,left,right,MPFR_RNDN);
        }
		else
        {
			mpfr_div(left,left,right,MPFR_RNDN);
        }
		mpfr_clear (right);
	}

	mpfr_init2(res,MY_PRECISION);
	mpfr_set(res,left,MPFR_RNDN);
	mpfr_clear (left);

	return;
}
#ifdef USE_MPFR
void parse_level3(const char* buffer, int *offset, mpfr_t res, mpfr_t* values) //minus sign
#else
void parse_level3(const char* buffer, int *offset, DBL_T& res, mpfr_t* values)
#endif
{
	
	char op=0;
	
	int my_offset;
	
	my_offset=*offset;
	if (buffer[my_offset]=='-')
	{
		op='-';
		++my_offset;
	}
    parse_level4(buffer,&my_offset,res,values);
	
    if (*offset==my_offset || *offset==INVALID_OFFSET)
	{

        *offset=INVALID_OFFSET;
		return;
	}
	skip_spaces(buffer, &my_offset);
	*offset=my_offset;
	
	if (op)
	{
		mpfr_t minus_one;
		mpfr_init2(minus_one,MPFR_PREC_MIN+2);
		mpfr_set_si (minus_one, -1, MPFR_RNDN);
		mpfr_mul(res,res,minus_one,MPFR_RNDN);
		mpfr_clear(minus_one);
	}
	
}
#ifdef USE_MPFR
void parse_level4(const char* buffer, int *offset, mpfr_t res, mpfr_t* values) //object itself
#else
void parse_level4(const char* buffer, int *offset, DBL_T& res, mpfr_t* values)
#endif
{
	
	int my_offset;
	my_offset=*offset;
	if (buffer[*offset]==0 || buffer[*offset]==')')//unbalanced?
	{
		*offset=INVALID_OFFSET;
		return;
	}
	if (buffer[my_offset]=='(')
	{
		++my_offset;
        parse_level1(buffer,&my_offset,res,values);
		
		if (buffer[my_offset]!=')')
		{
            *offset=INVALID_OFFSET;
			return;
		}
		++my_offset;
		*offset=my_offset;
		return;
	}

    if (values!=0)
    {
        if (CHECK_INITIALS(&buffer[my_offset],"x"))
        {
            mpfr_init2(res,MY_PRECISION);
            mpfr_set(res,values[0],MPFR_RNDN);
            my_offset+=STRLEN("x");
            *offset=my_offset;
            return;
        }
        if (CHECK_INITIALS(&buffer[my_offset],"y"))
        {
            mpfr_init2(res,MY_PRECISION);
            mpfr_set(res,values[1],MPFR_RNDN);
            my_offset+=STRLEN("y");
            *offset=my_offset;
            return;
        }
    }
    if (CHECK_INITIALS(&buffer[my_offset],"pi"))
	{
		mpfr_init2(res,MY_PRECISION);
		mpfr_const_pi (res,MPFR_RNDN);
        my_offset+=STRLEN("pi");
		*offset=my_offset;
		return;
	}
    if (CHECK_INITIALS(&buffer[my_offset],"e"))
	{
		mpfr_init2(res,MY_PRECISION);
		mpfr_set_si(res,1,MPFR_RNDN);
		mpfr_exp (res, res, MPFR_RNDN);
        my_offset+=STRLEN("e");
		*offset=my_offset;
		return;
	}
    if (CHECK_INITIALS(&buffer[my_offset],"pow"))
    {
        my_offset+=STRLEN("pow");
        *offset=my_offset;
        skip_spaces(buffer, &my_offset);
        if (buffer[my_offset]=='(')
        {
            ++my_offset;
            mpfr_t left;
            parse_level1(buffer,&my_offset,left,values);

            if (buffer[my_offset]!=',')
            {
                *offset=INVALID_OFFSET;
                return;
            }
            ++my_offset;
            mpfr_t right;
            parse_level1(buffer,&my_offset,right,values);
            if (buffer[my_offset]!=')')
            {
                *offset=INVALID_OFFSET;
                return;
            }
            ++my_offset;
            *offset=my_offset;
            mpfr_init2(res,MY_PRECISION);
            mpfr_pow(res,left,right,MPFR_RNDN);
            mpfr_clear (right);
            mpfr_clear (left);
            return;
        }
        else
        {
            *offset=INVALID_OFFSET;
            return;
        }
        return;
    }
    if (CHECK_INITIALS(&buffer[my_offset],"sin"))
    {
        my_offset+=STRLEN("sin");
        *offset=my_offset;
        skip_spaces(buffer, &my_offset);
        if (buffer[my_offset]=='(')
        {
        	++my_offset;
        	mpfr_t left;
			parse_level1(buffer,&my_offset,left,values);
			if (my_offset==INVALID_OFFSET)
			{
	            *offset=INVALID_OFFSET;
	            return;
			}
			if (buffer[my_offset]!=')')
			{
				*offset=INVALID_OFFSET;
				return;
			}
			++my_offset;
			*offset=my_offset;
            mpfr_init2(res,MY_PRECISION);
            mpfr_sin(res,left,MPFR_RNDN);
            mpfr_clear (left);
			return;
        }
        else
        {
            *offset=INVALID_OFFSET;
            return;
        }
    }
	if (is_Digit(buffer[my_offset])||buffer[my_offset]=='.'||buffer[my_offset]=='+')
	{
		mpfr_init2(res,MY_PRECISION);
		char* endptr;
		mpfr_strtofr (res, buffer+my_offset, &endptr, 10, MPFR_RNDN);
		*offset=my_offset=(int)((ptrdiff_t)endptr-(ptrdiff_t)buffer);
	}
	else
	{
		*offset=INVALID_OFFSET;
		return;
	}
}


void parse(const char* buffer)
{
	mpfr_t res;
	int offset=0;
    do{
        parse_level1(buffer, &offset,res,0);
        if (offset!=INVALID_OFFSET && (buffer[offset]==0||buffer[offset]==','))
        {
            mpfr_printf ("!! res = " TYPE_MOD "\n",res);
        }
        else
        {
    		mpfr_init2(res,MY_PRECISION);
    		mpfr_set_nan(res);
            break;
        }
    }
    while(buffer[offset++]==',');
    #ifdef USE_MPFR
    mpfr_free_cache();
    #endif
}
#ifdef USE_FLOAT
float parseXY(const char* buffer,float x,float y,bool* ok)
{
    float values[2];
    values[0]=x;
    values[1]=y;
    int offset=0;
    float res;
    parse_level1(buffer, &offset,res,values);
    //mpfr_printf ("!! res = " TYPE_MOD "\n",res);
    if (offset!=INVALID_OFFSET && buffer[offset]==0)
    {
    	*ok=true;
    }
    else
    {
    	*ok=false;
    }
    return res;

}
#endif
