
#include"MOrepoResultWriter.h"
int main ( )
{

	std::string aString;
	MOrepoResultWriter MOwriter = MOrepoResultWriter ( );

	aString = "1.0";
	MOwriter.setVersion ( aString );

	aString = "Gadegaard16_UFLP_Klose_p01_0";
	MOwriter.setInstanceName ( aString );

	aString = "Gadegaard16";
	MOwriter.setContributionName ( aString );

	MOwriter.setObjectives ( 2 );

	std::vector<std::string> objTypes = { "float" , "int" };
	MOwriter.setObjectiveTypes ( objTypes );

	std::vector<std::string> directions = { "min" , "min" };
	MOwriter.setDirections ( directions );

	MOwriter.setOptimal ( true );

	MOwriter.setCardinality ( 6 );

	std::vector<std::vector<double>> points = { { 28463.2 , 92 }, { 28549.60 , 72 }, { 28550.50 , 69 }, { 28585.80 , 65 }, { 28606.20 , 60 } , { 28619.70 , 57} };
	std::vector<std::string> types = { "null","null", "null", "null", "null", "null" };
	MOwriter.setPoints ( points, types );

	MOwriter.setValid ( true );

	MOwriter.setExecutionTime ( 13.274200 );
	
	aString = "Intel Core i7-4785T 2.2 GHz, 16 GB RAM, Linux Ubuntu 64bit";
	MOwriter.setMachineSpecs ( aString );

	aString = "Results from the paper 'A bi-objective approach to discrete cost-bottleneck location problems' by Gadegaard, Klose, Nielsen, Annals of Operations Research, 2016.";
	MOwriter.setComments ( aString );

	MOwriter.setSupportedCardinality ( 3 );

	MOwriter.setExtremeSupportedCardinality ( 3 );


	aString = "This is the miscellaneous entry.";
	MOwriter.setMisc ( aString );

	aString = "./Gadegaard16_UFLP_Klose_p01_0_results.json";
	MOwriter.setOutputFilePath ( aString );

	MOwriter.writeFile ( );
	return 0;
}

