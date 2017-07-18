#include"MOrepoResultWriter.h"

MOrepoResultWriter::MOrepoResultWriter ( ) : outputFilePath ("")
{

}

void MOrepoResultWriter::writeFile ( )
{
	try
	{
		
		std::ofstream outputStream;
		if ( "" == outputFilePath ) 
		{
			outputStream.open ( "./results.json" );
		}
		else
		{
			outputStream.open ( outputFilePath );
		}

		// Check if all the required fields are there
		if ( !results.isMember ( "version" ) )			requiredFiledMissing ( "version" );
		if ( !results.isMember ( "instanceName" ) )		requiredFiledMissing ( "instanceName" );
		if ( !results.isMember ( "contributionName" ) ) requiredFiledMissing ( "contributionName" );
		if ( !results.isMember ( "objectives" ) )		requiredFiledMissing ( "objectives" );
		if ( !results.isMember ( "objectiveType" ) )	requiredFiledMissing ( "objectiveType" );
		if ( !results.isMember ( "direction" ) )		requiredFiledMissing ( "direction" );
		if ( !results.isMember ( "optimal" ) )
		{
			// If we have not set optimal to either true or false, it should be null!
			results["optimal"] = Json::nullValue;
		}
		if ( !results.isMember ( "card" ) )				requiredFiledMissing ( "card" );
		if ( !results.isMember ( "points" ) )			requiredFiledMissing ( "points" );
		if ( !results.isMember ( "valid" ) )			requiredFiledMissing ( "valid" );


		Json::StyledWriter Fw;
		outputStream << Fw.write ( results );
		outputStream.close ( );

	}
	catch ( std::runtime_error& re )
	{
		std::cerr << "Runtime error in writeFile in the MOrepoResultWriter class : " << re.what ( ) << "\n";
	}
}


/*********************************************************************************/
void MOrepoResultWriter::setObjectiveTypes ( std::vector<std::string>& types )
{
	try
	{
		// If the number of objectives has been set, chekc if there is consistency
		if ( results.isMember ( "objectives" ) )
		{
			
			int objs = results["objectives"].asInt ( );
			if ( types.size() != objs ) 
			{
				throw std::runtime_error ( "The number of \"types\" does not equal the number of objectives\n" );
			}
		}

		// Else, simply continue
		Json::Value jsonTypes;
		for ( auto it = types.begin ( ); it != types.end ( ); ++it )
		{
			jsonTypes.append ( Json::Value ( *it ) );
		}
		results["objectiveType"] = jsonTypes;
	}
	catch ( std::runtime_error &re )
	{
		std::cerr << "Runtime error in setObjectiveTypes in the MOrepoResultWriter class : " << re.what ( ) << std::endl;
	}
}

/*********************************************************************************/
void MOrepoResultWriter::setPoints ( std::vector<std::vector<double>>& points, std::vector<std::string>& types )
{
	try
	{
		// First check if the cardinality and the number of points are consitent
		if ( results.isMember ( "card" ) )
		{
			int card = results["card"].asInt ( );
			if ( points.size ( ) != size_t ( card ) )
			{
				throw std::runtime_error ( "The cardinality does not equal the number of points\n" );
			}
		}

		// Second check if the number of objectives equal the number of types.
		if ( results.isMember ( "objectiveType" ) )
		{
			int sizeOfTypes = results["objectiveType"].size ( );
			//Iterate through all the points, and check if the dimensions equals the number of types
			for ( auto it = points.begin ( ); it != points.end ( ); ++it )
			{
				if ( it->size ( ) != sizeOfTypes )
				{
					throw std::runtime_error ( "Dimensions of points does not equal the number of types\n" );
				}
			}
		}

		// Now check if there is a type for each point!
		if ( !( points.size ( ) == types.size ( ) ) )
		{
			throw std::runtime_error ( "The number of points does not equal the number of types\n" );
		}

		// If we pass the tests, we should populate the points entry. Each entry should be an object in itself
		Json::Value JsonPoints;
		auto typesIt = types.begin ( );
		
		for ( auto pointsIt = points.begin ( ); pointsIt != points.end ( ); ++pointsIt, ++typesIt )
		{
			Json::Value aPoint;
			aPoint["type"] = *typesIt;
			int objIndex = 1;
			for ( auto objIt = pointsIt->begin ( ); objIt != pointsIt->end ( ); ++objIt )
			{
				std::string z ( "z" + std::to_string(objIndex));
				aPoint[z] = *objIt;
				++objIndex;
			}
			JsonPoints.append ( aPoint );
		}
		results["points"] = JsonPoints;
	}
	catch ( std::runtime_error &re )
	{
		std::cerr << "Runtime error in setPoints in the MOrepoResultWriter class : " << re.what ( ) << std::endl;
	}
}