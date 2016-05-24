double TemperaturaInicial()
{
	/* Preparo los datos de la funcion*/

	// Declaro una solucion inicial
	Solucion s0(GetInstance().getNodos());
	Solucion s1 = s0;

	// Hago que sea valida (No se salga de la capacidad)
	getValidSolution(s1, GetInstance().getMateriales());
	SetSol(s1);

	// Hago una solucion vecina a la actual
	Solucion s2 = Neighoperator();

	/* Caluclar Incremento E */
	double IE = 0;
	// Tengo que coger 10 soluciones aletarorias y 10 vecinas a esas
	for (int i = 0; i < 10; i++)
	{
		// Calculo IE sucesivamente
		IE += (s1.GetCoste() - s2.GetCoste());
		
		// Reinicio solucion
		s1 = s0;

		// Vuelvo a calcular soluciones aleatorias.
		getValidSolution(s1, GetInstance().getMateriales());
		SetSol(s1);
		s2 = Neighoperator();
	}

	IE = IE / 10;

	if(IE < 0)
		IE = abs(IE);

	return (-IE)/(log10(0.90));
}

/*
	* Aplica enfriamiento Simulado a una instancia
	* Devuelve un vector con los costes de las diferentes
	soluciones generadas en el algoritmo
*/
std::vector<double> EnfriamientoSimulado()
{
	std::vector<double> fitness(1000); // Aqui guardo los fitness
	double T = TemperaturaInicial(); // Calculo la Temperatura inicial
	
	Solucion sActual(GetInstance().getNodos()); // Solucion Actual
	Solucion sBest(GetInstance().getNodos()); // Mejor Solucion
	
	getValidSolution(sActual, GetInstance().getMateriales()); // Doy valores a la solucion actual
	
	SetSol(sActual); // La solucion del problema inicialmente es la actual
	sBest = GetSol(); // La mejor solucion es la actual
	
	unsigned int numIteraciones = 0;
	fitness[numIteraciones] = GetSol().GetCoste(); // El primer valor del fitness es el de la solucion actual
	
	while(numIteraciones < 1000) // 1000000 iteraciones por experimento
	{
		// Por cada iteracion, mejor solucion es la solucion actual del problema
		sBest = GetSol();
		
		for(int i = 0; i < GetInstance().getNodos() ; i++)
		{
			// La solucion actual es la vecina a la que tenia
			sActual = GetSol();
			getValidSolution(sActual, GetInstance().getMateriales());
			
			// Si es mejor que la que tenia la guardo como mejor
			sBest = seleccionar(sBest, sActual);
			
			// Compruebo si acepto la solucion actual como la solucion para la siguiente iteracion
			if(Aceptar(sActual, GetSol(), T))
				SetSol(sActual);
		}
		
		// Enfrio la temperatura
		T = Enfriar(T,numIteraciones);
		fitness[numIteraciones] = GetSol().GetCoste();
		numIteraciones++;
	}
	SetSol(sBest);
	
	return fitness;
}

bool Aceptar(Solucion &s1, Solucion &s2, double &T)
{
	double IE = s1.GetCoste() - s2.GetCoste();
	
	// Si acepta si mejora
	if (IE > 0)
		return true;
	
	long double PIE = exp(IE/T);
	
	if(PIE > 0.5)
		return true;
	else
		return false;
}