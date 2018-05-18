#include <default.c>
#include <dynamic.h>

// ** MAIN PARSER FUNCTIONS **

// Returns a handle for a dynamic array of AssetListEntry structs.
int ParseAssetList(const char* filename);
int ParseAssetListString(const char* file_contents);

// ** ASSET LIST PARSER **

// AssetListEntry : an asset list line item with a bitwise sub-strategy identifier.
typedef struct AssetListEntry
{
	char sName[32];		// Asset name
	var vPrice;
	var vSpread;
	var vRollLong;
	var vRollShort;
	var vPip;
	var vPipCost;
	var vMarginCost;
	var vLeverage;
	var vLotAmount;
	var vCommission;
	char sSymbol[32];
	int nStrategies; // Bitwise: Identify which sub-strageties need this Asset, 0 if not needed.
} AssetListEntry;


// parser helper functions:
AssetListEntry* ParseAssetListLine(const char* line);




AssetListEntry* ParseAssetListLine(const char* line)
{
	char* line_temp = (char*)malloc(strlen(line) + 1);
	strcpy(line_temp, line);
	char skip[2];
	strcpy(skip,",");
	static AssetListEntry output;
	memset(&output, 0, sizeof(output));
	// need to explicitly count commas and set pointers after the commas.
	int i;

	// pArgs: pointers to start of all 12 arguments - null if argument does not exist.
	char* pArgs[12];
	memset(pArgs, 0, 12 * sizeof(char*));

	char* position = line_temp;
	pArgs[0] = position;
	for (i = 1; strlen(position); position++)
	{
		if (position[0] == ',')
		{
			if (i < 12)
			{
				pArgs[i] = position + 1;
				i++;
			}
			position[0] = '\0';
		}
	}
	for (i = 0; i<12; i++)   // no more than 12 arguments will be accepted.
	{
		switch (i)
		{
		case 0: sscanf(pArgs[i], "%s", output.sName); break;
		case 1: sscanf(pArgs[i], "%lf", &output.vPrice); break;
		case 2: sscanf(pArgs[i], "%lf", &output.vSpread); break;
		case 3: sscanf(pArgs[i], "%lf", &output.vRollLong); break;
		case 4: sscanf(pArgs[i], "%lf", &output.vRollShort); break;
		case 5: sscanf(pArgs[i], "%lf", &output.vPip); break;
		case 6: sscanf(pArgs[i], "%lf", &output.vPipCost); break;
		case 7: sscanf(pArgs[i], "%lf", &output.vMarginCost); break;
		case 8: sscanf(pArgs[i], "%lf", &output.vLeverage); break;
		case 9: sscanf(pArgs[i], "%lf", &output.vLotAmount); break;
		case 10: sscanf(pArgs[i], "%lf", &output.vCommission); break;
		case 11: sscanf(pArgs[i], "%s", output.sSymbol); break; // optional
		default: break;
		}
	}

	free(line_temp);								// Cleanup
	if (i < 11) return NULL;						// Symbol is optional.  The other 11 fields are required.
	else if (!strlen(output.sName)) return NULL;	// Sanity check
	else return &output;
}

// returns handler to dynamic array of AssetListEntries or NULL if failure
int ParseAssetListString(const char* AssetListString)
{
	// this header must match.  Otherwise, the parse attempt is rejected.
	char header[128];
	strcpy(header,"Name,Price,Spread,RollLong,RollShort,PIP,PIPCost,MarginCost,Leverage,LotAmount,Commission,Symbol");

	// temporary string
	char* str = (char*)malloc(strlen(AssetListString) + 1);
	strcpy(str, AssetListString);

	int hLines = 0, hList = 0;

	// first, make sure the header is correct.
	if (strstr(str, header) != str)
	{
		free(str);
		return NULL;
	}

	const int line_size = 256;
	hLines = da_new(line_size);					// new dynamic array: Tokenized lines
	hList = da_new(sizeof(AssetListEntry));		// new dynamic array: AssetListEntry structs
	if (!hLines || !hList)
	{
		da_delete(hLines);
		da_delete(hList);
		free(str);
		return NULL;
	}

	// step 1 - tokenize the lines and save them in a separate store of memory.
	char skip[4];
	strcpy(skip,"\r\n ");
	char *token;
	token = strtok(str, skip);

	int i; //bool reset = false;// char duplicate[128];
	for (i = 0; token != NULL; i++)
	{
		//std::cout << "Token: " << token << std::endl;
		if (i)
		{
			da_push_back(hLines, token);
			//lines.push_back(token);
		}
		token = strtok(NULL, skip);
	}

	// sanity check: were any lines tokenized?
	if (!da_size(hLines))
	{
		da_delete(hLines);
		da_delete(hList);
		free(str);
		return NULL;
	}

	// step 2 - parse the individual strings for 12 arguments each.
	AssetListEntry* pEntry = NULL;
	for(i = 0; i<da_size(hLines); i++)
	{
		pEntry = ParseAssetListLine((const char*)da_data(hLines, i));
		if (!pEntry) continue;
		else da_push_back(hList, pEntry);
	}

	// cleanup
	da_delete(hLines);
	free(str);

	// successful? check size->
	if (da_size(hList))
		return hList;
	else
	{
		da_delete(hList);
		return NULL;
	}
	
}

int ParseAssetList(const char* filename)
{
	if(!file_length(filename)) return 0;
	return ParseAssetListString(file_content(filename));
}

