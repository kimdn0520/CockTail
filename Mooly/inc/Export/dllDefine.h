#pragma once

#ifdef MOOLY_EXPORTS

	#define MOOLY_ENGINE __declspec(dllexport)
	#define MOOLY_ENGINE_TEMPLATE_EXPORT

#else

	#define MOOLY_ENGINE __declspec(dllimport)
	#define MOOLY_ENGINE_TEMPLATE_EXPORT extern

#endif