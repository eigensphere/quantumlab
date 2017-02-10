/*
 *  qubit.cpp
 *
 *  Created by J. Fuentes on 09.07.11.
 *  Copyright 2011 IPN. All rights reserved.
 *
 */

//	========================================

#include <iostream>
#include <fstream>
#include <string>
#include <complex>
#include <cstdio>
#include <time.h>
#include <math.h>
#include <float.h>

#include "ciclos.h"

//	========================================

using namespace std;
using std::cout;
using std::cin;
using std::endl;
using std::ifstream;

//	========================================
//	CICLOS
//	========================================

#define DIM     128
#define VACIO   "\0"

//	========================================
//	DEFINICIONES
//	========================================

complex <double> C (0,1);

#define pi	3.1415926535897932384626433832795
#define	n	2 
#define	m	2

//	========================================
//	CARACTERIZACION DEL SISTEMA	
//	========================================

double	a;
double  f0;
double  f;
double	hb  = 1;                //6.626e-34;
double	w0;	              
double	w;              
double	ph	= pi;

//	TIEMPO/CICLOS

int     ciclos;
double	ti;              
double	te;		       
double	ta;	
double  tf;

//	========================================
//	COORDENADAS
//	========================================

double	x[N+1] = {0};
double	y[N+1] = {0};
double	z[N+1] = {0};

//	========================================
//	BASES DEL ESPACIO DE ESTADOS
//	========================================

complex <double> E1[n][m] = {{0,0},{1,0}};
complex <double> E2[n][m] = {{1,0},{0,0}};

//	========================================
//	MATRICES PAULI
//	========================================

complex <double> sx[n][m] = {{0,1},{1,0}};
complex <double> sy[n][m] = {{0,-C},{C,0}};
complex <double> sz[n][m] = {{1,0},{0,-1}};

//	========================================
//	MATRIZ IDENTIDAD
//	========================================

complex <double> I[n][m] = {{1,0},{0,1}};

//	========================================
//	CAPA DE CALCULO
//	========================================

complex <double> multiplica(complex <double> [][m],complex <double> [][m],complex <double> [][m]);
complex <double> escalar(complex <double> [][m],complex <double> [][m],complex <double> );
complex <double> transpone(complex <double> [][m],complex <double> [][m]);
complex <double> suma(complex <double> [][m],complex <double> [][m],complex <double> [][m]);
complex <double> resta(complex <double> [][m],complex <double> [][m],complex <double> [][m]);

//	========================================
//	CAPA DE SIMULACION
//	========================================

complex <double> mapeo (complex <double> [][m],int );
complex <double> simulador (complex <double> [][m],double ,double ,double ,double ,double ,int ,double ,double ,double ,int );
double  randomizar(double ,double );

//	========================================
//	CAPA DE LECTURA/ESCRITURA DE DATOS
//	========================================


int lDatos(void );
int eDatos(void );
complex <double> preparar (complex <double>, complex <double> , complex <double> [][m]);

//	========================================
//	MAIN()
//	========================================

int main (int argc, char * const argv[]) {
    
	//	----------------------------------------
	
    eDatos();
    
    //	----------------------------------------
    
	return EXIT_SUCCESS;
}

//	========================================
//	FUNCIONES
//	========================================

complex <double> preparar(complex <double> c1,complex <double> c2,complex <double> psi0[n][m]) {
	
	complex <double> psi01[n][m] = {0};
	complex <double> psi02[n][m] = {0};
	
	escalar(E1, psi01, c1);
	escalar(E2, psi02, c2);
	suma(psi01, psi02, psi0);
	
	mapeo(psi0,0);
	
	return EXIT_SUCCESS;
	
}

//	========================================

complex <double> mapeo(complex <double> psi0[n][m],int indice) {
	
	complex <double> psi0_t[n][m]	= {0};
	complex <double> aux[n][m]		= {0};
	complex <double> psi0x[n][m]	= {0};
	complex <double> psi0y[n][m]	= {0};
	complex <double> psi0z[n][m]	= {0};
	
	transpone(psi0, psi0_t);
	
	multiplica(psi0_t, sx, aux);
	multiplica(aux, psi0, psi0x);
	
	multiplica(psi0_t, sy, aux);
	multiplica(aux, psi0, psi0y);
	
	multiplica(psi0_t, sz, aux);
	multiplica(aux, psi0, psi0z);
	
	x[indice] = real(psi0x[0][0]);
	y[indice] = real(psi0y[0][0]);
	z[indice] = real(psi0z[0][0]);

	return EXIT_SUCCESS;
	
}

//	========================================

complex <double> simulador(complex <double> psi0[n][m],double hb,double w0,double w,double a,double ph,int ciclos,double ti,double te,double ta, int r) {
	
	int		i;
	int		j;
	int		p;
	int		q;
	
	double	A;
	double	t	= 0;
	double	tn	= 0;
	double	tm  = 0;
    
    tf = 0;
	
	double	w_r[2*N] = {0};
	
	complex <double>  U0[n][m] = {0};
	complex <double>  U1[n][m] = {0};
	complex <double>  U2[n][m] = {0};
	complex <double>  U3[n][m] = {0};
	complex <double>  UT[n][m] = {0};
	complex <double>   W[n][m] = {0};
	complex <double> psi[n][m] = {0};
	
	complex <double> aux1[n][m] = {0};
	complex <double> aux2[n][m] = {0};
	
	//srand(time(NULL));
	
	//	----------------------------------------
	//	BLOQUE DE RUIDO
	//	----------------------------------------
	
	if (r == 0) {
		for (i = 0; i < 2*ciclos; i++) {
			w_r[i] = w;
		}
	
	}else {
		for (i = 0; i < 2*ciclos; i++) {
			w_r[i] = w + randomizar(0,r);
		}
	}
	
	//	----------------------------------------
	//	CICLOS
	//	----------------------------------------
	
	j = 1;
	for (i = 0; i < 2*ciclos ; i++) {
		
		if ((i % 2) == 0) {
			A = a;
			t = te;
		} else {
			A = 0;
			t = ta;
		}
		
		tn = tn + t;    // t
		tm = tn - t;    // t_0
		        
        // -------------------------------
        // Termino Izq.: termino I 
        // -------------------------------
         
        escalar(I,aux1,cos(w/(2*hb)*(tn-tm)));
         
        // -------------------------------
        // Termino Izq.: termino II 
        // -------------------------------
         
        escalar(sz,aux2,C*sin(w/(2*hb)*(tn-tm)));
         
        // -------------------------------
        // Termino Izq. 
        // -------------------------------
         
        resta(aux1,aux2,U0);
        
        // -------------------------------
        // Termino Der.: termino I 
        // -------------------------------
         
        escalar(I,U1,cos(sqrt(pow(A,2.0) + pow((w0-w),2.0))*(tn-tm)/(2*hb)));
        
        // -------------------------------
        // Termino Der.: termino II 
        // -------------------------------
         
        if (w0-w == 0)  {
            U2[0][0] = 0;
            U2[1][1] = 0;
        }else{
            U2[0][0] =  (w0-w)/(float)sqrt(pow(A,2) + pow((w0-w),2));
            U2[1][1] = -(w0-w)/(float)sqrt(pow(A,2) + pow((w0-w),2));
        }
        if (A*cos(w*tn) == 0) {
            U2[0][1] = 0;
            U2[1][0] = 0;
        }else{
            U2[0][1] = A*cos(w*tn)/sqrt(pow(A,2.0) + pow((w0-w),2.0)) - C*A*sin(w*tn)/sqrt(pow(A,2.0) + pow((w0-w),2.0));
            U2[1][0] = A*cos(w*tn)/sqrt(pow(A,2.0) + pow((w0-w),2.0)) + C*A*sin(w*tn)/sqrt(pow(A,2.0) + pow((w0-w),2.0));
        }
   
        escalar(U2,U3,-C*sin(sqrt(pow(A,2.0) + pow((w0-w),2.0))*(tn-tm)/(2*hb)));
   
        // -------------------------------
        // Perturbacion Total
        // -------------------------------
         
        resta(U1,U3,W);
         
        // -------------------------------
        // Operador de Evolucion Total
        // -------------------------------
         
        multiplica(U0,W,UT);
        
        // -------------------------------
        // Evolucion del sistema
        // -------------------------------
		
		multiplica(UT,psi0,psi);
		
		for (p = 0; p < n; p++) {
			for (q = 0; q < m; q++) {
				psi0[p][q] = psi[p][q];
			}
		}
		
		if ( (i%2) == 0 ) {
			mapeo(psi,j);
			j++;
		}
	}
	
	tf = tn + ti;
    //std::cout <<"\n>> TIEMPO DE SIMULACION: "<< tf;
	
	return EXIT_SUCCESS;
}

//	========================================
//	OPERACIONES CON MATRICES
//	========================================

complex <double> multiplica(complex <double> a[n][m],complex <double> b[n][m],complex <double> c[n][m]) {
	
	int i;
	int j;
	int k;
	
	for (i = 0; i < n; i++) {
		for (j = 0; j < m; j++) {
			c[i][j] = 0;
			for (k = 0; k < m; k++) {
				c[i][j] += a[i][k]*b[k][j];
			}
		}
	}
	
	return EXIT_SUCCESS;	
}

//	========================================

complex <double> escalar(complex <double> a[][m], complex <double> b[][m], complex <double> e) {
	
	int i;
	int j;
	
	for (i = 0; i < n; i++) {
		for (j = 0; j < m; j++) {
			b[i][j] = e*a[i][j];
		}
	}
	return EXIT_SUCCESS;
}

//	========================================

complex <double> transpone(complex <double> a[n][m], complex <double> b[n][m]) {
	
	int i;
	int j;
	
	for (i = 0; i < n; i++) {
		for (j = 0; j < m; j++) {
			b[j][i] = conj(a[i][j]);
		}
	}
	return EXIT_SUCCESS;	
}

//	========================================

complex <double> suma(complex <double> a[n][m],complex <double> b[n][m],complex <double> c[n][m]) {
	
	int i;
	int j;
	
	for (i = 0; i < n; i++) {
		for (j = 0; j < m; j++) {
			c[i][j] = a[i][j] + b[i][j];
		}
	}
	return EXIT_SUCCESS;	
}

//	========================================

complex <double> resta(complex <double> a[n][m],complex <double> b[n][m],complex <double> c[n][m]) {
	
	int i;
	int j;
	
	for (i = 0; i < n; i++) {
		for (j = 0; j < m; j++) {
			c[i][j] = a[i][j] - b[i][j];
		}
	}
	return EXIT_SUCCESS;	
}

//	========================================
//	BLOQUE DE RUIDO
//	========================================

double randomizar(double mean, double var) {
	
	double	v;
	double	u1; 
	double	u2; 
	double	num;
	
	do {
		u1 = rand() / ((double) RAND_MAX); 
		u2 = rand() / ((double) RAND_MAX); 
		u1 = 2.0 * u1 - 1.0;               
		u2 = 2.0 * u2 - 1.0;               
		num = u1 * u1 + u2 * u2;
	} while(num >= 1);
	
	if(num == 0)
		v = 0;
	else
		v = sqrt(-2 * log(num) / num) * u1;
	
	return(mean + sqrt(var)*v);
}

//	========================================
//	IMPORTAR VARIABLES DE ENTRADA
//	========================================

int lDatos(void) {
    
    //  ...
    
    int     p;
    int     q;
    int     inicio;
    int     fin;
    int     iDatos;
    int     Px[7];
    char    s;
    
    string  buffer;
    
    //  ...
    
    char vCiclos[DIM]       = VACIO;
    char vTiempoe[DIM]      = VACIO;
    char vTiempoa[DIM]      = VACIO;
    char vTiempoi[DIM]      = VACIO;
    char vCampo[DIM]        = VACIO;
    char vFrecuenciac[DIM]  = VACIO;
    char vFrecuenciaq[DIM]  = VACIO;
        
    //	----------------------------------------
    
    ifstream indata;
    indata.open("variables.dat",ios::in);
    
    if(!indata) { 
        std::cout << "\nError: no se puede abrir archivo" << endl;
        return EXIT_FAILURE;
    }
    
    indata.seekg (0,ios::beg);
    inicio = indata.tellg();
    indata.seekg (0, ios::end);    
    fin = indata.tellg();
    iDatos = fin - inicio;
    
    //	----------------------------------------
    
    indata.seekg (0,ios::beg);
    
    while ( !indata.eof() ) {
        getline(indata,buffer);
        indata >> s;
    }
        
    //	----------------------------------------
    
    for (p = 0, q = 0 ; p < iDatos ; p++) {
        
        if ( buffer[p] == ' ') {
            Px[q] = p;
            q++;
        }
    }
    
    for (p = 0, q = 0 ; p < Px[0]; p++, q++) {
        vCiclos[q] = buffer[p];
    }
    
    for (p = Px[0]+1, q = 0; p < Px[1]; p++, q++) {
        vTiempoe[q] = buffer[p];
    }
    
    for (p = Px[1]+1, q = 0; p < Px[2]; p++, q++) {
        vTiempoa[q] = buffer[p];
    }
    
    for (p = Px[2]+1, q = 0; p < Px[3]; p++, q++) {
        vTiempoi[q] = buffer[p];
    }
    
    for (p = Px[3]+1, q = 0; p < Px[4]; p++, q++) {
        vCampo[q] = buffer[p];
    }
    
    for (p = Px[4]+1, q = 0; p < Px[5]; p++, q++) {
        vFrecuenciac[q] = buffer[p];
    }
    
    for (p = Px[5]+1, q = 0; p < iDatos; p++, q++) {
        vFrecuenciaq[q] = buffer[p];
    }
    
    indata.close();
    
    //	----------------------------------------
    
    ciclos  = atof(vCiclos);
    te      = atof(vTiempoe);
    ta      = atof(vTiempoa);
    ti      = atof(vTiempoi);
    a       = atof(vCampo);
    f       = atof(vFrecuenciac);
    f0      = atof(vFrecuenciaq);

    //	----------------------------------------
    
    return EXIT_SUCCESS;
}

int eDatos(void) {
    
    //	----------------------------------------
    
	FILE    *datos;
	datos = fopen("datos.dat","w");
	
	if (datos == NULL) {
        std::cout << "\n ERROR	[!]"<< endl;
        return EXIT_FAILURE;
	}
	
	//	----------------------------------------
	
	complex <double> psi0[n][m]	= {0};
    
    //	COEFICIENTES
	
	complex <double> a1 = 0;    //abs(cos(ph))*exp(-C*w*hb*ti/2.0);
	complex <double> a2 = 1;    //abs(sin(ph))*exp(-C*w*hb*ti/2.0);
    
    //	----------------------------------------
    
    preparar(a1,a2,psi0);
    
	//	######################################################################
	//	######################### VALORES ENTRADA ############################
	//	######################################################################
	
	//	NOTAS:														
	//	[1] CASO LIBRE:												
	//		w0 = w;
	//	[2] CASO CON PERTURBACION:
	//		w != w;
	//	----------------------------------------
    
	//	CARACTERIZACION DEL SISTEMA	
    
    // campo        -->   x 
    // tiempo       -->   y 
    // frecuencia   -->   z
    
    lDatos();
    
    //	VARIANZA
	//	0 sin ruido
	//	x con ruido
	
    //  ----------------------------------------
    
    w   = 2*pi*f;
    w0  = 2*pi*f0;

    //  ----------------------------------------
    
    simulador(psi0,hb,w0,w,a,ph,ciclos,ti,te,ta,0);
	    
	int i;
	for (i = 0; i <= ciclos; i++) {
		fprintf(datos,"%+.18e %+.18e %+.18e \n",x[i],y[i],z[i]);
	}
    fclose(datos);
    
    //	----------------------------------------
    
    FILE    *tiempo;
	tiempo = fopen("tiempo.dat","w");
	
	if (tiempo == NULL) {
        std::cout << "\n ERROR	[!]"<< endl;
        return EXIT_FAILURE;
	}
    
    fprintf(tiempo,"%f",tf);
    fclose(tiempo);
    
    return EXIT_SUCCESS;
}

//	========================================
//	E   O   F   [!]
//	========================================