#pragma once
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
#include "DATATYPES/VEC2/VEC2.h"
#include "ANNOTATIONS.h"
#include <stdbool.h>
typedef struct implGameObj GameObject;
		typedef struct {

		void(*Update)( GameObject* thisPtr,float deltaTime);
		void(*Destroy)(GameObject* thisPtr);
		}vTable;
	typedef struct implGameObj{
		vTable VTable;
		Vector2 Position;
	} GameObject;
	void InitGameObjectManager();
	/// <summary>
	/// only used as a return to share internal engine state with other Engine Modules.
	/// </summary>
	typedef struct {
		size_t count;
		__NON_OWNING(GameObject*)objects;
	}GameObjectList;
	//functions for the user below this line
	void RegisterGameObject(__OWNING(GameObject*) obj);
	
#ifdef __cplusplus
}
#endif // __cplusplus


