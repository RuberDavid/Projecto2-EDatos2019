#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

#define PERIOD 5
#define CARO .8
#define BARATO .4

typedef struct float_arr{
	float* arr;
	size_t size;
	size_t next;
} float_arr;

typedef enum criteria{ NOTHING, BUY, SELL }criteria;


bool vende= false;
const int capital_inicial= 10000;
double total= capital_inicial;

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
float_arr* new_arr(void);
void Free( float_arr* my_arr );
void push_back( float_arr* my_arr, const float val);
void read_float_arr(float_arr* my_arr);

int close_file(FILE *fptr);
FILE *open_file(const char *filename, const char *mode);

float promedio( float_arr* my_arr, int first, int last);

float maximum(float_arr* my_arr,  int first, int last);
float minimum(float_arr* my_arr,  int first, int last);
float stocastic( float max, float min,  float last);
criteria analize_data( float prom, float stoc, float max, float min );
void buy(float val);
void sell(float val);
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
int main()
{

	FILE* entrada;
	float val;
	float last_total;
	float_arr* prices= new_arr();
	criteria indicador=NOTHING; 
	int days_passed=0;
	float prom,
	      stoc,
	      max,
	      min;

	
	puts("\n\t ingrese el nombre del archivo con su extensión:");
	char nombre[31];
	scanf("%30s", nombre);
	entrada= open_file( nombre, "r");
	
	printf("%-10s%-10s%-10s%-10s%-10s%-10s\n\n", "DÍA", "VALOR", "RESPUESTA", "GANANCIA", "PERDIDA", "ACUMULADO");//DEBUG

	//analiza un periodo inicial
	days_passed=0;
	for(; days_passed< PERIOD; days_passed++)
	{
		fscanf(entrada, "%f", &val);
		push_back( prices, val);
	}
	
	//mientras haya acciones que comprar	
	while(  !feof(entrada) )
	{
		{
			prom= promedio(prices, days_passed-PERIOD, days_passed);
			max= maximum( prices, days_passed-PERIOD, days_passed);
			min= minimum( prices, days_passed-PERIOD, days_passed);

			stoc= stocastic(max, min, prices->arr[days_passed-1] );
			indicador= analize_data( prom, stoc, max, min);//TODO
		}
		last_total=total;
		//si se ha comprado
		if( vende )
		{
			//si el indicador da señales de venta 
			if( indicador == SELL  )
			{
				sell(val);
				puts("vendió");
			}

		//si no se ha vendido
		}else
		{
			//si el indicador da señales de compra y hay dinero suficiente
			if( indicador == BUY && val <= total )
			{
				buy(val);
				puts("compró");
			}
		}
		fscanf(entrada, "%f", &val);
		push_back( prices , val);
		days_passed++;
		printf("%-10d%-10.2f%-10s%-10.2f%-10.2f%-10.2f\n", days_passed, val, indicador==0 ? "nada" : indicador==1 ? "compra" : "venta", last_total-total, last_total-total, total); //TODO
	}
	// Informa sobre la Ganancia
	printf("\tGANANCIA :%f", total-capital_inicial);//TODO

	Free( prices );
	close_file(entrada);
}			
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//Funciónes sonbre  float_arr*
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

void read_float_arr(float_arr* my_arr)
{
	float val;

	for( int i=0; i<PERIOD ; i++)
	{
		scanf("%f", &val);
		push_back( my_arr ,val);
	}
}

void push_back( float_arr* my_arr, const float val)
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

void Free( float_arr* my_arr )//TODO
{
	free( my_arr->arr);
	free( my_arr);
}
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//Funciónes sobre FILE*
FILE *open_file(const char *filename, const char *mode)
/*		*/ 
{
	FILE *fptr;
	errno=0;

	if( (fptr=fopen( filename, mode))== NULL )
		fprintf(stderr, "el archivo %s no pudo abrirse \n\terror: %s\n",filename, strerror(errno) ); 
	return fptr;
}

int close_file(FILE *fptr)
/*		*/
{
	int s=0;
	if( fptr == NULL )//ignora este caso
		return 0;
	s=fclose(fptr);
	if( s== EOF )
		perror("error al cerrar");
	return s;
}
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


float promedio( float_arr* my_arr, int first, int last)
//promedio de intervalo  [first, last)
{
	float sum=0;

	for(int i=first; i<last ; i++)
		sum+= my_arr->arr[i];
	return sum/(last-first);

}

float maximum(float_arr* my_arr,  int first, int last)
//encuetra el máximo en el intervalo [first, last)
{
	float max= my_arr->arr[first];
	for(int i=first; i< last; i++)
		if( my_arr-> arr[i] > max )
			max= my_arr-> arr[i];
	return max;
}

float minimum(float_arr* my_arr,  int first, int last)
//encuentra el mínimo en el intervalo [first, last)
{
	float min= my_arr->arr[first];
	for(int i=first; i< last; i++)
		if( my_arr-> arr[i] < min )
			min= my_arr-> arr[i];
	return min;
}


float stocastic( float max, float min,  float last)
{
	if(max == min)
		return 0;
	return (last-min )/ (max-min);

}

criteria analize_data( float prom, float stoc, float max, float min )
{
	printf("prom: %10.2f\tprom/(max-min): %10.2f\tstoc: %10.2f\n", prom, prom/(max-min), stoc);//DEBUG
	//si la tendencia local sobrepasa al promedio y el criterio de "caro"
	if( stoc > prom/(max-min) && stoc > CARO )
		//emite señales de venta
		return SELL;
//si la tendencia local es menor que el promedio y el criterio de "barato"
	if( stoc < prom/(max-min) && stoc < BARATO )
		//emite señales de compra
		return BUY;

	return NOTHING;
}

void buy(float val)
{
	total-=val;
	vende= false;
}

void sell(float val)
{
	total+=val;
	vende= true;
}


//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


