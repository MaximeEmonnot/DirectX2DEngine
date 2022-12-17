#include "ParserEngine/JSONParser.h"

#include <fstream>
#include "rapidjson/istreamwrapper.h"

JSONParser::Reader::Reader(const Reader& copy)
{
	*this = copy;
}

JSONParser::Reader& JSONParser::Reader::operator=(const Reader& copy)
{
	document.CopyFrom(copy.document, document.GetAllocator());

	return *this;
}

void JSONParser::Reader::ReadFile(const std::string& filePath)
{
	std::ifstream file(filePath);
	if (!file)
		throw JSON_EXCEPTION("An exception has been caught during JSON file opening.\nPlease check JSON file path.");
	rapidjson::IStreamWrapper isw(file);
	document.ParseStream(isw);

	// Basic Exception Handling... 
	if (document.HasParseError())
	{
		switch (document.GetParseError())
		{
		case rapidjson::kParseErrorNone:
			throw JSON_EXCEPTION("An exception has been caught during JSON file parsing.\nError information: The document is empty.\nPlease check the document.");
		case rapidjson::kParseErrorDocumentEmpty:
			throw JSON_EXCEPTION("An exception has been caught during JSON file parsing.\nError information: The document root must not follow by other values.\nPlease check the document.");
		case rapidjson::kParseErrorDocumentRootNotSingular:
			throw JSON_EXCEPTION("An exception has been caught during JSON file parsing.\nError information: Invalid value.\nPlease check the document.");
		case rapidjson::kParseErrorValueInvalid:
			throw JSON_EXCEPTION("An exception has been caught during JSON file parsing.\nError information: Missing a name for object member.\nPlease check the document.");
		case rapidjson::kParseErrorObjectMissName:
			throw JSON_EXCEPTION("An exception has been caught during JSON file parsing.\nError information: Missing a colon after a name of object member.\nPlease check the document.");
		case rapidjson::kParseErrorObjectMissColon:
			throw JSON_EXCEPTION("An exception has been caught during JSON file parsing.\nError information: Missing a comma or '}' after an object member.\nPlease check the document.");
		case rapidjson::kParseErrorObjectMissCommaOrCurlyBracket:
			throw JSON_EXCEPTION("An exception has been caught during JSON file parsing.\nError information: Missing a comma or ']' after an array element.\nPlease check the document.");
		case rapidjson::kParseErrorArrayMissCommaOrSquareBracket:
			throw JSON_EXCEPTION("An exception has been caught during JSON file parsing.\nError information: Incorrect hex digit after \\u escape in string.\nPlease check the document.");
		case rapidjson::kParseErrorStringUnicodeEscapeInvalidHex:
			throw JSON_EXCEPTION("An exception has been caught during JSON file parsing.\nError information: \nPlease check the document.");
		case rapidjson::kParseErrorStringUnicodeSurrogateInvalid:
			throw JSON_EXCEPTION("An exception has been caught during JSON file parsing.\nError information: The surrogate pair in string is invalid.\nPlease check the document.");
		case rapidjson::kParseErrorStringEscapeInvalid:
			throw JSON_EXCEPTION("An exception has been caught during JSON file parsing.\nError information: Invalid escape character in string.\nPlease check the document.");
		case rapidjson::kParseErrorStringMissQuotationMark:
			throw JSON_EXCEPTION("An exception has been caught during JSON file parsing.\nError information: Missing a closing quotation mark in string.\nPlease check the document.");
		case rapidjson::kParseErrorStringInvalidEncoding:
			throw JSON_EXCEPTION("An exception has been caught during JSON file parsing.\nError information: Invalid encoding in string.\nPlease check the document.");
		case rapidjson::kParseErrorNumberTooBig:
			throw JSON_EXCEPTION("An exception has been caught during JSON file parsing.\nError information: Number too big to be stored in double.\nPlease check the document.");
		case rapidjson::kParseErrorNumberMissFraction:
			throw JSON_EXCEPTION("An exception has been caught during JSON file parsing.\nError information: Miss fraction part in number.\nPlease check the document.");
		case rapidjson::kParseErrorNumberMissExponent:
			throw JSON_EXCEPTION("An exception has been caught during JSON file parsing.\nError information: Miss exponent in number.\nPlease check the document.");
		case rapidjson::kParseErrorTermination:
			throw JSON_EXCEPTION("An exception has been caught during JSON file parsing.\nError information: Parsing was terminated.\nPlease check the document.");
		case rapidjson::kParseErrorUnspecificSyntaxError:
			throw JSON_EXCEPTION("An exception has been caught during JSON file parsing.\nError information: Unspecific syntax error.\nPlease check the document.");
		default:;
		}
	}

	file.close();
}

rapidjson::Value& JSONParser::Reader::GetValueOf(const std::string& valueName)
{
	if (!document.HasMember(valueName.c_str()))
		throw JSON_EXCEPTION("An exception has been caught during JSON Array Access.\nPlease check value name.");
	return document[valueName.c_str()];
}