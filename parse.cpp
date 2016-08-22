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
void skip_spaces(const char* buffer, int *offset)
{
	while (buffer[*offset] == ' '|| buffer[*offset] == '\t')
	{
		(*offset)++;
	}
}

#ifndef USE_FLOAT
#ifndef USE_LDBL
typedef double DBL_T;
#define STR_FUNC strtod
#define EXP_FUNC exp
#define TYPE_MOD "%lf"
#ifndef NAN
#define NAN nan("");
#endif
#else
typedef long double DBL_T;
#define STR_FUNC strtold
#define EXP_FUNC expl
#define TYPE_MOD "%Lf"
#ifndef NAN
#define NAN nanl("");
#endif
#endif
#else
typedef float DBL_T;
#define STR_FUNC strtof
#define EXP_FUNC expf
#define TYPE_MOD "%f"
#ifndef NAN
#define NAN nanf("");
#endif
#endif
#ifdef USE_MPFR
void parse_level1(const char* buffer, int *offset, mpfr_t res);
void parse_level2(const char* buffer, int *offset, mpfr_t res);
void parse_level3(const char* buffer, int *offset, mpfr_t res);
void parse_level4(const char* buffer, int *offset, mpfr_t res);
#else
#define MPFR_DECL_INIT(X,PERC) DBL_T X=NAN;
#define mpfr_init2(X,Y) {do{} while(0);}
#define mpfr_set(X,Y,METHOD) {do{X=Y;} while(0);}
#define mpfr_set_si(X,Y, METHOD)  {do{X=Y;} while(0);}
#define mpfr_clear(X) do{} while(0);
#define mpfr_add(RES,A,B,METHOD) {do{RES=A+B;} while(0);}
#define mpfr_mul(RES,A,B,METHOD) {do{RES=A*B;} while(0);}
#define mpfr_div(RES,A,B,METHOD) {do{RES=A/B;} while(0);}
#define mpfr_set_nan(X) {do{X=NAN;} while(0);}
#define mpfr_strtofr(X, STR, TAIL, BASE, METHOD) X=STR_FUNC(STR, TAIL);
#define mpfr_const_pi(X,METHOD) {do{X=M_PI;} while(0);}
#define mpfr_exp(RES,POWER,METHOD) {do{RES=EXP_FUNC(POWER);} while(0);}
#define MPFR_RNDN 0
typedef DBL_T mpfr_t;
void parse_level1(const char* buffer, int *offset, DBL_T& res);
void parse_level2(const char* buffer, int *offset, DBL_T& res);
void parse_level3(const char* buffer, int *offset, DBL_T& res);
void parse_level4(const char* buffer, int *offset, DBL_T& res);
#endif


#ifdef USE_MPFR
void parse_level1(const char* buffer, int *offset, mpfr_t res) //addition and subtraction
#else
void parse_level1(const char* buffer, int *offset, DBL_T& res) //addition and subtraction
#endif
{
#if MYDEBUG
	printf("parse_level1 %d\n",*offset);
#endif

	mpfr_t left;
	int my_offset;
	my_offset=*offset;

	

	parse_level2(buffer,&my_offset,left);
	#if MYDEBUG
	printf("parse_level1 a %d\n",*offset);
    #ifdef USE_MPFR
	mpfr_printf ("parse_level1 n = %.128Rf\n",left);
	#endif
    #endif
    if (*offset==my_offset || *offset==INVALID_OFFSET)
	{
		MPFR_DECL_INIT(mpfr_nan,MPFR_PREC_MIN);
		mpfr_set (res, mpfr_nan, MPFR_RNDN);
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

		parse_level2(buffer,&my_offset,right);
#if MYDEBUG
		printf("parse_level1 c %d %d\n",*offset,my_offset);
#endif
        if (*offset==my_offset || *offset==INVALID_OFFSET)
        {
            MPFR_DECL_INIT(mpfr_nan,MPFR_PREC_MIN);
            mpfr_set (res, mpfr_nan, MPFR_RNDN);
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
	if (buffer[my_offset]!='\n' && buffer[my_offset]!=')' && buffer[my_offset]!='\0')
	{
		mpfr_init2(res,MY_PRECISION);
		mpfr_set_nan(res);
        *offset=INVALID_OFFSET;
		return;
	}

	mpfr_init2(res,MY_PRECISION);
	mpfr_set(res,left,MPFR_RNDN);
	mpfr_clear (left);

	return;
}
#ifdef USE_MPFR
void parse_level2(const char* buffer, int *offset, mpfr_t res) //multiplication and division
#else
void parse_level2(const char* buffer, int *offset, DBL_T& res)
#endif
{
    mpfr_t left;

	
	int my_offset;
	skip_spaces(buffer, offset);
	my_offset=*offset;
	parse_level3(buffer,&my_offset,left);
	
	
    if (*offset==my_offset || *offset==INVALID_OFFSET)
	{
        #ifdef USE_MPFR
        mpfr_init2(res,MY_PRECISION);
        mpfr_set_nan(res);
        #else
        res=NAN;
        #endif
        *offset=INVALID_OFFSET;
        return;
	}
	skip_spaces(buffer, &my_offset);
	*offset=my_offset;
	
	while (buffer[my_offset]=='*' || buffer[my_offset]=='/')
	{
		
		char op=buffer[my_offset];
		++my_offset;
		mpfr_t right;
		parse_level3(buffer,&my_offset,right);
        if (*offset==my_offset || *offset==INVALID_OFFSET)
		{

            mpfr_init2(res,MY_PRECISION);
            mpfr_set_nan(res);
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
void parse_level3(const char* buffer, int *offset, mpfr_t res) //minus sign
#else
void parse_level3(const char* buffer, int *offset, DBL_T& res)
#endif
{
	
	char op=0;
	
	int my_offset;
	skip_spaces(buffer, offset);
	my_offset=*offset;
	if (buffer[my_offset]=='-')
	{
		op='-';
		++my_offset;
	}
	parse_level4(buffer,&my_offset,res);
	
    if (*offset==my_offset || *offset==INVALID_OFFSET)
	{
		mpfr_init2(res,MY_PRECISION);
		mpfr_set_nan(res);
        *offset=INVALID_OFFSET;
		return;
	}
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
void parse_level4(const char* buffer, int *offset, mpfr_t res) //object itself
#else
void parse_level4(const char* buffer, int *offset, DBL_T& res)
#endif
{
	
	int my_offset;
	skip_spaces(buffer, offset);
	my_offset=*offset;
	
	if (buffer[my_offset]=='(')
	{
		++my_offset;
		parse_level1(buffer,&my_offset,res);
		
		if (buffer[my_offset]!=')')
		{
			MPFR_DECL_INIT(mpfr_nan,MPFR_PREC_MIN);
			mpfr_set(res,mpfr_nan,MPFR_RNDN);
            *offset=INVALID_OFFSET;
			return;
		}
		++my_offset;
		*offset=my_offset;
		return;
	}
	if (buffer[*offset]==0)
	{
		MPFR_DECL_INIT(mpfr_nan,MPFR_PREC_MIN);
		mpfr_set(res,mpfr_nan,MPFR_RNDN);
		return;
	}
	if (strncmp(&buffer[my_offset],"pi",2)==0 && isalnum(buffer[my_offset+2])==0)
	{
		mpfr_init2(res,MY_PRECISION);
		mpfr_const_pi (res,MPFR_RNDN);
		my_offset+=2;
		*offset=my_offset;
		return;
	}
	if (strncmp(&buffer[my_offset],"e",1)==0 && isalnum(buffer[my_offset+1])==0)
	{
		mpfr_init2(res,MY_PRECISION);
		mpfr_set_si(res,1,MPFR_RNDN);
		mpfr_exp (res, res, MPFR_RNDN);
		my_offset+=1;
		*offset=my_offset;
		return;
	}

	
	
	
	mpfr_init2(res,MY_PRECISION);
	char* endptr;
	mpfr_strtofr (res, buffer+my_offset, &endptr, 10, MPFR_RNDN);
    *offset=my_offset=(int)((ptrdiff_t)endptr-(ptrdiff_t)buffer);
}

void parse(const char* buffer)
{
	mpfr_t res;
	int offset=0;
	parse_level1(buffer, &offset,res);
    if (offset!=INVALID_OFFSET)
    {
        #ifdef USE_MPFR
        mpfr_printf ("res = %.128Rf\n",res);
        #else
        printf  ("res = " TYPE_MOD "\n",res);
        #endif
    }
    #ifdef USE_MPFR
    mpfr_free_cache();
    #endif
}
