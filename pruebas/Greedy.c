// Codicioso:	Compra cuando esté más barato
// 		Vende  cuando esté más caro
#include <stdio.h>
#include <stdbool.h>

const int capital_inicial= 10000;
bool compre= false;
double total= capital_inicial;
double precio_de_compra=0;//TODO
double precio_actual;

void compra(void)
{
	precio_de_compra= precio_actual;
	total-= precio_actual;
	compre= false;
}

void vende(void)
{
	total+= precio_actual;
	compre= true;
}

int main()
{
	
	// Comprar
	// Miéntras haya algo que comprar
	while( scanf("%lf", &precio_actual) == 1 )
	{
		//Si compré
		if( compre )
		{
			// ¿está mas caro que cuando lo compré ?
			if( precio_actual > precio_de_compra )
			{
				//si: vende
				vende();
			}
		//Si vendí
		}else
		{
			// ¿está más barato que mi total?
			if( precio_actual <= total )
				//si: compra
				compra();
		}
		printf("%lf\n", total);

	}
	// Informa sobre la Ganancia
	printf("\t%lf", total-capital_inicial);
}

