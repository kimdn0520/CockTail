#include "pch.h"
#include "YAMLParser/YAMLParser.h"

void YAMLParser::OpenFile(const std::string& path, const std::string& saveName)
{
	try
	{
		yamlNodeList = YAML::LoadAllFromFile(path);
	}
	catch (YAML::Exception& e)
	{
		std::cerr << "Error parsing YAML document : " << e.what() << std::endl;
	}

	/// <summary>
	/// yaml-cpp 로는 유니티의 --- !u!29 &1 이거를 파싱할수가 없다.
	/// 그래서 직접 '---' 텍스트를 읽어서 ID값을 넣어주도록 하자
	/// </summary>
	FILE* file;
	fopen_s(&file, path.c_str(), "r");

	// feof(FILE* stream) - 파일의 끝에 도달하게되면 0이 아닌 값을 반환
	while (!feof(file))
	{
		fgets(line, 256, file);

		int ch_Index = 0;

		// --- 인 경우만 하자.
		if (line[ch_Index] == '-')
		{
			// &가 나올때까지 다음 문자로 간다.
			while (line[ch_Index] != '&')
			{
				ch_Index++;
			}

			ch_Index++;

			std::string fildID = "";
			int index = 0;

			while (line[ch_Index] >= '0' && line[ch_Index] <= '9')
			{
				fildID += line[ch_Index];
				ch_Index++;
			}

			fildIDList.emplace_back(fildID);
		}
	}

	scene = std::make_shared<YAMLBinaryData::Scene>();

	// yaml scene 순서가 엉킬수가 있다. 그렇기에 게임오브젝트부터 다 넣은다음 컴포넌트에서 게임오브젝트 id를 찾아서 넣어줘야한다.
	for (int i = 0; i < yamlNodeList.size(); i++)
	{
		if (yamlNodeList[i]["GameObject"])
		{
			YAMLBinaryData::GameObject gameObject;

			gameObject.gameObjectID = fildIDList[i];

			YAML::Node GameObject = yamlNodeList[i]["GameObject"];

			gameObject.name = GameObject["m_Name"].as<std::string>();

			gameObject.tag = GameObject["m_TagString"].as<std::string>();

			gameObject.layer = GameObject["m_Layer"].as<int>();

			scene->gameObjects.emplace_back(gameObject);
		}
	}

	for (int i = 0; i < yamlNodeList.size(); i++)
	{
		if (yamlNodeList[i]["RenderSettings"])
		{
			YAML::Node RenderSetting = yamlNodeList[i]["RenderSettings"];

			YAML::Node m_AmbientSkyColor = RenderSetting["m_AmbientSkyColor"];

			scene->ambientSkyColor.x = m_AmbientSkyColor["r"].as<float>();
			scene->ambientSkyColor.y = m_AmbientSkyColor["g"].as<float>();
			scene->ambientSkyColor.z = m_AmbientSkyColor["b"].as<float>();
			scene->ambientSkyColor.w = m_AmbientSkyColor["a"].as<float>();
		}
		else if (yamlNodeList[i]["Transform"])
		{
			std::shared_ptr<YAMLBinaryData::Transform> transform = std::make_shared<YAMLBinaryData::Transform>();

			transform->componentID = fildIDList[i];

			YAML::Node Transform = yamlNodeList[i]["Transform"];

			YAML::Node m_GameObject = Transform["m_GameObject"];

			transform->gameObjectID = m_GameObject["fileID"].as<std::string>();

			YAML::Node m_Children = Transform["m_Children"];

			for (int childCnt = 0; childCnt < m_Children.size(); childCnt++)
			{
				transform->childIDList.push_back(m_Children[childCnt]["fileID"].as<std::string>());
			}

			YAML::Node m_LocalPosition = Transform["m_LocalPosition"];

			transform->localPosition.x = m_LocalPosition["x"].as<float>();
			transform->localPosition.y = m_LocalPosition["y"].as<float>();
			transform->localPosition.z = m_LocalPosition["z"].as<float>();

			YAML::Node m_LocalRotation = Transform["m_LocalRotation"];
			YAMLBinaryData::Float3 rot = QuatToEuler(m_LocalRotation["x"].as<float>(), m_LocalRotation["y"].as<float>(), m_LocalRotation["z"].as<float>(), m_LocalRotation["w"].as<float>());
			transform->localRotation.x = rot.x;
			transform->localRotation.y = rot.y;
			transform->localRotation.z = rot.z;

			YAML::Node m_LocalScale = Transform["m_LocalScale"];

			transform->localScale.x = m_LocalScale["x"].as<float>();
			transform->localScale.y = m_LocalScale["y"].as<float>();
			transform->localScale.z = m_LocalScale["z"].as<float>();

			//scene->gameObjects.back().transform = transform;

			auto it = find_if(scene->gameObjects.begin(), scene->gameObjects.end(), [&id = transform->gameObjectID](YAMLBinaryData::GameObject s)->bool {return (s.gameObjectID == id); });

			if (it != scene->gameObjects.end())
				it->transform = transform;
		}
		else if (yamlNodeList[i]["MeshFilter"])
		{
			std::shared_ptr<YAMLBinaryData::MeshFilter> meshFilter = std::make_shared<YAMLBinaryData::MeshFilter>();

			meshFilter->componentID = fildIDList[i];

			YAML::Node MeshFilter = yamlNodeList[i]["MeshFilter"];

			YAML::Node m_GameObject = MeshFilter["m_GameObject"];

			meshFilter->gameObjectID = m_GameObject["fileID"].as<std::string>();

			YAML::Node m_Mesh = MeshFilter["m_Mesh"];

			int meshID = m_Mesh["fileID"].as<int>();

			switch (meshID)
			{
			case 10202:
			{
				meshFilter->meshName = "CubeMesh";
			}
			break;
			case 10207:
			{
				meshFilter->meshName = "SphereMesh";
			}
			break;
			case 10209:
			{
				meshFilter->meshName = "PlaneMesh";
			}
			break;
			default:
				break;
			}

			auto it = find_if(scene->gameObjects.begin(), scene->gameObjects.end(), [&id = meshFilter->gameObjectID](YAMLBinaryData::GameObject s)->bool {return (s.gameObjectID == id); });

			if (it != scene->gameObjects.end())
				it->meshFilter = meshFilter;
		}
		else if (yamlNodeList[i]["Camera"])
		{
			std::shared_ptr<YAMLBinaryData::Camera> camera = std::make_shared<YAMLBinaryData::Camera>();

			camera->componentID = fildIDList[i];

			YAML::Node Camera = yamlNodeList[i]["Camera"];

			YAML::Node m_GameObject = Camera["m_GameObject"];

			camera->gameObjectID = m_GameObject["fileID"].as<std::string>();

			camera->projectionMatrixMode = Camera["m_projectionMatrixMode"].as<int>();

			camera->nearPlane = Camera["near clip plane"].as<float>();

			camera->farPlane = Camera["far clip plane"].as<float>();

			auto it = find_if(scene->gameObjects.begin(), scene->gameObjects.end(), [&id = camera->gameObjectID](YAMLBinaryData::GameObject s)->bool {return (s.gameObjectID == id); });

			if (it != scene->gameObjects.end())
				it->camera = camera;
		}
		else if (yamlNodeList[i]["Light"])
		{
			// spot = 0, directional = 1, point = 2
			std::shared_ptr<YAMLBinaryData::Light> light = std::make_shared<YAMLBinaryData::Light>();

			light->componentID = fildIDList[i];

			YAML::Node Light = yamlNodeList[i]["Light"];

			YAML::Node m_GameObject = Light["m_GameObject"];

			light->gameObjectID = m_GameObject["fileID"].as<std::string>();

			light->type = Light["m_Type"].as<int>();

			YAML::Node m_Color = Light["m_Color"];

			light->color.x = m_Color["r"].as<float>();
			light->color.y = m_Color["g"].as<float>();
			light->color.z = m_Color["b"].as<float>();
			light->color.w = m_Color["a"].as<float>();

			light->intensity = Light["m_Intensity"].as<float>();

			light->range = Light["m_Range"].as<float>();

			light->outerSpotAngle = Light["m_SpotAngle"].as<float>();

			light->innerSpotAngle = Light["m_InnerSpotAngle"].as<float>();

			YAML::Node m_Shadows = Light["m_Shadows"];

			light->shadow = m_Shadows["m_Type"].as<int>();

			auto it = find_if(scene->gameObjects.begin(), scene->gameObjects.end(), [&id = light->gameObjectID](YAMLBinaryData::GameObject s)->bool {return (s.gameObjectID == id); });

			if (it != scene->gameObjects.end())
				it->light = light;
		}
		else if (yamlNodeList[i]["BoxCollider"])
		{
			std::shared_ptr<YAMLBinaryData::BoxCollider> boxCollider = std::make_shared<YAMLBinaryData::BoxCollider>();

			boxCollider->componentID = fildIDList[i];

			YAML::Node BoxCollider = yamlNodeList[i]["BoxCollider"];

			YAML::Node m_GameObject = BoxCollider["m_GameObject"];

			boxCollider->gameObjectID = m_GameObject["fileID"].as<std::string>();

			boxCollider->isTrigger = BoxCollider["m_IsTrigger"].as<int>();

			YAML::Node m_Size = BoxCollider["m_Size"];

			boxCollider->size.x = m_Size["x"].as<float>();
			boxCollider->size.y = m_Size["y"].as<float>();
			boxCollider->size.z = m_Size["z"].as<float>();

			YAML::Node m_Center = BoxCollider["m_Center"];

			boxCollider->center.x = m_Center["x"].as<float>();
			boxCollider->center.y = m_Center["y"].as<float>();
			boxCollider->center.z = m_Center["z"].as<float>();

			auto it = find_if(scene->gameObjects.begin(), scene->gameObjects.end(), [&id = boxCollider->gameObjectID](YAMLBinaryData::GameObject s)->bool {return (s.gameObjectID == id); });

			if (it != scene->gameObjects.end())
				it->boxCollider = boxCollider;
		}
		else if (yamlNodeList[i]["SphereCollider"])
		{
			std::shared_ptr<YAMLBinaryData::SphereCollider> sphereCollider = std::make_shared<YAMLBinaryData::SphereCollider>();

			sphereCollider->componentID = fildIDList[i];

			YAML::Node SphereCollider = yamlNodeList[i]["SphereCollider"];

			YAML::Node m_GameObject = SphereCollider["m_GameObject"];

			sphereCollider->gameObjectID = m_GameObject["fileID"].as<std::string>();

			sphereCollider->isTrigger = SphereCollider["m_IsTrigger"].as<bool>();

			sphereCollider->radius = SphereCollider["m_Radius"].as<float>();

			YAML::Node m_Center = SphereCollider["m_Center"];

			sphereCollider->center.x = m_Center["x"].as<float>();
			sphereCollider->center.y = m_Center["y"].as<float>();
			sphereCollider->center.z = m_Center["z"].as<float>();

			auto it = find_if(scene->gameObjects.begin(), scene->gameObjects.end(), [&id = sphereCollider->gameObjectID](YAMLBinaryData::GameObject s)->bool {return (s.gameObjectID == id); });

			if (it != scene->gameObjects.end())
				it->sphereCollider = sphereCollider;
		}
		else if (yamlNodeList[i]["PrefabInstance"])
		{
			YAMLBinaryData::Prefab prefabInstance;

			YAMLBinaryData::Transform transform;

			YAML::Node PrefabInstance = yamlNodeList[i]["PrefabInstance"];

			prefabInstance.prefabID = fildIDList[i];

			YAML::Node m_Modification = PrefabInstance["m_Modification"];

			YAML::Node m_Modifications = m_Modification["m_Modifications"];

			float localRotationW;

			for (YAML::const_iterator it = m_Modifications.begin(); it != m_Modifications.end(); ++it)
			{
				if (it->operator[]("propertyPath").as<std::string>().compare("m_Name") == 0)
				{
					prefabInstance.name = it->operator[]("value").as<std::string>();
				}
				else if (it->operator[]("propertyPath").as<std::string>().compare("m_LocalPosition.x") == 0)
				{
					transform.localPosition.x = it->operator[]("value").as<float>();
				}
				else if (it->operator[]("propertyPath").as<std::string>().compare("m_LocalPosition.y") == 0)
				{
					transform.localPosition.y = it->operator[]("value").as<float>();
				}
				else if (it->operator[]("propertyPath").as<std::string>().compare("m_LocalPosition.z") == 0)
				{
					transform.localPosition.z = it->operator[]("value").as<float>();
				}
				else if (it->operator[]("propertyPath").as<std::string>().compare("m_LocalRotation.x") == 0)
				{
					transform.localRotation.x = it->operator[]("value").as<float>();
				}
				else if (it->operator[]("propertyPath").as<std::string>().compare("m_LocalRotation.y") == 0)
				{
					transform.localRotation.y = it->operator[]("value").as<float>();
				}
				else if (it->operator[]("propertyPath").as<std::string>().compare("m_LocalRotation.z") == 0)
				{
					transform.localRotation.z = it->operator[]("value").as<float>();
				}
				else if (it->operator[]("propertyPath").as<std::string>().compare("m_LocalRotation.w") == 0)
				{
					localRotationW = it->operator[]("value").as<float>();
				}
			}

			YAMLBinaryData::Float3 rot = QuatToEuler(transform.localRotation.x, transform.localRotation.y, transform.localRotation.z, localRotationW);
			transform.localRotation.x = rot.x;
			transform.localRotation.y = rot.y;
			transform.localRotation.z = rot.z;

			prefabInstance.transform = transform;

			scene->prefabs.emplace_back(prefabInstance);

			OpenPrefab(prefabInstance.name);
		}
	}

	std::ofstream ofs("BinaryFile/Scene/" + saveName + ".scene", std::ios_base::binary);
	boost::iostreams::filtering_stream<boost::iostreams::output> buffer;
	buffer.push(boost::iostreams::zlib_compressor());
	buffer.push(ofs);
	boost::archive::binary_oarchive oa(buffer);

	oa << scene;
}

void YAMLParser::OpenPrefab(const std::string& prefabName)
{
	std::string delimiter = " ";
	std::string token;

	if (prefabName.find(delimiter) != std::string::npos)
	{
		token = prefabName.substr(0, prefabName.find(delimiter));
	}
	else
	{
		token = prefabName.substr(0, prefabName.length());
	}

	std::string path = prefabPath + token + ".prefab";

	// TopNode 정보들을 LoadAll 한다.
	std::vector<YAML::Node> yamlPrefabNodeList;

	// UnityScene 전용 ID 담기
	std::vector<std::string> fildPrefabIDList;

	try
	{
		yamlPrefabNodeList = YAML::LoadAllFromFile(path);
	}
	catch (YAML::Exception& e)
	{
		std::cerr << "Error parsing YAML document : " << e.what() << std::endl;
	}

	/// <summary>
	/// yaml-cpp 로는 유니티의 --- !u!29 &1 이거를 파싱할수가 없다.
	/// 그래서 직접 '---' 텍스트를 읽어서 ID값을 넣어주도록 하자
	/// </summary>
	FILE* file;
	fopen_s(&file, path.c_str(), "r");

	// feof(FILE* stream) - 파일의 끝에 도달하게되면 0이 아닌 값을 반환
	while (!feof(file))
	{
		fgets(line, 256, file);

		int ch_Index = 0;

		// --- 인 경우만 하자.
		if (line[ch_Index] == '-')
		{
			// &가 나올때까지 다음 문자로 간다.
			while (line[ch_Index] != '&')
			{
				ch_Index++;
			}

			ch_Index++;

			std::string fildID = "";
			int index = 0;

			while (line[ch_Index] >= '0' && line[ch_Index] <= '9')
			{
				fildID += line[ch_Index];
				ch_Index++;
			}

			fildPrefabIDList.emplace_back(fildID);
		}
	}

	std::shared_ptr<YAMLBinaryData::Scene> prefabScene = std::make_shared<YAMLBinaryData::Scene>();

	// yaml scene 순서가 엉킬수가 있다. 그렇기에 게임오브젝트부터 다 넣은다음 컴포넌트에서 게임오브젝트 id를 찾아서 넣어줘야한다.
	for (int i = 0; i < yamlPrefabNodeList.size(); i++)
	{
		if (yamlPrefabNodeList[i]["GameObject"])
		{
			YAMLBinaryData::GameObject gameObject;

			gameObject.gameObjectID = fildPrefabIDList[i];

			YAML::Node GameObject = yamlPrefabNodeList[i]["GameObject"];

			gameObject.name = GameObject["m_Name"].as<std::string>();

			gameObject.tag = GameObject["m_TagString"].as<std::string>();

			gameObject.layer = GameObject["m_Layer"].as<int>();

			prefabScene->gameObjects.emplace_back(gameObject);
		}
	}

	for (int i = 0; i < yamlPrefabNodeList.size(); i++)
	{
		if (yamlPrefabNodeList[i]["Transform"])
		{
			std::shared_ptr<YAMLBinaryData::Transform> transform = std::make_shared<YAMLBinaryData::Transform>();

			transform->componentID = fildPrefabIDList[i];

			YAML::Node Transform = yamlPrefabNodeList[i]["Transform"];

			YAML::Node m_GameObject = Transform["m_GameObject"];

			transform->gameObjectID = m_GameObject["fileID"].as<std::string>();

			YAML::Node m_Children = Transform["m_Children"];

			for (int childCnt = 0; childCnt < m_Children.size(); childCnt++)
			{
				transform->childIDList.push_back(m_Children[childCnt]["fileID"].as<std::string>());
			}

			YAML::Node m_LocalPosition = Transform["m_LocalPosition"];

			transform->localPosition.x = m_LocalPosition["x"].as<float>();
			transform->localPosition.y = m_LocalPosition["y"].as<float>();
			transform->localPosition.z = m_LocalPosition["z"].as<float>();

			YAML::Node m_LocalRotation = Transform["m_LocalRotation"];
			YAMLBinaryData::Float3 rot = QuatToEuler(m_LocalRotation["x"].as<float>(), m_LocalRotation["y"].as<float>(), m_LocalRotation["z"].as<float>(), m_LocalRotation["w"].as<float>());
			transform->localRotation.x = rot.x;
			transform->localRotation.y = rot.y;
			transform->localRotation.z = rot.z;

			YAML::Node m_LocalScale = Transform["m_LocalScale"];

			transform->localScale.x = m_LocalScale["x"].as<float>();
			transform->localScale.y = m_LocalScale["y"].as<float>();
			transform->localScale.z = m_LocalScale["z"].as<float>();

			//scene->gameObjects.back().transform = transform;

			auto it = find_if(prefabScene->gameObjects.begin(), prefabScene->gameObjects.end(), [&id = transform->gameObjectID](YAMLBinaryData::GameObject s)->bool {return (s.gameObjectID == id); });

			if (it != prefabScene->gameObjects.end())
				it->transform = transform;
		}
		else if (yamlPrefabNodeList[i]["BoxCollider"])
		{
			std::shared_ptr<YAMLBinaryData::BoxCollider> boxCollider = std::make_shared<YAMLBinaryData::BoxCollider>();

			boxCollider->componentID = fildIDList[i];

			YAML::Node BoxCollider = yamlPrefabNodeList[i]["BoxCollider"];

			YAML::Node m_GameObject = BoxCollider["m_GameObject"];

			boxCollider->gameObjectID = m_GameObject["fileID"].as<std::string>();

			boxCollider->isTrigger = BoxCollider["m_IsTrigger"].as<int>();

			YAML::Node m_Size = BoxCollider["m_Size"];

			boxCollider->size.x = m_Size["x"].as<float>();
			boxCollider->size.y = m_Size["y"].as<float>();
			boxCollider->size.z = m_Size["z"].as<float>();

			YAML::Node m_Center = BoxCollider["m_Center"];

			boxCollider->center.x = m_Center["x"].as<float>();
			boxCollider->center.y = m_Center["y"].as<float>();
			boxCollider->center.z = m_Center["z"].as<float>();

			auto it = find_if(prefabScene->gameObjects.begin(), prefabScene->gameObjects.end(), [&id = boxCollider->gameObjectID](YAMLBinaryData::GameObject s)->bool {return (s.gameObjectID == id); });

			if (it != prefabScene->gameObjects.end())
				it->boxCollider = boxCollider;
		}
		else if (yamlPrefabNodeList[i]["SphereCollider"])
		{
			std::shared_ptr<YAMLBinaryData::SphereCollider> sphereCollider = std::make_shared<YAMLBinaryData::SphereCollider>();

			sphereCollider->componentID = fildIDList[i];

			YAML::Node SphereCollider = yamlPrefabNodeList[i]["SphereCollider"];

			YAML::Node m_GameObject = SphereCollider["m_GameObject"];

			sphereCollider->gameObjectID = m_GameObject["fileID"].as<std::string>();

			sphereCollider->isTrigger = SphereCollider["m_IsTrigger"].as<bool>();

			sphereCollider->radius = SphereCollider["m_Radius"].as<float>();

			YAML::Node m_Center = SphereCollider["m_Center"];

			sphereCollider->center.x = m_Center["x"].as<float>();
			sphereCollider->center.y = m_Center["y"].as<float>();
			sphereCollider->center.z = m_Center["z"].as<float>();

			auto it = find_if(prefabScene->gameObjects.begin(), prefabScene->gameObjects.end(), [&id = sphereCollider->gameObjectID](YAMLBinaryData::GameObject s)->bool {return (s.gameObjectID == id); });

			if (it != prefabScene->gameObjects.end())
				it->sphereCollider = sphereCollider;
		}
	}

	// prfabScene 에서 prefab의 이름을 가진 게임오브젝트 찾기
	auto it = find_if(prefabScene->gameObjects.begin(), prefabScene->gameObjects.end(), [&name = token](YAMLBinaryData::GameObject s)->bool {return (s.name == name); });

	// 있다면 원래 scene에서 Scene이 갖고있는 Prefab들중 해당하는 Prefab에 정보 넣어줘야함
	if (it != prefabScene->gameObjects.end())
	{
		auto scenePrefabIt = find_if(scene->prefabs.begin(), scene->prefabs.end(), [&name = prefabName](YAMLBinaryData::Prefab s)->bool {return (s.name == name); });

		if (scenePrefabIt != scene->prefabs.end())
		{
			scenePrefabIt->transform.localScale = it->transform->localScale;
		
			scenePrefabIt->boxCollider = it->boxCollider;

			scenePrefabIt->sphereCollider = it->sphereCollider;
		}
	}
}

YAMLBinaryData::Float3 YAMLParser::QuatToEuler(float qx, float qy, float qz, float qw)
{
	YAMLBinaryData::Float3 result;

	//float x = qx;
	//float y = qy;
	//float z = qz;
	//float w = qw;

	//float ysqr = y * y;

	//// roll (x-axis rotation)
	//float t0 = 2.0 * (w * x + y * z);
	//float t1 = 1.0 - 2.0 * (x * x + ysqr);
	//result.x = atan2(t0, t1) * 180.0 / 3.141592;

	//// pitch (y-axis rotation)
	//float t2 = 2.0 * (w * y - z * x);
	//t2 = t2 > 1.0 ? 1.0 : t2;
	//t2 = t2 < -1.0 ? -1.0 : t2;
	//result.y = asin(t2) * 180.0 / 3.141592;

	//// yaw (z-axis rotation)
	//float t3 = 2.0 * (w * z + x * y);
	//float t4 = 1.0 - 2.0 * (ysqr + z * z);
	//result.z = atan2(t3, t4) * 180.0 / 3.141592;

	double x = qx;
	double y = qy;
	double z = qz;
	double w = qw;
	double sqx = x * x;
	double sqy = y * y;
	double sqz = z * z;
	double sqw = w * w;
	float exceptCheck = 2.0f * (w * x - y * z);
	float eulerX = 0.f;

	if (abs(exceptCheck) >= 1.f)
		eulerX = copysign(3.141592f / 2, exceptCheck);
	else
		eulerX = asin(2.0f * (w * x - y * z));

	float eulerY = atan2(2.0f * (x * z + w * y), (-sqx - sqy + sqz + sqw));
	float eulerZ = atan2(2.0f * (x * y + z * w), (-sqx + sqy - sqz + sqw));

	eulerX = eulerX * (180.0f / 3.141592654f);
	eulerY = eulerY * (180.0f / 3.141592654f);
	eulerZ = eulerZ * (180.0f / 3.141592654f);

	result.x = eulerX;
	result.y = eulerY;
	result.z = eulerZ;

	return result;
}


