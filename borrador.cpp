
// num_variables va a ser el numero de variables 
// num_restricciones es el numero de restricciones


//el progrma va a usar varias cosas importantes
/*

FO : la funcion objetivo que va a constar de un vector con los coeficientes de las variables xi
tableau : todo el tablero sin la ultima columna que es el valor de las basicas 
	tableau[i][num_variables + 1] es el valor de la variable basica en el i-esimo renglon
variables : va a ser un arreglo de cadenas con los nombres de las variables 
variables_bas: un cadena que tenga los nombres de las variables basicas
X0 : esta es la artificial para las artificiales
mix_tableu : para saber si es menor o mayor que la restriccion
mix_fo: para saber si la funcion objetivo es mayor o menor que 
factible : me va a decir si la funcion es factible hasta ese momento 


ademas de las variables mix_fo  que determinara si la FO es el minimo o el maximo 



*/


/*
las varibles del ld van a estar en tableau[i][max_tam-1]
*/


#include<bits/stdc++.h> //  esto contiene todas las librerías que existen en c++
#include <windows.h>
using namespace std;
const int max_tam = 51; ///////////////el tamaño del tablero maximo que vamos a usar para no andar creando vectores
const long double maximum_ld = 10000000;
#define borrar_pantalla system("CLS")
#define pausar system("pause");
#define ld double

// las variables van a ser de nivel global 
int num_variables = 0; //////////el numero de variables xi
int num_restricciones = 0; ///// pos el numero de restricciones
int num_variables_total = 0; //// la cantidad de varibles totales xi, si, ai
bool factible = true;

ld FO[max_tam];
ld tableau[max_tam][max_tam];
string variables[max_tam];
string variables_bas[max_tam];
ld X0[max_tam];
//char restricciones[max_tam];

char mix_tableau[max_tam]; // sirve para saber si cada restriccion es menor o mayor que 
char mix_fo = 'n';


string num_to_str(int num); // para convertir numero a cadena
bool AjustarVentana(int Ancho, int Alto); // para darle tamaño a la consola


////////////  las que ya tienen codigo tienen una serie de guiones
void iniciar_tableau();   //1.1--------------
void iniciar_variables(); //1.2 ----------------
void ingresar_fo();       //2.1-------------
void ingresar_tableau();  //2.2---------------
void cambiar_valor();     //2.3
void numero_variables();  //2.4------------------
void imprimir_fo();       //3.1------------------
void imprimir_restricciones();  //3.2------------------
void imprimir_tableau();  //3.3
void imprimir_tableau_segunda_fase(); //3.4
void imprimir_final();    //3.5
void crear_nombres_var(); //4.1------------------
void tablero_valido();    //4.2
void validar_tablero();   //4.3


void SIMPLEX_max();       //4.4
void SIMPLEX_min();       //4.5

void SIMPLEX_primera_parte(); //5.1
void SIMPLEX_segunda_parte(); // 5.2

bool ld_valido();
bool col_valida(int columna);
int main(){
	AjustarVentana(160,50);     // Para pantalla de resolución 1360*760
	// AjustarVentana(150,50);  // Para otras de menor resolución
	//cin.get();
	//cin.get();
	
	iniciar_variables();
	iniciar_tableau();

	numero_variables();
	ingresar_fo();
	ingresar_tableau();

	imprimir_restricciones();

	
	crear_nombres_var();
	system("pause");
	imprimir_tableau();

	SIMPLEX_primera_parte();
	if(factible)
		SIMPLEX_segunda_parte();
	
	return 0;
}

/*
las funciones las dclaro dese arriba pero aqui ya estan listadas y definidas

/////  1 iniciar 
1.1 - iniciar tableau (inicializa los valores del tableau para que no haya pedos)
1.2 - iniciar variables  (inicia vacias las cadenas para que no haya pedos)



/////// 2  ingresar
2.1 - ingresar funcion objetivo
2.2 - ingresar tablero 
2.3 - cambiar valor en la matriz
2.4 - numero de variables


/////// 3 imprimir
3.1 - imprimir_fo
3.2 imprimir restricciones
3.3 imprimir tableau ------------------------- falta una funcion que imprima todo el tableau (faltan las variables de exeso y olgura)
3.4 - imprimir tableau segunda fase
3.5 - imprimir final -------------------------


////// 4 procedimientos
4.1 - crear nombres de variables
4.2 - tablero_valido
4.3 - validar_tablero
4.3 - SIMPLEX_max        
4.4 - SIMPLEX_min   


/// 5 simplex con partes
5.1 simples min primera parte
5.2 simplex min segunda parte

////6
6.1 - verificar ld 
6.2 - verificar col, para verificar que hay positivos en la col pivote
6.3 - la infactibilidad cuando optimizas x0 y ai es basica ya está dentro de simplex primera fase


*/
//1.1
void iniciar_tableau(){
	for(int i = 0 ; i < max_tam; ++i){
		FO[i] = 0; //los indices de la funcion objetivo son todos 0
		X0[i] = 0; //los indices de la funcion objetivo artificial 
		for(int j = 0; j < max_tam; ++j)
			tableau[i][j] = 0; ////////// todos los elementos del tableau
	}
}

//1.2
void iniciar_variables(){
	factible = true;
	for(int i = 0; i < max_tam; ++i){
		variables[i].clear(); // los nombres de las variables los vacio 
		variables_bas[i].clear(); //los nombres de las variables basicas tambien lo vacio todo
		mix_tableau[i] = 'M';    /// inicializo las restricciones
		num_restricciones = 0;  ////inicializo el numero de restricciones 
		num_variables = 0;      //// inicializo el numero de variables
		num_variables_total = 0; /// incializo el numero de variables, variables de olgura y de exeso
	}
}


//2.1 
void ingresar_fo(){
	mix_fo = 'm';// mix fo dice si la fo se maximiza o si se minimiza
	do{
		system("CLS");
		cout<<"presione M para maximisar la funcion objetivo y m para minimizar\n";
		cin>>mix_fo;
		cout<<"\n";
	}while(mix_fo != 'm' && mix_fo != 'M');

	for(int i = 1; i <= num_variables; ++i){ //en este for se ingresa cada coeficiente de la FO
		system("CLS");
		cout<<"FO = ";
		for(int j = 1; j < i; ++j){ // en este for se imprimen todos los coeficientes que ya llevas guardados
			cout<<FO[j]<<" x"<<j;
			if(j < i - 1)
				cout<<" + ";
		}
		cout<<"\n\n";
		cout<<"ingrese coeficiente de x"<<i<<"\n";
		cin>>FO[i];
		cout<<"\n";
	}
}
//2.2
void ingresar_tableau(){
	for(int i = 1 ; i <= num_restricciones; ++i){ // el numero de restricciones es el numero de filas 
		
		char c = 'm'; //para escoger mayor que o menor que
		for(int j = 1; j <= num_variables; ++j){ // lee los elementos del tableau
			system("CLS");
			cout<<"restriccion "<<i<<"  ";
			for(int k = 1; k < j; ++k){ //en este for imprimes todos lo elementos que has guardado hasta el momento
				cout<<tableau[i][k]<<" x"<<k;
				if(k < j - 1)
					cout<<" + ";
			}
			cout<<"\n";
			cout<<"coeficiente de x"<<j<<"\n";
			cin>>tableau[i][j];
			cout<<"\n";
		}
		system("CLS");
		cout<<"restriccion "<<i<<"  ";
		for(int k = 1; k <= num_variables; ++k){ // cuando ya leiste toda la restriccion la imprimes
			cout<<tableau[i][k]<<" x"<<k;
			if(k < num_variables)
				cout<<" + ";
		}
		cout<<"\n";

		c = 'm';
		do{		
			if(c != 'm' && c != 'M'){
				system("CLS");
				cout<<"opcion invalida vuelve elegir\n";
			}
			cout<<"presinoe m para menor que y M para mayor que\n";
			cin>>c; //elegir si es mayor que o menor que
			mix_tableau[i] = c; // mix_tableau dice si la funcion es mayor o menor que
			cout<<"\n"; 
		}while(c != 'M' && c != 'm');

		if(c == 'M'){ ////////////////lee los valores con los que se limitan o los de "lado derecho"
			cout<<" mayor que ";
			cin>>tableau[i][max_tam - 1]; //los valores del lado derecho se guardan en la columna max_tam - 1  (la ultima)
			cout<<"\n";
			continue;
		}
		if(c == 'm'){
			cout<<" menor que ";
			cin>>tableau[i][max_tam - 1];//los valores del lado derecho se guardan en la columna max_tam - 1  (la ultima)
			cout<<"\n";
			continue;
		}
	}

}

//2.3
void cambiar_valor(){ // esta funcion no la he probado
	char c = 'x';
	while(true){
		int a = 0, b = 0;
		ld d = 0;
		imprimir_restricciones();
		cout<<"desea cambiar el valor de una variable? \n";
		cout<<"presione cualquier tecla para cambiar valor, presione la x en caso contrario\n";
		cin>>c;
		if(c == 'x')
			break;
		cout<<"numero de restriccion: "; 
		cin>>a;
		cout<<"\nnumero de variable: ";
		cin>>b;
		cout<<"valor: ";
		cin>>d;

		tableau[a][b] = d;
	}
}

//2.4 
void numero_variables(){ // solo lee el numero de variables y de restricciones
	cout<<"cuantas variables va a usar? \n";
	cin>>num_variables;
	cout<<"\ncuantas restricciones va a haber? \n";
	cin>>num_restricciones;
	cout<<"\n\n";
}


//3.1
void imprimir_fo(){ // imprime la funcion objetivo
	system("CLS");
	if(mix_fo == 'm')
		cout<<"min  ";
	if(mix_fo == 'M')
		cout<<"max  ";
	for(int i = 1; i <= num_variables; ++i){
		cout<<FO[i]<<" x"<<i;
		if(i < num_variables)
			cout<<" + ";
	}
	cout<<"\n";

}
//3.2
void imprimir_restricciones(){ //imprime todas las restricciones
	imprimir_fo();
	for(int i = 1; i <= num_restricciones; ++i){
		
		for(int j = 1; j <= num_variables; ++j){
			cout<< tableau[i][j]<<" x"<<j<<"\t";
		}
		if(mix_tableau[i] == 'm')
			cout<<" <= ";
		if(mix_tableau[i] == 'M')
			cout<<" >= ";
		cout<<tableau[i][max_tam - 1];
		cout<<"\n";
	}
}

//3.3
void imprimir_tableau(){ // imprimir el tableau ya con todas las variables
	//borrar_pantalla;
	imprimir_fo();
	cout<<"\n\n";
	cout<<"\t";
	for(int i = 1; i <= num_variables_total; ++i) // imprimo el nombre de las variables
		cout<<variables[i]<<"\t";
	cout<<"LD";//ipmrime lado derecho
	cout<<"\n";
	cout<<"X0\t";
	for(int i = 1; i <= num_variables_total; ++i)//imprimimos x0
		//cout<<X0[i]<<"\t";
		printf("%.3f\t",X0[i] );
	printf("%.3f\t",X0[max_tam - 1]); //el lado derecho
	cout<<"\n";
	cout<<"Z\t";
	for(int i = 1; i <= num_variables_total; ++i)//imprimimos la funcion objetivo
		//cout<<FO[i]<<"\t";
		printf("%.3f\t",FO[i] );
	printf("%.3f\t",FO[max_tam - 1] ); //el lado derecho

	cout<<"\n";
	for(int fila = 1; fila <= num_restricciones; ++fila){
		cout<<variables_bas[fila]<<"\t";  // imprimo el nombre de la varible basica en esa fila
		for(int columna = 1; columna <= num_variables_total; ++columna){
			//cout<<tableau[fila][columna]<<"\t";
			if(tableau[fila][columna] == 0){
				cout<<0.0<<"\t";
				continue;
			}
			printf("%.3f\t",tableau[fila][columna] );
			
		}
		printf("%.3f\t",tableau[fila][max_tam - 1] ); //el lado derecho
		cout<<"\n";
	}
}

//3.4
void imprimir_tableau_segunda_fase(){
	imprimir_fo();
	cout<<"\n\n";
	cout<<"\t";
	for(int i = 1; i <= num_variables + num_restricciones; ++i) // imprimo el nombre de las variables
		cout<<variables[i]<<"\t";
	cout<<"LD";//ipmrime lado derecho
	cout<<"\n";
	cout<<"Z\t";
	for(int i = 1; i <= num_variables + num_restricciones; ++i)//imprimimos la funcion objetivo
		//cout<<FO[i]<<"\t";
		printf("%.3f\t",FO[i] );
	printf("%.3f\t",FO[max_tam - 1] ); //el lado derecho

	cout<<"\n";
	for(int fila = 1; fila <= num_restricciones; ++fila){
		cout<<variables_bas[fila]<<"\t";  // imprimo el nombre de la varible basica en esa fila
		for(int columna = 1; columna <= num_variables + num_restricciones; ++columna){
			//cout<<tableau[fila][columna]<<"\t";
			if(tableau[fila][columna] == 0){
				cout<<"0.0\t";
				continue;
			}
			printf("%.3f\t",tableau[fila][columna] );
		}
		printf("%.3f\t",tableau[fila][max_tam - 1] ); //el lado derecho
		cout<<"\n";
	}

}

//3.5
void imprimir_final(){
	borrar_pantalla;
	cout<<"\nya terminaron las iteraciones\n\n";
	pausar;
	if(mix_fo == 'm'){
		imprimir_tableau_segunda_fase();
		cout<<"\n\n";
		printf("la funcion se minimiza con valor %.3f\n\n",FO[max_tam - 1]);
		for(int i = 1; i <= num_restricciones; ++i){
			cout<<variables_bas[i]; //imprimo la variable basica
			printf(" = %.3f\n", tableau[i][max_tam - 1]); ///imprimo el valor de la variable
		}
	}
	if(mix_fo == 'M'){
		imprimir_tableau_segunda_fase();
		cout<<"\n\n";
		printf("la funcion se maximiza con valor %.3f\n\n",(-1) * FO[max_tam - 1]);
		for(int i = 1; i <= num_restricciones; ++i){
			cout<<variables_bas[i]; //imprimo la variable basica
			printf(" = %.3f\n", tableau[i][max_tam - 1]); ///imprimo el valor de la variable
		}
	}
	cout<<"\n";

	return ;

}



//4.1
void crear_nombres_var(){
	for(int i = 1; i <= num_variables; ++i){ // creamos las variables que llevan x
		variables[i] = 'x' +  num_to_str(i);
	}
	num_variables_total  = num_variables + num_restricciones; // las variables x mas las de olgura

	
	int contador_a = 1;  //cuenta la cantidad de variables a (de exeso)

	for(int i = 1; i <= num_restricciones; ++i){
		
		variables[num_variables + i] = 's' + num_to_str(i); //la fila que tendra las variables
		
		if(mix_tableau[i] == 'm'){
			tableau[i][num_variables + i] = 1; //asigna el 1 en la columna de la var de olgura
			variables_bas[i] = 's' + num_to_str(i);//en la columna de variables basicas
		}
			
		if(mix_tableau[i] == 'M'){
			tableau[i][num_variables + i] = -1;
			tableau[i][num_variables + num_restricciones + contador_a] = 1;

			variables[num_variables + num_restricciones + contador_a] = 'a' + num_to_str(contador_a); //la fila que tendra las variables
			variables_bas[i] = 'a' + num_to_str(contador_a); //en la columna de variables basicas
			contador_a++;
			num_variables_total++;
		}
	}
//// comenta esto
	cout<<endl;
	for(int i = 1; i <= num_restricciones; ++i){
		cout<<"bas "<<i<<" - "<<variables_bas[i]<<endl;
	}
	cout<<endl;
	for(int i = 1; i <= num_variables_total; ++i){
		cout<<variables[i]<<"\t";
	}
	cout<<endl;

}

//4.2
bool tablero_valido(char mix_fo){ //est funcion me va a decir si el tablero es valido
	return true;
}


//4.3
void validar_tablero(){ //esta funcion ya ni se que va a hacer

}


//4.4 SIMPLEX_max
void SIMPLEX_max(){  // esta funcion va a hacer el simplex y 

}

void SIMPLEX_min(){

}


//5.1
void SIMPLEX_primera_parte(){
	//si no hay variables de exceso pos termino
	if(num_variables + num_restricciones == num_variables_total)
		return ;

	//convierto en negativo todo el renglon de la funcion objetivo si se minimiza la funcion objetivo 
	if(mix_fo == 'm')
		for(int i = 1; i < num_variables; ++i)
			if(FO[i] != 0)
				FO[i] = FO[i] * (-1);

	//le doy valor -1 a todo el vector x0
	int contador_a = num_variables_total - num_variables - num_restricciones; //la cantidad de variables a	
	for(int i = 1; i <= contador_a; ++i)
		X0[num_variables + num_restricciones + i] = -1; //le asignamos el -1 en las variables artificiales al x0
	
	imprimir_tableau();
	cout<<"\n\n aqui apenas asignamos a x0";
	system("pause");
	
	//tengo que sumar al renglon X0 todos los renglones de a0

	for(int fila = 1; fila <= num_restricciones; ++fila){
		if(variables_bas[fila][0] == 'a'){ ////////////////// si este renglon tiene una variable de exeso 
			for(int columna = 1; columna <= num_variables_total; ++columna) //sumo los elementos de la fila 
				X0[columna] += tableau[fila][columna];                  
				//le sumo a x0
			X0[max_tam - 1] += tableau[fila][max_tam - 1];
		}
	}

	imprimir_tableau();
	cout<<"\n\n aqui ya hicimos 0 las variables de a en X0";
	system("pause");

	//tengo que hacer un while

	while(true){
		//cout<<"\n\n\n\ntableeeeeeeeeeeeeeeeeeeeeau\n\n";
		system("pause");
		
		//imprimir_tableau();
		cout<<endl<<endl;
		bool validacion = ld_valido();
		if(validacion == false){
			factible = false;
			borrar_pantalla;
			imprimir_tableau();
			cout<<"la solucion es infactible pues el lado derecho tiene elementos negativos\n\n\n\n";
			pausar;
			return ;
		}

		//revisar si la funcion objetivo artificial ya es optima (X0)
		bool optima = true; //variable que identifica optimalidad
		for(int i =  1; i <= num_variables_total; ++i)
			if(X0[i] > 0){ //si hay un positivo, no es optima
				optima = false;
				break;
			}
		if(optima){// si ya es opitma tengo que ver que el ultimo valor sea 0
			if(X0[max_tam - 1] > 0){
				factible = false;
				borrar_pantalla;
				imprimir_tableau();
				cout<<"\n\n\n\nla funcion artificial es minima con valor mayor a 0\n";
				cout<<"por lo tanto la funcion es infactible\n\n\n\n";
				pausar;
			 	return ; // si la funcion artificial no se va a 0 quiere decir que no es factible y termino
			}
			//tengo que ademas verificar que no hay variables de exeso 
			for(int i = 1; i <= num_restricciones;++i)
				if(variables_bas[i][0] == 'a'){
					factible = false;
					borrar_pantalla;
					imprimir_tableau();
					cout<<"'\n\n\n\nla funcion artificial ya es minima pero aun hay variables artificiales con valor\n";
					cout<<"por lo tanto la funcion es infactible\n\n\n\n";
					pausar;

					return ;
				}
			return ; //si ya es optima ya acabe (ya no hay valores positivos en x0)
		}


		//escogo la columna pivote
		int col_pivote = 1, fila_pivote = 1;
		ld maxi = 0;
		for(int i = 1; i <= num_variables_total; ++i)
			if(X0[i] > maxi){
				maxi = X0[i];
				col_pivote = i;
			}
		//verifico que haya valores positivos en la columna pivote
		validacion = col_valida(col_pivote);
		if(validacion == false){
			factible = false;
			borrar_pantalla;
			imprimir_tableau();
			cout<<"\n\n\n\nla solucion no es acotada pues en la columna "<<col_pivote<<" no hay elementos positivos\n\n\n\n";
			pausar;
			return ;
		}

		//escogo la fila pivote
		ld mini = maximum_ld;  // este elemento puede variar mucho 
		for(int i = 1; i <= num_restricciones; ++i){
			ld aux = tableau[i][max_tam - 1] / tableau[i][col_pivote];
			if(aux > 0 and aux < mini){
				mini = aux;
				fila_pivote = i;
			}
		}


		// desmadro todo con la fila pivote 

		ld factor = tableau[fila_pivote][col_pivote]; // es el elemento entre el que vamos a dividir la fila pivote
		for(int i = 1; i < max_tam ; ++i)
			tableau[fila_pivote][i] /= factor;

		//hacemos un paso 
		for(int fila = 1; fila <= num_restricciones; ++fila){
			if(fila == fila_pivote)// si es la fila pivote me la salto
				continue;
			factor = tableau[fila][col_pivote] / tableau[fila_pivote][col_pivote];
			for(int columna = 1; columna <= max_tam-1; ++columna)// resto cada columna 
				tableau[fila][columna] = tableau[fila][columna] - factor * tableau[fila_pivote][columna];
			
		}

		//hago la eliminacion de la funcion objetivo 
		factor = FO[col_pivote] / tableau[fila_pivote][col_pivote];
		for(int columna = 1; columna <= max_tam-1; ++columna)
			FO[columna] = FO[columna] - factor * tableau[fila_pivote][columna];
		//hago la eliminacion en la fila de X0
		factor = X0[col_pivote] / tableau[fila_pivote][col_pivote];
		for(int columna = 1; columna <= max_tam-1; ++columna)
			X0[columna] = X0[columna] - factor * tableau[fila_pivote][columna];
		
		//cambiar los nombres de las variables basicas
		variables_bas[fila_pivote] = variables[col_pivote];	
		

		imprimir_tableau();
		cout<<"\n\n aqui acabamos una iteracion \n";
		system("pause");

	}//acaba el while



}

//5.2
void SIMPLEX_segunda_parte(){
	imprimir_tableau_segunda_fase();
	pausar;

	while(true){
		cout<<"a punto de hacer un paso \n\n";
		pausar;
		//verificar infactibilidad o no acotacion 

		bool validacion = ld_valido();
		if(validacion == false){
			factible = false;
			borrar_pantalla;
			imprimir_tableau_segunda_fase();
			cout<<"\n\n\n\nla solucion es infactible pues el lado derecho tiene elementos negativos\n\n\n\n";
			pausar;
			return ;
		}

		//buscar la columna pivote
		int col_pivote = 1, fila_pivote = 1;
		ld maxi = 0;
		for(int i = 1; i <= num_variables + num_restricciones; ++i)
			if(FO[i] > maxi){
				maxi = FO[i];
				col_pivote = i;
			}
		
		if(maxi == 0){
			imprimir_final();
			return ;
		}

		//verificar que haya valor positivo en la columna
		validacion = col_valida(col_pivote);
		if(validacion == false){
			borrar_pantalla;
			factible = false;
			imprimir_tableau_segunda_fase();
			cout<<"\n\n\n\nla solucion no es acotada pues no hay positivos en la fila "<<col_pivote<<"\n\n\n\n";
			pausar;
			return ;		
		}


		// buscar fila pivote
		ld mini = maximum_ld;  // este elemento puede variar mucho 
		for(int i = 1; i <= num_restricciones; ++i){
			ld aux = tableau[i][max_tam - 1] / tableau[i][col_pivote];
			if(aux > 0 and aux < mini){
				mini = aux;
				fila_pivote = i;
			}
		}

		// desmadro todo con la fila pivote 

		ld factor = tableau[fila_pivote][col_pivote]; // es el elemento entre el que vamos a dividir la fila pivote
		for(int i = 1; i < max_tam ; ++i)
			tableau[fila_pivote][i] /= factor;

		//hacemos un paso 
		for(int fila = 1; fila <= num_restricciones; ++fila){
			if(fila == fila_pivote)// si es la fila pivote me la salto
				continue;
			factor = tableau[fila][col_pivote] / tableau[fila_pivote][col_pivote];
			for(int columna = 1; columna <= max_tam-1; ++columna)// resto cada columna 
				tableau[fila][columna] = tableau[fila][columna] - factor * tableau[fila_pivote][columna];
			
		}

		//hago la eliminacion de la funcion objetivo 
		factor = FO[col_pivote] / tableau[fila_pivote][col_pivote];
		for(int columna = 1; columna <= max_tam-1; ++columna)
			FO[columna] = FO[columna] - factor * tableau[fila_pivote][columna];
		//hago la eliminacion en la fila de X0
		factor = X0[col_pivote] / tableau[fila_pivote][col_pivote];
		for(int columna = 1; columna <= max_tam-1; ++columna)
			X0[columna] = X0[columna] - factor * tableau[fila_pivote][columna];
		
		//cambiar los nombres de las variables basicas
		variables_bas[fila_pivote] = variables[col_pivote];	
		
		imprimir_tableau_segunda_fase();
		cout<<"\n\naqui acabamos una iteracion\n\n";
		pausar;



	}//acaba while

}



//6.1 
bool ld_valido(){
	for(int i = 1; i <= num_restricciones; ++i)
		if(tableau[i][max_tam-1] < 0) //si hay uno solo negativo ya no es valida
			return false;
	return true;
}

//6.2 
bool col_valida(int columna){
	for(int i = 1; i <= num_restricciones; ++i)
		if(tableau[i][columna] > 0) //si hay uno solo positivo ya es valida
			return true;
	return false;
}



//////////////////////////-------------------------------------------------------------------



string num_to_str(int num){ /// esta funcion resive un entero y lo devuelve pero en cadena 
	string answer;
	stack<char> answer_1;
	int aux = num;
	while(aux > 0){
		int a = aux % 10;
		answer_1.push(a + '0');
		aux /= 10;
	}
	while(!answer_1.empty()){
		answer.push_back(answer_1.top());
		answer_1.pop();
	}
	return answer;
}


bool AjustarVentana(int Ancho, int Alto) {
	_COORD Coordenada;
	Coordenada.X = Ancho;
	Coordenada.Y = Alto;

	_SMALL_RECT Rect;
	Rect.Top = 0;
	Rect.Left = 0;
	Rect.Right = Ancho - 1;
	Rect.Bottom = Alto - 1;

	// Obtener el handle de la consola
	HANDLE hConsola = GetStdHandle(STD_OUTPUT_HANDLE);

	// Ajustar el buffer al nuevo tamaño
	SetConsoleScreenBufferSize(hConsola, Coordenada);

	// Cambiar tamaño de consola a lo especificado en el buffer
	SetConsoleWindowInfo(hConsola, TRUE, &Rect);
	return TRUE;
}




///////////////////////////////////////////////////////////////////////////////////////////////////////////////




