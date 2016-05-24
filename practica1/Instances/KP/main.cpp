#include <iostream>
#include "InstanceKP.h"
//#include "Random.h"

using namespace std;

int main(int argc,char* argv[])
{
InstanceKP prueba(11154);

ifstream fich("knapPI_12_500_1000.csv");

fich>>prueba;

return 0;
}
