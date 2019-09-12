#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

typedef struct float_arr{
	float* arr;
	size_t size;
	size_t next;
} float_arr;

float_arr* new_arr(void)
{
	float_arr *ptr= malloc(sizeof(float_arr));
	if( ptr== NULL)
	{
		fprintf(stderr,"couldn't allocate memory");
		return NULL;
	}
	ptr->arr= malloc( sizeof(float) );
	if( ptr->arr == NULL )
		fprintf(stderr,"couldn't allocate memory");
	ptr->size=1;
	ptr->next=0;
	return ptr;
}

void push_back( float_arr* my_arr,const float val)
{
	if( my_arr->next >= my_arr->size )
	{
		float* ptr= realloc( (my_arr->arr), (my_arr->size)*2*sizeof(float) );
		if( ptr == NULL)
		{
			fprintf(stderr,"couldn't allocate memory");
			return;
		}
		my_arr->arr= ptr;
		(my_arr->size)*= 2;
	}	
	my_arr-> arr[ my_arr->next++ ] = val;
}




int main()
{
	float_arr* aver=new_arr();
	float val;
	while( scanf("%f", &val)  && !feof(stdin) )
		push_back( aver, val);

	for(int j=0; j< aver->next-1 ; j++)
		printf("%f\n", aver->arr[j] );

}

void insert(float_arr *my_arr, const size_t i, float val)
{
	if( i >=  my_arr->size )
	{
		realloc( my_arr->arr, my_arr->size*2*sizeof(float) );
		if( my_arr->arr == NULL)
			fprintf(stderr,"couldn't allocate memory");
		my_arr->size*=2;
	}

	my_arr->arr[i]=val;
}




