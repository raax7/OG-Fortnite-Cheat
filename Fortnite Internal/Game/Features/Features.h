#pragma once
#include <Windows.h>

namespace Features {
	template <typename T>
	class AutoRevertFeature {
	private:
		T* Address;
		T OriginalValue;
		T NewValue;

		bool EnabledLastFrame;
		bool* Enabled;
	public:
		AutoRevertFeature(T* Address, T NewValue, bool* Enabled) {
			if (Address == nullptr) delete this;
			if (Enabled == nullptr) delete this;

			this->OriginalValue = *Address;
			this->NewValue = NewValue;
			this->Address = Address;
			this->Enabled = Enabled;
		}

		~AutoRevertFeature() {
			if (Address == nullptr) return;
			*Address = OriginalValue;
		}

		void Tick() {
			if (Address == nullptr) delete this;
			if (Enabled == nullptr) delete this;

			if (Enabled && !EnabledLastFrame) {
				*Address = OriginalValue;
			}
			else if (!Enabled && EnabledLastFrame) {
				*Address = OriginalValue;
			}

			EnabledLastFrame = Enabled;
		}
	};



	namespace Aimbot {
		class Target;
	};

	namespace Exploits {
		class Vehicle;
	};

	namespace FortPawnHelper {
		class Bone;
	};
}