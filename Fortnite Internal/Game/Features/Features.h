#pragma once
#include <Windows.h>
#include <vector>
#include <memory>

#include "../../Utilities/Logger.h"

namespace Features {
    class IAutoRevertFeature {
    public:
        virtual ~IAutoRevertFeature() = default;

        virtual bool IsDuplicate(void* Address, bool* Enabled, uint8_t BitMask) const = 0;

        virtual bool Tick() = 0;
    };

    template <typename T>
    class AutoRevertFeature : public IAutoRevertFeature {
    private:
        T* Address;
        T OriginalValue;
        bool* Enabled;
    public:
        AutoRevertFeature(T* Address, bool* Enabled) : Address(Address), Enabled(Enabled) {
            if (Address && Enabled) {
                OriginalValue = *Address;
            }
        }

        ~AutoRevertFeature() override {
            if (Address) *Address = OriginalValue;
        }

        bool IsDuplicate(void* Address, bool* Enabled, uint8_t BitMask) const override {
            return (this->Address == Address) && (this->Enabled == Enabled);
        }

        bool Tick() override {
            if (Address && Enabled) {
                if (*Enabled == false) {
                    *Address = OriginalValue;

                    return false;
                }
            }
            else {
                return false;
            }

            return true;
        }
    };

    class AutoRevertBitFeature : public IAutoRevertFeature {
    private:
        uint8_t* Address;
        uint8_t BitMask;
        bool OriginalValue;
        bool* Enabled;

    public:
        AutoRevertBitFeature(uint8_t* Address, uint8_t BitMask, bool* Enabled) : Address(Address), BitMask(BitMask), Enabled(Enabled) {
            if (Address && Enabled) {
                OriginalValue = *Address & BitMask;
            }
        }

        ~AutoRevertBitFeature() override {
            if (Address && Enabled && !*Enabled) {
                if (OriginalValue) {
                    *Address |= BitMask; // Set the bit
                }
                else {
                    *Address &= ~BitMask; // Clear the bit
                }
            }
        }

        bool IsDuplicate(void* Address, bool* Enabled, uint8_t BitMask) const override {
            return (this->Address == Address) && (this->Enabled == Enabled) && (this->BitMask == BitMask);
        }

        bool Tick() override {
            if (Address && Enabled) {
                if (*Enabled == false) {
                    *Address = OriginalValue ? *Address | BitMask : *Address & ~BitMask;
                    return false;
                }
            }
            else {
                return false;
            }

            return true;
        }
    };

    inline std::vector<std::unique_ptr<IAutoRevertFeature>> AutoRevertFeatures;

    template <typename T>
    inline void CreateAutoRevertFeature(T* Address, bool* Enabled) {
        auto Feature = std::make_unique<AutoRevertFeature<T>>(Address, Enabled);

        // Check that there isn't already a feature with the same Address and Enabled
        for (const auto& ExistingFeature : AutoRevertFeatures) {
            if (ExistingFeature->IsDuplicate(Address, Enabled, 0)) {
                return;
            }
        }

        AutoRevertFeatures.push_back(std::move(Feature));
    }

    inline void CreateAutoRevertBitFeature(uint8_t* Address, uint8_t BitPosition, bool* Enabled) {
        auto Feature = std::make_unique<AutoRevertBitFeature>(Address, BitPosition, Enabled);

        // Check that there isn't already a feature with the same Address and Enabled
        for (const auto& ExistingFeature : AutoRevertFeatures) {
            if (ExistingFeature->IsDuplicate(Address, Enabled, BitPosition)) {
                return;
            }
        }

        AutoRevertFeatures.push_back(std::move(Feature));
    }

    inline void Tick() {
        auto it = AutoRevertFeatures.begin();
        while (it != AutoRevertFeatures.end()) {
            if ((*it)->Tick() == false) {
                it = AutoRevertFeatures.erase(it); // Remove the feature if Tick returns false
            }
            else {
                ++it;
            }
        }
    }



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