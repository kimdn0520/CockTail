#include "GamePch.h"
#include "JsonReader.h"

namespace GameEngineSpace
{
	JsonReader::JsonReader(const std::string& json_path)
	{
		doc = std::make_unique<rapidjson::Document>();

		std::ifstream file(json_path);

		std::stringstream read_Data;

		read_Data << file.rdbuf();

		std::string read_StringData = read_Data.str();
		
		int i = 0;

		while (read_StringData[i] != '[')
			i++;

		read_StringData = read_StringData.substr(i);

		doc->Parse(read_StringData.c_str());

		if(!file.is_open())
			MessageBox(0, TEXT("Json Open Failed"), 0, 0);

		std::string json_str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		
		if (doc->HasParseError())
			MessageBox(0, TEXT("Json ParseError"), 0, 0);
		
		file.close();
	}

	JsonReader::~JsonReader()
	{}

	std::string JsonReader::GetString(const std::string& key)
	{
		auto it = doc->FindMember(key.c_str());

		if (it != doc->MemberEnd() && it->value.IsString())
		{
			return it->value.GetString();
		}
		else 
		{
			MessageBox(0, TEXT("Json GetString() Failed"), 0, 0);
		
			return "";
		}
	}

	int JsonReader::GetInt(const std::string& key)
	{
		auto it = doc->FindMember(key.c_str());
		
		if (it != doc->MemberEnd() && it->value.IsInt())
		{
			return it->value.GetInt();
		}
		else
		{
			MessageBox(0, TEXT("Json GetInt() Failed."), 0, 0);
			
			return 0;
		}
	}

	float JsonReader::GetFloat(const std::string& key)
	{
		auto it = doc->FindMember(key.c_str());

		if (it != doc->MemberEnd() && it->value.IsFloat())
		{
			return it->value.GetFloat();
		}
		else
		{
			MessageBox(0, TEXT("Json GetFloat() Failed."), 0, 0);

			return 0.f;
		}
	}

	bool JsonReader::GetBool(const std::string& key)
	{
		auto it = doc->FindMember(key.c_str());

		if (it != doc->MemberEnd() && it->value.IsBool())
		{
			return it->value.GetBool();
		}
		else
		{
			MessageBox(0, TEXT("Json GetBool() Failed."), 0, 0);

			return false;
		}
	}

	GameEngineSpace::JsonObject JsonReader::GetJsonObject(const std::string& key)
	{
		if (doc->HasMember(key.c_str()) && doc->FindMember(key.c_str())->value.IsObject())
			return JsonObject(doc->FindMember(key.c_str())->value);
		else
		{
			MessageBox(0, TEXT("Json GetJsonObject Failed"), 0, 0);
		}
	}

	std::vector<GameEngineSpace::JsonObject> JsonReader::GetJsonObjects()
	{
		std::vector<GameEngineSpace::JsonObject> jsonObjects;

		for (auto it = doc->Begin(); it != doc->End(); ++it)
		{
			GameEngineSpace::JsonObject jsonObject(*it);

			jsonObjects.push_back(jsonObject);
		}

		return jsonObjects;
	}
}