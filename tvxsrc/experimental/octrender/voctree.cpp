
#include "voctree.hpp"

namespace tvx {

	uint32_t VoxelDword::getBits() {
		return *reinterpret_cast<uint32_t*>(this);
	}
	
	[[nodiscard]] uint_fast8_t VoxelDword::getChildMasked(uint_fast8_t mask) const { return data[0] & mask; }
	[[nodiscard]] uint_fast8_t VoxelDword::getNormal() const { return (data[1] & 0b00011111u); }
	[[nodiscard]] uint_fast8_t VoxelDword::getRed() const { return (data[1] & 0b11100000u) >> 5u; }
	[[nodiscard]] uint_fast8_t VoxelDword::getGreen() const { return (data[2] & 0b00000111u); }
	[[nodiscard]] uint_fast8_t VoxelDword::getBlue() const { return (data[2] & 0b00111000u) >> 3u; }
	[[nodiscard]] bool VoxelDword::getIsFilled() const { return data[2] & 0b01000000u; }
	[[nodiscard]] bool VoxelDword::getIsMetal() const { return data[2] & 0b10000000u; }
	[[nodiscard]] uint_fast8_t VoxelDword::getRoughness() const { return data[3] & 0b00001111u; }
	[[nodiscard]] uint_fast8_t VoxelDword::getLightness() const { return (data[3] & 0b11110000u) >> 4u; }
	
	void VoxelDword::setBits(uint32_t in) {
		*this = *reinterpret_cast<VoxelDword*>(&in);
	}
	void VoxelDword::setChildOn(OctCoordCartesian which) {
		data[0] |= (0b1u << static_cast<uint_fast8_t>(which));
	}
	void VoxelDword::setChildOff(OctCoordCartesian which) {
		data[0] &= ~(0b1u << static_cast<uint_fast8_t>(which));
	}
	void VoxelDword::setChildren(uint8_t in) {
		data[0] |= in;
	}
	void VoxelDword::setNormal(uint_fast8_t in) {
		data[1] = (data[1] & 0b11100000u) | (in & 0b00011111u);
	}
	void VoxelDword::setRed(uint_fast8_t in) {
		data[1] = (data[1] & 0b00011111u) | ((uint8_t)(in << 5u) & 0b11100000u);
	}
	void VoxelDword::setGreen(uint_fast8_t in) {
		data[2] = (data[2] & 0b11111000u) | (in & 0b00000111u);
	}
	void VoxelDword::setBlue(uint_fast8_t in) {
		data[2] = (data[2] & 0b11000111u) | ((uint8_t)(in << 3u) & 0b00111000u);
	}
	void VoxelDword::setIsFilled(bool in) {
		if (in) {
			data[2] |= 0b01000000u;
		} else {
			data[2] &= 0b10111111u;
		}
	}
	void VoxelDword::setIsMetal(bool in) {
		if (in) {
			data[2] |= 0b10000000u;
		} else {
			data[2] &= 0b01111111u;
		}
	}
	void VoxelDword::setRoughness(uint_fast8_t in) {
		data[3] = (data[3] & 0b11110000u) | (in & 0b00001111u);
	}
	void VoxelDword::setLightness(uint_fast8_t in) {
		data[3] = (data[3] & 0b00001111u) | ((uint8_t)(in << 4u) & 0b11110000u);
	}
	
}