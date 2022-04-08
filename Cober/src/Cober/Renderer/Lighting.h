#pragma once

#include <glm/glm.hpp>
namespace Cober {

#define C_WHITE Vector3f	(1.0f,	1.0f,	1.0f)
#define C_RED Vector3f		(1.0f,	0.0f,	0.0f)
#define C_GREEN Vector3f	(0.0f,	1.0f,	0.0f)
#define C_CYAN Vector3f		(0.0f,	1.0f,	1.0f)
#define C_BLUE Vector3f		(0.0f,	0.0f,	1.0f)
#define C_YELLOW Vector3f	(1.0f,	1.0f,	0.0f)

	struct LightAttenuation {
		float Constant;
		float Linear;
		float Exp;

		LightAttenuation();
		LightAttenuation(float a, float b, float c)
			: Constant(a), Linear(b), Exp(c) {}
	};

	struct DirectionalLight {
		glm::vec3 Direction, Color;
		float AmbientIntensity, DiffuseIntensity;
		
		DirectionalLight() {}
		DirectionalLight(const glm::vec3& direction, const glm::vec3& color, 
						 float ambient, float diffuse) 
			: Direction(direction), Color(color), 
			AmbientIntensity(ambient), DiffuseIntensity(diffuse) {}
	};

	struct PointLight {
		glm::vec3 Position, Color;
		float AmbientIntensity, DiffuseIntensity;
		LightAttenuation Attenuation;

		PointLight() {}
		PointLight(	const glm::vec3& position, const glm::vec3& color, 
					float ambient, float diffuse, 
					float linear, float exp) 
			:	Position(position), Color(color), 
			AmbientIntensity(ambient), DiffuseIntensity(diffuse),
			Attenuation({ 1.0f, linear, exp }) {}

		inline void SetPosition(const glm::vec3& position) { Position = position; }
	};

	struct SpotLight {
		glm::vec3 Direction, Position, Color;
		float CutOff, OuterCutOff;
		float AmbientIntensity, DiffuseIntensity;
		LightAttenuation Attenuation;

		SpotLight() {}
		SpotLight(	const glm::vec3 & direction, const glm::vec3 & position, const glm::vec3 & color, 
					float cutOff, float outerCutOff, 
					float ambient, float diffuse, 
					float linear, float exp) 
			: Direction(direction), Position(position), Color(color),
			CutOff(cutOff), OuterCutOff(outerCutOff), 
			AmbientIntensity(ambient), DiffuseIntensity(diffuse),
			Attenuation({ 1.0f, linear, exp }) {}

		inline void SetPosition(const glm::vec3& position) { Position = position; }
		inline void SetDirection(const glm::vec3& direction) { Direction = direction; }
	};
}
