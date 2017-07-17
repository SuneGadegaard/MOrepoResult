#pragma once

#include<fstream>
#include<iostream>
#include<string>
#include<vector>
#include<stdexcept>
#include<iomanip>
#include"json\json.h"
//#include"json\json-forwards.h"

class MOrepoResultWriter
{
private:
	Json::Value results;
	

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
};