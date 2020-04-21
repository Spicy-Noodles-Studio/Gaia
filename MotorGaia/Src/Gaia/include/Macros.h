#pragma once
#ifndef MACROS_H
#define MACROS_H


// HANDLE DATA MACROS


#define setInt(variable)                                       \
	if (!(ss >> variable)){                                                     \
		LOG("setInt: Invalid property with name \"%s\"", prop.first.c_str());     \
	}                                                                               \

#define setFloat(variable)                                    \
	if (!(ss >> variable)){                                                     \
		LOG("setFloat: Invalid property with name \"%s\"", prop.first.c_str());     \
	}                                                                               \

#define setVector3(variable)                                  \
	if (!(ss >> variable.x >> variable.y >> variable.z)){                       \
		LOG("setVector3: Invalid property with name \"%s\"", prop.first.c_str());   \
	}                                                                               \

#define setString(variable)                                   \
	if (!(ss >> variable)){                                                     \
		LOG("setString: Invalid property with name \"%s\"", prop.first.c_str());    \
	}                                                                               \

#define setBool(variable) \
	if (!(ss >> variable)){ \
		LOG("setBool: Invalid property with name \"%s\"", prop.first.c_str()); \
	}

//#define setAnimator() \
//	if (prop.first == "anim") \
//	{ \
//		std::string anim, mesh; \
//		if (ss >> anim >> mesh) { \
//			setMesh(mesh); \
//                            \
//			Ogre::AnimationState* aux = getAnimation(anim); \
//			aux->setEnabled(true); \
//			currentAnimation = anim; \
//		} \
//		else \
//			LOG("ANIMATOR: wrong value for property %s.\n", prop.first.c_str()); \
//	}
//
//#define setCamera() \
//	if (prop.second == "true") {\
//		if (gameObject->getScene()->getMainCamera() == nullptr) {\
//			isMainCamera = true;\
//			gameObject->getScene()->setMainCamera(this);\
//		}\
//		else {\
//			LOG("CAMERA: there's already a main Camera\n");\
//			}\
//	}\
//	else if (prop.second == "false") {\
//		isMainCamera = false;\
//	}\
//	else {\
//		LOG("CAMERA: %s value not valid for \"main\" property\n", prop.second.c_str());\
//	}
//
//#define setLight() \
//	if (prop.first == "type") { \
//		if (prop.second == "Point") \
//			setType(Point); \
//		else if (prop.second == "Spotlight") \
//			setType(Spotlight); \
//		else if (prop.second == "Directional") \
//			setType(Directional); \
//		else \
//			LOG("LIGHT: %s not valid light type\n", prop.second.c_str()); \
//	} \
//	else if (prop.first == "colour") { \
//		double x, y, z; \
//		if (ss >> x >> y >> z) \
//			setColour(x, y, z); \
//		else \
//			LOG("LIGHT: wrong value for property %s.\n", prop.first.c_str()); \
//	}
#endif


