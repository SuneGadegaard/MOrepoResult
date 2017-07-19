#pragma once

/** \mainpage An introduction to the MOrepoResultWriter
* \author Sune Lauth Gadegaard
* \version 1.0.1
*
* \section License
*
* Copyright 2015, Sune Lauth Gadegaard.
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
* If you use the software in any academic work, please make a reference to
*
* ``An appropriate reference should go here!''
* \latexonly
* \section{Description}
*
* This software provides a "converter" that given appropritate input generates a result file compatible witht format used on \href{https://github.com/MCDMSociety}{MOrepo}.
* The software basically consists of a single class "MOrepoResultWriter". After constructing an object of the class, the simple "set" methods should be used to set the appropriate
* entries, and finally, after setting atleast all required entries and the output file path, the \texttt{writeFile ( )} function should be called.
*
* \subsection{Jsoncpp}
* This implementation uses the open source C++ library \texttt{jsoncpp} to handle all the json--format related stuff. Therefore this library should be installed in order
* to compile the program. The library is open source under the MIT license, which means it is fully acceptable to distribute the source code. Therefore, a copy of the jsoncpp
* library is included in this directory. It is an amalgamated version of the source code, which means that only two header file (json.h and json-forwards.h) and one source code
* file (jsoncpp.cpp) should be added to yout project plus the MOrepoResultWriter.h and MOrepoResultWriter.cpp files. This leaves a reather easy to use library for handling
* result files in the MOrepo format.
* \endlatexonly
*
* \section Compiling
* The codes were compiled using the Visual Studio 2015 compiler on a Windows 10 machine. 
* The following flags were used: 
* \begin{quote}
*	/W3 /Ox /std:[c++14|c++latest]
* \end{quote}
* It has been succesfully tested using the gcc 4.9.2 compiler on windows using Code::blocks as well
*
*
* \latexonly
*   \section{Example of usage}
*   This section contains an example showing how the \texttt{converter} class can be used to generate a MOrepo compatible json file. In this example we assume the instance
*   is named "instance1", that the contribution is "Foo et al. 2017", and that the class dataReader has the appropriate get methods.

   \begin{lstlisting}
       // main.cpp
		#include"converter.h"
		#include"dataReader.h" // Assume you have written this yourself
		#include<vector>
		int main(int argc, char** argv){
			try{
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

				std::vector<std::vector<double>> points = { { 28463.2 , 92 }, { 28549.6 , 72 }, 
															{ 28550.5 , 69 }, { 28585.8 , 65 }, 
															{ 28606.2 , 60 } , { 28619.7 , 57} };
				std::vector<std::string> types = { "null","null", "null", "null", "null", "null" };
				MOwriter.setPoints ( points, types );

				MOwriter.setValid ( true );

				MOwriter.setExecutionTime ( 13.274200 );
	
				aString = "Intel Core i7-4785T 2.2 GHz, 16 GB RAM, Linux Ubuntu 64bit";
				MOwriter.setMachineSpecs ( aString );

				aString = "Results from the paper 
						  'A bi-objective approach to discrete cost-bottleneck location problems' 
						   by Gadegaard, Klose, Nielsen, Annals of Operations Research, 2016.";
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
			catch ( std::runtime_error& re )
			{
				std::cerr << re.what () << "\n";
			}
			catch(std::exception &e)
			{
				std::cerr << "Exception: " << e.what() << "\n";
			}
			catch(...)
			{
				std::cerr << "An unexpected exception was thrown. Caught in main.\n";
			}
		}
   \end{lstlisting}
 \endlatexonly
*
* \latexonly
* \section{Change log for MOrepoResultWriter.h and MOrepoResultWriter.cpp}
* \begin{center}
*     \begin{tabularx}{\textwidth}{llr X}\toprule
*        FILE:          &   \multicolumn{3}{l}{MOrepoResultWriter.h and MOrepoResultWriter.cpp}\\
*        Version:       &   \multicolumn{3}{l}{1.0.1}\\
*        \cmidrule{1-3}\\
*        CHANGE LOG:    &   DATE			&   VER.-NO.    &   CHANGES MADE\\ \midrule
*                       &   2017--07--01    &   1.0.0       & First implementation\\
*						&	2017--07--19	&	1.0.1		& Added setOutputFilePath function. 
															  Added a patch for the std::to\_stirng function, so it now compiles using gcc 4.9.2
															  The patch is in the namespace MOrepo.\\\bottomrule
*     \end{tabularx}
* \end{center}
* \endlatexonly
*/

#include<fstream>
#include<iostream>
#include<string>
#include<vector>
#include<stdexcept>
#include<iomanip>

#include"json/json.h"





namespace MOrepo
{
	template < typename T > std::string MOrepo_to_string ( const T& n )
	{
		std::ostringstream stm;
		stm << n;
		return stm.str ( );
	}
}

class MOrepoResultWriter
{
private:
	Json::Value results;
	
	std::string outputFilePath;

	inline void requiredFiledMissing ( const std::string& field ) { throw std::runtime_error( "You haven't specified the required field: " + field + "\n" ); }
public:
	MOrepoResultWriter ( );
	void writeFile ( );

	/*!
	 * \brief Sets the version of the resultfile
	 *
	 * Sets the version of the results file using the provided string
	 * \param version reference to a string. If the version is 5.4 the input should be a string "5.4"
	 */
	void setVersion ( std::string& version ) { results["version"] = version; }

	/*!
	 * Sets the name of the instance for which the result file contains information
	 * \param instanceName reference to a string containing the name of the instance for which the results are for.
	 */
	void setInstanceName ( std::string& instanceName ) { results["instanceName"] = instanceName; }

	/*!
	 * \brief Sets the name of the contribution.
	 *
	 * Sets the contribution name. It should be a string with the name of the contribution in which the instances and results have been published.
	 * \param contributionName reference to a string. Contains the name of the constribution, e.g. "Pedersen08".
	 */
	void setContributionName ( std::string& contributionName ) { results["contributionName"] = contributionName; }

	/*!
	 * Sets the number of objectives
	 * \param objectives integer specififying the number of objective functions of the multiobjective optimization problem.
	 */
	void setObjectives ( int objectives ) { results["objectives"] = objectives;	}

	/*!
	 * \brief Sets the objective types
 	 *
	 * Sets the objective types to either int, float, or null (if unknown).
	 * \param types reference to a vector of strings containing the type of each objective. That is if the i'th objective is integral, then objType[i] = "int"
	 * \note if the function "setObjectives()" has been called prior to calling setObjectiveTypes, and the number of objectives does not equal the lenght of the vector "types", a runtime error is thrown.
	 */
	void setObjectiveTypes ( std::vector<std::string>& types );

	/*!
	 * \brief Sets the directions of the objective functions
	 *
	 * Sets the directions of the objective functions. The directions can either be "min" or "max".
	 * \param directions reference to a vector of strings. If for example there are three objective functions where the two
	 * first are of the minimization-kind and the last is a maximization, we should specify a vector { "min" , "min" , "max" } as the function argument.
	 */
	void setDirections ( std::vector<std::string>& directions ) 
	{ 
		Json::Value JsonDirections; 
		for ( auto it = directions.begin ( ); it != directions.end ( ); ++it ) JsonDirections.append ( Json::Value ( *it ) );  
		results["direction"] = JsonDirections;
	};

	/*!
	 * Specifies whether the solution is an optimal solution to the specific instance or not.
	 * \param isOptimal boolean. If isOptimal = true, it is assumed that the solutions is optimal solution, and if itsOptimal = false, it has not been verified optimal, or it is known to be suboptimal
	 * \note If setOptimal has not been called when creating the results file, the entry "optimal" is set to null.
	 */
	void setOptimal ( bool isOptimal ) { results["optimal"] = isOptimal; }

	/*!
	 * Sets the cardinality of the non--dominated frontier
	 * \param cardinality integer specifying the number of points on the efficient frontier.
	 */
	void setCardinality ( int cardinality ) { results["card"] = cardinality; }

	/*!
	 * Sets the points and the point types
	 *
	 * \param points reference to a vector of vectors of doubles. points[i] contains the i'th point on the frontier and points[i][j] contains the j'th entry
	 * of the i'th non--dominated point.
	 * \param types reference to a vector of strings. Contains a specification of the type of each point. type can be either extreme supported ("se"),
	 * non-extreme supported ("sne"), supported (may be extreme or non--extreme) ("s"), unsuported ("un") or if this information is unknown ("null").
	 */
	void setPoints ( std::vector<std::vector<double>>& points, std::vector<std::string>& types );

	/*!
	 * \brief Sets the validity of the solution to either true or false.
	 *
	 * Sets the validity of the solution to either true or false. If valid is false, the solution might be in conflict with another solution on MOrepo.
	 * This will be sorted out eventually.
	 * \param valid boolean. If true, the solution is not in conflict with other known solutions. If false, it may be in conflict with a known solution.
	 */
	void setValid ( bool valid ) { results["valid"] = valid; }

	/*!
	 * \brief Sets the execution time.
	 *
	 * Sets the execution time, that is, the time in seconds it took to generate the solution contained in the results file.
	 * \param executionTime double containing the execution time in seconds.
	 */
	void setExecutionTime ( double executionTime ) { results["cpu"]["sec"] = executionTime; }

	/*!
	 * \brief Sets the machine specifics
	 *
	 * Sets the specs of the machine used to carry out the experiments, e.g. "Intel Core i7-4785T 2.2 GHz, 16 GB RAM, Linux Ubuntu 64bit"
	 *\param machineSpecs reference to a string. Contains the specs of the machine used to perform the experiments.
	 */
	void setMachineSpecs ( std::string& machineSpecs ) { results["cpu"]["machineSpec"] = machineSpecs; }

	/*!
	 * Sets the comments entry
	 * \param comments reference to a string. Contains the comment that should be attached to the result file.
	 */
	void setComments ( std::string& comments ) { results["comments"] = comments; }

	/*!
	 * Sets the cardinality of the set of supported non--dominated solutions
	 * \param suppCard integer containing the number of supported non--dominated solutions.
	 */
	void setSupportedCardinality ( int suppCard ) { results["suppCard"	] = suppCard; }

	/*!
	 * Sets the cardinality of the set of extreme supported non--dominated solutions
	 * \param extCard integer containing the number of extreme supported non--dominated solutions.
	 */
	void setExtremeSupportedCardinality ( int extCard ) { results["extCard"] = extCard; }

	/*!
	 * Sets the misc entry
	 * \param misc reference to a string containing the misc that should be attached to the result file
	 * \note This entry may be used as you like. It could e.g. contain an object with more detailed entries about the experiment.
	 */
	void setMisc ( std::string& misc ) { results["misc"] = misc; }

	/*!
	 * \breif Sets the output file path
	 *
	 * Sets the output file path. If this function is not called before calling writeFile(), then the output file will be "./results.json".
	 * \param outputPath reference to a string containing the path (relative or absolute) to the output file.
	 * \note If the file specified by the outputPath does not exist, then the file will be created. If it does exist, then the file content will be overwritten.
	 */
	void setOutputFilePath ( std::string& outputPath ) { outputFilePath = outputPath; }
};