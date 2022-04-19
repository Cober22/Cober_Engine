#pragma once

namespace Cober {

	class Timestep {
	public:
		Timestep(float time = 0.0f)
			: m_Time(time)
		{
		}

		operator float() const { return m_Time; }

		float GetSeconds() const { return m_Time; }
		float GetMilliseconds() const { return m_Time * 1000.0f; }
		float GetFrames() { return m_Frames; }
		void SetFrames(float frames) { m_Frames = frames; }
	private:
		float m_Time;
		float m_Frames;
	};
}